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
 
#ifndef _VBUTIL_
#define _VBUTIL_
extern void Newdebugcode(unsigned char code);
extern   UCHAR    XGINew_GetReg2(USHORT);
extern   ULONG    XGINew_GetReg3(USHORT);
extern   void     XGINew_SetRegOR(USHORT Port,USHORT Index,USHORT DataOR);
extern   void     XGINew_SetRegAND(USHORT Port,USHORT Index,USHORT DataAND);
extern   void     XGINew_SetRegANDOR(USHORT Port,USHORT Index,USHORT DataAND,USHORT DataOR);
#endif

