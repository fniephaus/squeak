/* sqPlatformSpecific.h -- platform-specific modifications to sq.h
 * 
 *   Copyright (C) 1996-2002 Ian Piumarta and other authors/contributors
 *     as listed elsewhere in this file.
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
 *   2. This notice must not be removed or altered in any source distribution.
 * 
 *   Using (or modifying this file for use) in any context other than Squeak
 *   changes these copyright conditions.  Read the file `COPYING' in the
 *   directory `platforms/unix/doc' before proceeding with any such use.
 * 
 *   You are not allowed to distribute a modified version of this file
 *   under its original name without explicit permission to do so.  If
 *   you change it, rename it.
 * 
 * Author: ian.piumarta@inria.fr
 * 
 * Last edited: 2002-06-08 02:34:43 by piumarta on emilia.inria.fr
 */

/* undefine clock macros (these are implemented as functions) */

#undef ioMSecs
#undef ioMicroMSecs
#undef ioLowResMSecs

#include <sys/types.h>

typedef off_t squeakFileOffsetType;
