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

typedef struct _XGI_VBModeStruct
{
	UCHAR  ModeID;
	UCHAR  VB_TVDelayIndex;
	UCHAR  VB_TVFlickerIndex;
	UCHAR  VB_TVPhaseIndex;
	UCHAR  VB_TVYFilterIndex;
	UCHAR  VB_LCDDelayIndex;
	UCHAR  _VB_LCDHIndex;
	UCHAR  _VB_LCDVIndex;
} XGI_VBModeStruct;

typedef struct _XGI_New_StResInfoStruct
{
	USHORT HTotal;
	USHORT VTotal;
} XGI_New_StResInfoStruct;

typedef struct _XGI_New_ModeResInfoStruct
{
	USHORT HTotal;
	USHORT VTotal;
	UCHAR  XChar;
	UCHAR  YChar;
} XGI_New_ModeResInfoStruct;



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
	XGIIOADDRESS XGI_P3ca;
	XGIIOADDRESS XGI_P3c6;
	XGIIOADDRESS XGI_P3c7;
	XGIIOADDRESS XGI_P3c8;
	XGIIOADDRESS XGI_P3c9;
	XGIIOADDRESS XGI_P3cb;
	XGIIOADDRESS XGI_P3cd;
	XGIIOADDRESS XGI_P3da;
	XGIIOADDRESS XGI_Part1Port;
	XGIIOADDRESS XGI_Part2Port;
	XGIIOADDRESS XGI_Part3Port;
	XGIIOADDRESS XGI_Part4Port;
	XGIIOADDRESS XGI_Part5Port;
	XGIIOADDRESS XGI_VidCapt;
	XGIIOADDRESS XGI_VidPlay;

    /* FIXME: This field is tested but is never set. */
	USHORT XGI_IF_DEF_CONEX;

	UCHAR  XGI_VGAINFO;
#ifndef LINUX_KERNEL
        USHORT XGI_CP1, XGI_CP2, XGI_CP3, XGI_CP4;
#endif
	BOOLEAN XGI_UseROM;
	BOOLEAN XGI_ROMNew;
	BOOLEAN PanelSelfDetected;
	int     XGI_CHOverScan;
	BOOLEAN XGI_CHSOverScan;
	BOOLEAN XGI_UseLCDA;
	int     XGI_UseOEM;
	USHORT  XGI_Backup70xx;
	BOOLEAN HaveEMI;
	BOOLEAN HaveEMILCD;
	BOOLEAN OverruleEMI;
	SHORT  PDC, PDCA;
	USHORT XGI_CRT1Mode;
	int    XGI_RAMType;
	UCHAR  XGI_ChannelAB;
	UCHAR  XGI_DataBusWidth;
	USHORT XGI_ModeType;
	USHORT XGI_VBInfo;
	USHORT XGI_TVMode;
	USHORT XGI_LCDResInfo;
	USHORT XGI_LCDTypeInfo;
	USHORT XGI_LCDInfo;
	USHORT XGI_VBType;
	USHORT XGI_VBExtInfo;
	USHORT XGI_YPbPr;
	USHORT XGI_SelectCRT2Rate;
	USHORT XGI_SetFlag;
	USHORT XGI_RVBHCFACT;
	USHORT XGI_RVBHCMAX;
	USHORT XGI_RVBHRS;
	USHORT XGI_VGAVT;
	USHORT XGI_VGAHT;
	USHORT XGI_VT;
	USHORT XGI_HT;

    /* FIXME: These two fields are read but never set. */
	USHORT XGI_VGAVDE;
	USHORT XGI_VGAHDE;

    /* FIXME: This field is set but never used. */
	USHORT XGI_DDC_Port;

	const XGI_StStruct          *XGI_SModeIDTable;
	XGI_New_StandTableStruct        *XGI_StandTable;
	const XGI_ExtStruct         *XGI_EModeIDTable;
	const XGI_Ext2Struct        *XGI_RefIndex;
	const XGI_VBModeStruct      *XGI_VBModeIDTable;
	const XGI_CRT1TableStruct   *XGI_CRT1Table;
	XGI_VCLKDataStruct    	    *XGI_VCLKData;
	XGI_VBVCLKDataStruct        *XGI_VBVCLKData;
	const XGI_New_StResInfoStruct   *XGI_StResInfo;
	const XGI_New_ModeResInfoStruct *XGI_ModeResInfo;

#ifndef LINUX_XF86
	UCHAR *pXGI_SR07;
	UCHAR *XGI_CR49;
	UCHAR *XGI_SR25;
	UCHAR *pXGI_SR1F;
	UCHAR *pXGI_SR21;
	UCHAR *pXGI_SR22;
	UCHAR *pXGI_SR23;
	UCHAR *pXGI_SR24;
	UCHAR *pXGI_SR31;
	UCHAR *pXGI_SR32;
	UCHAR *pXGI_SR33;
	UCHAR *pXGI_CRT2Data_1_2;
	UCHAR *pXGI_CRT2Data_4_D;
	UCHAR *pXGI_CRT2Data_4_E;
	UCHAR *pXGI_CRT2Data_4_10;
	const USHORT *pXGI_RGBSenseData;
	const USHORT *pXGI_VideoSenseData;
	const USHORT *pXGI_YCSenseData;
	const USHORT *pXGI_RGBSenseData2;
	const USHORT *pXGI_VideoSenseData2;
	const USHORT *pXGI_YCSenseData2;
#endif

	USHORT  PanelXRes, PanelHT;
	USHORT  PanelYRes, PanelVT;
	USHORT  PanelHRS,  PanelHRE;
  	USHORT 	PanelVRS,  PanelVRE;

	BOOLEAN UseCustomMode;
	BOOLEAN CRT1UsesCustomMode;
	USHORT  CHDisplay;
	USHORT  CHSyncStart;
	USHORT  CHSyncEnd;
	USHORT  CHTotal;
	USHORT  CHBlankStart;
	USHORT  CHBlankEnd;
	USHORT  CVDisplay;
	USHORT  CVSyncStart;
	USHORT  CVSyncEnd;
	USHORT  CVTotal;
	USHORT  CVBlankStart;
	USHORT  CVBlankEnd;
	ULONG   CDClock;
	ULONG   CFlags;
	UCHAR   CCRT1CRTC[17];
	UCHAR   CSR2B;
	UCHAR   CSR2C;
	USHORT  CSRClock;
	USHORT  CSRClock_CRT1;
	USHORT  CModeFlag;
	USHORT  CModeFlag_CRT1;
	USHORT  CInfoFlag;

	int	LVDSHL;

	BOOLEAN Backup;
	UCHAR Backup_Mode;
	UCHAR Backup_14;
	UCHAR Backup_15;
	UCHAR Backup_16;
	UCHAR Backup_17;
	UCHAR Backup_18;
	UCHAR Backup_19;
	UCHAR Backup_1a;
	UCHAR Backup_1b;
	UCHAR Backup_1c;
	UCHAR Backup_1d;

	int     UsePanelScaler;
	int	CenterScreen;

	USHORT  CP_Vendor, CP_Product;
	BOOLEAN CP_HaveCustomData;
	int     CP_PreferredX, CP_PreferredY, CP_PreferredIndex;
	int	CP_MaxX, CP_MaxY, CP_MaxClock;
	BOOLEAN CP_Supports64048075;
	int     CP_HDisplay[7], CP_VDisplay[7];	/* For Custom LCD panel dimensions */
    	int     CP_HTotal[7], CP_VTotal[7];
    	int     CP_HSyncStart[7], CP_VSyncStart[7];
    	int     CP_HSyncEnd[7], CP_VSyncEnd[7];
	int     CP_HBlankStart[7], CP_VBlankStart[7];
	int     CP_HBlankEnd[7], CP_VBlankEnd[7];
    	int     CP_Clock[7];
	BOOLEAN CP_DataValid[7];
	BOOLEAN CP_HSync_P[7], CP_VSync_P[7], CP_SyncValid[7];
} XGI_Private;

#endif
