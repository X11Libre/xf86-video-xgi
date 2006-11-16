/*
 * 2D acceleration for 5597/5598 and 6326
 * Definitions for the XGI engine communication
 *
 * Copyright (C) 1998, 1999 by Alan Hourihane, Wigan, England.
 * Parts Copyright (C) 2001-2004 Thomas Winischhofer, Vienna, Austria.
 *
 * Licensed under the following terms:
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appears in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * and that the name of the copyright holder not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission. The copyright holder makes no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as is" without expressed or implied warranty.
 *
 * THE COPYRIGHT HOLDER DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO
 * EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * Authors:  Alan Hourihane <alanh@fairlite.demon.co.uk>,
 *           Mike Chapman <mike@paranoia.com>,
 *           Juanjo Santamarta <santamarta@ctv.es>,
 *           Mitani Hiroshi <hmitani@drl.mei.co.jp>,
 *           David Thomas <davtom@dream.org.uk>,
 *	     Thomas Winischhofer <thomas@winischhofer.net>.
 */

#ifndef _XGI_315_ACCEL_H_
#define _XGI_315_ACCEL_H_
/* Definitions for the XGI engine communication. */

#define XGI315_HCSIZE  (16*1024)

#define PATREGSIZE 256

#define XGIG2_SCR2SCRCOPY
#define XGIG2_SOLIDFILL
#define XGIG2_8X8MONOPATFILL


/*
#define XGIG2_COLOREXPSCANLN
#define XGIG2_SOLIDLINE
#define XGIG2_DASHEDLINE
#define XGIG2_S2SCOLOREXPANFILL
#define XGIG2_8X8COLORPATFILL
#define XGIG2_C2SCOLOREXPANFILL
#define XGIG2_IMAGEWRITE
#define XGIG2_COLOREXPSCANLN
#define XGIG2_IMAGEWRITE
*/

#undef BR
#undef PBR
#define BR(x)   (0x8200 | (x) << 2)
#define PBR(x)  (0x8300 | (x) << 2)

/* Definitions for the engine command */
#define BITBLT                  0x00000000
#define COLOREXP                0x00000001
#define ENCOLOREXP              0x00000002
#define MULTIPLE_SCANLINE       0x00000003
#define LINE                    0x00000004
#define TRAPAZOID_FILL          0x00000005
#define TRANSPARENT_BITBLT      0x00000006

#define SRCVIDEO                0x00000000
#define SRCSYSTEM               0x00000010
#define SRCAGP                  0x00000020

#define PATFG                   0x00000000
#define PATPATREG               0x00000040
#define PATMONO                 0x00000080

#define X_INC                   0x00010000
#define X_DEC                   0x00000000
#define Y_INC                   0x00020000
#define Y_DEC                   0x00000000

#define NOCLIP                  0x00000000
#define NOMERGECLIP             0x04000000
#define CLIPENABLE              0x00040000
#define CLIPWITHOUTMERGE        0x04040000

#define OPAQUE                  0x00000000
#define TRANSPARENT             0x00100000

#define DSTAGP                  0x02000000
#define DSTVIDEO                0x02000000

/* Line */
#define LINE_STYLE              0x00800000
#define NO_RESET_COUNTER        0x00400000
#define NO_LAST_PIXEL           0x00200000

/* Macros to do useful things with the XGI BitBLT engine */


/*
   bit 31 2D engine: 1 is idle,
   bit 30 3D engine: 1 is idle,
   bit 29 Command queue: 1 is empty
*/

#define GR_SKPC_HEADER          0x16800000L
#define GR_NIL_CMD              0x168F0000L

#define Dst_Hight		0xFFF

#define BandSize 		0x10

/* typedef unsigned long ulong ; */

unsigned long    r_port, w_port ;

int     G2CmdQueLen;
int 	Alignment ;
/* int    GBCount = 0 ; */

/* Jong Lin 08-26-2005 */
extern unsigned int CurrentHDisplay;
extern unsigned int CurrentVDisplay;
extern unsigned int CurrentColorDepth;

/***********************************************************************
 *#define Volari_Idle \
 *  while( (MMIO_IN16(pXGI->IOBase, BR(16)+2) & 0xE000) != 0xE000){}; \
 *  while( (MMIO_IN16(pXGI->IOBase, BR(16)+2) & 0xE000) != 0xE000){}; \
 *  G2CmdQueLen=MMIO_IN16(pXGI->IOBase, 0x8240);
 ***********************************************************************/

/* Jong; 08222005 */
/* typedef __time_t long;*/ /* time_t;*/
#if 0
struct timespec
{ 
    int tv_sec;            /* Seconds.  */
    long int tv_nsec;           /* Nanoseconds.  */
};
#endif

/*
typedef struct timespec_ {
long tv_sec;
long tv_nsec;
} timespec;

int nanosleep(const struct timespec_ *, struct timespec_ *);
*/

/* #include <time.h> */

/*#if 0*/
#define Volari_Idle \
    {\
        ulong ulTemp ;\
\
	bool bIdle;\
	int nRet, i; \
	unsigned int WaitCount;\
/*	struct timespec rqtp, rmtp;*/ \
/*	rqtp.tv_sec=0;*/\
/*	rqtp.tv_nsec=1;*/\
\
	WaitCount=65535;\
	if(pXGI->Chipset == PCI_CHIP_XGIXG20)\
	{\
		WaitCount=1;\
		switch(CurrentHDisplay)\
		{\
			case 640:\
			if(CurrentColorDepth == 8)\
				WaitCount=1;\
			else if(CurrentColorDepth == 16)\
				WaitCount=1000;\
			else if(CurrentColorDepth == 24)\
				WaitCount=3000;\
			break;\
\
			case 800:\
			if(CurrentColorDepth == 8)\
				WaitCount=160;\
			else if(CurrentColorDepth == 16)\
				WaitCount=1200;\
			else if(CurrentColorDepth == 24)\
				WaitCount=4000;\
			break;\
\
			case 1024:\
			if(CurrentColorDepth == 8)\
				WaitCount=200;\
			else if(CurrentColorDepth == 16)\
				WaitCount=1600;\
			else if(CurrentColorDepth == 24)\
				WaitCount=6000;\
			break;\
\
			case 1280:\
			if(CurrentColorDepth == 8)\
				WaitCount=500;\
			else if(CurrentColorDepth == 16)\
				WaitCount=2000;\
			else if(CurrentColorDepth == 24)\
				WaitCount=8000;\
			break;\
\
			default:\
			break;\
		}\
	}\
\
        do\
        {\
/*            ulTemp = MMIO_IN32(pXGI->IOBase, 0x85CC) ;*/ \
\
	    bIdle=0;\
	    for (i=0; i<WaitCount; i++)\
	    {\
                ulTemp = MMIO_IN32(pXGI->IOBase, 0x85CC) ;\
  	    	if( ulTemp & 0x80000000 )\
             	{\
			bIdle=1;\
                	break ;\
             	}\
	    }\
	    if(bIdle == 1) break;\
\
	    /* rqtp.tv_sec=0;*/\
	    /* rqtp.tv_nsec=1;*/\
	    /* nRet=nanosleep(&rqtp,&rmtp);*/\  
\
	   if(pXGI->Chipset == PCI_CHIP_XGIXG20)\
	      usleep(1);\
\
           /* sleep(0);*/\
        }while(1) ;\
\
    }
/*#endif*/

#if 0
#define Volari_Idle \
    {\
        ulong ulTemp ;\
\
        do \
        {\
            ulTemp = MMIO_IN32(pXGI->IOBase, 0x85CC) ;\
            if( ulTemp & 0x80000000 )\
            {\
                break ;\
            }\
            usleep(50);\
            /* usleep(50);*/ \
        }while(1) ;\
\
        do \
        {\
            ulTemp = MMIO_IN32(pXGI->IOBase, 0x85CC) ;\
            if( ulTemp & 0x80000000 )\
            {\
                break ;\
            }\
           usleep(50);\
           /* usleep(50);*/ \
        }while(1) ;\
\
        do \
        {\
            ulTemp = MMIO_IN32(pXGI->IOBase, 0x85CC) ;\
            if( ulTemp & 0x80000000 )\
            {\
                break ;\
            }\
        }while(1) ;\
\
        do \
        {\
            ulTemp = MMIO_IN32(pXGI->IOBase, 0x85CC) ;\
            if( ulTemp & 0x80000000 )\
            {\
                break ;\
            }\
        }while(1) ;\
\
    }

#endif

#define Volari_GetSwWP() (unsigned long)(*(pXGI->pCQ_shareWritePort))
#define Volari_GetHwRP() (unsigned long)(MMIO_IN32(pXGI->IOBase, 0x85c8))

#define Volari_SyncWP\
    {\
        unsigned long p ;\
        \
        p = *(pXGI->pCQ_shareWritePort);\
        PDEBUG4(ErrorF("Volari_SyncWP(%08lx)\n",(p)));\
        MMIO_OUT32(pXGI->IOBase, 0x85c4,(p)) ;\
    }

#define Volari_UpdateHwWP(p)\
    {\
        PDEBUG4(ErrorF("Volari_UpdateHwWP(%08lx)\n",(p)));\
        *(pXGI->pCQ_shareWritePort) = (p) ;\
        MMIO_OUT32(pXGI->IOBase, 0x85c4, (p)) ;\
    }

#define Volari_UpdateSwWP(p)\
    {\
        PDEBUG4(ErrorF("Volari_UpdateSwWP(%08lx)\n",(p)));\
        *(pXGI->pCQ_shareWritePort) = (p) ;\
    }


#define Volari_ResetCmd \
    pXGI->CommandReg = 0 ;

#define Volari_SetupROP(rop) \
    pXGI->CommandReg |= (rop) << 8;

#define Volari_SetupCMDFlag(flags) \
    pXGI->CommandReg |= (flags);

#if 1
#define GuardBand(CS)\
{\
	unsigned long lTemp ;\
	int waitCount = 0, maxCount = 1000 ;\
	w_port = Volari_GetSwWP() ;\
	lTemp = (r_port - w_port - BandSize) & pXGI->cmdQueueSizeMask ;\
	if( lTemp < (CS) )\
	{\
		Volari_SyncWP;\
		w_port = Volari_GetSwWP() ;\
		do{\
			waitCount++ ;\
                	r_port = Volari_GetHwRP(); \
			lTemp = (r_port - w_port - BandSize)&pXGI->cmdQueueSizeMask ;\
			if( lTemp >= (CS) ) break ;\
			if( waitCount > maxCount ){\
				waitCount = 0 ;\
				maxCount /= 10 ; maxCount *= 2;\
				usleep(1) ;\
			}\
		}while(lTemp < (CS)) ;\
	}\
}
#else
#define GuardBand(CS)\
    {\
        unsigned long lTemp ; \
        \
        w_port = Volari_GetSwWP();  \
        lTemp = r_port - w_port + pXGI->cmdQueueSize ; \
        \
        if( ((lTemp & pXGI->cmdQueueSizeMask ) < (BandSize + CS)) && ( r_port != w_port ) ) \
        { \
            while ( 1 ) \
            { \
                r_port = Volari_GetHwRP(); \
                if(r_port == w_port) \
                    break; \
                lTemp = r_port - w_port + pXGI->cmdQueueSize ; \
                if( (lTemp & pXGI->cmdQueueSizeMask ) >= (BandSize + CS) ) \
                    break; \
                usleep(1);/*usleep(100);*//* Jong Lin 08-29-2005 */\
            } \
        } \
    }
#endif

/*
#define GuardBand(CS)\
    {\
        long lTemp ; \
        w_port = Volari_GetSwWP();  \
        while ( 1 ) \
        { \
            r_port = Volari_GetHwRP(); \
            if(r_port == w_port)\
                break; \
            lTemp = r_port - w_port + pXGI->cmdQueueSize ; \
            if( (lTemp & pXGI->cmdQueueSizeMask ) >= (BandSize + CS) ) \
                break; \
        }\
   }
*/

#define Volari_DoCMD\
    {\
        unsigned long ulTemp ;\
        \
        PDEBUG4(ErrorF("pXGI->CommandReg = %08lX\n", pXGI->CommandReg));\
        \
        ulTemp = Volari_GetSwWP() ;\
        \
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp) = \
            (CARD32)BE_SWAP32(GR_SKPC_HEADER + 0x823C) ;\
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+4) =\
            (CARD32)BE_SWAP32(pXGI->CommandReg) ;\
        if( pXGI->Chipset == PCI_CHIP_XGIXG40 ) \
        { \
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+8) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+12) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            ulTemp += 0x10 ;\
        } \
        else if( pXGI->Chipset == PCI_CHIP_XGIXG20 ) \
            ulTemp += 0x08 ;\
        ulTemp &= pXGI->cmdQueueSizeMask ;\
        Volari_UpdateHwWP(ulTemp) ;\
        \
    }

/**********************************************************************
#define Volari_SetupSRCBase(base) \
                if (G2CmdQueLen <= 0)  Volari_Idle;\
                MMIO_OUT32(pXGI->IOBase, BR(0), base);\
                G2CmdQueLen --;
 **********************************************************************/

#define Volari_SetupSRCBase(base) \
    {\
        unsigned long ulTemp ;\
        \
        ulTemp = Volari_GetSwWP() ;\
        \
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp) = \
            (CARD32)BE_SWAP32(GR_SKPC_HEADER + 0x8200) ;\
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+4) =\
            (CARD32)BE_SWAP32(base) ;\
        if( pXGI->Chipset == PCI_CHIP_XGIXG40 ) \
        { \
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+8) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+12) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            ulTemp += 0x10 ;\
        } \
        else if( pXGI->Chipset == PCI_CHIP_XGIXG20 ) \
            ulTemp += 0x08 ;\
        ulTemp &= pXGI->cmdQueueSizeMask ;\
        Volari_UpdateSwWP(ulTemp) ;\
        \
    }


/***********************************************************************
#define Volari_SetupSRCPitch(pitch) \
                if (G2CmdQueLen <= 0)  Volari_Idle;\
                MMIO_OUT16(pXGI->IOBase, BR(1), pitch);\
                G2CmdQueLen --;

***********************************************************************/

#define Volari_SetupSRCPitch(pitch) \
        {\
        unsigned long ulTemp ;\
        \
        ulTemp = Volari_GetSwWP() ;\
        \
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp) = \
            (CARD32)BE_SWAP32(GR_SKPC_HEADER + 0x8204) ;\
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+4) =\
            (CARD32)BE_SWAP32(pitch) ;\
        if( pXGI->Chipset == PCI_CHIP_XGIXG40 ) \
        { \
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+8) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+12) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            ulTemp += 0x10 ;\
        } \
        else if( pXGI->Chipset == PCI_CHIP_XGIXG20 ) \
            ulTemp += 0x08 ;\
        ulTemp &= pXGI->cmdQueueSizeMask ;\
        Volari_UpdateSwWP(ulTemp) ;\
        \
    }

/***********************************************************************
#define Volari_SetupSRCXY(x,y) \
                if (G2CmdQueLen <= 0)  Volari_Idle;\
                MMIO_OUT32(pXGI->IOBase, BR(2), (x)<<16 | (y) );\
                G2CmdQueLen --;
***********************************************************************/

#define Volari_SetupSRCXY(x,y) \
        {\
        unsigned long ulTemp ;\
        \
        ulTemp = Volari_GetSwWP() ;\
        \
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp) = \
            (CARD32)BE_SWAP32(GR_SKPC_HEADER + 0x8208) ;\
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+4) =\
            (CARD32)BE_SWAP32((x<<16)+(y&0xFFFF)) ;\
        if( pXGI->Chipset == PCI_CHIP_XGIXG40 ) \
        { \
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+8) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+12) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            ulTemp += 0x10 ;\
        } \
        else if( pXGI->Chipset == PCI_CHIP_XGIXG20 ) \
            ulTemp += 0x08 ;\
        ulTemp &= pXGI->cmdQueueSizeMask ;\
        Volari_UpdateSwWP(ulTemp) ;\
        \
    }

/***********************************************************************
#define Volari_SetupDSTBase(base) \
                if (G2CmdQueLen <= 0)  Volari_Idle;\
                MMIO_OUT32(pXGI->IOBase, BR(4), base);\
                G2CmdQueLen --;

***********************************************************************/

#define Volari_SetupDSTBase(base) \
        {\
        unsigned long ulTemp ;\
        \
        ulTemp = Volari_GetSwWP() ;\
        \
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp) = \
            (CARD32)BE_SWAP32(GR_SKPC_HEADER + 0x8210) ;\
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+4) =\
            (CARD32)BE_SWAP32(base) ;\
        if( pXGI->Chipset == PCI_CHIP_XGIXG40 ) \
        { \
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+8) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+12) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            ulTemp += 0x10 ;\
        } \
        else if( pXGI->Chipset == PCI_CHIP_XGIXG20 ) \
            ulTemp += 0x08 ;\
        ulTemp &= pXGI->cmdQueueSizeMask ;\
        Volari_UpdateSwWP(ulTemp) ;\
        \
    }

/***********************************************************************
#define Volari_SetupDSTXY(x,y) \
                if (G2CmdQueLen <= 0)  Volari_Idle;\
                MMIO_OUT32(pXGI->IOBase, BR(3), (x)<<16 | (y) );\
                G2CmdQueLen --;

***********************************************************************/

#define Volari_SetupDSTXY(x,y) \
        {\
        unsigned long ulTemp ;\
        \
        ulTemp = Volari_GetSwWP() ;\
        \
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp) = \
            (CARD32)BE_SWAP32(GR_SKPC_HEADER + 0x820C) ;\
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+4) =\
            (CARD32)BE_SWAP32(((x)<<16)+((y)&0xFFFF)) ;\
        if( pXGI->Chipset == PCI_CHIP_XGIXG40 ) \
        { \
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+8) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+12) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            ulTemp += 0x10 ;\
        } \
        else if( pXGI->Chipset == PCI_CHIP_XGIXG20 ) \
            ulTemp += 0x08 ;\
        ulTemp &= pXGI->cmdQueueSizeMask ;\
        Volari_UpdateSwWP(ulTemp) ;\
        \
    }

/***********************************************************************
#define Volari_SetupDSTRect(x,y) \
                if (G2CmdQueLen <= 0)  Volari_Idle;\
                MMIO_OUT32(pXGI->IOBase, BR(5), (y)<<16 | (x) );\
                G2CmdQueLen --;

***********************************************************************/

#define Volari_SetupDSTRect(x,y) \
        {\
        unsigned long ulTemp ;\
        \
        ulTemp = Volari_GetSwWP() ;\
        \
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp) = \
            (CARD32)BE_SWAP32(GR_SKPC_HEADER + 0x8214) ;\
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+4) =\
            (CARD32)BE_SWAP32(((y)<<16)|((x)&0xFFFF)) ;\
        if( pXGI->Chipset == PCI_CHIP_XGIXG40 ) \
        { \
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+8) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+12) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            ulTemp += 0x10 ;\
        } \
        else if( pXGI->Chipset == PCI_CHIP_XGIXG20 ) \
            ulTemp += 0x08 ;\
        ulTemp &= pXGI->cmdQueueSizeMask ;\
        Volari_UpdateSwWP(ulTemp) ;\
	Volari_UpdateHwWP(ulTemp) ;\
        \
    }

/***********************************************************************
#define Volari_SetupDSTColorDepth(bpp) \
                if (G2CmdQueLen <= 0)  Volari_Idle;\
                MMIO_OUT16(pXGI->IOBase, BR(1)+2, bpp);\
                G2CmdQueLen --;
***********************************************************************/

#define Volari_SetupDSTColorDepth(bpp) \
    pXGI->CommandReg |= ((ulong)(bpp))&(GENMASK(17:16)) ;

/***********************************************************************
#define Volari_SetupRect(w,h) \
                if (G2CmdQueLen <= 0)  Volari_Idle;\
                MMIO_OUT32(pXGI->IOBase, BR(6), (h)<<16 | (w) );\
                G2CmdQueLen --;
***********************************************************************/

#define Volari_SetupRect(w,h) \
        {\
        unsigned long ulTemp ;\
        \
        ulTemp = Volari_GetSwWP() ;\
        \
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp) = \
            (CARD32)BE_SWAP32(GR_SKPC_HEADER + 0x8218) ;\
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+4) =\
            (CARD32)BE_SWAP32(((h)<<16)|((w)&0xffff)) ;\
        if( pXGI->Chipset == PCI_CHIP_XGIXG40 ) \
        { \
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+8) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+12) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            ulTemp += 0x10 ;\
        } \
        else if( pXGI->Chipset == PCI_CHIP_XGIXG20 ) \
            ulTemp += 0x08 ;\
        ulTemp &= pXGI->cmdQueueSizeMask ;\
        Volari_UpdateSwWP(ulTemp) ;\
        \
    }

/***********************************************************************
#define Volari_SetupPATFG(color) \
                if (G2CmdQueLen <= 0)  Volari_Idle;\
                MMIO_OUT32(pXGI->IOBase, BR(7), color);\
                G2CmdQueLen --;
***********************************************************************/
/***********************************************************************
#define Volari_SetupPATBG(color) \
                if (G2CmdQueLen <= 0)  Volari_Idle;\
                MMIO_OUT32(pXGI->IOBase, BR(8), color);\
                G2CmdQueLen --;
***********************************************************************/
/***********************************************************************
#define Volari_SetupSRCFG(color) \
                if (G2CmdQueLen <= 0)  Volari_Idle;\
                MMIO_OUT32(pXGI->IOBase, BR(9), color);\
                G2CmdQueLen --;
***********************************************************************/
/***********************************************************************
#define Volari_SetupSRCBG(color) \
                if (G2CmdQueLen <= 0)  Volari_Idle;\
                MMIO_OUT32(pXGI->IOBase, BR(10), color);\
                G2CmdQueLen --;
***********************************************************************/

#define Volari_SetupPATFG(color) \
        {\
        unsigned long ulTemp ;\
        \
        ulTemp = Volari_GetSwWP() ;\
        \
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp) = \
            (CARD32)BE_SWAP32(GR_SKPC_HEADER + BR(7)) ;\
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+4) =\
            (CARD32)BE_SWAP32(color) ;\
        if( pXGI->Chipset == PCI_CHIP_XGIXG40 ) \
        { \
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+8) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+12) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            ulTemp += 0x10 ;\
        } \
        else if( pXGI->Chipset == PCI_CHIP_XGIXG20 ) \
            ulTemp += 0x08 ;\
        ulTemp &= pXGI->cmdQueueSizeMask ;\
        Volari_UpdateSwWP(ulTemp) ;\
	Volari_UpdateHwWP(ulTemp) ;\
        \
    }

#define Volari_SetupPATBG(color) \
        {\
        unsigned long ulTemp ;\
        \
        ulTemp = Volari_GetSwWP() ;\
        \
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp) = \
            (CARD32)BE_SWAP32(GR_SKPC_HEADER + BR(8)) ;\
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+4) =\
            (CARD32)BE_SWAP32(color) ;\
        if( pXGI->Chipset == PCI_CHIP_XGIXG40 ) \
        { \
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+8) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+12) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            ulTemp += 0x10 ;\
        } \
        else if( pXGI->Chipset == PCI_CHIP_XGIXG20 ) \
            ulTemp += 0x08 ;\
        ulTemp &= pXGI->cmdQueueSizeMask ;\
        Volari_UpdateSwWP(ulTemp) ;\
        \
    }

#define Volari_SetupSRCFG(color) \
        {\
        unsigned long ulTemp ;\
        \
        ulTemp = Volari_GetSwWP() ;\
        \
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp) = \
            (CARD32)BE_SWAP32(GR_SKPC_HEADER + BR(9)) ;\
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+4) =\
            (CARD32)BE_SWAP32(color) ;\
        if( pXGI->Chipset == PCI_CHIP_XGIXG40 ) \
        { \
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+8) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+12) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            ulTemp += 0x10 ;\
        } \
        else if( pXGI->Chipset == PCI_CHIP_XGIXG20 ) \
            ulTemp += 0x08 ;\
        ulTemp &= pXGI->cmdQueueSizeMask ;\
        Volari_UpdateSwWP(ulTemp) ;\
        \
    }

#define Volari_SetupSRCBG(color) \
        {\
        unsigned long ulTemp ;\
        \
        ulTemp = Volari_GetSwWP() ;\
        \
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp) = \
            (CARD32)BE_SWAP32(GR_SKPC_HEADER + BR(10)) ;\
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+4) =\
            (CARD32)BE_SWAP32(color) ;\
        if( pXGI->Chipset == PCI_CHIP_XGIXG40 ) \
        { \
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+8) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+12) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            ulTemp += 0x10 ;\
        } \
        else if( pXGI->Chipset == PCI_CHIP_XGIXG20 ) \
            ulTemp += 0x08 ;\
        ulTemp &= pXGI->cmdQueueSizeMask ;\
        Volari_UpdateSwWP(ulTemp) ;\
        \
    }

/***********************************************************************
#define Volari_SetupMONOPAT(p0,p1) \
                if (G2CmdQueLen <= 1)  Volari_Idle;\
                MMIO_OUT32(pXGI->IOBase, BR(11), p0);\
                MMIO_OUT32(pXGI->IOBase, BR(12), p1);\
                G2CmdQueLen =G2CmdQueLen-2;
***********************************************************************/

#define Volari_SetupMONOPAT0(p0) \
        {\
        unsigned long ulTemp ;\
        \
        ulTemp = Volari_GetSwWP() ;\
        \
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp) = \
            (CARD32)BE_SWAP32(GR_SKPC_HEADER + 0x822C) ;\
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+4) =\
            (CARD32)BE_SWAP32(p0) ;\
        if( pXGI->Chipset == PCI_CHIP_XGIXG40 ) \
        { \
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+8) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+12) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            ulTemp += 0x10 ;\
        } \
        else if( pXGI->Chipset == PCI_CHIP_XGIXG20 ) \
            ulTemp += 0x08 ;\
        ulTemp &= pXGI->cmdQueueSizeMask ;\
        Volari_UpdateSwWP(ulTemp) ;\
        \
    }

#define Volari_SetupMONOPAT1(p1) \
        {\
        unsigned long ulTemp ;\
        \
        ulTemp = Volari_GetSwWP() ;\
        \
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp) = \
            (CARD32)BE_SWAP32(GR_SKPC_HEADER + 0x8230) ;\
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+4) =\
            (CARD32)BE_SWAP32(p1) ;\
        if( pXGI->Chipset == PCI_CHIP_XGIXG40 ) \
        { \
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+8) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+12) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            ulTemp += 0x10 ;\
        } \
        else if( pXGI->Chipset == PCI_CHIP_XGIXG20 ) \
            ulTemp += 0x08 ;\
        ulTemp &= pXGI->cmdQueueSizeMask ;\
        Volari_UpdateSwWP(ulTemp) ;\
        \
    }
/***********************************************************************
#define Volari_SetupClipLT(left,top) \
                if (G2CmdQueLen <= 0)  Volari_Idle;\
                MMIO_OUT32(pXGI->IOBase, BR(13), ((left) & 0xFFFF) | (top)<<16 );\
                G2CmdQueLen--;
***********************************************************************/
/***********************************************************************
#define Volari_SetupClipRB(right,bottom) \
                if (G2CmdQueLen <= 0)  Volari_Idle;\
                MMIO_OUT32(pXGI->IOBase, BR(14), ((right) & 0xFFFF) | (bottom)<<16 );\
                G2CmdQueLen --;
***********************************************************************/

#define Volari_SetupClip(left,top,right,bottom) \
        {\
        unsigned long ulTemp ;\
        \
        ulTemp = Volari_GetSwWP() ;\
        \
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp) = \
            (CARD32)BE_SWAP32(GR_SKPC_HEADER + BR(13)) ;\
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+4) =\
            (CARD32)BE_SWAP32(((top)<<16)|((left)&0xffff)) ;\
        if( pXGI->Chipset == PCI_CHIP_XGIXG40 ) \
        { \
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+8) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+12) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            ulTemp += 0x10 ;\
        } \
        else if( pXGI->Chipset == PCI_CHIP_XGIXG20 ) \
            ulTemp += 0x08 ;\
        ulTemp &= pXGI->cmdQueueSizeMask ;\
        Volari_UpdateSwWP(ulTemp) ;\
        \
    }

#define Volari_SetupClipLT(left,top) \
        {\
        unsigned long ulTemp ;\
        \
        ulTemp = Volari_GetSwWP() ;\
        \
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp) = \
            (CARD32)BE_SWAP32(GR_SKPC_HEADER + BR(13)) ;\
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+4) =\
            (CARD32)BE_SWAP32(((top)<<16)|((left)&0xffff)) ;\
        if( pXGI->Chipset == PCI_CHIP_XGIXG40 ) \
        { \
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+8) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+12) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            ulTemp += 0x10 ;\
        } \
        else if( pXGI->Chipset == PCI_CHIP_XGIXG20 ) \
            ulTemp += 0x08 ;\
        ulTemp &= pXGI->cmdQueueSizeMask ;\
        Volari_UpdateSwWP(ulTemp) ;\
        \
    }

#define Volari_SetupClipRB(right,bottom) \
    {\
        unsigned long ulTemp ;\
        \
        ulTemp = Volari_GetSwWP() ;\
        \
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp) = \
            (CARD32)BE_SWAP32(GR_SKPC_HEADER + BR(14)) ;\
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+4) =\
            (CARD32)BE_SWAP32(((right) & 0xFFFF) | ((bottom)<<16))) ;\
        if( pXGI->Chipset == PCI_CHIP_XGIXG40 ) \
        { \
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+8) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+12) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            ulTemp += 0x10 ;\
        } \
        else if( pXGI->Chipset == PCI_CHIP_XGIXG20 ) \
            ulTemp += 0x08 ;\
        ulTemp &= pXGI->cmdQueueSizeMask ;\
        Volari_UpdateSwWP(ulTemp) ;\
        \
    }

/***********************************************************************
#define Volari_SetupROP(rop) \
        pXGI->CommandReg = (rop) << 8;

#define Volari_SetupCMDFlag(flags) \
        pXGI->CommandReg |= (flags);

#define Volari_DoCMD \
                if (G2CmdQueLen <= 1)  Volari_Idle;\
                MMIO_OUT32(pXGI->IOBase, BR(15), pXGI->CommandReg); \
                MMIO_OUT32(pXGI->IOBase, BR(16), 0);\
                G2CmdQueLen =G2CmdQueLen-2;
***********************************************************************/

/***********************************************************************
#define Volari_SetupX0Y0(x,y) \
                if (G2CmdQueLen <= 0)  Volari_Idle;\
                MMIO_OUT32(pXGI->IOBase, BR(2), (y)<<16 | (x) );\
                G2CmdQueLen --;
#define Volari_SetupX1Y1(x,y) \
                if (G2CmdQueLen <= 0)  Volari_Idle;\
                MMIO_OUT32(pXGI->IOBase, BR(3), (y)<<16 | (x) );\
                G2CmdQueLen --;
#define Volari_SetupLineCount(c) \
                if (G2CmdQueLen <= 0)  Volari_Idle;\
                MMIO_OUT16(pXGI->IOBase, BR(6), c);\
                G2CmdQueLen --;
#define Volari_SetupStylePeriod(p) \
                if (G2CmdQueLen <= 0)  Volari_Idle;\
                MMIO_OUT16(pXGI->IOBase, BR(6)+2, p);\
                G2CmdQueLen --;
#define Volari_SetupStyleLow(ls) \
                if (G2CmdQueLen <= 0)  Volari_Idle;\
                MMIO_OUT32(pXGI->IOBase, BR(11), ls);\
                G2CmdQueLen --;
#define Volari_SetupStyleHigh(ls) \
                if (G2CmdQueLen <= 0)  Volari_Idle;\
                MMIO_OUT32(pXGI->IOBase, BR(12), ls);\
                G2CmdQueLen --;
***********************************************************************/


/***********************************************************************
 * For Line Drawing
 ***********************************************************************/

#define Volari_SetupX0Y0(x,y) \
        {\
        unsigned long ulTemp ;\
        \
        ulTemp = Volari_GetSwWP() ;\
        \
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp) = \
            (CARD32)BE_SWAP32(GR_SKPC_HEADER + BR(2)) ;\
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+4) =\
            (CARD32)BE_SWAP32(((y)<<16) | ((x)&0xFFFF) ) ;\
        if( pXGI->Chipset == PCI_CHIP_XGIXG40 ) \
        { \
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+8) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+12) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            ulTemp += 0x10 ;\
        } \
        else if( pXGI->Chipset == PCI_CHIP_XGIXG20 ) \
            ulTemp += 0x08 ;\
        ulTemp &= pXGI->cmdQueueSizeMask ;\
        Volari_UpdateSwWP(ulTemp) ;\
        \
    }

#define Volari_SetupX1Y1(x,y) \
        {\
        unsigned long ulTemp ;\
        \
        ulTemp = Volari_GetSwWP() ;\
        \
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp) = \
            (CARD32)BE_SWAP32(GR_SKPC_HEADER + BR(3)) ;\
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+4) =\
            (CARD32)BE_SWAP32(((y)<<16) | ((x)&0xFFFF) ) ;\
        if( pXGI->Chipset == PCI_CHIP_XGIXG40 ) \
        { \
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+8) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+12) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            ulTemp += 0x10 ;\
        } \
        else if( pXGI->Chipset == PCI_CHIP_XGIXG20 ) \
            ulTemp += 0x08 ;\
        ulTemp &= pXGI->cmdQueueSizeMask ;\
        Volari_UpdateSwWP(ulTemp) ;\
        \
    }

#define Volari_SetupLineCount(c) \
    {\
        unsigned long ulTemp ;\
        \
        ulTemp = Volari_GetSwWP() ;\
        \
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp) = \
            (CARD32)BE_SWAP32(GR_SKPC_HEADER + BR(6) + 0x30000 ) ;\
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+4) =\
            (CARD32)BE_SWAP32(((c)&0xffff)) ;\
        if( pXGI->Chipset == PCI_CHIP_XGIXG40 ) \
        { \
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+8) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+12) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            ulTemp += 0x10 ;\
        } \
        else if( pXGI->Chipset == PCI_CHIP_XGIXG20 ) \
            ulTemp += 0x08 ;\
        ulTemp &= pXGI->cmdQueueSizeMask ;\
        Volari_UpdateSwWP(ulTemp) ;\
        \
    }

#define Volari_SetupStylePeriod(p) \
    {\
        unsigned long ulTemp ;\
        \
        ulTemp = Volari_GetSwWP() ;\
        \
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp) = \
            (CARD32)BE_SWAP32(GR_SKPC_HEADER + BR(6) + 0xC0000 ) ;\
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+4) =\
            (CARD32)BE_SWAP32(((p)<<16)) ;\
        if( pXGI->Chipset == PCI_CHIP_XGIXG40 ) \
        { \
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+8) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+12) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            ulTemp += 0x10 ;\
        } \
        else if( pXGI->Chipset == PCI_CHIP_XGIXG20 ) \
            ulTemp += 0x08 ;\
        ulTemp &= pXGI->cmdQueueSizeMask ;\
        Volari_UpdateSwWP(ulTemp) ;\
        \
    }

#define Volari_SetupStylePeriodCount(p,c) \
        {\
        unsigned long ulTemp ;\
        \
        ulTemp = Volari_GetSwWP() ;\
        \
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp) = \
            (CARD32)BE_SWAP32(GR_SKPC_HEADER + BR(6)) ;\
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+4) =\
            (CARD32)BE_SWAP32(((p)<<16)|((c)&0xffff)) ;\
        if( pXGI->Chipset == PCI_CHIP_XGIXG40 ) \
        { \
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+8) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+12) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            ulTemp += 0x10 ;\
        } \
        else if( pXGI->Chipset == PCI_CHIP_XGIXG20 ) \
            ulTemp += 0x08 ;\
        ulTemp &= pXGI->cmdQueueSizeMask ;\
        Volari_UpdateSwWP(ulTemp) ;\
        \
    }

#define Volari_SetupStyle(ls,hs) \
    {\
        unsigned long ulTemp ;\
        \
        ulTemp = Volari_GetSwWP() ;\
        \
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp) = \
            (CARD32)BE_SWAP32(GR_SKPC_HEADER + BR(11)) ;\
        *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+4) =\
            (CARD32)BE_SWAP32(ls) ;\
        if( pXGI->Chipset == PCI_CHIP_XGIXG40 ) \
        { \
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+8) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            *(CARD32 *)(pXGI->cmdQueueBase+ulTemp+12) = \
                (CARD32)BE_SWAP32(GR_NIL_CMD) ;\
            ulTemp += 0x10 ;\
        } \
        else if( pXGI->Chipset == PCI_CHIP_XGIXG20 ) \
            ulTemp += 0x08 ;\
        ulTemp &= pXGI->cmdQueueSizeMask ;\
        Volari_UpdateSwWP(ulTemp) ;\
        \
    }

#endif /* _XGI_315_ACCEL_H_ */

