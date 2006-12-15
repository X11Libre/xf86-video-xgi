#ifndef  _VBINIT_
#define  _VBINIT_
extern BOOLEAN XGIInitNew(PXGI_HW_DEVICE_INFO HwDeviceExtension);

extern void XGINew_SetModeScratch(PXGI_HW_DEVICE_INFO HwDeviceExtension,
    PVB_DEVICE_INFO pVBInfo);

extern void ReadVBIOSTablData(UCHAR ChipType, PVB_DEVICE_INFO pVBInfo);
#endif

