/*
 * XGI memcpy() routines (assembly)
 *
 * Copyright (C) 2004-2005 Thomas Winischhofer
 *
 * Idea and some code bits from via_memcpy.c which is
 * Copyright (C) 2004 Thomas Hellstroem, All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sub license,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE CODE SUPPLIER(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */
#include "config.h"

/* #define DEBUG */
#ifdef  DEBUG
#define PDEBUG(p)     p  /* RUNTIMEDEBUG(p) */
#else
#define PDEBUG(p)
#endif

/* Jong 01/15/2009; CPU flags (for memcpy() etc.) */
#define XGI_CPUFL_LIBC  0x0001
#define XGI_CPUFL_BI    0x0002
#define XGI_CPUFL_SSE   0x0004
#define XGI_CPUFL_MMX   0x0008
#define XGI_CPUFL_3DNOW 0x0010
#define XGI_CPUFL_MMX2  0x0020
#define XGI_CPUFL_BI2   0x0040
#define XGI_CPUFL_SSE2  0x0080
#define XGI_CPUFL_FLAG  0x8000

#define CPUBUFFERSIZE 2048       /* Size of /proc/cpuinfo buffer */
#define BUFFERSIZE (576 * 1152)  /* Matches 720x576 YUV420 */

/************************************************************************/
/*                   arch specific memcpy() routines                    */
/************************************************************************/

/* i386, AMD64 */

#define FENCE 			\
     __asm__ __volatile__( 	\
		  " sfence\n" 	\
		  :		\
		  :		\
		  : "memory");

#define FENCEMMS 		\
     __asm__ __volatile__ (	\
		  " sfence\n"	\
		  " emms\n"	\
		  :		\
		  :		\
		  : "memory");

#define FEMMS 			\
     __asm__ __volatile__(	\
		  " femms\n"	\
		  :		\
		  :		\
		  : "memory");

#define EMMS 			\
     __asm__ __volatile__(	\
		  " emms\n"	\
		  :		\
		  :		\
		  : "memory");

#define SSE_PREFETCH " prefetchnta "
#define NOW_PREFETCH " prefetch "

#define PREFETCH1(arch_prefetch,from)		\
    __asm__ __volatile__ (			\
		  arch_prefetch "(%0)\n"	\
		  arch_prefetch "32(%0)\n"	\
		  arch_prefetch "64(%0)\n"	\
		  arch_prefetch "96(%0)\n"	\
		  arch_prefetch "128(%0)\n"	\
		  arch_prefetch "160(%0)\n"	\
		  arch_prefetch "192(%0)\n"	\
		  arch_prefetch "256(%0)\n"	\
		  arch_prefetch "288(%0)\n"	\
		  : 				\
		  : "r" (from) );

#define PREFETCH2(arch_prefetch,from)		\
    __asm__ __volatile__ (			\
		  arch_prefetch "320(%0)\n"	\
		  : 				\
		  : "r" (from) );

#define PREFETCH3(arch_prefetch,from)		\
    __asm__ __volatile__ (			\
		  arch_prefetch "288(%0)\n"	\
		  : 				\
		  : "r" (from) );

#define small_memcpy_i386(to,from,n)					\
    {									\
	__asm__ __volatile__(						\
		  " cld\n"						\
		  " shrl $1, %%ecx\n"					\
		  " jnc 1f\n"						\
		  " movsb\n"						\
		"1: shrl $1, %%ecx\n"					\
		  " jnc 2f\n"						\
		  " movsw\n"						\
		"2: rep ; movsl"					\
		  : "=&D" (to), "=&S" (from)				\
		  : "c" (n), "0" ((long) to), "1" ((long) from) 	\
		  : "memory", "cc");					\
      PDEBUG(ErrorF("Jong-small_memcpy_i386(to,from,n)...\n"));		\
    }

#define small_memcpy_amd64(to,from,n)					\
    {									\
	__asm__ __volatile__(						\
		  " cld\n"						\
		  " shrq $1, %%rcx\n"					\
		  " jnc 1f\n"						\
		  " movsb\n"						\
		"1: shrq $1, %%rcx\n"					\
		  " jnc 2f\n"						\
		  " movsw\n"						\
		"2: shrq $1, %%rcx\n"					\
		  " jnc 3f\n"						\
		  " movsl\n"						\
		"3: rep ; movsq"					\
		  : "=&D" (to), "=&S" (from)				\
		  : "c" (n), "0" ((long) to), "1" ((long) from) 	\
		  : "memory", "cc");					\
      PDEBUG(ErrorF("Jong-small_memcpy_amd64(to,from,n)...\n"));		\
    }

#define MMX_CPY(prefetch,from,to,dummy,lcnt)				\
    __asm__ __volatile__ (						\
	        "1:\n"							\
		    prefetch "320(%1)\n"				\
	          " movq (%1), %%mm0\n"					\
		  " movq 8(%1), %%mm1\n"				\
		  " movq 16(%1), %%mm2\n"				\
		  " movq 24(%1), %%mm3\n"				\
		  " movq %%mm0, (%0)\n"					\
		  " movq %%mm1, 8(%0)\n"				\
		  " movq %%mm2, 16(%0)\n"				\
		  " movq %%mm3, 24(%0)\n"				\
		    prefetch "352(%1)\n"				\
		  " movq 32(%1), %%mm0\n"				\
		  " movq 40(%1), %%mm1\n"				\
		  " movq 48(%1), %%mm2\n"				\
		  " movq 56(%1), %%mm3\n"				\
		  " leal 64(%1),%1\n"					\
		  " movq %%mm0, 32(%0)\n"				\
		  " movq %%mm1, 40(%0)\n"				\
		  " movq %%mm2, 48(%0)\n"				\
		  " movq %%mm3, 56(%0)\n"				\
		  " decl %2\n"						\
		  " leal 64(%0),%0\n"					\
		  " jne 1b\n"						\
		  : "=&D"(to), "=&S"(from), "=&r"(dummy)		\
		  : "0" (to), "1" (from), "2" (lcnt) 			\
		  : "memory", "cc");	\
        PDEBUG(ErrorF("Jong-MMX_CPY(prefetch,from,to,dummy,lcnt)...\n"));


#define SSE_CPY(prefetch,from,to,dummy,lcnt)				\
    if((ULong) from & 15) {						\
	__asm__ __volatile__ (						\
		"1:\n"							\
		    prefetch "320(%1)\n"				\
		  " movups (%1), %%xmm0\n"				\
		  " movups 16(%1), %%xmm1\n"				\
		  " movntps %%xmm0, (%0)\n"				\
		  " movntps %%xmm1, 16(%0)\n"				\
		    prefetch "352(%1)\n"				\
		  " movups 32(%1), %%xmm2\n"				\
		  " movups 48(%1), %%xmm3\n"				\
		  " leal 64(%1),%1\n"					\
		  " movntps %%xmm2, 32(%0)\n"				\
		  " movntps %%xmm3, 48(%0)\n"				\
		  " decl %2\n"						\
		  " leal 64(%0),%0\n"					\
		  " jne 1b\n"						\
		  : "=&D"(to), "=&S"(from), "=&r"(dummy)		\
		  : "0" (to), "1" (from), "2" (lcnt)			\
		  : "memory", "cc"); 					\
	      PDEBUG(ErrorF("Jong-SSE_CPY(prefetch,from,to,dummy,lcnt)-1...\n")); \
    } else {								\
	__asm__ __volatile__ (						\
		"2:\n"							\
		    prefetch "320(%1)\n"				\
		  " movaps (%1), %%xmm0\n"				\
		  " movaps 16(%1), %%xmm1\n"				\
		  " movntps %%xmm0, (%0)\n"				\
		  " movntps %%xmm1, 16(%0)\n"				\
        	    prefetch "352(%1)\n"				\
		  " movaps 32(%1), %%xmm2\n"				\
		  " movaps 48(%1), %%xmm3\n"				\
		  " leal 64(%1),%1\n"					\
		  " movntps %%xmm2, 32(%0)\n"				\
		  " movntps %%xmm3, 48(%0)\n"				\
		  " decl %2\n"						\
		  " leal 64(%0),%0\n"					\
		  " jne 2b\n"						\
		  : "=&D"(to), "=&S"(from), "=&r"(dummy)		\
		  : "0" (to), "1" (from), "2" (lcnt)			\
		  : "memory", "cc");					\
	      PDEBUG(ErrorF("Jong-SSE_CPY(prefetch,from,to,dummy,lcnt)-2...\n")); \
    }

#define SSE64_CPY(prefetch,from,to,dummy,lcnt)				\
    if((ULong) from & 15) {						\
	__asm__ __volatile__ (						\
		"1:\n"							\
		    prefetch "320(%1)\n"				\
		  " movups (%1), %%xmm0\n"				\
		  " movups 16(%1), %%xmm1\n"				\
		  " movntps %%xmm0, (%0)\n"				\
		  " movntps %%xmm1, 16(%0)\n"				\
		    prefetch "352(%1)\n"				\
		  " movups 32(%1), %%xmm2\n"				\
		  " movups 48(%1), %%xmm3\n"				\
		  " leaq 64(%1),%1\n"					\
		  " movntps %%xmm2, 32(%0)\n"				\
		  " movntps %%xmm3, 48(%0)\n"				\
		  " decl %2\n"						\
		  " leaq 64(%0),%0\n"					\
		  " jne 1b\n"						\
		  : "=&D"(to), "=&S"(from), "=&r"(dummy)		\
		  : "0" (to), "1" (from), "2" (lcnt)			\
		  : "memory", "cc"); 					\
	      PDEBUG(ErrorF("Jong-SSE64_CPY(prefetch,from,to,dummy,lcnt)-1...\n")); \
    } else {								\
	__asm__ __volatile__ (						\
		"2:\n"							\
		    prefetch "320(%1)\n"				\
		  " movaps (%1), %%xmm0\n"				\
		  " movaps 16(%1), %%xmm1\n"				\
		  " movntps %%xmm0, (%0)\n"				\
		  " movntps %%xmm1, 16(%0)\n"				\
        	    prefetch "352(%1)\n"				\
		  " movaps 32(%1), %%xmm2\n"				\
		  " movaps 48(%1), %%xmm3\n"				\
		  " leaq 64(%1),%1\n"					\
		  " movntps %%xmm2, 32(%0)\n"				\
		  " movntps %%xmm3, 48(%0)\n"				\
		  " decl %2\n"						\
		  " leaq 64(%0),%0\n"					\
		  " jne 2b\n"						\
		  : "=&D"(to), "=&S"(from), "=&r"(dummy)		\
		  : "0" (to), "1" (from), "2" (lcnt)			\
		  : "memory", "cc");					\
	      PDEBUG(ErrorF("Jong-SSE64_CPY(prefetch,from,to,dummy,lcnt)-2...\n")); \
    }

#define MMXEXT_CPY(prefetch,from,to,dummy,lcnt)				\
    __asm__ __volatile__ (						\
		  ".p2align 4,,7\n"					\
		 "1:\n"							\
		    prefetch "320(%1)\n"				\
		  " movq (%1), %%mm0\n"					\
		  " movq 8(%1), %%mm1\n"				\
		  " movq 16(%1), %%mm2\n"				\
		  " movq 24(%1), %%mm3\n"				\
		  " movntq %%mm0, (%0)\n"				\
		  " movntq %%mm1, 8(%0)\n"				\
		  " movntq %%mm2, 16(%0)\n"				\
		  " movntq %%mm3, 24(%0)\n"				\
		    prefetch "352(%1)\n"				\
		  " movq 32(%1), %%mm0\n"				\
		  " movq 40(%1), %%mm1\n"				\
		  " movq 48(%1), %%mm2\n"				\
		  " movq 56(%1), %%mm3\n"				\
		  " leal 64(%1),%1\n"					\
		  " movntq %%mm0, 32(%0)\n"				\
		  " movntq %%mm1, 40(%0)\n"				\
		  " movntq %%mm2, 48(%0)\n"				\
		  " movntq %%mm3, 56(%0)\n"				\
		  " decl %2\n"						\
		  " leal 64(%0),%0\n"					\
		  " jne 1b\n"						\
		  : "=&D"(to), "=&S"(from), "=&r"(dummy)		\
		  : "0" (to), "1" (from), "2" (lcnt) 			\
		  : "memory", "cc");	\
	      PDEBUG(ErrorF("Jong-MMXEXT_CPY(prefetch,from,to,dummy,lcnt)...\n")); 


#define PREFETCH_FUNC(prefix,itype,ptype,begin,fence,small)		\
									\
/*    static void prefix##_memcpy(UChar *to,		*/		\
    void prefix##_memcpy(UChar *to,				\
				const UChar *from,			\
				int size)				\
    {									\
	int lcnt = size >> 6;						\
	int rest = size & 63;						\
	register int dummy;						\
								\
	PREFETCH1(ptype##_PREFETCH,from);				\
	begin;								\
    PDEBUG(ErrorF("Jong-After-begin()...\n"));	\
	if(lcnt) {							\
	   PDEBUG(ErrorF("Jong-Before-SSE_CPY()...\n"));		\
	   itype##_CPY(ptype##_PREFETCH,from,to,dummy,lcnt);		\
	   PDEBUG(ErrorF("Jong-After-SSE_CPY()...\n"));		\
	}								\
	if(rest) {							\
	   PDEBUG(ErrorF("Jong-Before-PREFETCH2()...\n"));	\
	   PREFETCH2(ptype##_PREFETCH,from);				\
	   PDEBUG(ErrorF("Jong-After-PREFETCH2()...\n"));	\
	   small(to, from, rest);					\
	   PDEBUG(ErrorF("Jong-After-small...\n"));		\
	   PREFETCH3(ptype##_PREFETCH,from);				\
	   PDEBUG(ErrorF("Jong-After-PREFETCH3()...\n"));	\
	}								\
    PDEBUG(ErrorF("Jong-Before-fence...\n"));	\
	fence;								\
    PDEBUG(ErrorF("Jong-PREFETCH_FUNC(prefix,itype,ptype,begin,fence,small)-end...\n"));	\
    }

#define NOPREFETCH_FUNC(prefix,itype,begin,fence,small)			\
									\
    static void prefix##_memcpy(UChar *to,				\
				const UChar *from,			\
				int size)				\
    {									\
	int lcnt = size >> 6;						\
	int rest = size & 63;						\
	register int dummy;						\
									\
	begin;								\
	if(lcnt) {							\
	   itype##_CPY("#",from,to,dummy,lcnt);				\
	}								\
	if(rest) {							\
	   small(to, from, rest);					\
	}								\
	fence;								\
    PDEBUG(ErrorF("Jong-NOPREFETCH_FUNC(prefix,itype,ptype,begin,fence,small)...\n"));	\
    }

/* Other archs */

/************************************************************************/
/*                   libc memcpy() wrapper - generic                    */
/************************************************************************/
#define UChar  unsigned char

void XGI_libc_memcpy(UChar *dst, const UChar *src, int size)
{
    PDEBUG(ErrorF("Jong-XGI_libc_memcpy()...\n"));
    memcpy(dst, src, size);
}

/************************************************************************/
/* We only do all that stuff under gcc; no idea what other compilers 	*/
/* would do with our asm code.  					*/
/************************************************************************/

#ifndef __GNUC__

unsigned int XGIGetCPUFlags(ScrnInfoPtr pScrn)
{
    PDEBUG(ErrorF("Jong-XGIGetCPUFlags(ScrnInfoPtr pScrn)...\n"));
    return 0;
}

vidCopyFunc XGIVidCopyInit(ScreenPtr pScreen, vidCopyFunc *UMemCpy, Bool from)
{
    PDEBUG(ErrorF("Jong-XGIVidCopyInit()...\n"));
    *UMemCpy = XGI_libc_memcpy;
    return XGI_libc_memcpy;
}

vidCopyFunc XGIVidCopyGetDefault(void)
{
    PDEBUG(ErrorF("Jong-XGIVidCopyGetDefault()...\n"));
    return XGI_libc_memcpy;
}

#else /* ! Everything below is gcc specific ! */

/************************************************************************/
/*                    Definitions for archs and OSes                    */
/************************************************************************/

#undef XGI_checkosforsse
#undef XGI_canBenchmark
#undef XGI_haveProc
#undef XGI_haveBuiltInMC

/* Jong Lin */
#if defined(__arm__) 
void XGI_builtin_memcpy_arm(UChar *to, const UChar *from, int n)
{
    long d1,d2,d3;

    PDEBUG(ErrorF("XGI_builtin_memcpy_arm_begin-size=%d...\n", n));

    __asm__  __volatile__(
	" start: \n"
		   /* " stmfd sp!, {r0-r12}\n" */ /* Save some working registers */
	" blockcopy: \n"
		   " movs r3,r2, lsr #3\n" /* Number of eight word multiples */
		   " beq copywords\n" /* Less than eight words to move? */
		   " stmfd sp!, {r4-r11}\n" /* Save some working registers */
	" octcopy: \n"
		   " ldmia r1!, {r4-r11}\n" /* Load 8 words from the source */
		   " stmia r0!, {r4-r11}\n" /* Put them at the destination */
		   " subs r3, r3, #1\n" /* Decrement the counter */
		   " bne octcopy\n" /* ... copy more */
		   " ldmfd sp!, {r4-r11}\n" /* Don't need these now - restore */
	" copywords: \n"
		   " ands r2, r2, #7\n" /* Number of odd words to copy */
		   " beq stop\n" /* No words left to copy? */
	" wordcopy: \n"
		  " ldr r3, [r1], #4\n" /* Load a word from the source */
		  " str r3, [r0], #4\n" /* and store it to the destination */
		  " subs r2, r2, #1\n" /* Decrement the counter */
		  " bne wordcopy\n" /*  ... copy more */
	" stop: \n"
		  /* "ldmfd sp!, {r0-r12}\n" */ /* Don't need these now - restore */
		  : 
		  :"r" ((long) to), "r" ((long) from), "r" ((unsigned long) n)
		  : "memory", "r3");

}
#endif

#if defined(__i386__) /* ***************************************** i386 */

#define XGI_checkosforsse 	/* Does this cpu support sse and do we need to check os? */
#define XGI_canBenchmark	/* Can we perform a benchmark? */
#ifdef XGI_LINUX
#define XGI_haveProc		/* Do we have /proc/cpuinfo or similar? */
#endif
#define XGI_haveBuiltInMC	/* Is there a built-in memcpy for this arch? */

/* Built-in memcpy for i386 */
#define size_t	int
static __inline void * builtin_memcpy(void * to, const void * from, size_t n)
{
    int d1,d2,d3;

    __asm__ __volatile__(
		  " cld\n"
		  " shrl $1, %%ecx\n"
		  " jnc 1f\n"
		  " movsb\n"
		"1: shrl $1, %%ecx\n"
		  " jnc 2f\n"
		  " movsw\n"
		"2: rep ; movsl\n"
		  : "=&c" (d1), "=&D" (d2), "=&S" (d3)
		  : "0" (n), "1" ((long) to), "2" ((long) from)
		  : "memory", "cc");

    PDEBUG(ErrorF("Jong-__inline builtin_memcpy() for i386...\n"));
    return(to);
}

/* Alternative for 586: Unroll loop, copy 32 bytes at a time */
static void XGI_builtin_memcp2(UChar *to, const UChar *from, int n)
{
    int d1,d2,d3;

    __asm__ __volatile__(
		  " movl %%edi, %%eax\n"
		  " cmpl $32, %%ecx\n"
		  " cld\n"
		  " jbe 3f\n"
		  " negl %%eax\n"		/* Align dest */
		  " andl $3, %%eax\n"
		  " subl %%eax, %%ecx\n"
		  " xchgl %%eax, %%ecx\n"
		  " rep ; movsb\n"
		  " movl %%eax, %%ecx\n"
		  " subl $32, %%ecx\n"
		  " js 2f\n"
		  " movl (%%edi), %%eax\n"
		"1: movl 28(%%edi), %%edx\n"   	/* Trick: Read-ahead */
		  " subl $32, %%ecx\n"
		  " movl (%%esi), %%eax\n"
		  " movl 4(%%esi), %%edx\n"
		  " movl %%eax, (%%edi)\n"
		  " movl %%edx, 4(%%edi)\n"
		  " movl 8(%%esi), %%eax\n"
		  " movl 12(%%esi), %%edx\n"
		  " movl %%eax, 8(%%edi)\n"
		  " movl %%edx, 12(%%edi)\n"
		  " movl 16(%%esi), %%eax\n"
		  " movl 20(%%esi), %%edx\n"
		  " movl %%eax, 16(%%edi)\n"
		  " movl %%edx, 20(%%edi)\n"
		  " movl 24(%%esi), %%eax\n"
		  " movl 28(%%esi), %%edx\n"
		  " movl %%eax, 24(%%edi)\n"
		  " movl %%edx, 28(%%edi)\n"
		  " leal 32(%%esi), %%esi\n"
		  " leal 32(%%edi), %%edi\n"
		  " jns 1b\n"
		"2: addl $32, %%ecx\n"
		"3: rep ; movsb"
		  : "=&c" (d1), "=&D" (d2), "=&S" (d3)
		  : "0" (n), "1" ((long) to), "2" ((long) from)
		  : "eax", "edx", "memory", "cc");

    PDEBUG(ErrorF("Jong-XGI_builtin_memcp2()-copy 32 bytes at a time-586...\n"));
}

static unsigned int taketime(void)	/* get current time (for benchmarking) */
{
    unsigned int eax;

    __asm__ volatile (
		" pushl %%ebx\n"
		" cpuid\n"
		" rdtsc\n"
		" popl %%ebx\n"
		: "=a" (eax)
		: "0" (0)
		: "ecx", "edx", "cc");

    return(eax);
}

#elif defined(__AMD64__) || defined(__amd64__) || defined(__x86_64__) /***************** AMD64 */

#define XGI_checkosforsse	/* Does this cpu support sse and do we need to check os? */
#define XGI_canBenchmark	/* Can we perform a benchmark? */
#ifdef XGI_LINUX
#define XGI_haveProc		/* Do we have /proc/cpuinfo or similar? */
#endif
#define XGI_haveBuiltInMC	/* Is there a built-in memcpy for this arch? */

/* Built-in memcpy for AMD64 */
static __inline void * builtin_memcpy(void * to, const void * from, int n)
{
    long d1, d2, d3;

    __asm__ __volatile__ (
		" cld\n"
		" rep ; movsq\n"
		" movq %4, %%rcx\n"
		" rep ; movsb"
		: "=%c" (d1), "=&D" (d2), "=&S" (d3)
		: "0" ((ULong)(n >> 3)), "q" ((ULong)(n & 7)),
		  "1" ((long) to), "2" ((long) from)
		: "memory");

    PDEBUG(ErrorF("Jong-builtin_memcpy() for AMD64...\n"));
    return(to);
}

/* Alternative: Unroll loop, copy 32 bytes at a time */
static void XGI_builtin_memcp2(UChar *to, const UChar *from, int n)
{
    long d1,d2,d3;

    __asm__ __volatile__(
		  " movq %%rdi, %%rax\n"
		  " cmpq $32, %%rcx\n"
		  " cld\n"			/* Pipeline; no other flags but DF */
		  " jbe 1f\n"
		  " negq %%rax\n"		/* Align dest */
		  " andq $7, %%rax\n"
		  " subq %%rax, %%rcx\n"
		  " xchgq %%rax, %%rcx\n"
		  " rep ; movsb\n"
		  " movq %%rax, %%rcx\n"
		  " subq $32, %%rcx\n"
		  " js 2f\n"
		  ".p2align 4\n"
		"3: subq $32, %%rcx\n"
		  " movq (%%rsi), %%rax\n"
		  " movq 8(%%rsi), %%rdx\n"
		  " movq 16(%%rsi), %%r8\n"
		  " movq 24(%%rsi), %%r9\n"
		  " movq %%rax, (%%rdi)\n"
		  " movq %%rdx, 8(%%rdi)\n"
		  " movq %%r8, 16(%%rdi)\n"
		  " movq %%r9, 24(%%rdi)\n"
		  " leaq 32(%%rsi), %%rsi\n"
		  " leaq 32(%%rdi), %%rdi\n"
		  " jns 3b\n"
		"2: addq $32, %%rcx\n"
		"1: rep ; movsb"
		  : "=&c" (d1), "=&D" (d2), "=&S" (d3)
		  :"0" ((ULong) n), "1" ((long) to), "2" ((long) from)
		  : "rax", "rdx", "r8", "r9", "memory", "cc");

    PDEBUG(ErrorF("Jong-XGI_builtin_memcp2()-copy 32 bytes at a time-AMD...\n"));
}

static unsigned int taketime(void)	/* get current time (for benchmarking) */
{
    unsigned int eax;

    __asm__ volatile (
		" pushq %%rbx\n"
		" cpuid\n"
		" rdtsc\n"
		" popq %%rbx\n"
		: "=a" (eax)
		: "0" (0)
		: "rcx", "rdx", "cc");

    return(eax);
}

#else		/* **************************************** Other archs */

/* 1. Can we do a benchmark?		*/
/* #define XGI_canBenchmark		*/

/* 2. Do we have /proc filesystem or similar for CPU information? */
/* #define XGI_haveproc			*/

/* 3. Optional: built-in memcpy()	*/
/* #define XGI_haveBuiltInMC		*/
/* static __inline void * builtin_memcpy(void * to, const void * from, int n)
   {
   }
*/

/* 4. Function for getting current time (for benchmarking)  */
/* static unsigned int taketime(void)
   {
   }
*/

#endif

/************************************************************************/
/*                   Generic built-in memcpy wrapper                    */
/************************************************************************/

#ifdef XGI_haveBuiltInMC
static void XGI_builtin_memcpy(UChar *dst, const UChar *src, int size)
{
    PDEBUG(ErrorF("Jong-XGI_builtin_memcpy()...\n"));
    builtin_memcpy(dst, src, size);
}
#endif

/**********************************************************************/
/*      Generic routines if /proc filesystem is available (Linux)     */
/**********************************************************************/

#ifdef XGI_haveProc
/* Linux: Read file (/proc/cpuinfo) into buffer */
static int XGI_ReadProc(char *buf, char *filename)
{
    FILE *cpuInfoFile;
    int count;

    if((cpuInfoFile = fopen(filename, "r")) == NULL) {
       return 0;
    }

    count = fread(buf, 1, CPUBUFFERSIZE, cpuInfoFile);
    if(ferror(cpuInfoFile)) {
       fclose(cpuInfoFile);
       return 0;
    }

    fclose(cpuInfoFile);

    if(count >= CPUBUFFERSIZE - 2) {
       return 0;
    }

    buf[count] = 0;

    return count;
}

/* Linux: Extract CPU speed from /proc/cpuinfo */
static char *XGI_GetCPUFreq(ScrnInfoPtr pScrn, char *buf, double *cpuFreq)
{
    char *frqBuf, *endBuf;

    (*cpuFreq) = 0.0;

    if((frqBuf = strstr(buf,"cpu MHz\t\t:"))) {
       frqBuf += 11;
       (*cpuFreq) = strtod(frqBuf, &endBuf);
       if(endBuf == frqBuf) frqBuf = NULL;
       if((*cpuFreq) < 10.0) frqBuf = NULL; /* sanity check */
       if(frqBuf) {
          xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "CPU frequency %.2fMhz\n", (*cpuFreq));
       }
    }

    return frqBuf;
}
#endif /* haveProc */

/**********************************************************************/
/*                      Arch-specific routines                        */
/**********************************************************************/

#ifdef XGI_checkosforsse   /* Common i386, AMD64  */

#ifdef XGICHECKOSSSE

#ifndef XFree86LOADER
#include <setjmp.h>
#endif

static jmp_buf sigill_return;

static void sigill_handler(void)
{
    longjmp(sigill_return, 1);
}
#endif

#endif

#ifdef __i386__   /* i386 specific *************************************/
#define ULong	unsigned long

PREFETCH_FUNC(XGI_sse,SSE,SSE,,FENCE,small_memcpy_i386)
PREFETCH_FUNC(XGI_mmxext,MMXEXT,SSE,EMMS,FENCEMMS,small_memcpy_i386)
PREFETCH_FUNC(XGI_now,MMX,NOW,FEMMS,FEMMS,small_memcpy_i386)
NOPREFETCH_FUNC(XGI_mmx,MMX,EMMS,EMMS,small_memcpy_i386)

#define Def_FL  (XGI_CPUFL_LIBC | XGI_CPUFL_BI | XGI_CPUFL_BI2)  /* Default methods */

#define cpuid(op, eax, ebx, ecx, edx) 		\
    __asm__ __volatile__ (			\
		" pushl %%ebx\n"		\
		" cpuid\n"			\
		" movl %%ebx, %1\n"		\
		" popl %%ebx\n"			\
		: "=a" (eax), "=r" (ebx), 	\
		  "=c" (ecx), "=d" (edx)	\
		: "a" (op)			\
		: "cc")

#elif defined(__AMD64__) || defined(__amd64__) || defined(__x86_64__) /* AMD64 specific ***** */

PREFETCH_FUNC(XGI_sse,SSE64,SSE,,FENCE,small_memcpy_amd64)

#endif

#endif /* GNU C */
