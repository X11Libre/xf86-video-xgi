#ifndef _VBUTIL_
#define _VBUTIL_
extern   void     NewDelaySeconds( int );
extern   void     Newdebugcode( UCHAR );
extern   void     XGINew_SetReg1(USHORT, USHORT, USHORT);
extern   void     XGINew_SetReg3(USHORT, USHORT);
extern   UCHAR    XGINew_GetReg1(USHORT, USHORT);
extern   UCHAR    XGINew_GetReg2(USHORT);
extern   void     XGINew_SetReg4(USHORT, ULONG);
extern   ULONG    XGINew_GetReg3(USHORT);
extern   void     XGINew_ClearDAC( PUCHAR ) ;
extern   void     XGINew_SetRegOR(USHORT Port,USHORT Index,USHORT DataOR);
extern   void     XGINew_SetRegAND(USHORT Port,USHORT Index,USHORT DataAND);
extern   void     XGINew_SetRegANDOR(USHORT Port,USHORT Index,USHORT DataAND,USHORT DataOR);
#endif

