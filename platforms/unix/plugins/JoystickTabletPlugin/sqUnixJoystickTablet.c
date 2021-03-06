/* sqUnixJoystickTablet.c -- support for joysticks and graphics tablets
 * 
 *   Copyright (C) 1996-2004 by Ian Piumarta and other authors/contributors
 *                              listed elsewhere in this file.
 *   All rights reserved.
 *   
 *   This file is part of Unix Squeak.
 * 
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 * 
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 * 
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *   SOFTWARE.
 */

/* Author: Ian.Piumarta@INRIA.Fr
 *
 * Last edited: 2006-10-18 10:09:31 by piumarta on emilia.local
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
