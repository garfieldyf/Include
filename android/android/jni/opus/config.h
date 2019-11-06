/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Get CPU Info by asm method */
//#define CPU_INFO_BY_ASM

/* Get CPU Info by c method */
#define CPU_INFO_BY_C

/* Custom modes */
//#define CUSTOM_MODES

/* Do not build the float API */
//#define DISABLE_FLOAT_API

/* Disable bitstream fixes from RFC 8251 */
//#define DISABLE_UPDATE_DRAFT

/* Assertions */
#define ENABLE_ASSERTIONS

/* Hardening */
#define ENABLE_HARDENING

/* Debug fixed-point implementation */
#define FIXED_DEBUG

/* Compile as fixed-point (for machines without a fast enough FPU) */
#define FIXED_POINT

/* Float approximations */
//#define FLOAT_APPROX

/* Fuzzing */
//#define FUZZING

/* Define to 1 if you have the <alloca.h> header file. */
//#define HAVE_ALLOCA_H

/* NE10 library is installed on host. Make sure it is on target! */
//#define HAVE_ARM_NE10

/* Define to 1 if you have the <dlfcn.h> header file. */
//#define HAVE_DLFCN_H

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H

/* Define to 1 if you have the `lrint' function. */
//#define HAVE_LRINT

/* Define to 1 if you have the `lrintf' function. */
//#define HAVE_LRINTF

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H

/* Define to 1 if you have the `__malloc_hook' function. */
//#define HAVE___MALLOC_HOOK

/* Define to the sub-directory where libtool stores uninstalled libraries. */
//#define LT_OBJDIR

/* Make use of ARM asm optimization */
//#define OPUS_ARM_ASM

/* Use generic ARMv4 inline asm optimizations */
//#define OPUS_ARM_INLINE_ASM

/* Use ARMv5E inline asm optimizations */
//#define OPUS_ARM_INLINE_EDSP

/* Use ARMv6 inline asm optimizations */
//#define OPUS_ARM_INLINE_MEDIA

/* Use ARM NEON inline asm optimizations */
//#define OPUS_ARM_INLINE_NEON

/* Define if assembler supports EDSP instructions */
//#define OPUS_ARM_MAY_HAVE_EDSP

/* Define if assembler supports ARMv6 media instructions */
//#define OPUS_ARM_MAY_HAVE_MEDIA

/* Define if compiler supports NEON instructions */
//#define OPUS_ARM_MAY_HAVE_NEON

/* Compiler supports ARMv7/Aarch64 Neon Intrinsics */
//#define OPUS_ARM_MAY_HAVE_NEON_INTR

/* Define if binary requires Aarch64 Neon Intrinsics */
//#define OPUS_ARM_PRESUME_AARCH64_NEON_INTR

/* Define if binary requires EDSP instruction support */
//#define OPUS_ARM_PRESUME_EDSP

/* Define if binary requires ARMv6 media instruction support */
//#define OPUS_ARM_PRESUME_MEDIA

/* Define if binary requires NEON instruction support */
//#define OPUS_ARM_PRESUME_NEON

/* Define if binary requires NEON intrinsics support */
//#define OPUS_ARM_PRESUME_NEON_INTR

/* This is a build of OPUS */
#define OPUS_BUILD

/* Run bit-exactness checks between optimized and c implementations */
//#define OPUS_CHECK_ASM

/* Use run-time CPU capabilities detection */
//#define OPUS_HAVE_RTCD

/* Compiler supports X86 AVX Intrinsics */
//#define OPUS_X86_MAY_HAVE_AVX

/* Compiler supports X86 SSE Intrinsics */
//#define OPUS_X86_MAY_HAVE_SSE

/* Compiler supports X86 SSE2 Intrinsics */
//#define OPUS_X86_MAY_HAVE_SSE2

/* Compiler supports X86 SSE4.1 Intrinsics */
//#define OPUS_X86_MAY_HAVE_SSE4_1

/* Define if binary requires AVX intrinsics support */
//#define OPUS_X86_PRESUME_AVX

/* Define if binary requires SSE intrinsics support */
//#define OPUS_X86_PRESUME_SSE

/* Define if binary requires SSE2 intrinsics support */
//#define OPUS_X86_PRESUME_SSE2

/* Define if binary requires SSE4.1 intrinsics support */
//#define OPUS_X86_PRESUME_SSE4_1

/* Define to the address where bug reports for this package should be sent. */
//#define PACKAGE_BUGREPORT

/* Define to the full name of this package. */
//#define PACKAGE_NAME

/* Define to the full name and version of this package. */
//#define PACKAGE_STRING

/* Define to the one symbol short name of this package. */
//#define PACKAGE_TARNAME

/* Define to the home page for this package. */
//#define PACKAGE_URL

/* Define to the version of this package. */
//#define PACKAGE_VERSION

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS

/* Make use of alloca */
//#define USE_ALLOCA

/* Use C99 variable-size arrays */
#define VAR_ARRAYS

/* Define to empty if `const' does not conform to ANSI C. */
//#define const

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
#define inline
#endif

/* Define to the equivalent of the C99 'restrict' keyword, or to
   nothing if this is not supported.  Do not define if restrict is
   supported directly.  */
#define restrict
/* Work around a bug in Sun C++: it does not support _Restrict or
   __restrict__, even though the corresponding Sun C compiler ends up with
   "#define restrict _Restrict" or "#define restrict __restrict__" in the
   previous line.  Perhaps some future version of Sun C++ will work with
   restrict; if so, hopefully it defines __RESTRICT like Sun C does.  */
#if defined __SUNPRO_CC && !defined __RESTRICT
# define _Restrict
# define __restrict__
#endif
