#ifndef MISC_H
#define MISC_H
#include <QtGlobal>
#include <stdio.h>

typedef int  sraw_t;   /* "sample raw" - a type which can contain 32 bit signeds */
typedef int  slen_t;   /* "sample length" - signed, must count sample numbers (up to 1e7) */

typedef struct {      /* "sample window" */
  qint64 start;
  qint64 length;
} swin_t;

#define ABS(x) (((x) < 0) ? -(x) : (x))
#define FRND(x) ((x) < 0 ? ((x) - 0.5) : ((x) + 0.5))

#ifndef MIN
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif

#ifndef MAX
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif

#if !defined(Q_OS_LINUX)
int strcasecmp(const char *s1, const char *s2);
/* returns 1 if s ends with m */
int strend(char *s, char *m);
#endif

/* emulate some ANSI C definitions (made for a Sparc/SunOS 4.1.4) */
#ifndef SEEK_SET
# define SEEK_SET 0
#endif
#ifndef SEEK_CUR
# define SEEK_CUR 1
#endif
#ifndef SEEK_END
# define SEEK_END 2
#endif

/* ------------------------------------------------------------------
  text file handling
*/
#define EEPIOMAX 1024
typedef char asciiline_t[EEPIOMAX];

/*
  you must supply at least EEPIOMAX chars for reading in line
  an asciiline_t buffer will suffice
  return: fgets return (NULL on EOF or error, else line)
*/
char *asciiread(FILE *f, char *line);

/*
  return: nonzero on success
*/
int  asciiwrite(FILE *f, char *line);

#endif
