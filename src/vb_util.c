/* Copyright (C) 2003-2006 by XGI Technology, Taiwan.
 *
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation on the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT.  IN NO EVENT SHALL XGI AND/OR
 *  ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "osdef.h"
#include "vb_def.h"
#include "vgatypes.h"
#include "vb_struct.h"

#ifdef LINUX_KERNEL
#include "XGIfb.h"
#include <asm/io.h>
#include <linux/types.h>
#endif

#ifdef LINUX_XF86
#include "xf86.h"
#include "xf86PciInfo.h"
#include "xgi.h"
#include "xgi_regs.h"
#endif


/**
 * Write value to the PC's 0x80 debug port.
 * 
 * \bug
 * I'm pretty sure the debug 0x80 only exists on PCs.  Should this routine
 * be a no-op on non-x86 and non-x86-64 architectures?
 */
void Newdebugcode(unsigned char code)
{
    OutPortByte(0x80, code);
}
