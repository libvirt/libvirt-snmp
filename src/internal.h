/*
 * internal.h: internal definitions just used by code from the library
 *
 * Copy:  Copyright (C) 2005-2006, 2010-2011 Red Hat, Inc.
 *
 * See libvirt's COPYING.LIB for the License of this software
 *
 */

#ifndef __INTERNAL_H__
# define __INTERNAL_H__

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <stddef.h>
# include <errno.h>
# include <string.h>
# include <libvirt/libvirt.h>
# include <libvirt/virterror.h>

/* C99 uses __func__.  __FUNCTION__ is legacy. */
# ifndef __GNUC__
#  define __FUNCTION__ __func__
# endif

# ifdef __GNUC__

#  ifndef __GNUC_PREREQ
#   if defined __GNUC__ && defined __GNUC_MINOR__
#    define __GNUC_PREREQ(maj, min)                                        \
    ((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#   else
#    define __GNUC_PREREQ(maj,min) 0
#   endif

/* Work around broken limits.h on debian etch */
#   if defined _GCC_LIMITS_H_ && ! defined ULLONG_MAX
#    define ULLONG_MAX   ULONG_LONG_MAX
#   endif

#  endif /* __GNUC__ */

/**
 * ATTRIBUTE_UNUSED:
 *
 * Macro to flag conciously unused parameters to functions
 */
#  ifndef ATTRIBUTE_UNUSED
#   define ATTRIBUTE_UNUSED __attribute__((__unused__))
#  endif

/**
 * ATTRIBUTE_SENTINEL:
 *
 * Macro to check for NULL-terminated varargs lists
 */
#  ifndef ATTRIBUTE_SENTINEL
#   if __GNUC_PREREQ (4, 0)
#    define ATTRIBUTE_SENTINEL __attribute__((__sentinel__))
#   else
#    define ATTRIBUTE_SENTINEL
#   endif
#  endif

/**
 * ATTRIBUTE_FMT_PRINTF
 *
 * Macro used to check printf like functions, if compiling
 * with gcc.
 *
 * We use gnulib which guarentees we always have GNU style
 * printf format specifiers even on broken Win32 platforms
 * hence we have to force 'gnu_printf' for new GCC
 */
#  ifndef ATTRIBUTE_FMT_PRINTF
#   if __GNUC_PREREQ (4, 4)
#    define ATTRIBUTE_FMT_PRINTF(fmtpos,argpos) __attribute__((__format__ (gnu_printf, fmtpos,argpos)))
#   else
#    define ATTRIBUTE_FMT_PRINTF(fmtpos,argpos) __attribute__((__format__ (printf, fmtpos,argpos)))
#   endif
#  endif

#  ifndef ATTRIBUTE_RETURN_CHECK
#   if __GNUC_PREREQ (3, 4)
#    define ATTRIBUTE_RETURN_CHECK __attribute__((__warn_unused_result__))
#   else
#    define ATTRIBUTE_RETURN_CHECK
#   endif
#  endif

/**
 * ATTRIBUTE_PACKED
 *
 * force a structure to be packed, i.e. not following architecture and
 * compiler best alignments for its sub components. It's needed for example
 * for the network filetering code when defining the content of raw
 * ethernet packets.
 * Others compiler than gcc may use something different e.g. #pragma pack(1)
 */
#  ifndef ATTRIBUTE_PACKED
#   if __GNUC_PREREQ (3, 3)
#    define ATTRIBUTE_PACKED __attribute__((packed))
#   else
#    error "Need an __attribute__((packed)) equivalent"
#   endif
#  endif

#  ifndef ATTRIBUTE_NONNULL
#   if __GNUC_PREREQ (3, 3)
#    define ATTRIBUTE_NONNULL(m) __attribute__((__nonnull__(m)))
#   else
#    define ATTRIBUTE_NONNULL(m)
#   endif
#  endif

# else
#  ifndef ATTRIBUTE_UNUSED
#   define ATTRIBUTE_UNUSED
#  endif
#  ifndef ATTRIBUTE_FMT_PRINTF
#   define ATTRIBUTE_FMT_PRINTF(...)
#  endif
#  ifndef ATTRIBUTE_RETURN_CHECK
#   define ATTRIBUTE_RETURN_CHECK
#  endif
# endif             /* __GNUC__ */

extern int verbose;

#define VIR_DEBUG0(fmt) if (verbose) fprintf(stderr, "%s:%d :: " fmt "\n", \
        __func__, __LINE__)
#define VIR_DEBUG(fmt, ...) if (verbose) fprintf(stderr, "%s:%d: " fmt "\n", \
        __func__, __LINE__, __VA_ARGS__)
#define VIR_WARN(fmt, ...) if (verbose) fprintf(stderr, "%s:%d: " fmt "\n", \
        __func__, __LINE__, __VA_ARGS__)

#endif
