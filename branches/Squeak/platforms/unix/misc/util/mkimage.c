/* mkimage.c -- generate gas source for initialised data
 * 
 *   Copyright (C) 1996 1997 1998 1999 2000 2001 Ian Piumarta and individual
 *      authors/contributors listed elsewhere in this file.
 *   All rights reserved.
 *   
 *   This file is part of Unix Squeak.
 * 
 *   This file is distributed in the hope that it will be useful, but WITHOUT
 *   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *   FITNESS FOR A PARTICULAR PURPOSE.
 *   
 *   You may use and/or distribute this file ONLY as part of Squeak, under
 *   the terms of the Squeak License as described in `LICENSE' in the base of
 *   this distribution, subject to the following restrictions:
 * 
 *   1. The origin of this software must not be misrepresented; you must not
 *      claim that you wrote the original software.  If you use this software
 *      in a product, an acknowledgment to the original author(s) (and any
 *      other contributors mentioned herein) in the product documentation
 *      would be appreciated but is not required.
 * 
 *   2. This notice may not be removed or altered in any source distribution.
 * 
 *   Using or modifying this file for use in any context other than Squeak
 *   changes these copyright conditions.  Read the file `COPYING' in the base
 *   of the distribution before proceeding with any such use.
 * 
 *   You are STRONGLY DISCOURAGED from distributing a modified version of
 *   this file under its original name without permission.  If you must
 *   change it, rename it first.
 */

/* Last edited: Mon Aug 21 01:47:04 2000 by piumarta (Ian Piumarta) on emilia
 */

#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  if (argc != 2)
    {
      fprintf(stderr, "usage: %s sectionName\n", argv[0]);
      exit(1);
    }
  printf("\n\t.data");
  printf("\n\t.globl %s_start", argv[1]);
  printf("\n%s_start:", argv[1]);
  {
    unsigned char buf[8192];
    int nBytes= 0;
    while ((nBytes= read(0, buf, sizeof(buf))) > 0)
      {
	unsigned char *ptr= buf;
	int pos= 0;
	while (nBytes > 3)
	  {
	    if (pos == 0)
	      printf("\n\t.long\t");
	    else
	      printf(", ");
	    printf("%ld", *(long *)ptr);
	    if (++pos == 16)
	      pos= 0;
	    ptr+= 4;
	    nBytes-= 4;
	  }
	pos= 0;
	while (nBytes)
	  {
	    if (pos == 0)
	      printf("\n\t.byte\t");
	    else
	      printf(", ");
	    printf("%d", *ptr);
	    if (++pos == 16)
	      pos= 0;
	    ++ptr;
	    --nBytes;
	  }
      }
  }
  printf("\n\t.globl %s_end", argv[1]);
  printf("\n%s_end:\n", argv[1]);
  return 0;
}
