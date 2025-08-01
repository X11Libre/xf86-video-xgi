/*
 * Global data and definitions
 *
 * Copyright (C) 2001-2004 by Thomas Winischhofer, Vienna, Austria
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1) Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2) Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3) The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author:     	Thomas Winischhofer <thomas@winischhofer.net>
 *
 */


/* For calculating refresh rate index (CR33) */
static const struct _xgi_vrate {
    uint16_t idx;
    uint16_t xres;
    uint16_t yres;
    uint16_t refresh;
    BOOLEAN XGI730valid32bpp;
} xgix_vrate[] = {
	{1,  320,  200,  70,  TRUE},
	{1,  320,  240,  60,  TRUE},
	{1,  400,  300,  60,  TRUE},
    {1,  512,  384,  60,  TRUE},
	{1,  640,  400,  72,  TRUE},
	{1,  640,  480,  60,  TRUE}, 
	{2,  640,  480,  72,  TRUE}, 
	{3,  640,  480,  75,  TRUE},
	{4,  640,  480,  85,  TRUE}, 
	{5,  640,  480, 100,  TRUE}, 
	{6,  640,  480, 120,  TRUE},
	{7,  640,  480, 160, FALSE}, 
	{8,  640,  480, 200, FALSE},
	{1,  720,  480,  60,  TRUE},
	{1,  720,  576,  58,  TRUE},
	{1,  768,  576,  58,  TRUE},
	{1,  800,  480,  60,  TRUE}, 
	{2,  800,  480,  75,  TRUE}, 
	{3,  800,  480,  85,  TRUE},
	{1,  800,  600,  56,  TRUE}, 
	{2,  800,  600,  60,  TRUE}, 
	{3,  800,  600,  72,  TRUE},
	{4,  800,  600,  75,  TRUE}, 
	{5,  800,  600,  85,  TRUE}, 
	{6,  800,  600, 105,  TRUE},
	{7,  800,  600, 120, FALSE}, 
	{8,  800,  600, 160, FALSE},
	{1,  848,  480,  39,  TRUE}, 
	{2,  848,  480,  60,  TRUE},
	{1,  856,  480,  39,  TRUE}, 
	{2,  856,  480,  60,  TRUE},
	{1, 1024,  576,  60,  TRUE}, 
	{2, 1024,  576,  75,  TRUE}, 
	{3, 1024,  576,  85,  TRUE},
	{1, 1024,  600,  60,  TRUE},
	{1, 1024,  768,  43,  TRUE}, 
	{2, 1024,  768,  60,  TRUE}, 
	{3, 1024,  768,  70, FALSE},
	{4, 1024,  768,  75, FALSE}, 
	{5, 1024,  768,  85,  TRUE}, 
	{6, 1024,  768, 100,  TRUE},
	{7, 1024,  768, 120,  TRUE},
	{1, 1152,  768,  60,  TRUE},
	{1, 1152,  864,  75,  TRUE}, 
	{2, 1152,  864,  84, FALSE},
	{1, 1280,  720,  60,  TRUE}, 
	{2, 1280,  720,  75, FALSE}, 
	{3, 1280,  720,  85,  TRUE},
	{1, 1280,  768,  60,  TRUE},
	{1, 1280,  800,  60,  TRUE},
	{1, 1280,  960,  60,  TRUE}, 
	{2, 1280,  960,  85,  TRUE},
	{1, 1280, 1024,  43, FALSE}, 
	{2, 1280, 1024,  60,  TRUE}, 
	{3, 1280, 1024,  75, FALSE},
	{4, 1280, 1024,  85,  TRUE},
	{1, 1360,  768,  60,  TRUE},
	{1, 1400, 1050,  60,  TRUE}, 
	{2, 1400, 1050,  75,  TRUE},
	{1, 1600, 1200,  60,  TRUE}, 
	{2, 1600, 1200,  65,  TRUE}, 
	{3, 1600, 1200,  70,  TRUE},
	{4, 1600, 1200,  75,  TRUE}, 
	{5, 1600, 1200,  85,  TRUE}, 
	{6, 1600, 1200, 100,  TRUE},
	{7, 1600, 1200, 120,  TRUE},
	{1, 1680, 1050,  60,  TRUE},
	{1, 1920, 1440,  60,  TRUE}, 
	{2, 1920, 1440,  65,  TRUE}, 
	{3, 1920, 1440,  70,  TRUE},
	{4, 1920, 1440,  75,  TRUE}, 
	{5, 1920, 1440,  85,  TRUE}, 
	{6, 1920, 1440, 100,  TRUE},
	{1, 2048, 1536,  60,  TRUE}, 
	{2, 2048, 1536,  65,  TRUE}, 
	{3, 2048, 1536,  70,  TRUE},
	{4, 2048, 1536,  75,  TRUE}, 
	{5, 2048, 1536,  85,  TRUE},
	{0,    0,    0,   0, FALSE}
};


/*     TV filters for video bridges
 */
static const struct _XGITVFilter301 {
	unsigned char filter[7][4];
} XGITVFilter301[] = {
	{{ {0x00,0xE0,0x10,0x60},   /* NTSCFilter - 320 */
	   {0x00,0xEE,0x10,0x44},
	   {0x00,0xF4,0x10,0x38},
	   {0xF8,0xF4,0x18,0x38},
	   {0xFC,0xFB,0x14,0x2A},
	   {0x00,0x00,0x10,0x20},
	   {0x00,0x04,0x10,0x18} }},
	{{ {0xF5,0xEE,0x1B,0x44},   /* NTSCFilter - 640 */
	   {0xF8,0xF4,0x18,0x38},
	   {0xEB,0x04,0x25,0x18},
	   {0xF1,0x05,0x1F,0x16},
	   {0xF6,0x06,0x1A,0x14},
	   {0xFA,0x06,0x16,0x14},
	   {0x00,0x04,0x10,0x18} }},
	{{ {0xEB,0x04,0x25,0x18},   /* NTSCFilter - 720 */
	   {0xE7,0x0E,0x29,0x04},
	   {0xEE,0x0C,0x22,0x08},
	   {0xF6,0x0B,0x1A,0x0A},
	   {0xF9,0x0A,0x17,0x0C},
	   {0xFC,0x0A,0x14,0x0C},
	   {0x00,0x08,0x10,0x10} }},
	{{ {0xEC,0x02,0x24,0x1C},   /* NTSCFilter - 800/400 */
	   {0xF2,0x04,0x1E,0x18},
	   {0xEB,0x15,0x25,0xF6},
	   {0xF4,0x10,0x1C,0x00},
	   {0xF8,0x0F,0x18,0x02},
	   {0x00,0x04,0x10,0x18},
	   {0x01,0x06,0x0F,0x14} }},
	{{ {0x00,0xE0,0x10,0x60},   /* PALFilter - 320 */
	   {0x00,0xEE,0x10,0x44},
	   {0x00,0xF4,0x10,0x38},
	   {0xF8,0xF4,0x18,0x38},
	   {0xFC,0xFB,0x14,0x2A},
	   {0x00,0x00,0x10,0x20},
	   {0x00,0x04,0x10,0x18} }},
	{{ {0xF5,0xEE,0x1B,0x44},   /* PALFilter - 640 */
	   {0xF8,0xF4,0x18,0x38},
	   {0xF1,0xF7,0x1F,0x32},
	   {0xF5,0xFB,0x1B,0x2A},
	   {0xF9,0xFF,0x17,0x22},
	   {0xFB,0x01,0x15,0x1E},
	   {0x00,0x04,0x10,0x18} }},
	{{ {0xF5,0xEE,0x1B,0x2A},   /* PALFilter - 720 */
	   {0xEE,0xFE,0x22,0x24},
	   {0xF3,0x00,0x1D,0x20},
	   {0xF9,0x03,0x17,0x1A},
	   {0xFB,0x02,0x14,0x1E},
	   {0xFB,0x04,0x15,0x18},
	   {0x00,0x06,0x10,0x14} }},
	{{ {0xF5,0xEE,0x1B,0x44},   /* PALFilter - 800/400 */
	   {0xF8,0xF4,0x18,0x38},
	   {0xFC,0xFB,0x14,0x2A},
	   {0xEB,0x05,0x25,0x16},
	   {0xF1,0x05,0x1F,0x16},
	   {0xFA,0x07,0x16,0x12},
	   {0x00,0x07,0x10,0x12} }}
};

static const struct _XGITVFilter301B {
	unsigned char filter[7][7];
} XGITVFilter301B[] = {
	{{ {0x01,0x02,0xfb,0xf8,0x06,0x27,0x3a},   /* NTSC - 640 */
	   {0x01,0x02,0xfe,0xf7,0x03,0x27,0x3c},
	   {0x01,0x01,0x00,0xf6,0x00,0x28,0x40},
	   {0xff,0x03,0x02,0xf6,0xfc,0x27,0x46},
	   {0xff,0x01,0x04,0xf8,0xfa,0x27,0x46},
	   {0xff,0x01,0x05,0xf9,0xf7,0x26,0x4a},
	   {0xff,0xff,0x05,0xfc,0xf4,0x24,0x52} }},
	{{ {0x01,0x00,0xfb,0xfb,0x0b,0x25,0x32},   /* NTSC - 720 (?) */
	   {0x01,0x01,0xfb,0xf9,0x09,0x26,0x36},
	   {0x01,0x02,0xfc,0xf8,0x06,0x27,0x38},
	   {0x01,0x02,0xfe,0xf7,0x03,0x27,0x3c},
	   {0x01,0x03,0xff,0xf6,0x00,0x27,0x40},
	   {0xff,0x03,0x02,0xf6,0xfe,0x27,0x42},
	   {0xff,0x02,0x03,0xf7,0xfb,0x27,0x46} }},
	{{ {0x01,0xfe,0xfb,0xfe,0x0e,0x23,0x2e},   /* NTSC - 800 */
	   {0x01,0xff,0xfb,0xfc,0x0c,0x25,0x30},
	   {0x01,0x00,0xfb,0xfa,0x0a,0x26,0x34},
	   {0x01,0x01,0xfc,0xf8,0x08,0x26,0x38},
	   {0x01,0x02,0xfd,0xf7,0x06,0x27,0x38},
	   {0x01,0x02,0xfe,0xf7,0x03,0x27,0x3c},
	   {0xff,0x03,0x00,0xf6,0x00,0x27,0x42} }},
	{{ {0xff,0xfd,0xfe,0x05,0x11,0x1e,0x24},   /* NTSC - 1024 */
	   {0xff,0xfd,0xfd,0x04,0x11,0x1f,0x26},
	   {0xff,0xfd,0xfc,0x02,0x10,0x22,0x28},
	   {0xff,0xff,0xfc,0x00,0x0f,0x22,0x28},
	   {0x01,0xfe,0xfb,0xff,0x0e,0x23,0x2c},
	   {0x01,0xff,0xfb,0xfd,0x0d,0x24,0x2e},
	   {0x01,0xff,0xfb,0xfb,0x0c,0x25,0x32} }},
	{{ {0x01,0x02,0xfb,0xf8,0x06,0x27,0x3a},   /* PAL - 640 */
	   {0x01,0x02,0xfe,0xf7,0x03,0x27,0x3c},
	   {0x01,0x01,0x00,0xf6,0x00,0x28,0x40},
	   {0xff,0x03,0x02,0xf6,0xfc,0x27,0x46},
	   {0xff,0x01,0x04,0xf8,0xfa,0x27,0x46},
	   {0xff,0x01,0x05,0xf9,0xf7,0x26,0x4a},
	   {0xff,0xff,0x05,0xfc,0xf4,0x24,0x52} }},
	{{ {0x01,0x00,0xfb,0xfb,0x0b,0x25,0x32},   /* PAL - 720/768 */
	   {0x01,0x01,0xfb,0xf9,0x09,0x26,0x36},
	   {0x01,0x02,0xfc,0xf8,0x06,0x27,0x38},
	   {0x01,0x02,0xfe,0xf7,0x03,0x27,0x3c},
	   {0x01,0x03,0xff,0xf6,0x00,0x27,0x40},
	   {0xff,0x03,0x02,0xf6,0xfe,0x27,0x42},
	   {0xff,0x02,0x03,0xf7,0xfb,0x27,0x46} }},
	{{ {0x01,0xfe,0xfb,0xfe,0x0e,0x23,0x2e},   /* PAL - 800 */
	   {0x01,0xff,0xfb,0xfc,0x0c,0x25,0x30},
	   {0x01,0x00,0xfb,0xfa,0x0a,0x26,0x34},
	   {0x01,0x01,0xfc,0xf8,0x08,0x26,0x38},
	   {0x01,0x02,0xfd,0xf7,0x06,0x27,0x38},
	   {0x01,0x02,0xfe,0xf7,0x03,0x27,0x3c},
	   {0xff,0x03,0x00,0xf6,0x00,0x27,0x42} }},
	{{ {0xff,0xfd,0xfe,0x05,0x11,0x1e,0x24},   /* PAL - 1024 */
	   {0xff,0xfd,0xfd,0x04,0x11,0x1f,0x26},
	   {0xff,0xfd,0xfc,0x02,0x10,0x22,0x28},
	   {0xff,0xff,0xfc,0x00,0x0f,0x22,0x28},
	   {0x01,0xfe,0xfb,0xff,0x0e,0x23,0x2c},
	   {0x01,0xff,0xfb,0xfd,0x0d,0x24,0x2e},
	   {0x01,0xff,0xfb,0xfb,0x0c,0x25,0x32} }}
};

/*     TV scaling data for video bridges
 */
typedef struct _XGITVVScale {
        unsigned short ScaleVDE;
	int sindex;
	unsigned short RealVDE;
	unsigned short reg[24];
} MyXGITVVScale, *MyXGITVVScalePtr;

static const MyXGITVVScale XGITVVScale[] = {
      { 0x01D6, 3, 480,					/* NTSC 640 */
        { 0x037C, 0x02B0, 0x00EF, 0x01FA, 0x01E7, 0x01E9,
          0x0000, 0x004C, 0x008F, 0x037C, 0x01FB, 0x00D4,
	  0x037C, 0x02CB, 0x0049, 0x01FB, 0x01EE, 0x01F0,
	  0x0000, 0x004C, 0x008F, 0x037C, 0x01FB, 0x00E0 }
      },
      { 0x01CC, 2, 480,
        { 0x0369, 0x02AD, 0x00E7, 0x01FF, 0x01E8, 0x01EB,
	  0x0000, 0x004C, 0x008F, 0x0369, 0x0200, 0x00D4,
	  0x0369, 0x02C6, 0x003A, 0x0200, 0x01F0, 0x01F3,
	  0x0000, 0x004C, 0x008F, 0x0369, 0x0200, 0x00E0 }
      },
      { 0x01C2, 1, 480,
        { 0x0356, 0x02AB, 0x00E0, 0x0204, 0x01E9, 0x01EC,
	  0x0000, 0x004C, 0x008F, 0x0356, 0x0205, 0x00D4,
	  0x0356, 0x02C1, 0x002B, 0x0205, 0x01F3, 0x01F6,
	  0x0000, 0x004C, 0x008F, 0x0356, 0x0205, 0x00E0 }
      },
      { 0x01B8, 0, 480, /* default */
        { 0x0343, 0x02A9, 0x00DA, 0x0209, 0x01EA, 0x01ED,
	  0x0000, 0x004C, 0x008F, 0x0343, 0x020A, 0x00D4,
	  0x0343, 0x02BD, 0x001F, 0x020A, 0x01F5, 0x01F8,
	  0x0000, 0x004C, 0x008F, 0x0343, 0x020A, 0x00E0 }
      },
      { 0x01AE, -1, 480,
        { 0x035B, 0x02AC, 0x00E3, 0x020E, 0x01EC, 0x01F0,
	  0x0000, 0x0050, 0x008F, 0x035B, 0x020F, 0x0152,
	  0x035B, 0x02C3, 0x0031, 0x020F, 0x01F8, 0x01FC,
	  0x0000, 0x0050, 0x008F, 0x035B, 0x020F, 0x015E }
      },
      { 0x01A4, -2, 480,
        { 0x0347, 0x02A9, 0x00DB, 0x0213, 0x01ED, 0x01F1,
	  0x0000, 0x0050, 0x008F, 0x0347, 0x0214, 0x0102,
	  0x0347, 0x02BE, 0x0022, 0x0214, 0x01FA, 0x01FE,
	  0x0000, 0x0050, 0x008F, 0x0347, 0x0214, 0x010E }
      },
      { 0x019A, -3, 480,
        { 0x0333, 0x02A7, 0x00D4, 0x0218, 0x01EE, 0x01F2,
	  0x0000, 0x0050, 0x008F, 0x0333, 0x0219, 0x016A,
	  0x0333, 0x02B9, 0x0013, 0x0219, 0x01FD, 0x0201,
	  0x0000, 0x0050, 0x008F, 0x0333, 0x0219, 0x016A }
      },
      { 0x01D6, 3, 480,					/* NTSC 720 */
        { 0x037C, 0x0307, 0x005D, 0x01FB, 0x01EE, 0x01F0,
	  0x0000, 0x004C, 0x008F, 0x037C, 0x01FB, 0x0090 }
      },
      { 0x01CC, 2, 480,
        { 0x0369, 0x0302, 0x004E, 0x0200, 0x01F0, 0x01F3,
	  0x0000, 0x004C, 0x008F, 0x0369, 0x0200, 0x0090 }
      },
      { 0x01C2, 1, 480,
        { 0x0356, 0x02FD, 0x003F, 0x0205, 0x01F3, 0x01F6,
	  0x0000, 0x004C, 0x008F, 0x0356, 0x0205, 0x0090 }
      },
      { 0x01B8, 0, 480, /* default */
        { 0x0343, 0x02F9, 0x0033, 0x020A, 0x01F5, 0x01F8,
	  0x0000, 0x004C, 0x008F, 0x0343, 0x020A, 0x0090 }
      },
      { 0x01AE, -1, 480,
        { 0x035B, 0x02FF, 0x0045, 0x020F, 0x01F8, 0x01FC,
	  0x0000, 0x0050, 0x008F, 0x035B, 0x020F, 0x010E }
      },
      { 0x01A4, -2, 480,
        { 0x0347, 0x02FA, 0x0036, 0x0214, 0x01FA, 0x01FE,
	  0x0000, 0x0050, 0x008F, 0x0347, 0x0214, 0x00BE }
      },
      { 0x019A, -3, 480,
        { 0x0333, 0x02F5, 0x0027, 0x0219, 0x01FD, 0x0201,
	  0x0000, 0x0050, 0x008F, 0x0333, 0x0219, 0x0136 }
      },
      { 0x01D6, 3, 600,					/* NTSC 800 */
        { 0x0438, 0x0353, 0x0099, 0x0272, 0x025F, 0x0261,
	  0x0000, 0x0073, 0x008F, 0x0438, 0x0273, 0x020A,
	  0x0438, 0x0372, 0x00FE, 0x0273, 0x0266, 0x0268,
	  0x0000, 0x0073, 0x008F, 0x0438, 0x0273, 0x020A }
      },
      { 0x01CC, 2, 600,
        { 0x0421, 0x0350, 0x0090, 0x0277, 0x0260, 0x0263,
	  0x0000, 0x0073, 0x008F, 0x0421, 0x0278, 0x020A,
	  0x0421, 0x036C, 0x00EC, 0x0278, 0x0268, 0x026B,
	  0x0000, 0x0073, 0x008F, 0x0421, 0x0278, 0x020A }
      },
      { 0x01C2, 1, 600,
        { 0x0413, 0x034F, 0x008C, 0x027C, 0x0261, 0x0264,
	  0x0000, 0x0074, 0x008F, 0x0413, 0x027D, 0x01FE,
	  0x0413, 0x0369, 0x00E3, 0x027D, 0x026B, 0x026E,
	  0x0000, 0x0074, 0x008F, 0x0413, 0x027D, 0x020C }
      },
      { 0x01B8, 0, 600, /* default */
        { 0x041F, 0x0350, 0x0090, 0x0281, 0x0262, 0x0265,
	  0x0000, 0x0078, 0x008F, 0x041F, 0x0282, 0x0220,
	  0x041F, 0x036C, 0x00EC, 0x0282, 0x026D, 0x0270,
	  0x0001, 0x0078, 0x008F, 0x041F, 0x0282, 0x0220 }
      },
      { 0x01AE, -1, 600,
        { 0x0407, 0x034D, 0x0087, 0x0286, 0x0264, 0x0268,
	  0x0000, 0x0078, 0x008F, 0x0407, 0x0287, 0x0220,
	  0x0407, 0x0366, 0x00DA, 0x0287, 0x0270, 0x0274,
	  0x0001, 0x0078, 0x008F, 0x0407, 0x0287, 0x0220 }
      },
      { 0x01A4, -2, 600,
        { 0x03EF, 0x034A, 0x007E, 0x028B, 0x0265, 0x0269,
	  0x0000, 0x0078, 0x008F, 0x03EF, 0x028C, 0x0220,
	  0x03EF, 0x0360, 0x00C8, 0x028C, 0x0272, 0x0276,
	  0x0001, 0x0078, 0x008F, 0x03EF, 0x028C, 0x0220 }
      },
      { 0x019A, -3, 600,
        { 0x0429, 0x0351, 0x0093, 0x0290, 0x0266, 0x026A,
	  0x0000, 0x0082, 0x008F, 0x0429, 0x0291, 0x024E,
	  0x0429, 0x036E, 0x00F2, 0x0291, 0x0275, 0x0279,
	  0x0001, 0x0082, 0x008F, 0x0429, 0x0291, 0x024E }
      },
      { 0x0230, 3, 480,					/* PAL 640 */
        { 0x0371, 0x02AE, 0x00EA, 0x01FF, 0x01E8, 0x01EB,
	  0x0000, 0x0007, 0x0010, 0x0371, 0x0200, 0x0032,
  	  0x0371, 0x02C8, 0x0040, 0x0200, 0x01F0, 0x01F3,
	  0x0000, 0x000E, 0x0020, 0x0371, 0x0200, 0x0032 }
      },
      { 0x0226, 2, 480,
        { 0x0383, 0x02B1, 0x00F2, 0x0204, 0x01E9, 0x01EC,
	  0x0000, 0x0005, 0x000B, 0x0383, 0x0205, 0x0032,
	  0x0383, 0x02CD, 0x004F, 0x0205, 0x01F3, 0x01F6,
	  0x0000, 0x0005, 0x000B, 0x0383, 0x0205, 0x0032 }
      },
      { 0x021C, 1, 480,
        { 0x035F, 0x02AC, 0x00E4, 0x0209, 0x01EA, 0x01ED,
	  0x0000, 0x0004, 0x0009, 0x035F, 0x020A, 0x0032,
	  0x035F, 0x02C4, 0x0034, 0x020A, 0x01F5, 0x01F8,
	  0x0000, 0x0004, 0x0009, 0x035F, 0x020A, 0x0032 }
      },
      { 0x0212, 0, 480, /* default */
        { 0x034F, 0x02AA, 0x00DE, 0x020E, 0x01EC, 0x01F0,
	  0x0000, 0x0004, 0x0009, 0x034F, 0x020F, 0x0032,
	  0x034F, 0x02C0, 0x0028, 0x020F, 0x01F8, 0x01FC,
	  0x0000, 0x0004, 0x0009, 0x034F, 0x020F, 0x0032 }
      },
      { 0x0208, -1, 480,
        { 0x033F, 0x02A8, 0x00D8, 0x0213, 0x01ED, 0x01F1,
	  0x0000, 0x0004, 0x0009, 0x033F, 0x0214, 0x0032,
	  0x033F, 0x02BC, 0x001C, 0x0214, 0x01FA, 0x01FE,
	  0x0000, 0x0004, 0x0009, 0x033F, 0x0214, 0x0032 }
      },
      { 0x01FE, -2, 480,
        { 0x0395, 0x02B3, 0x00F8, 0x0218, 0x01EE, 0x01F2,
	  0x0000, 0x0001, 0x0002, 0x0395, 0x0219, 0x0032,
	  0x0395, 0x02D1, 0x005B, 0x0219, 0x01FD, 0x0201,
	  0x0000, 0x0001, 0x0002, 0x0395, 0x0219, 0x0032 }
      },
      { 0x01F4, -3, 480,
        { 0x0383, 0x02B1, 0x00F2, 0x021D, 0x01EF, 0x01F3,
	  0x0000, 0x0001, 0x0002, 0x0383, 0x021E, 0x0032,
	  0x0383, 0x02CD, 0x004F, 0x021E, 0x01FF, 0x0203,
	  0x0000, 0x0001, 0x0002, 0x0383, 0x021E, 0x0032 }
      },
      { 0x0230, 2, 576,					/* PAL 720 */
        { 0x03BF, 0x0318, 0x0090, 0x0260, 0x0250, 0x0253,
	  0x0000, 0x0004, 0x0007, 0x03BF, 0x0260, 0x00E0,
	  0x6954, 0x6C6C, 0x5320, 0x666F, 0x6169, 0x4220,
	  0x7265, 0x746E, 0x7373, 0x6E6F, 0x0260, 0x00E0 }
      },
      { 0x0226, 1, 576,
        { 0x03DD, 0x031F, 0x00A5, 0x0265, 0x0253, 0x0256,
	  0x0000, 0x0003, 0x0005, 0x03DD, 0x0265, 0x013B,
	  0x7242, 0x756F, 0x6867, 0x2074, 0x6F74, 0x7920,
	  0x756F, 0x6220, 0x2079, 0x6F6E, 0x2074, 0x2061  }
      },
      { 0x021C, 0, 576, /* default */
        { 0x0437, 0x0336, 0x00EA, 0x026A, 0x0255, 0x0258,
	  0x0000, 0x0002, 0x0003, 0x0437, 0x026A, 0x0180,
	  0x656D, 0x6572, 0x5720, 0x7A69, 0x7261, 0x2064,
	  0x7562, 0x2074, 0x6874, 0x2065, 0x0274, 0x01CE }
      },
      { 0x0212, -1, 576,
        { 0x0423, 0x0331, 0x00DB, 0x026F, 0x0258, 0x025C,
	  0x0001, 0x0002, 0x0003, 0x0423, 0x026F, 0x01CA,
	  0x6957, 0x617A, 0x6472, 0x4520, 0x7478, 0x6172,
	  0x726F, 0x6964, 0x616E, 0x7269, 0x3A65, 0x01CE }
      },
      { 0x0208, -2, 576,
        { 0x040F, 0x032C, 0x00CC, 0x0274, 0x025A, 0x025E,
	  0x0000, 0x0002, 0x0003, 0x040F, 0x0274, 0x01CA,
	  0x6854, 0x6D6F, 0x7361, 0x5720, 0x6E69, 0x7369,
	  0x6863, 0x6F68, 0x6566, 0x2172, 0x027E, 0x01CA }
      },
      { 0x01FE, -3, 576,
        { 0x03FB, 0x0327, 0x00BD, 0x0279, 0x025D, 0x0261,
	  0x0000, 0x0002, 0x0003, 0x03FB, 0x0279, 0x01CA,
	   }
      },
      { 0x01F4, -4, 576,
        { 0x03E7, 0x0322, 0x00AE, 0x027E, 0x025F, 0x0263,
	  0x0000, 0x0002, 0x0003, 0x03E7, 0x027E, 0x01CA,
	  0x6854, 0x7369, 0x7320, 0x6170, 0x6563, 0x6620,
	  0x726F, 0x7320, 0x6C61, 0x0365, 0x027F, 0x01FE }
      },
      { 0x0230, 3, 600,					/* PAL 800 */
        { 0x047F, 0x035C, 0x00B4, 0x0277, 0x0260, 0x0263,
	  0x0000, 0x0005, 0x0007, 0x047F, 0x0278, 0x0170,
	  0x047F, 0x0384, 0x0034, 0x0278, 0x0268, 0x026B,
	  0x0000, 0x0005, 0x0007, 0x047F, 0x0278, 0x017E }
      },
      { 0x0226, 2, 600,
        { 0x044B, 0x0356, 0x00A1, 0x027C, 0x0261, 0x0264,
	  0x0000, 0x0019, 0x0024, 0x044B, 0x027D, 0x0150,
	  0x044B, 0x0377, 0x000D, 0x027D, 0x026B, 0x026E,
	  0x0000, 0x0019, 0x0024, 0x044B, 0x027D, 0x015E }
      },
      { 0x021C, 1, 600,
        { 0x0437, 0x0353, 0x0099, 0x0281, 0x0262, 0x0265,
	  0x0000, 0x0019, 0x0024, 0x0437, 0x0282, 0x0150,
	  0x0437, 0x0372, 0x00FE, 0x0282, 0x026D, 0x0270,
	  0x0000, 0x0019, 0x0024, 0x0437, 0x0282, 0x015E }
      },
      { 0x0212, 0, 600, /* default */
        { 0x0423, 0x0351, 0x0092, 0x0286, 0x0264, 0x0268,
	  0x0000, 0x0019, 0x0024, 0x0423, 0x0287, 0x01C8,
	  0x0423, 0x036D, 0x00EF, 0x0287, 0x0270, 0x0274,
	  0x0000, 0x0019, 0x0024, 0x0423, 0x0287, 0x01D6 }
      },
      { 0x0208, -1, 600,
        { 0x040F, 0x034E, 0x008A, 0x028B, 0x0265, 0x0269,
	  0x0000, 0x0019, 0x0024, 0x040F, 0x028C, 0x01A0,
	  0x040F, 0x0368, 0x00E0, 0x028C, 0x0272, 0x0276,
	  0x0000, 0x0019, 0x0024, 0x040F, 0x028C, 0x01AE }
      },
      { 0x01FE, -2, 600,
        { 0x03FB, 0x034C, 0x0083, 0x0290, 0x0266, 0x026A,
	  0x0000, 0x0019, 0x0024, 0x03FB, 0x0291, 0x01C8,
	  0x03FB, 0x0363, 0x00D1, 0x0291, 0x0275, 0x0279,
	  0x0000, 0x0019, 0x0024, 0x03FB, 0x0291, 0x01D6 }
      },
      { 0x01F4, -3, 600,
        { 0x0437, 0x0353, 0x0099, 0x0295, 0x0267, 0x026B,
	  0x0000, 0x0003, 0x0004, 0x0437, 0x0296, 0x01BF,
	  0x0437, 0x0372, 0x00FE, 0x0296, 0x0277, 0x027B,
	  0x0000, 0x0003, 0x0004, 0x0437, 0x0296, 0x01BA }
      },
};

static unsigned const char XGIScalingP1Regs[] = {
	0x08,0x09,0x0b,0x0c,0x0d,0x0e,0x10,0x11,0x12
};
static unsigned const char XGIScalingP4Regs[] = {
	0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b
};

static const unsigned char XGI301CScaling[] = {

    /* NTSC/PAL-M/525ip 640x480 */

    0x03,0x1C,0x03,0x7E,0x01,0x1C,0x05,0x7E,0x00,0x1C,0x06,0x7E,0x7F,0x1B,0x09,0x7D,
    0x7E,0x1A,0x0B,0x7D,0x7D,0x19,0x0D,0x7D,0x7D,0x18,0x0F,0x7C,0x7C,0x16,0x12,0x7C,
    0x7C,0x14,0x14,0x7C,0x7C,0x12,0x16,0x7C,0x7C,0x0F,0x18,0x7D,0x7C,0x0D,0x19,0x7E,
    0x7D,0x0B,0x1A,0x7E,0x7D,0x09,0x1B,0x7F,0x7E,0x06,0x1C,0x00,0x7E,0x05,0x1C,0x01,

    0x03,0x1B,0x03,0x7F,0x02,0x1B,0x05,0x7E,0x00,0x1B,0x07,0x7E,0x7F,0x1B,0x09,0x7D,
    0x7E,0x1A,0x0B,0x7D,0x7D,0x19,0x0D,0x7D,0x7D,0x17,0x10,0x7C,0x7C,0x16,0x12,0x7C,
    0x7C,0x14,0x14,0x7C,0x7C,0x12,0x16,0x7C,0x7C,0x10,0x17,0x7D,0x7C,0x0D,0x19,0x7E,
    0x7D,0x0B,0x1A,0x7E,0x7D,0x09,0x1B,0x7F,0x7E,0x07,0x1B,0x00,0x7E,0x05,0x1B,0x02,

    0x04,0x1A,0x04,0x7E,0x02,0x1B,0x05,0x7E,0x01,0x1A,0x07,0x7E,0x00,0x1A,0x09,0x7D,
    0x7F,0x19,0x0B,0x7D,0x7E,0x18,0x0D,0x7D,0x7D,0x17,0x10,0x7C,0x7D,0x15,0x12,0x7C,
    0x7C,0x14,0x14,0x7C,0x7C,0x12,0x15,0x7D,0x7C,0x10,0x17,0x7D,0x7C,0x0D,0x18,0x7F,
    0x7D,0x0B,0x19,0x7F,0x7D,0x09,0x1A,0x00,0x7D,0x07,0x1A,0x02,0x7E,0x05,0x1B,0x02,

    0x04,0x1A,0x04,0x7E,0x03,0x1A,0x06,0x7D,0x01,0x1A,0x08,0x7D,0x00,0x19,0x0A,0x7D,
    0x7F,0x19,0x0C,0x7C,0x7E,0x18,0x0E,0x7C,0x7E,0x17,0x10,0x7B,0x7D,0x15,0x12,0x7C,
    0x7D,0x13,0x13,0x7D,0x7C,0x12,0x15,0x7D,0x7C,0x10,0x17,0x7D,0x7C,0x0E,0x18,0x7E,
    0x7D,0x0C,0x19,0x7E,0x7D,0x0A,0x19,0x00,0x7D,0x08,0x1A,0x01,0x7E,0x06,0x1A,0x02,

    0x05,0x19,0x05,0x7D,0x03,0x19,0x06,0x7E,0x02,0x19,0x08,0x7D,0x01,0x19,0x0A,0x7C,
    0x00,0x18,0x0C,0x7C,0x7F,0x17,0x0E,0x7C,0x7E,0x16,0x10,0x7C,0x7D,0x15,0x11,0x7D,
    0x7D,0x13,0x13,0x7D,0x7D,0x11,0x15,0x7D,0x7D,0x10,0x16,0x7D,0x7D,0x0E,0x17,0x7E,
    0x7D,0x0C,0x18,0x7F,0x7D,0x0A,0x19,0x00,0x7D,0x08,0x19,0x02,0x7D,0x06,0x19,0x04,

    0x05,0x18,0x05,0x7E,0x04,0x19,0x07,0x7C,0x02,0x18,0x08,0x7E,0x01,0x18,0x0A,0x7D,
    0x00,0x17,0x0C,0x7D,0x7F,0x17,0x0E,0x7C,0x7E,0x15,0x0F,0x7E,0x7E,0x14,0x11,0x7D,
    0x7D,0x13,0x13,0x7D,0x7D,0x11,0x14,0x7E,0x7D,0x0F,0x15,0x7F,0x7D,0x0E,0x17,0x7E,
    0x7D,0x0C,0x17,0x00,0x7D,0x0A,0x18,0x01,0x7D,0x08,0x18,0x03,0x7D,0x07,0x19,0x03,

    0x05,0x18,0x05,0x7E,0x04,0x18,0x07,0x7D,0x03,0x18,0x09,0x7C,0x02,0x17,0x0A,0x7D,
    0x01,0x17,0x0C,0x7C,0x00,0x16,0x0E,0x7C,0x7F,0x15,0x0F,0x7D,0x7E,0x14,0x11,0x7D,
    0x7E,0x12,0x12,0x7E,0x7D,0x11,0x14,0x7E,0x7D,0x0F,0x15,0x7F,0x7D,0x0E,0x16,0x7F,
    0x7D,0x0C,0x17,0x00,0x7D,0x0A,0x17,0x02,0x7D,0x09,0x18,0x02,0x7D,0x07,0x18,0x04,

    /* NTSC/PAL-M/525ip 720x480 */

    0x03,0x1C,0x03,0x7E,0x01,0x1C,0x05,0x7E,0x00,0x1C,0x06,0x7E,0x7F,0x1B,0x09,0x7D,
    0x7E,0x1A,0x0B,0x7D,0x7D,0x19,0x0D,0x7D,0x7D,0x18,0x0F,0x7C,0x7C,0x16,0x12,0x7C,
    0x7C,0x14,0x14,0x7C,0x7C,0x12,0x16,0x7C,0x7C,0x0F,0x18,0x7D,0x7C,0x0D,0x19,0x7E,
    0x7D,0x0B,0x1A,0x7E,0x7D,0x09,0x1B,0x7F,0x7E,0x06,0x1C,0x00,0x7E,0x05,0x1C,0x01,

    0x03,0x1B,0x03,0x7F,0x02,0x1B,0x05,0x7E,0x00,0x1B,0x07,0x7E,0x7F,0x1B,0x09,0x7D,
    0x7E,0x1A,0x0B,0x7D,0x7D,0x19,0x0D,0x7D,0x7D,0x17,0x10,0x7C,0x7C,0x16,0x12,0x7C,
    0x7C,0x14,0x14,0x7C,0x7C,0x12,0x16,0x7C,0x7C,0x10,0x17,0x7D,0x7C,0x0D,0x19,0x7E,
    0x7D,0x0B,0x1A,0x7E,0x7D,0x09,0x1B,0x7F,0x7E,0x07,0x1B,0x00,0x7E,0x05,0x1B,0x02,

    0x04,0x1A,0x04,0x7E,0x02,0x1B,0x05,0x7E,0x01,0x1A,0x07,0x7E,0x00,0x1A,0x09,0x7D,
    0x7F,0x19,0x0B,0x7D,0x7E,0x18,0x0D,0x7D,0x7D,0x17,0x10,0x7C,0x7D,0x15,0x12,0x7C,
    0x7C,0x14,0x14,0x7C,0x7C,0x12,0x15,0x7D,0x7C,0x10,0x17,0x7D,0x7C,0x0D,0x18,0x7F,
    0x7D,0x0B,0x19,0x7F,0x7D,0x09,0x1A,0x00,0x7D,0x07,0x1A,0x02,0x7E,0x05,0x1B,0x02,

    0x04,0x1A,0x04,0x7E,0x03,0x1A,0x06,0x7D,0x01,0x1A,0x08,0x7D,0x00,0x19,0x0A,0x7D,
    0x7F,0x19,0x0C,0x7C,0x7E,0x18,0x0E,0x7C,0x7E,0x17,0x10,0x7B,0x7D,0x15,0x12,0x7C,
    0x7D,0x13,0x13,0x7D,0x7C,0x12,0x15,0x7D,0x7C,0x10,0x17,0x7D,0x7C,0x0E,0x18,0x7E,
    0x7D,0x0C,0x19,0x7E,0x7D,0x0A,0x19,0x00,0x7D,0x08,0x1A,0x01,0x7E,0x06,0x1A,0x02,

    0x05,0x19,0x05,0x7D,0x03,0x19,0x06,0x7E,0x02,0x19,0x08,0x7D,0x01,0x19,0x0A,0x7C,
    0x00,0x18,0x0C,0x7C,0x7F,0x17,0x0E,0x7C,0x7E,0x16,0x10,0x7C,0x7D,0x15,0x11,0x7D,
    0x7D,0x13,0x13,0x7D,0x7D,0x11,0x15,0x7D,0x7D,0x10,0x16,0x7D,0x7D,0x0E,0x17,0x7E,
    0x7D,0x0C,0x18,0x7F,0x7D,0x0A,0x19,0x00,0x7D,0x08,0x19,0x02,0x7D,0x06,0x19,0x04,

    0x05,0x18,0x05,0x7E,0x04,0x19,0x07,0x7C,0x02,0x18,0x08,0x7E,0x01,0x18,0x0A,0x7D,
    0x00,0x17,0x0C,0x7D,0x7F,0x17,0x0E,0x7C,0x7E,0x15,0x0F,0x7E,0x7E,0x14,0x11,0x7D,
    0x7D,0x13,0x13,0x7D,0x7D,0x11,0x14,0x7E,0x7D,0x0F,0x15,0x7F,0x7D,0x0E,0x17,0x7E,
    0x7D,0x0C,0x17,0x00,0x7D,0x0A,0x18,0x01,0x7D,0x08,0x18,0x03,0x7D,0x07,0x19,0x03,

    0x05,0x18,0x05,0x7E,0x04,0x18,0x07,0x7D,0x03,0x18,0x09,0x7C,0x02,0x17,0x0A,0x7D,
    0x01,0x17,0x0C,0x7C,0x00,0x16,0x0E,0x7C,0x7F,0x15,0x0F,0x7D,0x7E,0x14,0x11,0x7D,
    0x7E,0x12,0x12,0x7E,0x7D,0x11,0x14,0x7E,0x7D,0x0F,0x15,0x7F,0x7D,0x0E,0x16,0x7F,
    0x7D,0x0C,0x17,0x00,0x7D,0x0A,0x17,0x02,0x7D,0x09,0x18,0x02,0x7D,0x07,0x18,0x04,

    /* NTSC/PAL-M/525i 800x600 */

    0x07,0x15,0x07,0x7D,0x05,0x15,0x08,0x7E,0x04,0x15,0x09,0x7E,0x03,0x15,0x0B,0x7D,
    0x02,0x14,0x0C,0x7E,0x01,0x14,0x0D,0x7E,0x00,0x13,0x0F,0x7E,0x00,0x12,0x10,0x7E,
    0x7F,0x11,0x11,0x7F,0x7E,0x10,0x12,0x00,0x7E,0x0F,0x13,0x00,0x7E,0x0D,0x14,0x01,
    0x7D,0x0C,0x14,0x03,0x7D,0x0B,0x15,0x03,0x7D,0x09,0x15,0x05,0x7D,0x08,0x15,0x06,

    0x07,0x15,0x07,0x7D,0x06,0x15,0x08,0x7D,0x05,0x15,0x09,0x7D,0x04,0x14,0x0B,0x7D,
    0x03,0x14,0x0C,0x7D,0x02,0x13,0x0D,0x7E,0x01,0x13,0x0E,0x7E,0x00,0x12,0x10,0x7E,
    0x7F,0x11,0x11,0x7F,0x7F,0x10,0x12,0x7F,0x7E,0x0E,0x13,0x01,0x7E,0x0D,0x13,0x02,
    0x7E,0x0C,0x14,0x02,0x7D,0x0B,0x14,0x04,0x7D,0x09,0x15,0x05,0x7D,0x08,0x15,0x06,

    0x07,0x14,0x07,0x7E,0x06,0x14,0x08,0x7E,0x05,0x14,0x0A,0x7D,0x04,0x14,0x0B,0x7D,
    0x03,0x13,0x0C,0x7E,0x02,0x13,0x0D,0x7E,0x01,0x12,0x0E,0x7F,0x00,0x11,0x0F,0x00,
    0x00,0x10,0x10,0x00,0x7F,0x0F,0x11,0x01,0x7F,0x0E,0x12,0x01,0x7E,0x0D,0x13,0x02,
    0x7E,0x0C,0x13,0x03,0x7E,0x0B,0x14,0x03,0x7D,0x0A,0x14,0x05,0x7D,0x08,0x14,0x07,

    0x07,0x14,0x07,0x7E,0x06,0x14,0x09,0x7D,0x05,0x14,0x0A,0x7D,0x04,0x13,0x0B,0x7E,
    0x03,0x13,0x0C,0x7E,0x02,0x12,0x0D,0x7F,0x01,0x12,0x0E,0x7F,0x01,0x11,0x0F,0x7F,
    0x00,0x10,0x10,0x00,0x7F,0x0F,0x11,0x01,0x7F,0x0E,0x12,0x01,0x7E,0x0D,0x12,0x03,
    0x7E,0x0C,0x13,0x03,0x7E,0x0B,0x13,0x04,0x7E,0x0A,0x14,0x04,0x7D,0x09,0x14,0x06,

    0x08,0x13,0x08,0x7D,0x07,0x13,0x09,0x7D,0x05,0x13,0x0A,0x7E,0x04,0x13,0x0B,0x7E,
    0x04,0x12,0x0C,0x7E,0x03,0x12,0x0D,0x7E,0x02,0x11,0x0E,0x7F,0x01,0x10,0x0F,0x00,
    0x00,0x10,0x10,0x00,0x00,0x0F,0x10,0x01,0x7F,0x0E,0x11,0x02,0x7F,0x0D,0x12,0x02,
    0x7E,0x0C,0x12,0x04,0x7E,0x0B,0x13,0x04,0x7E,0x0A,0x13,0x05,0x7E,0x09,0x13,0x06,

    0x08,0x13,0x08,0x7D,0x07,0x13,0x09,0x7D,0x06,0x12,0x0A,0x7E,0x05,0x12,0x0B,0x7E,
    0x04,0x12,0x0C,0x7E,0x03,0x11,0x0D,0x7F,0x02,0x11,0x0E,0x7F,0x01,0x10,0x0E,0x01,
    0x01,0x0F,0x0F,0x01,0x00,0x0E,0x10,0x02,0x00,0x0E,0x11,0x01,0x7F,0x0D,0x11,0x03,
    0x7F,0x0C,0x12,0x03,0x7E,0x0B,0x12,0x05,0x7E,0x0A,0x12,0x06,0x7E,0x09,0x13,0x06,

    0x08,0x12,0x08,0x7E,0x07,0x12,0x09,0x7E,0x06,0x12,0x0A,0x7E,0x05,0x12,0x0B,0x7E,
    0x04,0x11,0x0C,0x7F,0x03,0x11,0x0D,0x7F,0x02,0x10,0x0D,0x01,0x02,0x10,0x0E,0x00,
    0x01,0x0F,0x0F,0x01,0x00,0x0E,0x10,0x02,0x00,0x0D,0x10,0x03,0x7F,0x0D,0x11,0x03,
    0x7F,0x0C,0x11,0x04,0x7F,0x0B,0x12,0x04,0x7E,0x0A,0x12,0x06,0x7E,0x09,0x12,0x07,

    /* PAL/PAL-N 640x480 */

    0x00,0x20,0x00,0x00,0x7F,0x20,0x02,0x7F,0x7D,0x20,0x04,0x7F,0x7D,0x1F,0x06,0x7E,
    0x7C,0x1D,0x09,0x7E,0x7C,0x1B,0x0B,0x7E,0x7C,0x19,0x0E,0x7D,0x7C,0x17,0x11,0x7C,
    0x7C,0x14,0x14,0x7C,0x7C,0x11,0x17,0x7C,0x7D,0x0E,0x19,0x7C,0x7E,0x0B,0x1B,0x7C,
    0x7E,0x09,0x1D,0x7C,0x7F,0x06,0x1F,0x7C,0x7F,0x04,0x20,0x7D,0x00,0x02,0x20,0x7E,

    0x00,0x20,0x00,0x00,0x7F,0x20,0x02,0x7F,0x7D,0x20,0x04,0x7F,0x7D,0x1F,0x06,0x7E,
    0x7C,0x1D,0x09,0x7E,0x7C,0x1B,0x0B,0x7E,0x7C,0x19,0x0E,0x7D,0x7C,0x17,0x11,0x7C,
    0x7C,0x14,0x14,0x7C,0x7C,0x11,0x17,0x7C,0x7D,0x0E,0x19,0x7C,0x7E,0x0B,0x1B,0x7C,
    0x7E,0x09,0x1D,0x7C,0x7F,0x06,0x1F,0x7C,0x7F,0x04,0x20,0x7D,0x00,0x02,0x20,0x7E,

    0x00,0x20,0x00,0x00,0x7F,0x20,0x02,0x7F,0x7D,0x20,0x04,0x7F,0x7D,0x1F,0x06,0x7E,
    0x7C,0x1D,0x09,0x7E,0x7C,0x1B,0x0B,0x7E,0x7C,0x19,0x0E,0x7D,0x7C,0x17,0x11,0x7C,
    0x7C,0x14,0x14,0x7C,0x7C,0x11,0x17,0x7C,0x7D,0x0E,0x19,0x7C,0x7E,0x0B,0x1B,0x7C,
    0x7E,0x09,0x1D,0x7C,0x7F,0x06,0x1F,0x7C,0x7F,0x04,0x20,0x7D,0x00,0x02,0x20,0x7E,

    0x00,0x20,0x00,0x00,0x7F,0x20,0x02,0x7F,0x7D,0x20,0x04,0x7F,0x7D,0x1F,0x06,0x7E,
    0x7C,0x1D,0x09,0x7E,0x7C,0x1B,0x0B,0x7E,0x7C,0x19,0x0E,0x7D,0x7C,0x17,0x11,0x7C,
    0x7C,0x14,0x14,0x7C,0x7C,0x11,0x17,0x7C,0x7D,0x0E,0x19,0x7C,0x7E,0x0B,0x1B,0x7C,
    0x7E,0x09,0x1D,0x7C,0x7F,0x06,0x1F,0x7C,0x7F,0x04,0x20,0x7D,0x00,0x02,0x20,0x7E,

    0x00,0x20,0x00,0x00,0x7F,0x20,0x02,0x7F,0x7D,0x20,0x04,0x7F,0x7D,0x1F,0x06,0x7E,
    0x7C,0x1D,0x09,0x7E,0x7C,0x1B,0x0B,0x7E,0x7C,0x19,0x0E,0x7D,0x7C,0x17,0x11,0x7C,
    0x7C,0x14,0x14,0x7C,0x7C,0x11,0x17,0x7C,0x7D,0x0E,0x19,0x7C,0x7E,0x0B,0x1B,0x7C,
    0x7E,0x09,0x1D,0x7C,0x7F,0x06,0x1F,0x7C,0x7F,0x04,0x20,0x7D,0x00,0x02,0x20,0x7E,

    0x00,0x20,0x00,0x00,0x7F,0x20,0x02,0x7F,0x7D,0x20,0x04,0x7F,0x7D,0x1F,0x06,0x7E,
    0x7C,0x1D,0x09,0x7E,0x7C,0x1B,0x0B,0x7E,0x7C,0x19,0x0E,0x7D,0x7C,0x17,0x11,0x7C,
    0x7C,0x14,0x14,0x7C,0x7C,0x11,0x17,0x7C,0x7D,0x0E,0x19,0x7C,0x7E,0x0B,0x1B,0x7C,
    0x7E,0x09,0x1D,0x7C,0x7F,0x06,0x1F,0x7C,0x7F,0x04,0x20,0x7D,0x00,0x02,0x20,0x7E,

    0x00,0x20,0x00,0x00,0x7F,0x20,0x02,0x7F,0x7D,0x20,0x04,0x7F,0x7D,0x1F,0x06,0x7E,
    0x7C,0x1D,0x09,0x7E,0x7C,0x1B,0x0B,0x7E,0x7C,0x19,0x0E,0x7D,0x7C,0x17,0x11,0x7C,
    0x7C,0x14,0x14,0x7C,0x7C,0x11,0x17,0x7C,0x7D,0x0E,0x19,0x7C,0x7E,0x0B,0x1B,0x7C,
    0x7E,0x09,0x1D,0x7C,0x7F,0x06,0x1F,0x7C,0x7F,0x04,0x20,0x7D,0x00,0x02,0x20,0x7E,

    /* PAL/PAL-N 720x576, 768x576 */

    0x03,0x1C,0x03,0x7E,0x01,0x1C,0x05,0x7E,0x00,0x1C,0x07,0x7D,0x7F,0x1B,0x09,0x7D,
    0x7E,0x1A,0x0B,0x7D,0x7D,0x19,0x0D,0x7D,0x7D,0x18,0x0F,0x7C,0x7C,0x16,0x12,0x7C,
    0x7C,0x14,0x14,0x7C,0x7C,0x12,0x16,0x7C,0x7C,0x0F,0x18,0x7D,0x7C,0x0D,0x19,0x7E,
    0x7D,0x0B,0x1A,0x7E,0x7D,0x09,0x1B,0x7F,0x7E,0x07,0x1C,0x7F,0x7E,0x05,0x1C,0x01,

    0x03,0x1B,0x03,0x7F,0x02,0x1B,0x05,0x7E,0x01,0x1B,0x07,0x7D,0x7F,0x1B,0x09,0x7D,
    0x7E,0x1A,0x0B,0x7D,0x7E,0x19,0x0D,0x7C,0x7D,0x17,0x10,0x7C,0x7C,0x16,0x12,0x7C,
    0x7C,0x14,0x14,0x7C,0x7C,0x12,0x16,0x7C,0x7C,0x10,0x17,0x7D,0x7C,0x0D,0x19,0x7E,
    0x7D,0x0B,0x1A,0x7E,0x7D,0x09,0x1B,0x7F,0x7D,0x07,0x1B,0x01,0x7E,0x05,0x1B,0x02,

    0x04,0x1A,0x04,0x7E,0x02,0x1B,0x05,0x7E,0x01,0x1A,0x07,0x7E,0x00,0x1A,0x09,0x7D,
    0x7F,0x19,0x0B,0x7D,0x7E,0x18,0x0D,0x7D,0x7D,0x17,0x10,0x7C,0x7D,0x15,0x12,0x7C,
    0x7C,0x14,0x14,0x7C,0x7C,0x12,0x15,0x7D,0x7C,0x10,0x17,0x7D,0x7C,0x0D,0x18,0x7F,
    0x7D,0x0B,0x19,0x7F,0x7D,0x09,0x1A,0x00,0x7D,0x07,0x1A,0x02,0x7E,0x05,0x1B,0x02,

    0x04,0x1A,0x04,0x7E,0x03,0x1A,0x06,0x7D,0x01,0x1A,0x08,0x7D,0x00,0x1A,0x09,0x7D,
    0x7F,0x19,0x0B,0x7D,0x7E,0x18,0x0E,0x7C,0x7E,0x17,0x10,0x7B,0x7D,0x15,0x12,0x7C,
    0x7D,0x13,0x13,0x7D,0x7C,0x12,0x15,0x7D,0x7C,0x10,0x17,0x7D,0x7C,0x0E,0x18,0x7E,
    0x7D,0x0B,0x19,0x7F,0x7D,0x09,0x1A,0x00,0x7D,0x08,0x1A,0x01,0x7E,0x06,0x1A,0x02,

    0x04,0x19,0x04,0x7F,0x03,0x19,0x06,0x7E,0x02,0x19,0x08,0x7D,0x01,0x19,0x0A,0x7C,
    0x7F,0x18,0x0C,0x7D,0x7F,0x17,0x0E,0x7C,0x7E,0x16,0x10,0x7C,0x7D,0x15,0x11,0x7D,
    0x7D,0x13,0x13,0x7D,0x7D,0x11,0x15,0x7D,0x7C,0x10,0x16,0x7E,0x7C,0x0E,0x17,0x7F,
    0x7D,0x0C,0x18,0x7F,0x7D,0x0A,0x19,0x00,0x7D,0x08,0x19,0x02,0x7D,0x06,0x19,0x04,

    0x05,0x19,0x05,0x7D,0x03,0x19,0x06,0x7E,0x02,0x19,0x08,0x7D,0x01,0x18,0x0A,0x7D,
    0x00,0x18,0x0C,0x7C,0x7F,0x17,0x0E,0x7C,0x7E,0x16,0x0F,0x7D,0x7E,0x14,0x11,0x7D,
    0x7D,0x13,0x13,0x7D,0x7D,0x11,0x14,0x7E,0x7D,0x0F,0x16,0x7E,0x7D,0x0E,0x17,0x7E,
    0x7D,0x0C,0x18,0x7F,0x7D,0x0A,0x18,0x01,0x7D,0x08,0x19,0x02,0x7D,0x06,0x19,0x04,

    0x05,0x18,0x05,0x7E,0x04,0x18,0x07,0x7D,0x03,0x18,0x08,0x7D,0x01,0x18,0x0A,0x7D,
    0x00,0x17,0x0C,0x7D,0x7F,0x16,0x0E,0x7D,0x7F,0x15,0x0F,0x7D,0x7E,0x14,0x11,0x7D,
    0x7D,0x13,0x13,0x7D,0x7D,0x11,0x14,0x7E,0x7D,0x0F,0x15,0x7F,0x7D,0x0E,0x16,0x7F,
    0x7D,0x0C,0x17,0x00,0x7D,0x0A,0x18,0x01,0x7D,0x08,0x18,0x03,0x7D,0x07,0x18,0x04,

    /* PAL/PAL-N 800x600 */

    0x04,0x1A,0x04,0x7E,0x02,0x1A,0x05,0x7F,0x01,0x1A,0x07,0x7E,0x00,0x1A,0x09,0x7D,
    0x7F,0x19,0x0B,0x7D,0x7E,0x18,0x0D,0x7D,0x7D,0x17,0x10,0x7C,0x7D,0x15,0x12,0x7C,
    0x7C,0x14,0x14,0x7C,0x7C,0x12,0x15,0x7D,0x7C,0x10,0x17,0x7D,0x7C,0x0D,0x18,0x7F,
    0x7D,0x0B,0x19,0x7F,0x7D,0x09,0x1A,0x00,0x7D,0x07,0x1A,0x02,0x7E,0x05,0x1A,0x03,

    0x04,0x1A,0x04,0x7E,0x03,0x1A,0x06,0x7D,0x01,0x1A,0x08,0x7D,0x00,0x19,0x0A,0x7D,
    0x7F,0x19,0x0C,0x7C,0x7E,0x18,0x0E,0x7C,0x7E,0x17,0x10,0x7B,0x7D,0x15,0x12,0x7C,
    0x7D,0x13,0x13,0x7D,0x7C,0x12,0x15,0x7D,0x7C,0x10,0x17,0x7D,0x7C,0x0E,0x18,0x7E,
    0x7D,0x0C,0x19,0x7E,0x7D,0x0A,0x19,0x00,0x7D,0x08,0x1A,0x01,0x7E,0x06,0x1A,0x02,

    0x05,0x19,0x05,0x7D,0x03,0x19,0x06,0x7E,0x02,0x19,0x08,0x7D,0x01,0x19,0x0A,0x7C,
    0x00,0x18,0x0C,0x7C,0x7F,0x17,0x0E,0x7C,0x7E,0x16,0x10,0x7C,0x7D,0x15,0x11,0x7D,
    0x7D,0x13,0x13,0x7D,0x7D,0x11,0x15,0x7D,0x7C,0x10,0x16,0x7E,0x7C,0x0E,0x17,0x7F,
    0x7D,0x0C,0x18,0x7F,0x7D,0x0A,0x19,0x00,0x7D,0x08,0x19,0x02,0x7D,0x06,0x19,0x04,

    0x05,0x19,0x05,0x7D,0x03,0x19,0x06,0x7E,0x02,0x19,0x08,0x7D,0x01,0x18,0x0A,0x7D,
    0x00,0x18,0x0C,0x7C,0x7F,0x17,0x0E,0x7C,0x7E,0x16,0x0F,0x7D,0x7E,0x14,0x11,0x7D,
    0x7D,0x13,0x13,0x7D,0x7D,0x11,0x14,0x7E,0x7D,0x0F,0x16,0x7E,0x7D,0x0E,0x17,0x7E,
    0x7D,0x0C,0x18,0x7F,0x7D,0x0A,0x18,0x01,0x7D,0x08,0x19,0x02,0x7D,0x06,0x19,0x04,

    0x05,0x18,0x05,0x7E,0x04,0x18,0x07,0x7D,0x03,0x18,0x08,0x7D,0x01,0x18,0x0A,0x7D,
    0x00,0x17,0x0C,0x7D,0x7F,0x16,0x0E,0x7D,0x7F,0x15,0x0F,0x7D,0x7E,0x14,0x11,0x7D,
    0x7D,0x13,0x13,0x7D,0x7D,0x11,0x14,0x7E,0x7D,0x0F,0x15,0x7F,0x7D,0x0E,0x16,0x7F,
    0x7D,0x0C,0x17,0x00,0x7D,0x0A,0x18,0x01,0x7D,0x08,0x18,0x03,0x7D,0x07,0x18,0x04,

    0x06,0x18,0x06,0x7C,0x04,0x18,0x07,0x7D,0x03,0x18,0x09,0x7C,0x02,0x17,0x0A,0x7D,
    0x01,0x17,0x0C,0x7C,0x00,0x16,0x0E,0x7C,0x7F,0x15,0x0F,0x7D,0x7E,0x14,0x11,0x7D,
    0x7E,0x12,0x12,0x7E,0x7D,0x11,0x14,0x7E,0x7D,0x0F,0x15,0x7F,0x7D,0x0E,0x16,0x7F,
    0x7D,0x0C,0x17,0x00,0x7D,0x0A,0x17,0x02,0x7D,0x09,0x18,0x02,0x7D,0x07,0x18,0x04,

    0x06,0x17,0x06,0x7D,0x05,0x17,0x07,0x7D,0x03,0x17,0x09,0x7D,0x02,0x17,0x0A,0x7D,
    0x01,0x16,0x0C,0x7D,0x00,0x15,0x0E,0x7D,0x7F,0x14,0x0F,0x7E,0x7F,0x13,0x11,0x7D,
    0x7E,0x12,0x12,0x7E,0x7E,0x11,0x13,0x7E,0x7D,0x0F,0x14,0x00,0x7D,0x0E,0x15,0x00,
    0x7D,0x0C,0x16,0x01,0x7D,0x0A,0x17,0x02,0x7D,0x09,0x17,0x03,0x7D,0x07,0x17,0x05
};


/* Mandatory functions */
static void XGIIdentify(int flags);
static Bool XGIPreInit(ScrnInfoPtr pScrn, int flags);
static Bool XGIScreenInit(ScreenPtr pScreen, int argc, char **argv);
static Bool XGIEnterVT(ScrnInfoPtr pScrn);
static void XGILeaveVT(ScrnInfoPtr pScrn);
static Bool XGICloseScreen(ScreenPtr pScreen);
static Bool XGISaveScreen(ScreenPtr pScreen, int mode);
static Bool XGISwitchMode(ScrnInfoPtr pScrn, DisplayModePtr mode);
static void XGIAdjustFrame(ScrnInfoPtr pScrn, int x, int y);
static Bool XGISaveScreenDH(ScreenPtr pScreen, int mode);

/* Optional functions */
static void XGIFreeScreen(ScrnInfoPtr pScreen);

static int      XGIValidMode(ScrnInfoPtr arg, DisplayModePtr mode, Bool verbose,
                             int flags);
/* Internally used functions */
static Bool    XGIMapMem(ScrnInfoPtr pScrn);
static Bool    XGIUnmapMem(ScrnInfoPtr pScrn);
static void    XGISave(ScrnInfoPtr pScrn);
static void    XGIRestore(ScrnInfoPtr pScrn);
static Bool    XGIModeInit(ScrnInfoPtr pScrn, DisplayModePtr mode);
static void    XGIModifyModeInfo(DisplayModePtr mode);
static void    XGIPreSetMode(ScrnInfoPtr pScrn, DisplayModePtr mode, int viewmode);
static void    XGIPostSetMode(ScrnInfoPtr pScrn, XGIRegPtr xgiReg);

USHORT XGI_CalcModeIndex(ScrnInfoPtr pScrn, DisplayModePtr mode,
    unsigned long VBFlags);
unsigned char  XGI_GetSetBIOSScratch(ScrnInfoPtr pScrn, USHORT offset, unsigned char value);
#ifdef DEBUG
static void    XGIDumpModeInfo(ScrnInfoPtr pScrn, DisplayModePtr mode);
#endif

extern BOOLEAN XGIBIOSSetMode(VB_DEVICE_INFO *XGI_Pr,
    PXGI_HW_DEVICE_INFO HwDeviceExtension, ScrnInfoPtr pScrn,
    DisplayModePtr mode);

extern void     XGI_New_GetVBType(VB_DEVICE_INFO *XGI_Pr, PXGI_HW_DEVICE_INFO);

extern BOOLEAN 	XGIBIOSSetModeCRT1(VB_DEVICE_INFO *XGI_Pr,
    PXGI_HW_DEVICE_INFO HwDeviceExtension, ScrnInfoPtr pScrn,
    DisplayModePtr mode);
