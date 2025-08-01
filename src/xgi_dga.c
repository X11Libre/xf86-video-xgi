/*
 *  DGA handling
 *
 * Copyright (C) 2000 by Alan Hourihane, Sychdyn, North Wales, UK.
 * Copyright (C) 2001-2004 by Thomas Winischhofer, Vienna, Austria
 *
 * Portions from radeon_dga.c which is
 *          Copyright 2000 ATI Technologies Inc., Markham, Ontario, and
 *                         VA Linux Systems Inc., Fremont, California.
 *
 * Licensed under the following terms:
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the providers not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  The providers make no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 *
 * THE PROVIDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE PROVIDERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * Authors:  Alan Hourihane, <alanh@fairlite.demon.co.uk>
 *           Thomas Winischhofer <thomas@winischhofer.net>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86Pci.h"
#include "xgi.h"
#include "xgi_regs.h"
#include "dgaproc.h"

static Bool XGI_OpenFramebuffer(ScrnInfoPtr, char **, unsigned char **,
                                int *, int *, int *);

static Bool XGI_SetMode(ScrnInfoPtr, DGAModePtr);
static void XGI_Sync(ScrnInfoPtr);
static int  XGI_GetViewport(ScrnInfoPtr);
static void XGI_SetViewport(ScrnInfoPtr, int, int, int);
static void XGI_FillRect(ScrnInfoPtr, int, int, int, int, unsigned long);
static void XGI_BlitRect(ScrnInfoPtr, int, int, int, int, int, int);

static
DGAFunctionRec XGIDGAFuncs = {
   XGI_OpenFramebuffer,
   NULL,
   XGI_SetMode,
   XGI_SetViewport,
   XGI_GetViewport,
   XGI_Sync,
   XGI_FillRect,
   XGI_BlitRect,
   NULL
};


static DGAModePtr
XGISetupDGAMode(
   ScrnInfoPtr pScrn,
   DGAModePtr modes,
   int *num,
   int bitsPerPixel,
   int depth,
   Bool pixmap,
   int secondPitch,
   unsigned long red,
   unsigned long green,
   unsigned long blue,
   short visualClass
){
   XGIPtr pXGI = XGIPTR(pScrn);
   DGAModePtr newmodes = NULL, currentMode;
   DisplayModePtr pMode, firstMode;
   int otherPitch, Bpp = bitsPerPixel >> 3;
   Bool oneMore;

   pMode = firstMode = pScrn->modes;

   while(pMode) {

	otherPitch = secondPitch ? secondPitch : pMode->HDisplay;

	if(pMode->HDisplay != otherPitch) {

	    newmodes = realloc(modes, (*num + 2) * sizeof(DGAModeRec));
	    oneMore  = TRUE;

	} else {

	    newmodes = realloc(modes, (*num + 1) * sizeof(DGAModeRec));
	    oneMore  = FALSE;

	}

	if(!newmodes) {
	    free(modes);
	    return NULL;
	}
	modes = newmodes;

SECOND_PASS:

	currentMode = modes + *num;
	(*num)++;

	currentMode->mode           = pMode;
	currentMode->flags          = DGA_CONCURRENT_ACCESS;
	if(pixmap)
	    currentMode->flags     |= DGA_PIXMAP_AVAILABLE;
	if(!pXGI->NoAccel) {
	    currentMode->flags     |= DGA_FILL_RECT | DGA_BLIT_RECT;
	}
	if(pMode->Flags & V_DBLSCAN)
	    currentMode->flags     |= DGA_DOUBLESCAN;
	if(pMode->Flags & V_INTERLACE)
	    currentMode->flags     |= DGA_INTERLACED;
	currentMode->byteOrder      = pScrn->imageByteOrder;
	currentMode->depth          = depth;
	currentMode->bitsPerPixel   = bitsPerPixel;
	currentMode->red_mask       = red;
	currentMode->green_mask     = green;
	currentMode->blue_mask      = blue;
	currentMode->visualClass    = visualClass;
	currentMode->viewportWidth  = pMode->HDisplay;
	currentMode->viewportHeight = pMode->VDisplay;
	currentMode->xViewportStep  = 1;
	currentMode->yViewportStep  = 1;
	currentMode->viewportFlags  = DGA_FLIP_RETRACE;
	currentMode->offset         = 0;
	currentMode->address        = pXGI->FbBase;

	if(oneMore) {

	    /* first one is narrow width */
	    currentMode->bytesPerScanline = (((pMode->HDisplay * Bpp) + 3) & ~3L);
	    currentMode->imageWidth   = pMode->HDisplay;
	    currentMode->imageHeight  = pMode->VDisplay;
	    currentMode->pixmapWidth  = currentMode->imageWidth;
	    currentMode->pixmapHeight = currentMode->imageHeight;
	    currentMode->maxViewportX = currentMode->imageWidth -
					currentMode->viewportWidth;
	    /* this might need to get clamped to some maximum */
	    currentMode->maxViewportY = (currentMode->imageHeight -
					 currentMode->viewportHeight);
	    oneMore = FALSE;
	    goto SECOND_PASS;

	} else {

	    currentMode->bytesPerScanline = ((otherPitch * Bpp) + 3) & ~3L;
	    currentMode->imageWidth       = otherPitch;
	    currentMode->imageHeight      = pMode->VDisplay;
	    currentMode->pixmapWidth      = currentMode->imageWidth;
	    currentMode->pixmapHeight     = currentMode->imageHeight;
	    currentMode->maxViewportX     = (currentMode->imageWidth -
					     currentMode->viewportWidth);
            /* this might need to get clamped to some maximum */
	    currentMode->maxViewportY     = (currentMode->imageHeight -
					     currentMode->viewportHeight);
	}

	pMode = pMode->next;
	if(pMode == firstMode)
	   break;
    }

    return modes;
}


Bool
XGIDGAInit(ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
   XGIPtr pXGI = XGIPTR(pScrn);
   DGAModePtr modes = NULL;
   int num = 0;

   /* 8 */
   /* We don't support 8bpp modes in dual head or MergedFB mode,
    * so don't offer them to DGA either.
    */
   if (!IS_DUAL_HEAD(pXGI) && !(pXGI->MergedFB)) {
       modes = XGISetupDGAMode(pScrn, modes, &num, 8, 8,
			       (pScrn->bitsPerPixel == 8),
			       ((pScrn->bitsPerPixel != 8)
				? 0 : pScrn->displayWidth),
			       0, 0, 0, PseudoColor);
   }

   /* 16 */
   modes = XGISetupDGAMode(pScrn, modes, &num, 16, 16,
			   (pScrn->bitsPerPixel == 16),
			   ((pScrn->depth != 16)
				? 0 : pScrn->displayWidth),
			   0xf800, 0x07e0, 0x001f, TrueColor);

    /* 32 */
    modes = XGISetupDGAMode(pScrn, modes, &num, 32, 24,
			    (pScrn->bitsPerPixel == 32),
			    ((pScrn->bitsPerPixel != 32)
			     ? 0 : pScrn->displayWidth),
			    0xff0000, 0x00ff00, 0x0000ff, TrueColor);

    pXGI->numDGAModes = num;
    pXGI->DGAModes = modes;

    if (num) {
	return DGAInit(pScreen, &XGIDGAFuncs, modes, num);
    }
    else {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "No DGA-suitable modes found, disabling DGA\n");
	return TRUE;
    }
}


static Bool
XGI_SetMode(
   ScrnInfoPtr pScrn,
   DGAModePtr pMode
){
   static XGIFBLayout BackupLayouts[MAXSCREENS];
   int index = pScrn->pScreen->myNum;
   XGIPtr pXGI = XGIPTR(pScrn);

    if(!pMode) { /* restore the original mode */

        if(pXGI->DGAactive) {
           /* put the ScreenParameters back */
	   memcpy(&pXGI->CurrentLayout, &BackupLayouts[index], sizeof(XGIFBLayout));
        }

	pScrn->currentMode = pXGI->CurrentLayout.mode;

        (*pScrn->SwitchMode)(pScrn, pScrn->currentMode);
        (*pScrn->AdjustFrame)(pScrn, pScrn->frameX0, pScrn->frameY0);
        pXGI->DGAactive = FALSE;

    } else {	/* set new mode */

        if(!pXGI->DGAactive) {
	    /* save the old parameters */
            memcpy(&BackupLayouts[index], &pXGI->CurrentLayout, sizeof(XGIFBLayout));
            pXGI->DGAactive = TRUE;
    	}

	pXGI->CurrentLayout.bitsPerPixel = pMode->bitsPerPixel;
	pXGI->CurrentLayout.depth        = pMode->depth;
	pXGI->CurrentLayout.displayWidth = pMode->bytesPerScanline / (pMode->bitsPerPixel >> 3);

        (*pScrn->SwitchMode)(pScrn, pMode->mode);
	/* TW: Adjust viewport to 0/0 after mode switch */
	/* This should fix the vmware-in-dualhead problems */
        (*pScrn->AdjustFrame)(pScrn, 0, 0);
    }

    return TRUE;
}

static int
XGI_GetViewport(ScrnInfoPtr pScrn)
{
    (void) pScrn;

    /* There are never pending Adjusts */
    return 0;
}

static void
XGI_SetViewport(ScrnInfoPtr pScrn, int x, int y, int flags)
{
    (*pScrn->AdjustFrame)(pScrn, x, y);
}

static void
XGI_FillRect (
   ScrnInfoPtr pScrn,
   int x, int y, int w, int h,
   unsigned long color
){
}

static void
XGI_Sync(
   ScrnInfoPtr pScrn
){
}

static void
XGI_BlitRect(
   ScrnInfoPtr pScrn,
   int srcx, int srcy,
   int w, int h,
   int dstx, int dsty
){
    XGIPtr pXGI = XGIPTR(pScrn);
}

static Bool
XGI_OpenFramebuffer(
   ScrnInfoPtr pScrn,
   char **name,
   unsigned char **mem,
   int *size,
   int *offset,
   int *flags
){
    XGIPtr pXGI = XGIPTR(pScrn);

    *name = NULL;       /* no special device */
    *mem = (unsigned char*)pXGI->FbAddress;
    *size = pXGI->maxxfbmem;
    *offset = 0;
    *flags = DGA_NEED_ROOT;

    return TRUE;
}
