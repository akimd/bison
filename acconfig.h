#ifndef CONFIG_H
#define CONFIG_H
@TOP@

/* Define as 1 if realloc must be declared even if <stdlib.h> is
   included.  */
#undef NEED_DECLARATION_REALLOC

/* Define as 1 if calloc must be declared even if <stdlib.h> is
   included.  */
#undef NEED_DECLARATION_CALLOC
@BOTTOM@

#if defined(PROTOTYPES) || defined(__cplusplus)
# define PARAMS(p) p
#else
# define PARAMS(p) ()
#endif

#endif  /* CONFIG_H */
