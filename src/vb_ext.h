#ifndef  _VBEXT_
#define  _VBEXT_

extern   void     XGISetDPMS( PXGI_HW_DEVICE_INFO pXGIHWDE , ULONG VESA_POWER_STATE ) ;
extern   void     XGI_GetSenseStatus( PXGI_HW_DEVICE_INFO HwDeviceExtension , PVB_DEVICE_INFO pVBInfo );
#ifdef WIN2000
extern   BOOLEAN  XGI_DySense( PHW_DEVICE_EXTENSION pHWDE , PUCHAR ujConnectStatus );
#endif /* WIN2000 */

#endif
