#include <cairo.h>
#include <cairo-ft.h>

#define flag(something) fprintf(stderr, "%s|%d|%s warning: FLAG %p\n", __FILE__, __LINE__, __FUNCTION__, something);
