/*
 * Global definitions for init.c and init301.c
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

#ifndef _INITDEF_
#define _INITDEF_

#define VB_XGI301	      	0x0001 /*301b*/
#define VB_XGI301B        	0x0002
#define VB_XGI302B        	0x0004
#define VB_XGI301LV     	0x0008 /*301lv*/
#define VB_XGI302LV     	0x0010
#define VB_XGI302ELV		0x0020
#define VB_XGI301C              0x0040 /* for 301C */
#define VB_NoLCD        	0x8000
#define VB_XGI301BLV302BLV      (VB_XGI301B|VB_XGI301C|VB_XGI302B|VB_XGI301LV|VB_XGI302LV|VB_XGI302ELV)
#define VB_XGI301LV302LV        (VB_XGI301LV|VB_XGI302LV|VB_XGI302ELV)
#define VB_XGIVB		(VB_XGI301 | VB_XGI301BLV302BLV)

/* VBInfo */
/* CR 30 */
#define SetSimuScanMode         0x0001
#define SwitchToCRT2            0x0002
#define SetCRT2ToAVIDEO         0x0004
#define SetCRT2ToSVIDEO         0x0008
#define SetCRT2ToSCART          0x0010
#define SetCRT2ToLCD            0x0020
#define SetCRT2ToRAMDAC         0x0040
#define SetCRT2ToHiVisionTV     0x0080
#define SetCRT2ToCHYPbPr       	SetCRT2ToHiVisionTV  /* for Chrontel   */

/* CR 31 */
#define SetNTSCTV               0x0000
/* #define SetPALTV                0x0100    Deprecated here, now in TVMode */
#define SetInSlaveMode          0x0200
#define SetNotSimuMode          0x0400
#define SetNotSimuTVMode        SetNotSimuMode
#define SetDispDevSwitch        0x0800
#define SetCRT2ToYPbPr          0x0800
#define LoadDACFlag             0x1000
#define DisableCRT2Display      0x2000
#define DriverMode              0x4000
#define HotKeySwitch            0x8000
#define SetCRT2ToLCDA           0x8000

/* v-- Needs change in xgi_vga.c if changed (GPIO) --v */
#define SetCRT2ToTV1            (SetCRT2ToHiVisionTV | SetCRT2ToSCART | SetCRT2ToSVIDEO | SetCRT2ToAVIDEO)
#define SetCRT2ToTV             (SetCRT2ToYPbPr | SetCRT2ToTV1)

/* XGI_ModeType */
#define ModeInfoFlag            0x0007
#define IsTextMode              0x0007
#define ModeText                0x0000
#define ModeCGA                 0x0001
#define ModeEGA                 0x0002       /* 16 colors mode */
#define ModeVGA                 0x0003       /* 256 colors mode */
#define Mode15Bpp               0x0004       /* 15 Bpp Color Mode */
#define Mode16Bpp               0x0005       /* 16 Bpp Color Mode */
#define Mode24Bpp               0x0006       /* 24 Bpp Color Mode */
#define Mode32Bpp               0x0007       /* 32 Bpp Color Mode */

#define DACInfoFlag             0x0018
#define MemoryInfoFlag          0x01e0
#define MemorySizeShift         5

/* modeflag */
#define Charx8Dot               0x0200
#define LineCompareOff          0x0400
#define CRT2Mode                0x0800
#define HalfDCLK                0x1000
#define NoSupportSimuTV         0x2000
#define DoubleScanMode          0x8000

/* Infoflag */
#define SupportTV               0x0008
#define SupportTV1024           0x0800  /*301b*/
#define SupportCHTV 		0x0800
#define SupportHiVisionTV       0x0010
/* #define SupportYPbPr750p        0x1000 */
#define SupportLCD              0x0020
#define SupportRAMDAC2          0x0040	/* All           (<= 100Mhz) */
/*#define SupportRAMDAC2_135      0x0100   All except DH (<= 135Mhz) */
/*#define SupportRAMDAC2_162      0x0200   B, C          (<= 162Mhz) */
/*#define SupportRAMDAC2_202      0x0400   C             (<= 202Mhz) */
#define InterlaceMode           0x0080
#define SyncPP                  0x0000
#define SyncPN                  0x4000
#define SyncNP                  0x8000
#define SyncNN                  0xc000

/* SetFlag */
#define ProgrammingCRT2         0x0001
#define LowModeTests		0x0002

/* CR32 (Newer 630, and 315 series)

   [0]   VB connected with CVBS
   [1]   VB connected with SVHS
   [2]   VB connected with SCART
   [3]   VB connected with LCD
   [4]   VB connected with CRT2 (secondary VGA)
   [5]   CRT1 monitor is connected
   [6]   VB connected with Hi-Vision TV
   [7]   <= 330: VB connected with DVI combo connector
         >= 661: VB connected to YPbPr
*/

/* CR35 (300 series only) */
#define TVOverScan              0x10
#define TVOverScanShift         4

/* CR35 (661 series only)

   [0]    1 = PAL, 0 = NTSC
   [1]    1 = NTSC-J (if D0 = 0)
   [2]    1 = PALM (if D0 = 1)
   [3]    1 = PALN (if D0 = 1)
   [4]    1 = Overscan (Chrontel only)
   [7:5]  (only if D2 in CR38 is set)
          000  525i
 	  001  525p
	  010  750p
	  011  1080i (or HiVision on 301, 301B)

   These bits are being translated to TVMode flag.

*/

/*
   CR37

   [0]   Set 24/18 bit (0/1) RGB to LVDS/TMDS transmitter (set by BIOS)
   [3:1] External chip
         300 series:
	    001   XGI301 (never seen)
	    010   LVDS
	    011   LVDS + Tumpion Zurac
	    100   LVDS + Chrontel 7005
	    110   Chrontel 7005
	  315/330 series
	    001   XGI30x (never seen)
	    010   LVDS
	    011   LVDS + Chrontel 7019
	  660 series [2:1] only:
	     reserved (now in CR38)
	  All other combinations reserved
   [3]    661 only: Pass 1:1 data
   [4]    LVDS: 0: Panel Link expands / 1: Panel Link does not expand
          30x:  0: Bridge scales      / 1: Bridge does not scale = Panel scales (if possible)
   [5]    LCD polarity select
          0: VESA DMT Standard
	  1: EDID 2.x defined
   [6]    LCD horizontal polarity select
          0: High active
	  1: Low active
   [7]    LCD vertical polarity select
          0: High active
	  1: Low active
*/

/* CR37: LCDInfo */
#define LCDRGB18Bit           0x0001
#define LCDNonExpanding       0x0010
#define LCDSync               0x0020
#define LCDPass11             0x0100   /* 0: center screen, 1: Pass 1:1 data */

#define DontExpandLCD	      LCDNonExpanding
#define LCDNonExpandingShift       4
#define DontExpandLCDShift    LCDNonExpandingShift
#define LCDSyncBit            0x00e0  /* H/V polarity & sync ID */
#define LCDSyncShift               6

/* CR38 (661 and later)
  D[7:5]  000 No VB
          001 301 series VB
	  010 LVDS
	  011 Chrontel 7019
	  100 Conexant
  D2      Enable YPbPr output (see CR35)
  D[1:0]  LCDA (like before)
*/

#define EnablePALMN             0x40   /* Romflag: 1 = Allow PALM/PALN */

/* CR39 (661 and later)
   D[1:0] YPbPr Aspect Ratio
          00 4:3 letterbox
	  01 4:3
	  10 16:9
	  11 4:3
*/

/* CR3B (651+301C)
   D[1:0] YPbPr Aspect Ratio
          ?
*/


/* Timing data */
#define NTSCHT                  1716
#define NTSC2HT                 1920
#define NTSCVT                  525
#define PALHT                   1728
#define PALVT                   625
#define StHiTVHT                892
#define StHiTVVT                1126
#define StHiTextTVHT            1000
#define StHiTextTVVT            1126
#define ExtHiTVHT               2100
#define ExtHiTVVT               1125

/* ------------------------------ */

#define SetSCARTOutput          0x01

#define HotPlugFunction         0x08

#define StStructSize            0x06

#define XGI_VIDEO_CAPTURE       0x00 - 0x30
#define XGI_VIDEO_PLAYBACK      0x02 - 0x30
#define XGI_CRT2_PORT_04        0x04 - 0x30
#define XGI_CRT2_PORT_10        0x10 - 0x30
#define XGI_CRT2_PORT_12        0x12 - 0x30
#define XGI_CRT2_PORT_14        0x14 - 0x30

#define ADR_CRT2PtrData         0x20E
#define offset_Zurac            0x210   /* TW: Trumpion Zurac data pointer */
#define ADR_LVDSDesPtrData      0x212
#define ADR_LVDSCRT1DataPtr     0x214
#define ADR_CHTVVCLKPtr         0x216
#define ADR_CHTVRegDataPtr      0x218

#define LVDSDataLen             6
/*#define LVDSDesDataLen          3 */
#define ActiveNonExpanding      0x40
#define ActiveNonExpandingShift 6
#define ActivePAL               0x20
#define ActivePALShift          5
/*#define ModeSwitchStatus        0x0F */
#define SoftTVType              0x40
#define SoftSettingAddr         0x52
#define ModeSettingAddr         0x53

#define _PanelType00             0x00
#define _PanelType01             0x08
#define _PanelType02             0x10
#define _PanelType03             0x18
#define _PanelType04             0x20
#define _PanelType05             0x28
#define _PanelType06             0x30
#define _PanelType07             0x38
#define _PanelType08             0x40
#define _PanelType09             0x48
#define _PanelType0A             0x50
#define _PanelType0B             0x58
#define _PanelType0C             0x60
#define _PanelType0D             0x68
#define _PanelType0E             0x70
#define _PanelType0F             0x78

#define PRIMARY_VGA       	0      /* 1: XGI is primary vga 0:XGI is secondary vga */

#define BIOSIDCodeAddr          0x235  /* Offsets to ptrs in BIOS image */
#define OEMUtilIDCodeAddr       0x237
#define VBModeIDTableAddr       0x239
#define OEMTVPtrAddr            0x241
#define PhaseTableAddr          0x243
#define NTSCFilterTableAddr     0x245
#define PALFilterTableAddr      0x247
#define OEMLCDPtr_1Addr         0x249
#define OEMLCDPtr_2Addr         0x24B
#define LCDHPosTable_1Addr      0x24D
#define LCDHPosTable_2Addr      0x24F
#define LCDVPosTable_1Addr      0x251
#define LCDVPosTable_2Addr      0x253
#define OEMLCDPIDTableAddr      0x255

#define VBModeStructSize        5
#define PhaseTableSize          4
#define FilterTableSize         4
#define LCDHPosTableSize        7
#define LCDVPosTableSize        5
#define OEMLVDSPIDTableSize     4
#define LVDSHPosTableSize       4
#define LVDSVPosTableSize       6

#define VB_ModeID               0
#define VB_TVTableIndex         1
#define VB_LCDTableIndex        2
#define VB_LCDHIndex            3
#define VB_LCDVIndex            4

#define OEMLCDEnable            0x0001
#define OEMLCDDelayEnable       0x0002
#define OEMLCDPOSEnable         0x0004
#define OEMTVEnable             0x0100
#define OEMTVDelayEnable        0x0200
#define OEMTVFlickerEnable      0x0400
#define OEMTVPhaseEnable        0x0800
#define OEMTVFilterEnable       0x1000

#define OEMLCDPanelIDSupport    0x0080

/*
  =============================================================
   			  for 315 series
  =============================================================
*/
#define SoftDRAMType        0x80
#define SoftSetting_OFFSET  0x52
#define SR07_OFFSET  0x7C
#define SR15_OFFSET  0x7D
#define SR16_OFFSET  0x81
#define SR17_OFFSET  0x85
#define SR19_OFFSET  0x8D
#define SR1F_OFFSET  0x99
#define SR21_OFFSET  0x9A
#define SR22_OFFSET  0x9B
#define SR23_OFFSET  0x9C
#define SR24_OFFSET  0x9D
#define SR25_OFFSET  0x9E
#define SR31_OFFSET  0x9F
#define SR32_OFFSET  0xA0
#define SR33_OFFSET  0xA1

#define CR40_OFFSET  0xA2
#define SR25_1_OFFSET  0xF6
#define CR49_OFFSET  0xF7

#define VB310Data_1_2_Offset  0xB6
#define VB310Data_4_D_Offset  0xB7
#define VB310Data_4_E_Offset  0xB8
#define VB310Data_4_10_Offset 0xBB

#define RGBSenseDataOffset    0xBD
#define YCSenseDataOffset     0xBF
#define VideoSenseDataOffset  0xC1
#define OutputSelectOffset    0xF3

#define ECLK_MCLK_DISTANCE  0x14

#endif
