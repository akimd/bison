/* system-dependent definitions for Bison.
   Copyright 2000 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#ifndef BISON_SYSTEM_H
#define BISON_SYSTEM_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>

#include <assert.h>

#ifdef MSDOS
# include <io.h>
#endif

#ifdef _MSC_VER
# include <stdlib.h>
# include <process.h>
# define getpid _getpid
#endif

#if HAVE_STDLIB_H
# include <stdlib.h>
#endif

/* The following test is to work around the gross typo in
   systems like Sony NEWS-OS Release 4.0C, whereby EXIT_FAILURE
   is defined to 0, not 1.  */
#if !EXIT_FAILURE
# undef EXIT_FAILURE
# define EXIT_FAILURE 1
#endif

#ifndef EXIT_SUCCESS
# define EXIT_SUCCESS 0
#endif

#if HAVE_UNISTD_H
# include <unistd.h>
#endif

#if defined(STDC_HEADERS) || defined(HAVE_STRING_H)
# include <string.h>
/* An ANSI string.h and pre-ANSI memory.h might conflict.  */
# if !defined(STDC_HEADERS) && defined(HAVE_MEMORY_H)
#  include <memory.h>
# endif /* not STDC_HEADERS and HAVE_MEMORY_H */
# ifndef bcopy
#  define bcopy(src, dst, num) memcpy((dst), (src), (num))
# endif
#else /* not STDC_HEADERS and not HAVE_STRING_H */
# include <strings.h>
/* memory.h and strings.h conflict on some systems.  */
#endif /* not STDC_HEADERS and not HAVE_STRING_H */

#if defined(STDC_HEADERS) || defined(HAVE_CTYPE_H)
# include <ctype.h>
#endif

#include <errno.h>
#ifndef errno
extern int errno;
#endif

/* AIX requires this to be the first thing in the file.  */
#ifndef __GNUC__
# if HAVE_ALLOCA_H
#  include <alloca.h>
# else
#  ifdef _AIX
 #pragma alloca
#  else
#   ifndef alloca /* predefined by HP cc +Olibcalls */
char *alloca ();
#   endif
#  endif
# endif
#endif

#if PROTOTYPES
# define PARAMS(p) p
#else
# define PARAMS(p) ()
#endif

# include "xalloc.h"

/*---------------------.
| Missing prototypes.  |
`---------------------*/

#if !HAVE_DECL_STPCPY
char *stpcpy PARAMS ((char *dest, const char *src));
#endif

#if !HAVE_DECL_STRCHR
char *strchr(const char *s, int c);
#endif

#if !HAVE_DECL_STRSPN
size_t strspn(const char *s, const char *accept);
#endif

#if !HAVE_DECL_STRNLEN
size_t strnlen PARAMS ((const char *s, size_t maxlen));
#endif

#if !HAVE_DECL_MEMCHR
void *memchr PARAMS ((const void *s, int c, size_t n));
#endif

#if !HAVE_DECL_MEMRCHR
void *memrchr PARAMS ((const void *s, int c, size_t n));
#endif



/*-----------------.
| GCC extensions.  |
`-----------------*/

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
# if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 5) || __STRICT_ANSI__
#  define __attribute__(Spec) /* empty */
# endif
/* The __-protected variants of `format' and `printf' attributes
   are accepted by gcc versions 2.6.4 (effectively 2.7) and later.  */
# if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 7)
#  define __format__ format
#  define __printf__ printf
# endif
#endif


/*------.
| NLS.  |
`------*/

#ifdef HAVE_LOCALE_H
# include <locale.h>
#endif
#ifndef HAVE_SETLOCALE
# define setlocale(Category, Locale)
#endif

#include "libgettext.h"
#define _(Msgid)  gettext (Msgid)
#define N_(Msgid) (Msgid)


/*-------------------------------.
| Fix broken compilation flags.  |
`-------------------------------*/

#ifndef LOCALEDIR
# define LOCALEDIR "/usr/local/share/locale"
#endif


/*-----------.
| Booleans.  |
`-----------*/

#ifndef TRUE
# define TRUE	(1)
# define FALSE	(0)
#endif
typedef int bool;


/*-----------.
| Obstacks.  |
`-----------*/

# define obstack_chunk_alloc xmalloc
# define obstack_chunk_free  free
# include "obstack.h"

#define obstack_sgrow(Obs, Str) \
  obstack_grow (Obs, Str, strlen (Str))

#define obstack_fgrow1(Obs, Format, Arg1)	\
do {						\
  char buf[4096];				\
  sprintf (buf, Format, Arg1);			\
  obstack_grow (Obs, buf, strlen (buf));	\
} while (0)

#define obstack_fgrow2(Obs, Format, Arg1, Arg2)	\
do {						\
  char buf[4096];				\
  sprintf (buf, Format, Arg1, Arg2);		\
  obstack_grow (Obs, buf, strlen (buf));	\
} while (0)

#define obstack_fgrow3(Obs, Format, Arg1, Arg2, Arg3)	\
do {							\
  char buf[4096];					\
  sprintf (buf, Format, Arg1, Arg2, Arg3);		\
  obstack_grow (Obs, buf, strlen (buf));		\
} while (0)

#define obstack_fgrow4(Obs, Format, Arg1, Arg2, Arg3, Arg4)	\
do {								\
  char buf[4096];						\
  sprintf (buf, Format, Arg1, Arg2, Arg3, Arg4);		\
  obstack_grow (Obs, buf, strlen (buf));			\
} while (0)


/*---------------------------------.
| Machine-dependencies for Bison.  |
`---------------------------------*/

#ifdef	eta10
# define	MAXSHORT	2147483647
# define	MINSHORT	-2147483648
#else
# define	MAXSHORT	32767
# define	MINSHORT	-32768
#endif

#if defined (MSDOS) && !defined (__GO32__)
# define	BITS_PER_WORD	16
# define MAXTABLE	16383
#else
# define	BITS_PER_WORD	32
# define MAXTABLE	32767
#endif

#define	WORDSIZE(n)	(((n) + BITS_PER_WORD - 1) / BITS_PER_WORD)
#define	SETBIT(x, i)	((x)[(i)/BITS_PER_WORD] |= (1<<((i) % BITS_PER_WORD)))
#define RESETBIT(x, i)	((x)[(i)/BITS_PER_WORD] &= ~(1<<((i) % BITS_PER_WORD)))
#define BITISSET(x, i)	(((x)[(i)/BITS_PER_WORD] & (1<<((i) % BITS_PER_WORD))) != 0)


/* Extensions to use for the output files. */

#ifdef VMS
  /* VMS. */
# define EXT_TAB	"_tab"
# define EXT_OUTPUT	".output"
# define EXT_STYPE_H	"_stype"
# define EXT_GUARD_C	"_guard"
#else /* ! VMS */
# ifdef MSDOS
   /* MS DOS. */
#  define EXT_TAB	"_tab"
#  define EXT_OUTPUT	".out"
#  define EXT_STYPE_H	".sth"
#  define EXT_GUARD_C	".guc"
# else /* ! MSDOS */
  /* Standard. */
#  define EXT_TAB	".tab"
#  define EXT_OUTPUT	".output"
#  define EXT_STYPE_H	".stype"
#  define EXT_GUARD_C	".guard"
# endif /* ! MSDOS */
#endif /* ! VMS */

#if defined (VMS) & !defined (__VMS_POSIX)
# ifndef BISON_SIMPLE
#  define BISON_SIMPLE "GNU_BISON:[000000]BISON.SIMPLE"
# endif
# ifndef BISON_HAIRY
#  define BISON_HARIRY "GNU_BISON:[000000]BISON.HAIRY"
# endif
#endif

#if defined (_MSC_VER)
# ifndef BISON_SIMPLE
#  define BISON_SIMPLE "c:/usr/local/lib/bison.simple"
# endif
# ifndef BISON_HAIRY
#  define BISON_HAIRY "c:/usr/local/lib/bison.hairy"
# endif
#endif


/* As memcpy, but for shorts.  */
#define shortcpy(Dest, Src, Num) \
  memcpy (Dest, Src, Num * sizeof (short))

/* Free a linked list. */
#define LIST_FREE(Type, List)			\
do {						\
  Type *_node, *_next;				\
  for (_node = List; _node; _node = _next)	\
    {						\
      _next = _node->next;			\
      XFREE (_node);				\
    }						\
} while (0)

/*---------------------------------.
| Debugging the memory allocator.  |
`---------------------------------*/

# if WITH_DMALLOC
#  define DMALLOC_FUNC_CHECK
#  include <dmalloc.h>
# endif /* WITH_DMALLOC */

#endif  /* BISON_SYSTEM_H */
