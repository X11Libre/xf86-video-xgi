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




void XGINew_SetReg1( USHORT , USHORT , USHORT ) ;
void XGINew_SetReg2( USHORT , USHORT , USHORT ) ;
void XGINew_SetReg3( USHORT , USHORT ) ;
void XGINew_SetReg4( USHORT , ULONG ) ;
UCHAR XGINew_GetReg1( USHORT , USHORT) ;
UCHAR XGINew_GetReg2( USHORT ) ;
ULONG XGINew_GetReg3( USHORT ) ;
void XGINew_ClearDAC( PUCHAR ) ;
void     XGINew_SetRegANDOR(USHORT Port,USHORT Index,USHORT DataAND,USHORT DataOR);
void     XGINew_SetRegOR(USHORT Port,USHORT Index,USHORT DataOR);
void     XGINew_SetRegAND(USHORT Port,USHORT Index,USHORT DataAND);


/* --------------------------------------------------------------------- */
/* Function : XGINew_SetReg1 */
/* Input : */
/* Output : */
/* Description : SR CRTC GR */
/* --------------------------------------------------------------------- */
void XGINew_SetReg1( USHORT port , USHORT index , USHORT data )
{
#ifdef LINUX_XF86
    OutPortByte( ( PUCHAR )(ULONG)port , index ) ;
//udelay(1);
    OutPortByte( ( PUCHAR )(ULONG)port + 1 , data ) ;
DelayUS(1);
#else
    OutPortByte( ( PUCHAR )port , index ) ;
//udelay(1);
    OutPortByte( ( PUCHAR )port + 1 , data ) ;
#endif
}


/* --------------------------------------------------------------------- */
/* Function : XGINew_SetReg2 */
/* Input : */
/* Output : */
/* Description : AR( 3C0 ) */
/* --------------------------------------------------------------------- */
/*void XGINew_SetReg2( USHORT port , USHORT index , USHORT data )
{
    InPortByte( ( PUCHAR )port + 0x3da - 0x3c0 ) ;
    OutPortByte( XGINew_P3c0 , index ) ;
    OutPortByte( XGINew_P3c0 , data ) ;
    OutPortByte( XGINew_P3c0 , 0x20 ) ;
}*/


/* --------------------------------------------------------------------- */
/* Function : */
/* Input : */
/* Output : */
/* Description : */
/* --------------------------------------------------------------------- */
void XGINew_SetReg3( USHORT port , USHORT data )
{
    OutPortByte( port , data ) ;
}


/* --------------------------------------------------------------------- */
/* Function : XGINew_SetReg4 */
/* Input : */
/* Output : */
/* Description : */
/* --------------------------------------------------------------------- */
void XGINew_SetReg4( USHORT port , ULONG data )
{
    OutPortLong( port , data ) ;
}


/* --------------------------------------------------------------------- */
/* Function : XGINew_GetReg1 */
/* Input : */
/* Output : */
/* Description : */
/* --------------------------------------------------------------------- */
UCHAR XGINew_GetReg1( USHORT port , USHORT index )
{
    UCHAR data ;

#ifdef LINUX_XF86
    OutPortByte( ( PUCHAR )(ULONG)port , index ) ;
//udelay(1);
    data = InPortByte( ( PUCHAR )(ULONG)port + 1 ) ;
#else
    OutPortByte( ( PUCHAR )port , index ) ;
//udelay(1);	
    data = InPortByte( ( PUCHAR )port + 1 ) ;
#endif

    return( data ) ;
}


/* --------------------------------------------------------------------- */
/* Function : XGINew_GetReg2 */
/* Input : */
/* Output : */
/* Description : */
/* --------------------------------------------------------------------- */
UCHAR XGINew_GetReg2( USHORT port )
{
    UCHAR data ;

    data = InPortByte( port ) ;

    return( data ) ;
}


/* --------------------------------------------------------------------- */
/* Function : XGINew_GetReg3 */
/* Input : */
/* Output : */
/* Description : */
/* --------------------------------------------------------------------- */
ULONG XGINew_GetReg3( USHORT port )
{
    ULONG data ;

    data = InPortLong( port ) ;

    return( data ) ;
}



/* --------------------------------------------------------------------- */
/* Function : XGINew_SetRegANDOR */
/* Input : */
/* Output : */
/* Description : */
/* --------------------------------------------------------------------- */
void XGINew_SetRegANDOR( USHORT Port , USHORT Index , USHORT DataAND , USHORT DataOR )
{
    USHORT temp ;

    temp = XGINew_GetReg1( Port , Index ) ;		/* XGINew_Part1Port index 02 */
    temp = ( temp & ( DataAND ) ) | DataOR ;
    XGINew_SetReg1( Port , Index , temp ) ;
}


/* --------------------------------------------------------------------- */
/* Function : XGINew_SetRegAND */
/* Input : */
/* Output : */
/* Description : */
/* --------------------------------------------------------------------- */
void XGINew_SetRegAND(USHORT Port,USHORT Index,USHORT DataAND)
{
    USHORT temp ;

    temp = XGINew_GetReg1( Port , Index ) ;	/* XGINew_Part1Port index 02 */
    temp &= DataAND ;
    XGINew_SetReg1( Port , Index , temp ) ;
}


/* --------------------------------------------------------------------- */
/* Function : XGINew_SetRegOR */
/* Input : */
/* Output : */
/* Description : */
/* --------------------------------------------------------------------- */
void XGINew_SetRegOR( USHORT Port , USHORT Index , USHORT DataOR )
{
    USHORT temp ;

    temp = XGINew_GetReg1( Port , Index ) ;	/* XGINew_Part1Port index 02 */
    temp |= DataOR ;
    XGINew_SetReg1( Port , Index , temp ) ;
}



/* --------------------------------------------------------------------- */
/* Function : */
/* Input : */
/* Output : */
/* Description : */
/* --------------------------------------------------------------------- */
void XGINew_ClearDAC( PUCHAR port )
{
    int i ;

    OutPortByte( port , 0 ) ;
    port++ ;
    for( i = 0 ; i < 256 * 3 ; i++ )
    {
        OutPortByte( port , 0 ) ;
    }
}


/* --------------------------------------------------------------------- */
/* Function : Newdebugcode */
/* Input : */
/* Output : */
/* Description : */
/* --------------------------------------------------------------------- */
void Newdebugcode( UCHAR code )
{
#ifndef LINUX_XF86
    ULONG ultemp ;
#endif
    OutPortByte ( 0x80 , code ) ;
    /* OutPortByte ( 0x300 , code ) ; */
}



