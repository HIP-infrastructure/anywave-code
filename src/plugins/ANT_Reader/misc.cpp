#include "misc.h"
#include <string.h>  
#include <stdlib.h>                                                    
#include <ctype.h>  

#ifndef Q_OS_LINUX
int strcasecmp(const char *s1, const char *s2)
{
  int i, r;
  char *buf1 = (char *)malloc(strlen(s1) + 1);
  char *buf2 = (char *)malloc(strlen(s2) + 1);
  
  i = 0;
  do {
    buf1[i] = tolower(s1[i]);
    i++;
  } while(s1[i-1]);
  
  i = 0;
  do {
    buf2[i] = tolower(s2[i]);
    i++;
  } while(s2[i-1]);
  
  r = strcmp(buf1, buf2);
  
  free(buf1); free(buf2);
  return r;
}

int strend(char *s, char *m)
{
  int l1, l2;
  int x;
  
  l1 = strlen(s);
  l2 = strlen(m);
  if (l2 > l1)
    x = 1;
  else 
    x = strcmp(s + l1 - l2, m);
  
  if (x)
    return 0;
  else
    return 1;
}
#endif

int asciiwrite(FILE *f, char *line)
{
  /* return nonzero on success */
  return (fputs(line, f) != EOF);
}

char *asciiread(FILE *f, char *line)
{
  return fgets(line, EEPIOMAX, f);
}
