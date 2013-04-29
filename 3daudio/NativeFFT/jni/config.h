/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */
// Manually modified 3/10/2013 by Paula Ning (pning@hmc.edu)
// to define sizeof()s on host PC.

/* Define to compile in long-double precision. */
#undef BENCHFFT_LDOUBLE

/* Define to compile in quad precision. */
#undef BENCHFFT_QUAD

/* Define to compile in single precision. */
#define BENCHFFT_SINGLE	1

/* Define to one of `_getb67', `GETB67', `getb67' for Cray-2 and Cray-YMP
   systems. This function is required for `alloca.c' support on those systems.
   */
#undef CRAY_STACKSEG_END

/* Define to 1 if using `alloca.c'. */
#undef C_ALLOCA

/* Define to disable Fortran wrappers. */
#undef DISABLE_FORTRAN

/* Define to dummy `main' function (if any) required to link to the Fortran
   libraries. */
#undef F77_DUMMY_MAIN

/* Define to a macro mangling the given C identifier (in lower and upper
   case), which must not contain underscores, for linking with Fortran. */
#undef F77_FUNC

/* As F77_FUNC, but for C identifiers containing underscores. */
#undef F77_FUNC_

/* Define if F77_FUNC and F77_FUNC_ are equivalent. */
#undef F77_FUNC_EQUIV

/* Define if F77 and FC dummy `main' functions are identical. */
#undef FC_DUMMY_MAIN_EQ_F77

/* C compiler name and flags */
#define FFTW_CC	"gcc"

/* Define to enable extra FFTW debugging code. */
#undef FFTW_DEBUG

/* Define to enable alignment debugging hacks. */
#undef FFTW_DEBUG_ALIGNMENT

/* Define to enable debugging malloc. */
#undef FFTW_DEBUG_MALLOC

/* Define to enable the use of alloca(). */
#undef FFTW_ENABLE_ALLOCA

/* Define to compile in long-double precision. */
#undef FFTW_LDOUBLE

/* Define to compile in quad precision. */
#undef FFTW_QUAD

/* Define to enable pseudorandom estimate planning for debugging. */
#undef FFTW_RANDOM_ESTIMATOR

/* Define to compile in single precision. */
#define FFTW_SINGLE	1

/* Define to 1 if you have the `abort' function. */
#undef HAVE_ABORT

/* Define to 1 if you have `alloca', as a function or macro. */
#undef HAVE_ALLOCA

/* Define to 1 if you have <alloca.h> and it should be used (not on Ultrix).
   */
#undef HAVE_ALLOCA_H

/* Define to enable Altivec optimizations. */
#undef HAVE_ALTIVEC

/* Define to 1 if you have the <altivec.h> header file. */
#undef HAVE_ALTIVEC_H

/* Define to enable AVX optimizations. */
#undef HAVE_AVX

/* Define to 1 if you have the `BSDgettimeofday' function. */
#undef HAVE_BSDGETTIMEOFDAY

/* Define to 1 if you have the `clock_gettime' function. */
#undef HAVE_CLOCK_GETTIME

/* Define to 1 if you have the `cosl' function. */
#undef HAVE_COSL

/* Define to 1 if you have the <c_asm.h> header file. */
#undef HAVE_C_ASM_H

/* Define to 1 if you have the declaration of `cosl', and to 0 if you don't.
   */
#undef HAVE_DECL_COSL

/* Define to 1 if you have the declaration of `cosq', and to 0 if you don't.
   */
#undef HAVE_DECL_COSQ

/* Define to 1 if you have the declaration of `drand48', and to 0 if you
   don't. */
#undef HAVE_DECL_DRAND48

/* Define to 1 if you have the declaration of `memalign', and to 0 if you
   don't. */
#define HAVE_DECL_MEMALIGN	0

/* Define to 1 if you have the declaration of `posix_memalign', and to 0 if
   you don't. */
#define HAVE_DECL_POSIX_MEMALIGN	0

/* Define to 1 if you have the declaration of `sinl', and to 0 if you don't.
   */
#undef HAVE_DECL_SINL

/* Define to 1 if you have the declaration of `sinq', and to 0 if you don't.
   */
#undef HAVE_DECL_SINQ

/* Define to 1 if you have the declaration of `srand48', and to 0 if you
   don't. */
#undef HAVE_DECL_SRAND48

/* Define to 1 if you have the <dlfcn.h> header file. */
#undef HAVE_DLFCN_H

/* Define to 1 if you don't have `vprintf' but do have `_doprnt.' */
#undef HAVE_DOPRNT

/* Define to 1 if you have the `drand48' function. */
#undef HAVE_DRAND48

/* Define if you have a machine with fused multiply-add */
#undef HAVE_FMA

/* Define to 1 if you have the `gethrtime' function. */
#undef HAVE_GETHRTIME

/* Define to 1 if you have the `gettimeofday' function. */
#undef HAVE_GETTIMEOFDAY

/* Define to 1 if hrtime_t is defined in <sys/time.h> */
#undef HAVE_HRTIME_T

/* Define to 1 if you have the <intrinsics.h> header file. */
#undef HAVE_INTRINSICS_H

/* Define to 1 if you have the <inttypes.h> header file. */
#undef HAVE_INTTYPES_H

/* Define if the isnan() function/macro is available. */
#undef HAVE_ISNAN

/* Define to 1 if you have the <libintl.h> header file. */
#undef HAVE_LIBINTL_H

/* Define to 1 if you have the `m' library (-lm). */
#undef HAVE_LIBM

/* Define to 1 if you have the `quadmath' library (-lquadmath). */
#undef HAVE_LIBQUADMATH

/* Define to 1 if you have the <limits.h> header file. */
#undef HAVE_LIMITS_H

/* Define to 1 if the compiler supports `long double' */
#undef HAVE_LONG_DOUBLE

/* Define to 1 if you have the `mach_absolute_time' function. */
#undef HAVE_MACH_ABSOLUTE_TIME

/* Define to 1 if you have the <mach/mach_time.h> header file. */
#undef HAVE_MACH_MACH_TIME_H

/* Define to 1 if you have the <malloc.h> header file. */
#undef HAVE_MALLOC_H

/* Define to 1 if you have the `memalign' function. */
#undef HAVE_MEMALIGN

/* Define to 1 if you have the <memory.h> header file. */
#undef HAVE_MEMORY_H

/* Define to 1 if you have the `memset' function. */
#undef HAVE_MEMSET

/* Define to enable use of MIPS ZBus cycle-counter. */
#undef HAVE_MIPS_ZBUS_TIMER

/* Define if you have the MPI library. */
#undef HAVE_MPI

/* Define to enable ARM NEON optimizations. */
#undef HAVE_NEON

/* Define if OpenMP is enabled */
#undef HAVE_OPENMP

/* Define to 1 if you have the `posix_memalign' function. */
#undef HAVE_POSIX_MEMALIGN

/* Define if you have POSIX threads libraries and header files. */
#undef HAVE_PTHREAD

/* Define to 1 if you have the `read_real_time' function. */
#undef HAVE_READ_REAL_TIME

/* Define to 1 if you have the `sinl' function. */
#undef HAVE_SINL

/* Define to 1 if you have the `snprintf' function. */
#undef HAVE_SNPRINTF

/* Define to 1 if you have the `sqrt' function. */
#undef HAVE_SQRT

/* Define to enable SSE/SSE2 optimizations. */
#undef HAVE_SSE2

/* Define to 1 if you have the <stddef.h> header file. */
#undef HAVE_STDDEF_H

/* Define to 1 if you have the <stdint.h> header file. */
#undef HAVE_STDINT_H

/* Define to 1 if you have the <stdlib.h> header file. */
#undef HAVE_STDLIB_H

/* Define to 1 if you have the <strings.h> header file. */
#undef HAVE_STRINGS_H

/* Define to 1 if you have the <string.h> header file. */
#undef HAVE_STRING_H

/* Define to 1 if you have the `sysctl' function. */
#undef HAVE_SYSCTL

/* Define to 1 if you have the <sys/stat.h> header file. */
#undef HAVE_SYS_STAT_H

/* Define to 1 if you have the <sys/sysctl.h> header file. */
#undef HAVE_SYS_SYSCTL_H

/* Define to 1 if you have the <sys/time.h> header file. */
#undef HAVE_SYS_TIME_H

/* Define to 1 if you have the <sys/types.h> header file. */
#undef HAVE_SYS_TYPES_H

/* Define to 1 if you have the `tanl' function. */
#undef HAVE_TANL

/* Define if we have a threads library. */
#undef HAVE_THREADS

/* Define to 1 if you have the `time_base_to_time' function. */
#undef HAVE_TIME_BASE_TO_TIME

/* Define to 1 if the system has the type `uintptr_t'. */
#undef HAVE_UINTPTR_T

/* Define to 1 if you have the <unistd.h> header file. */
#undef HAVE_UNISTD_H

/* Define to 1 if you have the `vprintf' function. */
#undef HAVE_VPRINTF

/* Define to 1 if you have the `_mm_free' function. */
#undef HAVE__MM_FREE

/* Define to 1 if you have the `_mm_malloc' function. */
#undef HAVE__MM_MALLOC

/* Define if you have the UNICOS _rtc() intrinsic. */
#undef HAVE__RTC

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#undef LT_OBJDIR

/* Define to 1 if your C compiler doesn't accept -c and -o together. */
#undef NO_MINUS_C_MINUS_O

/* Name of package */
#define PACKAGE "fftw3"

/* Define to the address where bug reports for this package should be sent. */
#undef PACKAGE_BUGREPORT

/* Define to the full name of this package. */
#undef PACKAGE_NAME

/* Define to the full name and version of this package. */
#undef PACKAGE_STRING

/* Define to the one symbol short name of this package. */
#undef PACKAGE_TARNAME

/* Define to the home page for this package. */
#undef PACKAGE_URL

/* Define to the version of this package. */
#define PACKAGE_VERSION "3"

/* Define to necessary symbol if this constant uses a non-standard name on
   your system. */
#undef PTHREAD_CREATE_JOINABLE

/* The size of `double', as computed by sizeof. */
#define SIZEOF_DOUBLE	8

/* The size of `fftw_r2r_kind', as computed by sizeof. */
#undef SIZEOF_FFTW_R2R_KIND

/* The size of `float', as computed by sizeof. */
#define SIZEOF_FLOAT	4

/* The size of `int', as computed by sizeof. */
#define SIZEOF_INT	4

/* The size of `long', as computed by sizeof. */
#define SIZEOF_LONG	4

/* The size of `long long', as computed by sizeof. */
#define SIZEOF_LONG_LONG	8

/* The size of `MPI_Fint', as computed by sizeof. */
#undef SIZEOF_MPI_FINT

/* The size of `ptrdiff_t', as computed by sizeof. */
#define SIZEOF_PTRDIFF_T	4

/* The size of `size_t', as computed by sizeof. */
#define SIZEOF_SIZE_T	4

/* The size of `unsigned int', as computed by sizeof. */
#define SIZEOF_UNSIGNED_INT	4

/* The size of `unsigned long', as computed by sizeof. */
#define SIZEOF_UNSIGNED_LONG	4

/* The size of `unsigned long long', as computed by sizeof. */
#define SIZEOF_UNSIGNED_LONG_LONG	8

/* The size of `void *', as computed by sizeof. */
#define SIZEOF_VOID_P	4

/* If using the C implementation of alloca, define if you know the
   direction of stack growth for your system; otherwise it will be
   automatically deduced at runtime.
	STACK_DIRECTION > 0 => grows toward higher addresses
	STACK_DIRECTION < 0 => grows toward lower addresses
	STACK_DIRECTION = 0 => direction of growth unknown */
#undef STACK_DIRECTION

/* Define to 1 if you have the ANSI C header files. */
#undef STDC_HEADERS

/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. */
#undef TIME_WITH_SYS_TIME

/* Define if we have and are using POSIX threads. */
#undef USING_POSIX_THREADS

/* Version number of package */
#define VERSION "3"

/* Use common Windows Fortran mangling styles for the Fortran interfaces. */
#undef WINDOWS_F77_MANGLING

/* Include g77-compatible wrappers in addition to any other Fortran wrappers.
   */
#undef WITH_G77_WRAPPERS

/* Use our own aligned malloc routine; mainly helpful for Windows systems
   lacking aligned allocation system-library routines. */
#undef WITH_OUR_MALLOC

/* Use low-precision timers, making planner very slow */
#undef WITH_SLOW_TIMER

/* Define to empty if `const' does not conform to ANSI C. */
#undef const

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
#undef inline
#endif

/* Define to `unsigned int' if <sys/types.h> does not define. */
#undef size_t
