# sed script for DJGPP specific editing of config.hin

$ a\
\
\
/*  DJGPP specific defines.  */\
\
#include <unistd.h>\
#define TAB_EXT     ((pathconf(NULL, _PC_NAME_MAX) > 12) ? ".tab" : "_tab")\
#define OUTPUT_EXT  ((pathconf(NULL, _PC_NAME_MAX) > 12) ? ".output" : ".out")\
\
#define DEFAULT_TMPDIR  "/dev/env/DJDIR/tmp"



