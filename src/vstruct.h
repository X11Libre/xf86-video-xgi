/*
 * General structure definitions for universal mode switching modules
 *
 * Copyright (C) 2001-2004 by Thomas Winischhofer, Vienna, Austria
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License as published by
 * * the Free Software Foundation; either version 2 of the named License,
 * * or any later version.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) The name of the author may not be used to endorse or promote products
 * *    derived from this software without specific prior written permission.
 * *
 * * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESSED OR
 * * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: 	Thomas Winischhofer <thomas@winischhofer.net>
 *
 */

#ifndef _VSTRUCT_
#define _VSTRUCT_

#include "vb_struct.h"

typedef struct _XGI_Private
{
#ifdef LINUX_KERNEL
        XGIIOADDRESS RelIO;
#endif
	XGIIOADDRESS XGI_P3c4;
	XGIIOADDRESS XGI_P3d4;
	XGIIOADDRESS XGI_P3c0;
	XGIIOADDRESS XGI_P3ce;
	XGIIOADDRESS XGI_P3c2;

    /* FIXME: This field is set but never used. */
	XGIIOADDRESS XGI_P3ca;

	XGIIOADDRESS XGI_P3c6;
	XGIIOADDRESS XGI_P3c7;
	XGIIOADDRESS XGI_P3c8;
	XGIIOADDRESS XGI_P3c9;

    /* FIXME: These three fields are set but never used. */
	XGIIOADDRESS XGI_P3cb;
	XGIIOADDRESS XGI_P3cd;
	XGIIOADDRESS XGI_P3da;

	XGIIOADDRESS XGI_Part1Port;

    /* FIXME: These two fields are set but never used. */
	XGIIOADDRESS XGI_Part2Port;
	XGIIOADDRESS XGI_Part3Port;

	XGIIOADDRESS XGI_Part4Port;
	XGIIOADDRESS XGI_Part5Port;

    /* FIXME: These two fields are set but never used. */
	XGIIOADDRESS XGI_VidCapt;
	XGIIOADDRESS XGI_VidPlay;

    /* FIXME: This field is tested but is never set. */
	USHORT XGI_IF_DEF_CONEX;

	UCHAR  XGI_VGAINFO;

    /* FIXME: These 9 fields are set but never used. */
	BOOLEAN PanelSelfDetected;
	int     XGI_UseOEM;
	USHORT  XGI_Backup70xx;
	BOOLEAN HaveEMI;
	BOOLEAN HaveEMILCD;
	BOOLEAN OverruleEMI;
	SHORT  PDC, PDCA;
	USHORT XGI_CRT1Mode;

    /* FIXME: These two fields are tested but is never set. */
	USHORT XGI_ModeType;
	USHORT XGI_VBInfo;

	USHORT XGI_VBType;

    /* FIXME: This field is set but never used. */
	USHORT XGI_SelectCRT2Rate;

	USHORT XGI_SetFlag;

    /* FIXME: These two fields are read but never set. */
	USHORT XGI_VGAVDE;
	USHORT XGI_VGAHDE;

    /* FIXME: This field is set but never used. */
	USHORT XGI_DDC_Port;

    /* FIXME: These pointers are accessed in various parts of the code, but
     * FIXME: I can't see any place that they are initialized.
     */
	const XGI_StStruct          *XGI_SModeIDTable;
	XGI_New_StandTableStruct        *XGI_StandTable;
	const XGI_ExtStruct         *XGI_EModeIDTable;
	const XGI_Ext2Struct        *XGI_RefIndex;
	const XGI_CRT1TableStruct   *XGI_CRT1Table;
	XGI_VCLKDataStruct    	    *XGI_VCLKData;
	XGI_VBVCLKDataStruct        *XGI_VBVCLKData;
	const XGI_StResInfoStruct   *XGI_StResInfo;
	const XGI_ModeResInfoStruct *XGI_ModeResInfo;

    /* FIXME: These three fields are set but never used. */
	int	LVDSHL;
	int     UsePanelScaler;
	int	CenterScreen;
} XGI_Private;

#endif
