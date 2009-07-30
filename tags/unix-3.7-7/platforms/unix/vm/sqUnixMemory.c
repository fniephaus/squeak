/* sqUnixMemory.c -- dynamic memory management
 * 
 * Author: Ian.Piumarta@INRIA.Fr
 * 
 *   Copyright (C) 1996-2004 by Ian Piumarta and other authors/contributors
 *                              listed elsewhere in this file.
 *   All rights reserved.
 *   
 *   This file is part of Unix Squeak.
 * 
 *      You are NOT ALLOWED to distribute modified versions of this file
 *      under its original name.  If you modify this file then you MUST
 *      rename it before making your modifications available publicly.
 * 
 *   This file is distributed in the hope that it will be useful, but WITHOUT
 *   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *   FITNESS FOR A PARTICULAR PURPOSE.
 *   
 *   You may use and/or distribute this file ONLY as part of Squeak, under
 *   the terms of the Squeak License as described in `LICENSE' in the base of
 *   this distribution, subject to the following additional restrictions:
 * 
 *   1. The origin of this software must not be misrepresented; you must not
 *      claim that you wrote the original software.  If you use this software
 *      in a product, an acknowledgment to the original author(s) (and any
 *      other contributors mentioned herein) in the product documentation
 *      would be appreciated but is not required.
 * 
 *   2. You must not distribute (or make publicly available by any
 *      means) a modified copy of this file unless you first rename it.
 * 
 *   3. This notice must not be removed or altered in any source distribution.
 * 
 *   Using (or modifying this file for use) in any context other than Squeak
 *   changes these copyright conditions.  Read the file `COPYING' in the
 *   directory `platforms/unix/doc' before proceeding with any such use.
 * 
 * Last edited: 2003-02-11 14:36:21 by piumarta on emilia.inria.fr
 */

/* Note:
 * 
 *   The code allows memory to be overallocated; i.e., the initial
 *   block is reserved via mmap() and then the unused portion
 *   munmap()ped from the top end.  This is INHERENTLY DANGEROUS since
 *   malloc() may randomly map new memory in the block we "reserved"
 *   and subsequently unmap()ped.  Enabling this causes crashes in
 *   Croquet, which makes heavy use of the FFI and thus calls malloc()
 *   all over the place.
 *   
 *   For this reason, overallocateMemory is DISABLED by default.
 *   
 *   The upshot of all this is that Squeak will claim (and hold on to)
 *   ALL of the available virtual memory (or at least 75% of it) when
 *   it starts up.  If you can't live with that, use the -memory
 *   option to allocate a fixed size heap.
 */


#include "sq.h"
#include "config.h"
#include "debug.h"

#if defined(HAVE_MMAP)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>

#if !defined(MAP_ANON)
# if defined(MAP_ANONYMOUS)
#   define MAP_ANON MAP_ANONYMOUS
# else
#   define MAP_ANON 0
# endif
#endif

#define MAP_PROT	(PROT_READ | PROT_WRITE)
#define MAP_FLAGS	(MAP_ANON | MAP_PRIVATE)

extern int useMmap;

/*xxx THESE SHOULD BE COMMAND-LINE/ENVIRONMENT OPTIONS */
int overallocateMemory	= 0;	/* see notes above */

static int   devZero	= -1;
static char *heap	=  0;
static int   heapSize	=  0;
static int   heapLimit	=  0;

static int	    pageSize = 0;
static unsigned int pageMask = 0;

#define valign(x)	((x) & pageMask)

static int min(int x, int y) { return (x < y) ? x : y; }
static int max(int x, int y) { return (x > y) ? x : y; }


/* answer the address of (minHeapSize <= N <= desiredHeapSize) bytes of memory. */

void *sqAllocateMemory(int minHeapSize, int desiredHeapSize)
{
  if (!useMmap)
    return malloc(desiredHeapSize);

  if (heap)
    {
      fprintf(stderr, "sqAllocateMemory: already called\n");
      exit(1);
    }
  pageSize= getpagesize();
  pageMask= ~(pageSize - 1);

  dprintf(("sqAllocateMemory: pageSize 0x%x (%d), mask 0x%x\n", pageSize, pageSize, pageMask));

#if (!MAP_ANON)
  if ((devZero= open("/dev/zero", O_RDWR)) < 0)
    {
      perror("sqAllocateMemory: /dev/zero");
      return 0;
    }
#endif

  dprintf(("sqAllocateMemory: /dev/zero descriptor %d\n", devZero));
  dprintf(("sqAllocateMemory: min heap %d, desired %d\n", minHeapSize, desiredHeapSize));

  heapLimit= valign(max(desiredHeapSize, useMmap));

  while ((!heap) && (heapLimit >= minHeapSize))
    {
      dprintf(("sqAllocateMemory: mapping 0x%08x bytes (%d Mbytes)\n", heapLimit, heapLimit >> 20));
      if (MAP_FAILED == (heap= mmap(0, heapLimit, MAP_PROT, MAP_FLAGS, devZero, 0)))
	{
	  heap= 0;
	  heapLimit= valign(heapLimit / 4 * 3);
	}
    }

  if (!heap)
    {
      fprintf(stderr, "sqAllocateMemory: failed to allocate at least %d bytes)\n", minHeapSize);
      useMmap= 0;
      return malloc(desiredHeapSize);
    }

  heapSize= heapLimit;

  if (overallocateMemory)
    sqShrinkMemoryBy(heap + heapLimit, heapLimit - desiredHeapSize);

  return heap;
}


/* grow the heap by delta bytes.  answer the new end of memory. */

int sqGrowMemoryBy(int oldLimit, int delta)
{
  if (useMmap)
    {
      int newSize=  min(valign((char *)oldLimit - heap + delta), heapLimit);
      int newDelta= newSize - heapSize;
      dprintf(("sqGrowMemory: %p By: %d(%d) (%d -> %d)\n", oldLimit, newDelta, delta, heapSize, newSize));
      assert(0 == (newDelta & ~pageMask));
      assert(0 == (newSize  & ~pageMask));
      assert(newDelta >= 0);
      if (newDelta)
	{
	  dprintf(("was: %p %p %p = 0x%x (%d) bytes\n", heap, heap + heapSize, heap + heapLimit, heapSize, heapSize));
	  if (overallocateMemory)
	    {
	      char *base= heap + heapSize;
	      dprintf(("remap: %p + 0x%x (%d)\n", base, newDelta, newDelta));
	      if (MAP_FAILED == mmap(base, newDelta, MAP_PROT, MAP_FLAGS | MAP_FIXED, devZero, heapSize))
		{
		  perror("mmap");
		  return oldLimit;
		}
	    }
	  heapSize += newDelta;
	  dprintf(("now: %p %p %p = 0x%x (%d) bytes\n", heap, heap + heapSize, heap + heapLimit, heapSize, heapSize));
	  assert(0 == (heapSize  & ~pageMask));
	}
      return (int)heap + heapSize;
    }
  return oldLimit;
}


/* shrink the heap by delta bytes.  answer the new end of memory. */

int sqShrinkMemoryBy(int oldLimit, int delta)
{
  if (useMmap)
    {
      int newSize=  max(0, valign((char *)oldLimit - heap - delta));
      int newDelta= heapSize - newSize;
      dprintf(("sqGrowMemory: %p By: %d(%d) (%d -> %d)\n", oldLimit, newDelta, delta, heapSize, newSize));
      assert(0 == (newDelta & ~pageMask));
      assert(0 == (newSize  & ~pageMask));
      assert(newDelta >= 0);
      if (newDelta)
	{
	  dprintf(("was: %p %p %p = 0x%x (%d) bytes\n", heap, heap + heapSize, heap + heapLimit, heapSize, heapSize));
	  if (overallocateMemory)
	    {
	      char *base= heap + heapSize - newDelta;
	      dprintf(("unmap: %p + 0x%x (%d)\n", base, newDelta, newDelta));
	      if (munmap(base, newDelta) < 0)
		{
		  perror("unmap");
		  return oldLimit;
		}
	    }
	  heapSize -= newDelta;
	  dprintf(("now: %p %p %p = 0x%x (%d) bytes\n", heap, heap + heapSize, heap + heapLimit, heapSize, heapSize));
	  assert(0 == (heapSize  & ~pageMask));
	}
      return (int)heap + heapSize;
    }
  return oldLimit;
}


/* answer the number of bytes available for growing the heap. */

int sqMemoryExtraBytesLeft(int includingSwap)
{
  return useMmap ? (heapLimit - heapSize) : 0;
}


#else  /* !HAVE_MMAP */

void *sqAllocateMemory(int minHeapSize, int desiredHeapSize)	{ return malloc(desiredHeapSize); }
int   sqGrowMemoryBy(int oldLimit, int delta)			{ return oldLimit; }
int   sqShrinkMemoryBy(int oldLimit, int delta)			{ return oldLimit; }
int   sqMemoryExtraBytesLeft(int includingSwap)			{ return 0; }

#endif




#if defined(TEST_MEMORY)

#define MBytes	*1024*1024

int main()
{
  char *mem= sqAllocateMemory(4 MBytes, 40 MBytes);
  printf("memory allocated at %p\n", mem);
  sqShrinkMemoryBy((int)heap + heapSize, 5 MBytes);
  sqGrowMemoryBy((int)heap + heapSize, 1 MBytes);
  sqGrowMemoryBy((int)heap + heapSize, 1 MBytes);
  sqGrowMemoryBy((int)heap + heapSize, 1 MBytes);
  sqGrowMemoryBy((int)heap + heapSize, 100 MBytes);
  sqShrinkMemoryBy((int)heap + heapSize, 105 MBytes);
  return 0;
}

#endif /* defined(TEST_MEMORY) */