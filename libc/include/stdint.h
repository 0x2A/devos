/*
 * Copyright (c) 2003 Steve Underwood <steveu@coppice.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS `AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $Id: stdint.h,v 1.5 2006/11/15 14:34:57 coppice Exp $
 * 
 * Note 1:
 *
 * modified for DevOS: 
 * Replaces definition of 16bit types from int to short
 * Replaced definition of 32bit types from long int to int
 */

#if !defined(__STDINT_H_)
#define __STDINT_H_

/*
 *	ISO C99: 7.18 Integer types <stdint.h>
 */

#ifndef __int8_t_defined
#define __int8_t_defined
typedef signed char                             int8_t;
typedef short                                   int16_t;
typedef int		                                int32_t;
typedef long long					            int64_t;

typedef unsigned char                           uint8_t;
typedef unsigned short                          uint16_t;
typedef unsigned int	                        uint32_t;
typedef unsigned long long	    				uint64_t;

#if __MSP430X__
typedef long int __attribute__((__a20__))            int20_t;
typedef unsigned long int __attribute__((__a20__))   uint20_t;
#endif /* __MSP430X__ */
#endif

/* Small types.  */

/* Signed.  */
typedef signed char                             int_least8_t;
typedef short                                   int_least16_t;
typedef int		                                int_least32_t;
__extension__ typedef long long int             int_least64_t;

/* Unsigned.  */
typedef unsigned char                           uint_least8_t;
typedef unsigned short                          uint_least16_t;
typedef unsigned int 		                    uint_least32_t;
typedef unsigned long long					    uint_least64_t;


/* Fast types.  */

/* Signed.  */
typedef signed char                             int_fast8_t;
typedef short                                   int_fast16_t;
typedef int		                                int_fast32_t;
__extension__ typedef long long int             int_fast64_t;

/* Unsigned.  */
typedef unsigned char                           uint_fast8_t;
typedef unsigned short                          uint_fast16_t;
typedef unsigned int		                    uint_fast32_t;
typedef unsigned long long     uint_fast64_t;


/* Types for `void *' pointers.  */
#if !defined(__intptr_t_defined)
#define __intptr_t_defined
#if __MSP430X__ & __MSP430_CPUX_TARGET_D20__
typedef int20_t                                  intptr_t;
typedef uint20_t                                 uintptr_t;
#else
typedef int16_t                                  intptr_t;
typedef uint16_t                                 uintptr_t;
#endif
#endif

/* Largest integral types.  */
__extension__ typedef long long int             intmax_t;
__extension__ typedef unsigned long long int    uintmax_t;


/* The ISO C99 standard specifies that in C++ implementations these
   macros should only be defined if explicitly requested.  */
#if !defined __cplusplus || defined __STDC_LIMIT_MACROS

/* Limits of integral types.  */

/* Maximum of signed integral types.  */
#define INT8_MAX                127
#define INT16_MAX               32767
#if __MSP430X__
#define INT20_MAX               524287L
#endif /* __MSP430X__ */
#define INT32_MAX               2147483647L
#define INT64_MAX               9223372036854775807LL

/* Minimum of signed integral types.  */
#define INT8_MIN                (-INT8_MAX - 1)
#define INT16_MIN               (-INT16_MAX - 1)
#if __MSP430X__
#define INT20_MIN               (-INT20_MAX - 1)
#endif /* __MSP430X__ */
#define INT32_MIN               (-INT32_MAX - 1)
#define INT64_MIN               (-INT64_MAX - 1)

/* Maximum of unsigned integral types.  */
#define UINT8_MAX               255U
#define UINT16_MAX              65535U
#if __MSP430X__
#define UINT20_MAX              1048575UL
#endif /* __MSP430X__ */
#define UINT32_MAX              4294967295UL
#define UINT64_MAX              18446744073709551615ULL


/* Minimum of signed integral types having a minimum size.  */
#define INT_LEAST8_MIN          INT8_MIN
#define INT_LEAST16_MIN         INT16_MIN
#define INT_LEAST32_MIN         INT32_MIN
#define INT_LEAST64_MIN         INT64_MIN
/* Maximum of signed integral types having a minimum size.  */
#define INT_LEAST8_MAX          INT8_MAX
#define INT_LEAST16_MAX         INT16_MAX
#define INT_LEAST32_MAX         INT32_MAX
#define INT_LEAST64_MAX         INT64_MAX

/* Maximum of unsigned integral types having a minimum size.  */
#define UINT_LEAST8_MAX	        UINT8_MAX
#define UINT_LEAST16_MAX        UINT16_MAX
#define UINT_LEAST32_MAX        UINT32_MAX
#define UINT_LEAST64_MAX        UINT64_MAX


/* Minimum of fast signed integral types having a minimum size.  */
#define INT_FAST8_MIN           INT8_MIN
#define INT_FAST16_MIN          INT16_MIN
#define INT_FAST32_MIN          INT32_MIN
#define INT_FAST64_MIN          INT64_MIN
/* Maximum of fast signed integral types having a minimum size.  */
#define INT_FAST8_MAX           INT8_MAX
#define INT_FAST16_MAX          INT16_MAX
#define INT_FAST32_MAX          INT32_MAX
#define INT_FAST64_MAX          INT64_MAX

/* Maximum of fast unsigned integral types having a minimum size.  */
#define UINT_FAST8_MAX          UINT8_MAX
#define UINT_FAST16_MAX         UINT16_MAX
#define UINT_FAST32_MAX         UINT32_MAX
#define UINT_FAST64_MAX         UINT64_MAX

/* Values to test for integral types holding `void *' pointer.  */
#if __MSP430X__ & __MSP430_CPUX_TARGET_D20__
#define INTPTR_MIN              INT20_MIN
#define INTPTR_MAX              INT20_MAX
#define UINTPTR_MAX             UINT20_MAX
#else /* __MSP430X__ */
#define INTPTR_MIN              INT16_MIN
#define INTPTR_MAX              INT16_MAX
#define UINTPTR_MAX             UINT16_MAX
#endif /* __MSP430X__ */

/* Minimum for largest signed integral type.  */
#define INTMAX_MIN              INT64_MIN
/* Maximum for largest signed integral type.  */
#define INTMAX_MAX              INT64_MAX

/* Maximum for largest unsigned integral type.  */
#define UINTMAX_MAX             UINT64_MAX

/* Limits of other integer types.  */

/* Limits of `ptrdiff_t' type.  */
#if __MSP430X__ & __MSP430_CPUX_TARGET_A20__
#define PTRDIFF_MAX             INT32_MAX
#else
#define PTRDIFF_MAX             INT16_MAX
#endif
#define PTRDIFF_MIN             (-PTRDIFF_MAX - 1)

/* Limits of `sig_atomic_t'.  */
#define SIG_ATOMIC_MIN          INT16_MIN
#define SIG_ATOMIC_MAX          INT16_MAX

/* Limit of `size_t' type.  */
#if __MSP430X__ & __MSP430_CPUX_TARGET_A20__
#define SIZE_MAX                UINT32_MAX
#else /* -ma20 */
#define SIZE_MAX                UINT16_MAX
#endif /* -ma20 */

/* Limits of `wchar_t'.  */
#ifndef WCHAR_MIN
/* These constants might also be defined in <wchar.h>.  */
#define WCHAR_MIN               __WCHAR_MIN
#define WCHAR_MAX               __WCHAR_MAX
#endif

/* Limits of `wint_t'.  */
#define WINT_MIN                (0U)
#define WINT_MAX                (65535U)

#endif	/* C++ && limit macros */

/* The ISO C99 standard specifies that in C++ implementations these
   should only be defined if explicitly requested.  */
#if !defined __cplusplus || defined __STDC_CONSTANT_MACROS

/* Signed.  */
#define INT8_C(c)               c
#define INT16_C(c)              c
#define INT32_C(c)              c ## L
#define INT64_C(c)              c ## LL

/* Unsigned.  */
#define UINT8_C(c)              c ## U
#define UINT16_C(c)             c ## U
#define UINT32_C(c)             c ## UL
#define UINT64_C(c)             c ## ULL

/* Maximal type.  */
#define INTMAX_C(c)             c ## LL
#define UINTMAX_C(c)            c ## ULL

#if __MSP430X__
#define INT20_C(c)              c ## L
#define UINT20_C(c)             c ## UL
#endif /* __MSP430X__ */

#endif  /* C++ && constant macros */
#endif