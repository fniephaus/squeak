/* sqUnixJoystickTablet.c -- support for joysticks and graphics tablets
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
 */

/* Author: Ian.Piumarta@INRIA.Fr
 *
 * Last edited: 2002-10-26 14:34:03 by piumarta on emilia.inria.fr
 */

#include "sq.h"
#include "JoystickTabletPlugin.h"

/* we don't have any joysticks */

void *joySticks= 0;

int joystickRead(int index)	{ return 0; }
int joystickInit(void)		{ return 0; }

/* we don't have any tablets either */

int tabletInit(void)
{
  return 0;
}

int tabletGetParameters(int cursorIndex, int result[])
{
  return 0;
}

int tabletRead(int cursorIndex, int result[])
{
  return 0;
}

int tabletResultSize(void)
{
  return 0;
}

int joystickShutdown(void)
{
  return 0;
}
