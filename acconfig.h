#ifndef CONFIG_H
#define CONFIG_H
@TOP@

/* Name of package.  */
#undef PACKAGE

/* Version of package.  */
#undef VERSION

/* Define if the compiler understands prototypes.  */
#undef PROTOTYPES
@BOTTOM@

#if defined(PROTOTYPES) || defined(__cplusplus)
# define PARAMS(p) p
#else
# define PARAMS(p) ()
#endif

#endif  /* CONFIG_H */
