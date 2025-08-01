/*
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <unistd.h>

#include "xf86.h"
#include "xf86_OSproc.h"

#include "xf86Pci.h"

#include <compiler.h>
#include <miline.h>

#include "xgi_accel.h"
#include "xgi_regs.h"
#include "xgi.h"
#include "vb_def.h"

#include <xf86fbman.h>

#define DEV_HEIGHT	0xfff	/* "Device height of destination bitmap" */

/* Jong 07/24/2008; performance tuning */
long	g_srcbase=-1;
long	g_dstbase=-1;
int		g_src_x=-1;
int		g_src_y=-1;
int		g_dst_x=-1;
int		g_dst_y=-1;
int		g_width=-1;
int		g_height=-1;

int		g_Clipping_x1 = 0;
int		g_Clipping_y1 = 0;
int		g_Clipping_x2 = 4096;
int		g_Clipping_y2 = 4096;

int     g_LineColor=-1;
int     g_SolidColor=-1;
int     g_Rop=-1;
int		g_MonoPatFgColor=-1;
int		g_MonoPatBgColor=-1;
int		g_MonoPat0=-1;
int		g_MonoPat1=-1;

/* Jong 01/19/2009; for EXA */
int		g_DstRectX = -1; 
int		g_DstRectY = -1; 
int		g_SrcPitch = -1;
long	g_SrcBase = -1;
long	g_DstBase = -1;
int		g_Fg = -1;

/* struct timeval g_t0; */

int	g_trans_color=0;

/*************************************************************************/

void Volari_Sync(ScrnInfoPtr pScrn);

static void Volari_SetupForScreenToScreenCopy(ScrnInfoPtr pScrn,
                                int xdir, int ydir, int rop,
                                unsigned int planemask, int trans_color);
static void Volari_SubsequentScreenToScreenCopy(ScrnInfoPtr pScrn,
                                int x1, int y1, int x2, int y2,
                                int width, int height);
static void Volari_SetupForSolidFill(ScrnInfoPtr pScrn, int color,
                                int rop, unsigned int planemask);
static void Volari_SubsequentSolidFillRect(ScrnInfoPtr pScrn,
                                int x, int y, int w, int h);
static void Volari_SetupForMonoPatternFill(ScrnInfoPtr pScrn,
                                int patx, int paty, int fg, int bg,
                                int rop, unsigned int planemask);
static void Volari_SubsequentMonoPatternFill(ScrnInfoPtr pScrn,
                                int patx, int paty,
                                int x, int y, int w, int h);

static void Volari_SetupForSolidLine(ScrnInfoPtr pScrn, int color, int rop,
			unsigned int planemask);

static void Volari_SubsequentSolidTwoPointLine(ScrnInfoPtr pScrn,
			int x1, int y1, int x2, int y2, int flags);

static void Volari_SubsequentSolidHorzVertLine(ScrnInfoPtr pScrn,
			int x, int y, int len, int dir);

static void Volari_SetupForDashedLine(ScrnInfoPtr pScrn,
			int fg, int bg, int rop, unsigned int planemask,
			int length, unsigned char *pattern);

static void Volari_SubsequentDashedTwoPointLine(ScrnInfoPtr pScrn,
			int x1, int y1, int x2, int y2,
			int flags, int phase);

static void Volari_SetClippingRectangle(ScrnInfoPtr pScrn, int x1, int y1, int x2, int y2);
static void Volari_DisableClipping(ScrnInfoPtr pScrn);

#ifdef XGI_USE_EXA		/* Jong 01/13/2009; support EXA */
void XGIEXASync(ScreenPtr pScreen, int marker);
void XGIScratchSave(ScreenPtr pScreen, ExaOffscreenArea *area);
Bool XGIUploadToScratch(PixmapPtr pSrc, PixmapPtr pDst);

static Bool XGIPrepareSolid(PixmapPtr pPixmap, int alu, Pixel planemask, Pixel fg);
static void XGISolid(PixmapPtr pPixmap, int x1, int y1, int x2, int y2);
static void XGIDoneSolid(PixmapPtr pPixmap);
static Bool XGIPrepareCopy(PixmapPtr pSrcPixmap, PixmapPtr pDstPixmap, int xdir, int ydir,
					int alu, Pixel planemask);
static void XGICopy(PixmapPtr pDstPixmap, int srcX, int srcY, int dstX, int dstY, int width, int height);
static void XGIDoneCopy(PixmapPtr pDstPixmap);

static Bool XGICheckComposite(int op, PicturePtr pSrcPicture, PicturePtr pMaskPicture,
				PicturePtr pDstPicture);
static Bool XGIPrepareComposite(int op, PicturePtr pSrcPicture, PicturePtr pMaskPicture,
				PicturePtr pDstPicture, PixmapPtr pSrc, PixmapPtr pMask, PixmapPtr pDst);
static void XGIComposite(PixmapPtr pDst, int srcX, int srcY, int maskX, int maskY, int dstX, int dstY,
				int width, int height);
static void XGIDoneComposite(PixmapPtr pDst);
#endif /* EXA */

void Volari_EnableAccelerator(ScrnInfoPtr pScrn) ;
static void Volari_InitCmdQueue(ScrnInfoPtr pScrn) ;
static void Volari_DisableDualPipe(ScrnInfoPtr pScrn) ;
static void Volari_DisableCmdQueue(ScrnInfoPtr pScrn) ;

/* Jong 01/07/2008; force to disable 2D */
extern Bool ForceToDisable2DEngine(ScrnInfoPtr pScrn);

extern int FbDevExist;

#define DEBUG_ACCEL_DYNAMICALLY

#if X_BYTE_ORDER == X_BIG_ENDIAN
#define BE_SWAP32(x) lswapl(x)
#else 
static uint32_t BE_SWAP32 (uint32_t val)
{
	PACCELDEBUG(ErrorF("X_LITTLE_ENDIAN...\n"));
	return val;
}
#endif


#ifdef DEBUG
static void dump_cq_read_pointer(unsigned int cqrp)
{
    static const char *const field_name[8] = {
        "all idle",
        "hardware queues empty",
        "2D idle",
        "3D idle",
        "hardware command queue empty",
        "2D queue empty",
        "3D queue empty",
        "software command queue empty",
    };
    unsigned i;

    xf86DrvMsg(0, X_INFO, "IO(0x85CC) = 0x%08x\n", cqrp);
    for (i = 31; i > 23; i--) {
        if ((cqrp & (1U << i)) != 0) {
            xf86DrvMsg(0, X_INFO, "    %s\n", field_name[31 - i]);
        }
    }
}
#endif /* DEBUG */


void Volari_SetDefaultIdleWait(XGIPtr pXGI, unsigned HDisplay, 
                               unsigned depth)
{
    static const unsigned wait_table[5][4] = {
        {     1,   1,    1,    1 },
        { 65535,   1, 1000, 3000 },
        { 65535, 160, 1200, 4000 },
        { 65535, 200, 1600, 6000 },
        { 65535, 500, 2000, 8000 }
    };

    if (pXGI->Chipset == PCI_CHIP_XGIXG20) {
        unsigned i;

        switch (HDisplay) {
        case 640:  i = 1; break;
        case 800:  i = 2; break;
        case 1024: i = 3; break;
        case 1280: i = 4; break;
        default:   i = 0; break;
        }

        pXGI->idle_wait_count = wait_table[i][3 & (depth / 8)];
    }
    else {
        pXGI->idle_wait_count = 65535;
    }
}

void Volari_Idle(XGIPtr pXGI)
{
    int  i;
#ifdef DEBUG
    unsigned int last_cqrp = 0;
#endif /* DEBUG */

    do {
        int bIdle = 0;
        unsigned int cqrp;

        for (i = 0; i < pXGI->idle_wait_count; i++) {
            cqrp = MMIO_IN32(pXGI->IOBase, 0x85CC);
            if (cqrp & IDLE_ALL) {
                bIdle = 1;
                break;
            }
        }

        if (bIdle)
            break;

#ifdef DEBUG
        if (last_cqrp != cqrp) {
            dump_cq_read_pointer(cqrp);
            last_cqrp = cqrp;
        }

        /* sleep(1); */
#endif /* DEBUG */

        /* if (pXGI->Chipset == PCI_CHIP_XGIXG20)
            usleep(1); */
    } while (1);
}


void
Volari_EnableAccelerator(ScrnInfoPtr pScrn)
{
    XGIPtr  pXGI = XGIPTR(pScrn);

    PDEBUG(ErrorF("Volari_EnableAccelerator()\n")) ;

    switch (pXGI->Chipset) {
		case PCI_CHIP_XGIXG20:
		case PCI_CHIP_XGIXG21: /* Jong 01/07/2008; support new XG21 */
		case PCI_CHIP_XGIXG27:
		case PCI_CHIP_XGIXG40:
		default:
			orXGIIDXREG(XGISR, 0x1E, 
						SR1E_ENABLE_3D_TRANSFORM_ENGINE
						| SR1E_ENABLE_2D
						| SR1E_ENABLE_3D_AGP_VERTEX_FETCH
						| SR1E_ENABLE_3D_COMMAND_PARSER
						| SR1E_ENABLE_3D);

			/* Jong 01/07/2008; force to disable 2D */
			if(pXGI->Chipset == PCI_CHIP_XGIXG21)
			{
				if(ForceToDisable2DEngine(pScrn))
				{
				   andXGIIDXREG(XGISR, 0x1E, 0xBF) ;
				}
			}

			break;
    }


    if( pXGI->TurboQueue )
    {
        Volari_InitCmdQueue(pScrn) ;
    }
}

/* Jong@08252009; reset variables for register */
void ResetVariableFor2DRegister()
{
	g_srcbase=-1;
	g_dstbase=-1;
	g_src_x=-1;
	g_src_y=-1;
	g_dst_x=-1;
	g_dst_y=-1;
	g_width=-1;
	g_height=-1;

	g_Clipping_x1 = 0;
	g_Clipping_y1 = 0;
	g_Clipping_x2 = 4096;
	g_Clipping_y2 = 4096;

	g_LineColor=-1;
	g_SolidColor=-1;
	g_Rop=-1;
	g_MonoPatFgColor=-1;
	g_MonoPatBgColor=-1;
	g_MonoPat0=-1;
	g_MonoPat1=-1;

	/* Jong 01/19/2009; for EXA */
	g_DstRectX = -1; 
	g_DstRectY = -1; 
	g_SrcPitch = -1;
	g_SrcBase = -1;
	g_DstBase = -1;
	g_Fg = -1;

	g_trans_color=0;
}

static void
Volari_InitCmdQueue(ScrnInfoPtr pScrn)
{
    XGIPtr pXGI = XGIPTR(pScrn);
    unsigned long ulXGITempRP ;
    unsigned long ulCR55 ;
    unsigned long ulSR26 ;
    unsigned long temp ;

	ResetVariableFor2DRegister();

    PDEBUG(ErrorF("Volari_InitCmdQueue()\n"));
    PDEBUG(ErrorF( "pXGI->IOBase = 0x%08lX, [%04X] = 0x%08lX\n",(unsigned long)(pXGI->IOBase),0x85c0, XGIMMIOLONG(0x85c0))) ;
    PDEBUG(ErrorF( "pXGI->IOBase = 0x%08lX, [%04X] = 0x%08lX\n",(unsigned long)(pXGI->IOBase),0x85c4, XGIMMIOLONG(0x85c4))) ;
    PDEBUG(ErrorF( "pXGI->IOBase = 0x%08lX, [%04X] = 0x%08lX\n",(unsigned long)(pXGI->IOBase),0x85c8, XGIMMIOLONG(0x85c8))) ;
    PDEBUG(ErrorF( "pXGI->IOBase = 0x%08lX, [%04X] = 0x%08lX\n",(unsigned long)(pXGI->IOBase),0x85cc, XGIMMIOLONG(0x85cc))) ;

    inXGIIDXREG(XGICR, 0x55, ulCR55) ;
    andXGIIDXREG(XGICR, 0x55, 0x33) ;
    orXGIIDXREG(XGISR, 0x26, 1) ;    /* reset cmd queue */

    w_port = Volari_GetSwWP() ;      /* GuardBand()  Init   */
    r_port = Volari_GetHwRP() ;

    if(( pXGI->Chipset == PCI_CHIP_XGIXG20 )||( pXGI->Chipset == PCI_CHIP_XGIXG21 )||( pXGI->Chipset == PCI_CHIP_XGIXG27 ))
    {
        Alignment = 1 ;		/* 64 bits   */

        switch(pXGI->cmdQueueSize)
        {
        case 64*1024:
            ulSR26 = 0x40 + 0x00 ;
            break ;
        case 128*1024:
            ulSR26 = 0x40 + 0x04 ;
            break ;
        default:
            /* reset the command queue information */

            pXGI->cmdQueueSize = 128*1024 ; /* reset the command queue */
            pXGI->cmdQueueSizeMask = pXGI->cmdQueueSize - 1 ;

            /* Jong 09/18/2007; bug fixing for ??? */
            if( FbDevExist && (pXGI->Chipset != PCI_CHIP_XGIXG20 ) && (pXGI->Chipset != PCI_CHIP_XGIXG21 ) && (pXGI->Chipset != PCI_CHIP_XGIXG27 ) )
            {
                if( pScrn->videoRam < 8*1024 )
                {
                    pXGI->cmdQueueOffset = 4*1024*1024 - pXGI->cmdQueueSize ;
                }
		else if( pScrn->videoRam < 16*1024 )
		{
		    pXGI->cmdQueueOffset = 8*1024*1024 - pXGI->cmdQueueSize ;
		}
                else
                {
                    pXGI->cmdQueueOffset = 13*1024*1024 - pXGI->cmdQueueSize ;
                }
            }
            else
            {
                pXGI->cmdQueueOffset = (pScrn->videoRam)*1024 - pXGI->cmdQueueSize ;
            }

            pXGI->cmdQueueLen  = 0 ;
            pXGI->cmdQueueLenMin = 0x200 ;
            pXGI->cmdQueueLenMax = pXGI->cmdQueueSize - pXGI->cmdQueueLenMin ;

            ulSR26 = 0x40 ;
            break ;
        }
    }
    else
    {
        Alignment = 2 ;		/* 128 bits   */

        switch(pXGI->cmdQueueSize)
        {
        case 512*1024:
            ulSR26 = 0x40 + 0x00 ;
            break ;
        case 1024*1024:
            ulSR26 = 0x40 + 0x04 ;
            break ;
        case 2*1024*1024:
            ulSR26 = 0x40 + 0x08 ;
            break ;
        case 4*1024*1024:
            ulSR26 = 0x40 + 0x0C ;
            break ;
        default:
            /* reset the command queue information */

            pXGI->cmdQueueSize = 512*1024 ; /* reset the command queue */
            pXGI->cmdQueueSizeMask = pXGI->cmdQueueSize - 1 ;

            /* Jong 09/18/2007; bug fixing for ??? */
            if( FbDevExist && (pXGI->Chipset != PCI_CHIP_XGIXG20 ) && (pXGI->Chipset != PCI_CHIP_XGIXG21 ) && (pXGI->Chipset != PCI_CHIP_XGIXG27 ))
            {
                if( pScrn->videoRam < 8*1024 )
                {
                    pXGI->cmdQueueOffset = 4*1024*1024 - pXGI->cmdQueueSize ;
                }
		else if( pScrn->videoRam < 16*1024 )
		{
		    pXGI->cmdQueueOffset = 8*1024*1024 - pXGI->cmdQueueSize ;
		}
                else
                {
                    pXGI->cmdQueueOffset = 13*1024*1024 - pXGI->cmdQueueSize ;
                }
            }
            else
            {
                pXGI->cmdQueueOffset = (pScrn->videoRam)*1024 - pXGI->cmdQueueSize ;
            }

            pXGI->cmdQueueLen  = 0 ;
            pXGI->cmdQueueLenMin = 0x200 ;
            pXGI->cmdQueueLenMax = pXGI->cmdQueueSize - pXGI->cmdQueueLenMin ;

            ulSR26 = 0x40 ;
            break ;
        }
    }

    pXGI->CursorOffset = pXGI->cmdQueueOffset - VOLARI_CURSOR_SHAPE_SIZE;

    temp = (unsigned long)pXGI->FbBase ;
    temp += pXGI->cmdQueueOffset ;
    pXGI->cmdQueueBase = (unsigned char *)temp ;

    PDEBUG(ErrorF( "pXGI->FbBase = 0x%lX\n", pXGI->FbBase )) ;
    PDEBUG(ErrorF( "pXGI->cmdQueueOffset = 0x%lX\n", pXGI->cmdQueueOffset )) ;
    PDEBUG(ErrorF( "pXGI->cmdQueueBase = 0x%lX\n", pXGI->cmdQueueBase )) ;

    outXGIIDXREG(XGISR, 0x26, ulSR26) ;

    ulXGITempRP=Volari_GetHwRP() ;

    PDEBUG(ErrorF( "pXGI->IOBase = 0x%08lX, [%04X] = 0x%08lX\n",(unsigned long)(pXGI->IOBase),0x85c0, XGIMMIOLONG(0x85c0))) ;
    PDEBUG(ErrorF( "pXGI->IOBase = 0x%08lX, [%04X] = 0x%08lX\n",(unsigned long)(pXGI->IOBase),0x85c4, XGIMMIOLONG(0x85c4))) ;
    PDEBUG(ErrorF( "pXGI->IOBase = 0x%08lX, [%04X] = 0x%08lX\n",(unsigned long)(pXGI->IOBase),0x85c8, XGIMMIOLONG(0x85c8))) ;
    PDEBUG(ErrorF( "pXGI->IOBase = 0x%08lX, [%04X] = 0x%08lX\n",(unsigned long)(pXGI->IOBase),0x85cc, XGIMMIOLONG(0x85cc))) ;

    /* XGI315 */
    pXGI->cmdQueue_shareWP_only2D = ulXGITempRP;

    Volari_UpdateHwWP(ulXGITempRP) ;

    MMIO_OUT32(pXGI->IOBase, 0x85C0, pXGI->cmdQueueOffset) ;

    outXGIIDXREG(XGICR, 0x55, ulCR55) ;

    if(pXGI->Chipset == PCI_CHIP_XGIXG40)
    {
        Volari_Idle(pXGI);
        Volari_DisableDualPipe(pScrn) ;
        Volari_Idle(pXGI);

    }

	PDEBUG(ErrorF("Volari_InitCmdQueue() done.\n")) ;
    PDEBUG(ErrorF( "pXGI->IOBase = 0x%08lX, [%04X] = 0x%08lX\n",(unsigned long)(pXGI->IOBase),0x85c0, XGIMMIOLONG(0x85c0))) ;
    PDEBUG(ErrorF( "pXGI->IOBase = 0x%08lX, [%04X] = 0x%08lX\n",(unsigned long)(pXGI->IOBase),0x85c4, XGIMMIOLONG(0x85c4))) ;
    PDEBUG(ErrorF( "pXGI->IOBase = 0x%08lX, [%04X] = 0x%08lX\n",(unsigned long)(pXGI->IOBase),0x85c8, XGIMMIOLONG(0x85c8))) ;
    PDEBUG(ErrorF( "pXGI->IOBase = 0x%08lX, [%04X] = 0x%08lX\n",(unsigned long)(pXGI->IOBase),0x85cc, XGIMMIOLONG(0x85cc))) ;
}

static void
Volari_DisableDualPipe(ScrnInfoPtr pScrn)
{
    XGIPtr pXGI = XGIPTR(pScrn) ;
    unsigned long ulTemp ;
	unsigned long ulValue = MMIO_IN32(pXGI->IOBase, 0x8240) ;
	ulValue |= 1 << 10 ; /* D[10] = 1, Disable Dual Pipe. */

    ulTemp = Volari_GetSwWP() ;

    *(uint32_t *)(pXGI->cmdQueueBase+ulTemp) = (uint32_t)BE_SWAP32(GR_SKPC_HEADER + 0x8240) ;
    *(uint32_t *)(pXGI->cmdQueueBase+ulTemp+4) = (uint32_t)BE_SWAP32(ulValue) ;

    if( pXGI->Chipset == PCI_CHIP_XGIXG40 )
    {
        *(uint32_t *)(pXGI->cmdQueueBase+ulTemp+8) = (uint32_t)BE_SWAP32(GR_NIL_CMD) ;
        *(uint32_t *)(pXGI->cmdQueueBase+ulTemp+12) = (uint32_t)BE_SWAP32(GR_NIL_CMD) ;

        ulTemp += 0x10 ;
    }
    else if(( pXGI->Chipset == PCI_CHIP_XGIXG20 ) || ( pXGI->Chipset == PCI_CHIP_XGIXG21 ) || ( pXGI->Chipset == PCI_CHIP_XGIXG27 ))
        ulTemp += 0x08 ;

    ulTemp &= pXGI->cmdQueueSizeMask ;
    Volari_UpdateHwWP(ulTemp) ;
}

void
Volari_DisableAccelerator(ScrnInfoPtr pScrn)
{
    XGIPtr pXGI = XGIPTR(pScrn) ;

    PDEBUG(ErrorF("Volari_DisableAccelerator(pScrn)\n")) ;

    Volari_Idle(pXGI);

    if( pXGI->TurboQueue )
    {
        Volari_DisableCmdQueue(pScrn) ;
    }

    andXGIIDXREG(XGISR, 0x1E, 
                 ~(SR1E_ENABLE_3D_TRANSFORM_ENGINE
                   | SR1E_ENABLE_2D
                   | SR1E_ENABLE_3D_AGP_VERTEX_FETCH
                   | SR1E_ENABLE_3D_COMMAND_PARSER
                   | SR1E_ENABLE_3D));
}

static void
Volari_DisableCmdQueue(ScrnInfoPtr pScrn)
{
    XGIPtr pXGI = XGIPTR(pScrn) ;
    orXGIIDXREG(XGISR, 0x26, 0x01) ;
    andXGIIDXREG(XGISR, 0x26, 0x0C) ;
}

void
Volari_InitializeAccelerator(ScrnInfoPtr pScrn)
{
    XGIPtr pXGI = XGIPTR(pScrn);

    pXGI->DoColorExpand = FALSE;
}

Bool
Volari_AccelInit(ScreenPtr pScreen)
{
    ScrnInfoPtr       pScrn = xf86ScreenToScrn(pScreen);
    XGIPtr            pXGI = XGIPTR(pScrn);
    int               reservedFbSize;
    long               UsableFbSize;
    unsigned char     *AvailBufBase;
    BoxRec            Avail;
    int               i;


    Avail.x1 = 0; Avail.y1 = 0; Avail.x2 = 0; Avail.y2 = 0;

    PDEBUG1(ErrorF("Volari_AccelInit()\n" )) ;

#ifdef XGI_USE_EXA
		PDEBUG(ErrorF("--- EXA ---\n" )) ;
	   if(pXGI->useEXA) 
	   {
		  int obase = 0;

		PDEBUG(ErrorF("--- Enable EXA ---\n" )) ;

	      if(!(pXGI->EXADriverPtr = exaDriverAlloc()))
		  {
			  ErrorF("Failed to allocate EXADriverPtr!\n");
			  return FALSE;
		  }

	      /* data */
		PDEBUG(ErrorF("--- Xorg7 and above - 1 ---\n" )) ;
		  pXGI->EXADriverPtr->exa_major = 2;
		  pXGI->EXADriverPtr->exa_minor = 0;

		  pXGI->EXADriverPtr->memoryBase = pXGI->FbBase;
	      pXGI->EXADriverPtr->memorySize = pXGI->maxxfbmem;
	    if(!obase) {
	         obase = pScrn->displayWidth * pScrn->virtualY * (pScrn->bitsPerPixel >> 3);
	    }

		PDEBUG(ErrorF("--- Xorg7 and above - 2 ---\n" )) ;
		pXGI->EXADriverPtr->offScreenBase = obase;
		if (pXGI->EXADriverPtr->memorySize > pXGI->EXADriverPtr->offScreenBase) {
			PDEBUG(ErrorF("--- Xorg7 and above - 3 ---\n" )) ;
			pXGI->EXADriverPtr->flags = EXA_OFFSCREEN_PIXMAPS;
		}
		else {
		    pXGI->NoXvideo = TRUE;
		    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			       "Not enough video RAM for offscreen memory manager. Xv disabled\n");
		}

		PDEBUG(ErrorF("--- Xorg7 and above - 4 ---\n" )) ;
		pXGI->EXADriverPtr->pixmapOffsetAlign = 32; /* 16; */	/* src/dst: double quad word boundary */
		pXGI->EXADriverPtr->pixmapPitchAlign = 4;	/* pitch:   double word boundary      */
		pXGI->EXADriverPtr->maxX = 4095;
		pXGI->EXADriverPtr->maxY = 4095;

		PDEBUG(ErrorF("Use EXA for HW acceleration for Xorg7 and above...\n"));

		/* Sync */
		pXGI->EXADriverPtr->WaitMarker = XGIEXASync;

		/* Solid fill */
		pXGI->EXADriverPtr->PrepareSolid = XGIPrepareSolid;
		pXGI->EXADriverPtr->Solid = XGISolid;
		pXGI->EXADriverPtr->DoneSolid = XGIDoneSolid;

		/* Copy */
		pXGI->EXADriverPtr->PrepareCopy = XGIPrepareCopy;
		pXGI->EXADriverPtr->Copy = XGICopy;
		pXGI->EXADriverPtr->DoneCopy = XGIDoneCopy;

		pXGI->EXADriverPtr->CheckComposite = XGICheckComposite;
		pXGI->EXADriverPtr->PrepareComposite = XGIPrepareComposite;
		pXGI->EXADriverPtr->Composite = XGIComposite;
		pXGI->EXADriverPtr->DoneComposite = XGIDoneComposite;
	   }

	if(pXGI->useEXA) 
	{
	   /* if(!pSiS->NoAccel) { */
		if(!exaDriverInit(pScreen, pXGI->EXADriverPtr)) {
			 return FALSE;
	    } 

	    /* Reserve locked offscreen scratch area of 128K for glyph data */
	    pXGI->exa_scratch = exaOffscreenAlloc(pScreen, 128 * 1024, 16, TRUE,
						XGIScratchSave, pXGI);
	    if(pXGI->exa_scratch) 
		{
			 pXGI->exa_scratch_next = pXGI->exa_scratch->offset;

			 pXGI->EXADriverPtr->UploadToScratch = XGIUploadToScratch;
		}
            return TRUE;
	}
#endif /* EXA */
    return TRUE;
}

void
Volari_Sync(ScrnInfoPtr pScrn)
{
        XGIPtr pXGI = XGIPTR(pScrn);

        PDEBUG1(ErrorF("Volari_Sync()\n"));
        pXGI->DoColorExpand = FALSE;
        Volari_Idle(pXGI);
}

static int xgiG2_ALUConv[] =
{
    0x00,       /* dest = 0;            0,      GXclear,        0 */
    0x88,       /* dest &= src;         DSa,    GXand,          0x1 */
    0x44,       /* dest = src & ~dest;  SDna,   GXandReverse,   0x2 */
    0xCC,       /* dest = src;          S,      GXcopy,         0x3 */
    0x22,       /* dest &= ~src;        DSna,   GXandInverted,  0x4 */
    0xAA,       /* dest = dest;         D,      GXnoop,         0x5 */
    0x66,       /* dest = ^src;         DSx,    GXxor,          0x6 */
    0xEE,       /* dest |= src;         DSo,    GXor,           0x7 */
    0x11,       /* dest = ~src & ~dest; DSon,   GXnor,          0x8 */
    0x99,       /* dest ^= ~src ;       DSxn,   GXequiv,        0x9 */
    0x55,       /* dest = ~dest;        Dn,     GXInvert,       0xA */
    0xDD,       /* dest = src|~dest ;   SDno,   GXorReverse,    0xB */
    0x33,       /* dest = ~src;         Sn,     GXcopyInverted, 0xC */
    0xBB,       /* dest |= ~src;        DSno,   GXorInverted,   0xD */
    0x77,       /* dest = ~src|~dest;   DSan,   GXnand,         0xE */
    0xFF,       /* dest = 0xFF;         1,      GXset,          0xF */
};
/* same ROP but with Pattern as Source */
static int xgiG2_PatALUConv[] =
{
    0x00,       /* dest = 0;            0,      GXclear,        0 */
    0xA0,       /* dest &= src;         DPa,    GXand,          0x1 */
    0x50,       /* dest = src & ~dest;  PDna,   GXandReverse,   0x2 */
    0xF0,       /* dest = src;          P,      GXcopy,         0x3 */
    0x0A,       /* dest &= ~src;        DPna,   GXandInverted,  0x4 */
    0xAA,       /* dest = dest;         D,      GXnoop,         0x5 */
    0x5A,       /* dest = ^src;         DPx,    GXxor,          0x6 */
    0xFA,       /* dest |= src;         DPo,    GXor,           0x7 */
    0x05,       /* dest = ~src & ~dest; DPon,   GXnor,          0x8 */
    0xA5,       /* dest ^= ~src ;       DPxn,   GXequiv,        0x9 */
    0x55,       /* dest = ~dest;        Dn,     GXInvert,       0xA */
    0xF5,       /* dest = src|~dest ;   PDno,   GXorReverse,    0xB */
    0x0F,       /* dest = ~src;         Pn,     GXcopyInverted, 0xC */
    0xAF,       /* dest |= ~src;        DPno,   GXorInverted,   0xD */
    0x5F,       /* dest = ~src|~dest;   DPan,   GXnand,         0xE */
    0xFF,       /* dest = 0xFF;         1,      GXset,          0xF */
};

/* Jong 01/13/2009; support EXA */
#ifdef XGI_USE_EXA  /* ---------------------------- EXA -------------------------- */
void XGIEXASync(ScreenPtr pScreen, int marker)
{
	XGIPtr pXGI = XGIPTR(xf86ScreenToScrn(pScreen));

	PACCELDEBUG(ErrorF("XGIEXASync()...\n"));

	Volari_Idle;
}

static Bool
XGIPrepareSolid(PixmapPtr pPixmap, int alu, Pixel planemask, Pixel fg)
{
	ScrnInfoPtr pScrn = xf86ScreenToScrn(pPixmap->drawable.pScreen);
	XGIPtr pXGI = XGIPTR(pScrn);
	uint16_t pitch;

	PACCELDEBUG(ErrorF("XGIPrepareSolid...\n"));

	/* Planemask not supported */
	if((planemask & ((1 << pPixmap->drawable.depth) - 1)) !=
				(1 << pPixmap->drawable.depth) - 1) {
	   return FALSE;
	}

	if((pPixmap->drawable.bitsPerPixel != 8) &&
	   (pPixmap->drawable.bitsPerPixel != 16) &&
	   (pPixmap->drawable.bitsPerPixel != 32))
	   return FALSE;

	/* Check that the pitch matches the hardware's requirements. Should
	 * never be a problem due to pixmapPitchAlign and fbScreenInit.
	 */
	if(((pitch = exaGetPixmapPitch(pPixmap)) & 3))
	   return FALSE;

	PACCELDEBUG(ErrorF("pitch=%d...\n", pitch));

    Volari_ResetCmd ; 

	Volari_SetupPATFG((int)fg)

	Volari_SetupDSTColorDepth((pPixmap->drawable.bitsPerPixel >> 4) << 16);
	Volari_SetupDSTRect(pitch, DEV_HEIGHT) 

	Volari_SetupROP(xgiG2_PatALUConv[alu])
	Volari_SetupCMDFlag(PATFG | BITBLT)

	pXGI->fillDstBase = (uint32_t)exaGetPixmapOffset(pPixmap); /* FBOFFSET is not used for Z-series */
	PACCELDEBUG(ErrorF("pXGI->fillDstBase=0x%x...\n", pXGI->fillDstBase));

	return TRUE;
}

static void
XGISolid(PixmapPtr pPixmap, int x1, int y1, int x2, int y2)
{
	ScrnInfoPtr pScrn = xf86ScreenToScrn(pPixmap->drawable.pScreen);
	XGIPtr pXGI = XGIPTR(pScrn);
	uint32_t  Command;

	PACCELDEBUG(ErrorF("XGISolid...\n"));
	PACCELDEBUG(ErrorF("pXGI->CommandReg = 0x%x...\n", pXGI->CommandReg));

	Volari_SetupDSTXY(x1, y1)
    Volari_SetupRect(x2-x1, y2-y1) ;
	Volari_SetupDSTBase(pXGI->fillDstBase);
	Volari_DoCMD
}

static void
XGIDoneSolid(PixmapPtr pPixmap)
{
	PACCELDEBUG(ErrorF("XGIDoneSolid()...\n"));
}

static Bool
XGIPrepareCopy(PixmapPtr pSrcPixmap, PixmapPtr pDstPixmap, int xdir, int ydir,
					int alu, Pixel planemask)
{
	ScrnInfoPtr pScrn = xf86ScreenToScrn(pDstPixmap->drawable.pScreen);
	XGIPtr pXGI = XGIPTR(pScrn);
	uint32_t srcbase, dstbase;
	uint16_t srcpitch, dstpitch;

	PACCELDEBUG(ErrorF("XGIPrepareCopy()...\n"));
	PACCELDEBUG(ErrorF("pSrcPixmap->devPrivate.ptr=0x%x, pDstPixmap->devPrivate.ptr=0x%x...\n", 
						pSrcPixmap->devPrivate.ptr, pDstPixmap->devPrivate.ptr));

	/* Planemask not supported */
	if((planemask & ((1 << pSrcPixmap->drawable.depth) - 1)) !=
				(1 << pSrcPixmap->drawable.depth) - 1) {
	   return FALSE;
	}

	if((pDstPixmap->drawable.bitsPerPixel != 8) &&
	   (pDstPixmap->drawable.bitsPerPixel != 16) &&
	   (pDstPixmap->drawable.bitsPerPixel != 32))
	   return FALSE;

	/* Jong 07/15/2009; bug fixing for moving window on console; might need stretch bitblt to set bitblt direction */
	if((xdir != 0) || (ydir != 0)) return FALSE;

	/* Check that the pitch matches the hardware's requirements. Should
	 * never be a problem due to pixmapPitchAlign and fbScreenInit.
	 */
	if((srcpitch = exaGetPixmapPitch(pSrcPixmap)) & 3)
	   return FALSE;
	if((dstpitch = exaGetPixmapPitch(pDstPixmap)) & 3)
	   return FALSE;

	srcbase = (uint32_t)exaGetPixmapOffset(pSrcPixmap); /* FBOFFSET is not used for Z-series */;

	dstbase = (uint32_t)exaGetPixmapOffset(pDstPixmap); /* FBOFFSET is not used for Z-series */

	/* TODO: Will there eventually be overlapping blits?
	 * If so, good night. Then we must calculate new base addresses
	 * which are identical for source and dest, otherwise
	 * the chips direction-logic will fail. Certainly funny
	 * to re-calculate x and y then...
	 */

    Volari_ResetCmd ;

	Volari_SetupDSTColorDepth((pDstPixmap->drawable.bitsPerPixel >> 4) << 16);
	Volari_SetupSRCPitch(srcpitch)
	Volari_SetupDSTRect(dstpitch, DEV_HEIGHT)

    Volari_SetupROP(xgiG2_ALUConv[alu]) 

	Volari_SetupSRCBase(srcbase)
	Volari_SetupDSTBase(dstbase)

	return TRUE;
}

static void
XGICopy(PixmapPtr pDstPixmap, int srcX, int srcY, int dstX, int dstY, int width, int height)
{
	ScrnInfoPtr pScrn = xf86ScreenToScrn(pDstPixmap->drawable.pScreen);
	XGIPtr pXGI = XGIPTR(pScrn);
	uint32_t  Command;

	PACCELDEBUG(ErrorF("XGICopy()...\n"));
	PACCELDEBUG(ErrorF("srcX=%d, srcY=%d, dstX=%d, dstY=%d...\n", srcX, srcY, dstX, dstY));
	PACCELDEBUG(ErrorF("pDstPixmap->devPrivate.ptr=0x%x...\n", pDstPixmap->devPrivate.ptr));

	Volari_SetupSRCXY(srcX, srcY)
	Volari_SetupDSTXY(dstX, dstY)

	Volari_SetupRect(width, height)
	Volari_DoCMD
}

static void
XGIDoneCopy(PixmapPtr pDstPixmap)
{
	PACCELDEBUG(ErrorF("XGIDoneCopy()...\n"));
}

static Bool
XGICheckComposite(int op, PicturePtr pSrcPicture, PicturePtr pMaskPicture,
				PicturePtr pDstPicture)
{
	ScrnInfoPtr pScrn = xf86ScreenToScrn(pDstPicture->pDrawable->pScreen);
	XGIPtr pXGI = XGIPTR(pScrn);

	PACCELDEBUG(ErrorF("XGICheckComposite()...\n"));

	xf86DrvMsg(0, 0, "CC: %d Src %x (fi %d ca %d) Msk %x (%d %d) Dst %x (%d %d)\n",
		op, pSrcPicture->format, pSrcPicture->filter, pSrcPicture->componentAlpha,
		pMaskPicture ? pMaskPicture->format : 0x2011, pMaskPicture ? pMaskPicture->filter : -1,
			pMaskPicture ? pMaskPicture->componentAlpha : -1,
		pDstPicture->format, pDstPicture->filter, pDstPicture->componentAlpha);

	if(pSrcPicture->transform || (pMaskPicture && pMaskPicture->transform) || pDstPicture->transform) {
		xf86DrvMsg(0, 0, "CC: src tr %p msk %p dst %p  !!!!!!!!!!!!!!!\n",
			pSrcPicture->transform,
			pMaskPicture ? pMaskPicture->transform : 0,
			pDstPicture->transform);
        }

	return FALSE;
}

static Bool
XGIPrepareComposite(int op, PicturePtr pSrcPicture, PicturePtr pMaskPicture,
				PicturePtr pDstPicture, PixmapPtr pSrc, PixmapPtr pMask, PixmapPtr pDst)
{
	ScrnInfoPtr pScrn = xf86ScreenToScrn(pDst->drawable.pScreen);
	XGIPtr pXGI = XGIPTR(pScrn);

	PACCELDEBUG(ErrorF("XGIPrepareComposite()...\n"));

    Volari_ResetCmd ;

	return FALSE;
}

static void
XGIComposite(PixmapPtr pDst, int srcX, int srcY, int maskX, int maskY, int dstX, int dstY,
				int width, int height)
{
	ScrnInfoPtr pScrn = xf86ScreenToScrn(pDst->drawable.pScreen);
	XGIPtr pXGI = XGIPTR(pScrn);

	PACCELDEBUG(ErrorF("XGIComposite()...\n"));
}

static void
XGIDoneComposite(PixmapPtr pDst)
{
}

/************************************************************************/
/*                   libc memcpy() wrapper - generic                    */
/************************************************************************/
static void XGI_libc_memcpy(UChar *dst, const UChar *src, int size)
{
	PACCELDEBUG(ErrorF("XGI_libc_memcpy()...\n"));
    memcpy(dst, src, size);
}

extern void XGI_sse_memcpy(UChar *to, const UChar *from, int size);
extern void XGI_builtin_memcpy_arm(UChar *to, const UChar *from, int size);

void XGIMemCopyToVideoRam(XGIPtr pXGI, unsigned char *to, unsigned char *from, int size)
{
	int align = (ULONG)to & 31;
	int align_size = 0;

 	if(align)
	{
	  align_size = size > align ? align : size;
	  
	  PDEBUG(ErrorF("XGI_libc_memcpy()...align_size=%d bytes\n", align_size));
	  XGI_libc_memcpy(to, from, align_size);

	  size -= align_size;
	}

	if( size > 0)
	{
#if defined(__arm__) 
	  PDEBUG(ErrorF("XGI_builtin_memcpy_arm()...\n"));

	  XGI_builtin_memcpy_arm(to+align, from+align, size/4);

	  if(((ULONG)size & 3) )
	  {
	    PDEBUG(ErrorF("XGI_libc_memcpy...%d\n", (ULONG)size & 3));
	    XGI_libc_memcpy(to+align+(size/4)*4, from+align+(size/4)*4, (ULONG)size & 3);
	  }
#else
	  PDEBUG(ErrorF("XGI_libc_memcpy()...\n"));
	  XGI_libc_memcpy(to+align, from+align, size);
#endif
	}
}

void XGIMemCopyFromVideoRam(XGIPtr pXGI, unsigned char *to, unsigned char *from, int size)
{
	int align = (ULONG)to & 31;
	int align_size = 0;

 	if(align)
	{
	  align_size = size > align ? align : size;
	  
	  PDEBUG(ErrorF("XGI_libc_memcpy()...align_size=%d bytes\n", align_size));
	  XGI_libc_memcpy(to, from, align_size);

	  size -= align_size;
	}

	if( size > 0)
	{
#if defined(__arm__) 
	  PDEBUG(ErrorF("XGI_builtin_memcpy_arm()...\n"));
	  XGI_builtin_memcpy_arm(to+align, from+align, size/4);

	  if(((ULONG)size & 3) )
	  {
	    PDEBUG(ErrorF("XGI_libc_memcpy...%d\n", (ULONG)size & 3));
	    XGI_libc_memcpy(to+align+(size/4)*4, from+align+(size/4)*4, (ULONG)size & 3);
	  }
#else
	  PDEBUG(ErrorF("XGI_libc_memcpy()...\n"));
	  XGI_libc_memcpy(to+align, from+align, size);
#endif
	}
}

Bool
XGIUploadToScratch(PixmapPtr pSrc, PixmapPtr pDst)
{
	ScrnInfoPtr pScrn = xf86ScreenToScrn(pSrc->drawable.pScreen);
	XGIPtr pXGI = XGIPTR(pScrn);
	unsigned char *src, *dst;
	int src_pitch = exaGetPixmapPitch(pSrc);
	int dst_pitch, size, w, h, bytes;

#ifdef DEBUG_ACCEL_DYNAMICALLY
	{
		/* Jong - Good method for performance evaluation */
		/*-----------------------------------------------*/ \
		uint8_t CR37=0x00; /* Jong 09/11/2008; [7] for disable/enable NULL function dynamically */
		/* Jong 09/12/2008; disable NULL function of HW acceleration dynamically by CR37[7] */
		/* After test, this extra IO doesn't have significant loading */
		CR37=XGI_GetReg(pXGI->XGI_Pr->P3d4, 0x37);
		if((CR37 & 0x80) != 0)
			return TRUE;
	}
#endif

	w = pSrc->drawable.width;

	dst_pitch = ((w * (pSrc->drawable.bitsPerPixel >> 3)) +
		     pXGI->EXADriverPtr->pixmapPitchAlign - 1) &
		    ~(pXGI->EXADriverPtr->pixmapPitchAlign - 1);

	size = dst_pitch * pSrc->drawable.height;

	if(size > pXGI->exa_scratch->size)
	   return FALSE;


	pXGI->exa_scratch_next = (pXGI->exa_scratch_next +
				  pXGI->EXADriverPtr->pixmapOffsetAlign - 1) &
				  ~(pXGI->EXADriverPtr->pixmapOffsetAlign - 1);

	if(pXGI->exa_scratch_next + size >
	   pXGI->exa_scratch->offset + pXGI->exa_scratch->size) {
	   (pXGI->EXADriverPtr->WaitMarker)(pSrc->drawable.pScreen, 0);
	   pXGI->exa_scratch_next = pXGI->exa_scratch->offset;
	}

	memcpy(pDst, pSrc, sizeof(*pDst));
	pDst->devKind = dst_pitch;
	pDst->devPrivate.ptr = pXGI->EXADriverPtr->memoryBase + pXGI->exa_scratch_next;

	pXGI->exa_scratch_next += size;

	src = pSrc->devPrivate.ptr;
	src_pitch = exaGetPixmapPitch(pSrc);
	dst = pDst->devPrivate.ptr;

	bytes = (src_pitch < dst_pitch) ? src_pitch : dst_pitch;

	h = pSrc->drawable.height;

	Volari_Sync(pScrn);

	while(h--) {
	   XGIMemCopyToVideoRam(pXGI, dst, src, size); 
	   src += src_pitch;
	   dst += dst_pitch;
	}

	return TRUE;
}

void XGIScratchSave(ScreenPtr pScreen, ExaOffscreenArea *area)
{
	XGIPtr pXGI = XGIPTR(xf86ScreenToScrn(pScreen));
	pXGI->exa_scratch = NULL;
}
#endif /* EXA */

/* Jong@08112009 */
void XGIDumpCMDQueue(ScrnInfoPtr pScrn)
{
    XGIPtr pXGI = XGIPTR(pScrn);

    int i ;
    unsigned long SwWP ;

    ErrorF("----------------------------------------------------------------------\n") ;
    ErrorF("CMD Queue\n") ;
    ErrorF("----------------------------------------------------------------------\n") ;

	SwWP = Volari_GetSwWP() ;
    ErrorF("SwWP=0x%lx\n", SwWP) ;
    ErrorF("pXGI->cmdQueueBase=%p\n", pXGI->cmdQueueBase) ;
	for( i = 0 ; i < SwWP ; i+=0x04 )
	{
		ErrorF("[%04X]: %08lX\n",i,
		    (unsigned long)*(uint32_t *)(pXGI->cmdQueueBase+i));
	}
}

/* Jong@09182009; test for line missing */
static void Volari_SetupForSolidLine(ScrnInfoPtr pScrn, int color, int rop,
			unsigned int planemask)
{
	PDEBUG(ErrorF("Null -  Volari_SetupForSolidLine()...\n"));
}

static void Volari_SubsequentSolidTwoPointLine(ScrnInfoPtr pScrn,
			int x1, int y1, int x2, int y2, int flags)
{
	PDEBUG(ErrorF("Null -  Volari_SubsequentSolidTwoPointLine()...\n"));
}

static void Volari_SubsequentSolidHorzVertLine(ScrnInfoPtr pScrn,
			int x, int y, int len, int dir)
{
	PDEBUG(ErrorF("Null -  Volari_SubsequentSolidHorzVertLine()...\n"));
}

static void Volari_SetupForDashedLine(ScrnInfoPtr pScrn,
			int fg, int bg, int rop, unsigned int planemask,
			int length, unsigned char *pattern)
{
	PDEBUG(ErrorF("Null -  Volari_SetupForDashedLine()...\n"));
}

static void Volari_SubsequentDashedTwoPointLine(ScrnInfoPtr pScrn,
			int x1, int y1, int x2, int y2,
			int flags, int phase)
{
	PDEBUG(ErrorF("Null -  Volari_SubsequentDashedTwoPointLine()...\n"));
}

static void Volari_SetClippingRectangle(ScrnInfoPtr pScrn, int x1, int y1, int x2, int y2)
{
	PDEBUG(ErrorF("Null -  Volari_SetClippingRectangle()...\n"));
}

static void Volari_DisableClipping(ScrnInfoPtr pScrn)
{
	PDEBUG(ErrorF("Null -  Volari_DisableClipping()...\n"));
}
