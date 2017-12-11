#include "trg.h"

#include <stdint.h>
#include <string.h>
#include "cnt.h"
#include "eepmem.h"

#define TRG_OK  0
#define TRG_EOF 1
#define TRG_ERR 2

int trg_header_read(trg_t *trg, FILE *Trg, double cntperiod)
{
  asciiline_t line;
  char* line_p1;

  double period;
  int    blocksize;
  int n;

  asciiread(Trg, line);
  n = sscanf(line, "%lf%d", &period, &blocksize);

  if (   n != 2
      || ABS(cntperiod / period - 1.0) > 1e-5    )
  {
    return TRG_ERR;
  }

  /* Get the extra_header_info */

  line_p1= line;
  strtod(line_p1, &line_p1);
  strtol(line_p1, &line_p1, 10);

  trg_init_header(trg, line_p1);

  return TRG_OK;
}

trg_t* trg_copy(const trg_t* trg)
{
  trg_t* new_trg;

 new_trg = (trg_t *) malloc(sizeof(trg_t));
  memcpy(new_trg, trg, sizeof(trg_t));

  new_trg->v = (trgentry_t *) malloc(sizeof(trgentry_t) * trg->c);
 // new_trg->v = new trgentry_t[trg->c];
  memcpy(new_trg->v, trg->v, sizeof(trgentry_t) *trg->c);

  return new_trg;
}

int trg_header_read_unchecked(trg_t *trg, FILE *Trg, double *period, short *chanc)
{
  asciiline_t line;
  int    blocksize;
  double p;
  int n;
  char* line_p1;

  asciiread(Trg, line);

  n = sscanf(line, "%lf%d", &p, &blocksize);

  if (n != 2)
  {
    return TRG_ERR;
  }


  *period = p;
  *chanc = (blocksize - 4) / 2;

  /* Get the extra_header_info */

  line_p1= line;
  strtod(line_p1, &line_p1);
  strtol(line_p1, &line_p1, 10);

  trg_init_header(trg, line_p1);

  return TRG_OK;

}

int trg_line_read_2_or_4_items(asciiline_t line, double period, trgentry_t *t)
{
  double time;
  asciiline_t code;
  asciiline_t cls_text;
  char cls_code;

  int n;

  n = sscanf(line, "%lf%8s%3s %c", &time, code, cls_text, &cls_code);

  if(n > 2)
  {
    if(strncmp(cls_text, "cls", 3))
      return TRG_EOF;

    /* We have a classified trigger */
    t->cls_code = cls_code;
  }
  else
  {
    t->cls_code = '\0';
  }

  /* no valid trigger line - check eof */
  if (n < 2) {
    return TRG_EOF;
  }
  else {
    t->sample = (quint64) (time / period + 0.5);
    strncpy(t->code, code, TRG_CODE_LENGTH);
    (t->code)[TRG_CODE_LENGTH] = '\0';
    return TRG_OK;
  }
}

int trg_line_read_3_or_5_items(asciiline_t line, double period, trgentry_t *t)
{
  double time;
  quint64 pos;
  asciiline_t code;
  asciiline_t cls_text;
  char cls_code;

  int n;
#ifdef Q_OS_WIN
  n = sscanf(line, "%lf%I64d%8s%3s %c", &time, &pos, code, cls_text, &cls_code);
#else
  n = sscanf(line, "%lf%d%8s%3s %c", &time, &pos, code, cls_text, &cls_code);
#endif
  if(n > 3)
  {
    if(strncmp(cls_text, "cls", 3))
      return TRG_EOF;

    /* We have a classified trigger */
    t->cls_code = cls_code;
  }
  else
  {
    t->cls_code = '\0';
  }

  /* no valid trigger line - check eof */
  if (n < 3) {
    return TRG_EOF;
  }
  else {
    t->sample = (quint64) (time / period + 0.5);
    strncpy(t->code, code, TRG_CODE_LENGTH);
    (t->code)[TRG_CODE_LENGTH] = '\0';
    return TRG_OK;
  }
}

/* wrapper function around compatibility-mode functions:
   this makes the second column (sample point) optional
 */
int trg_line_read(FILE *Trg, double period, trgentry_t *t)
{
  int r;
  asciiline_t line;

  asciiread(Trg, line);

  if (feof(Trg))
   return TRG_EOF;


  r = trg_line_read_3_or_5_items(line, period, t);
  if( r == TRG_EOF )
    r = trg_line_read_2_or_4_items(line, period, t);
  return r;
}

/* public --------------------------------------------- */

void trg_init_header(trg_t *trg, asciiline_t line)
{
  int n;
  strncpy(trg->extra_header_text, line, EEPIOMAX);
  trg->extra_header_text[EEPIOMAX-1]=0;
  for(n=0; trg->extra_header_text[n] != 0; n++)
  {
    if(trg->extra_header_text[n] == '\n')
    {
      trg->extra_header_text[n] = '\0';
      n--; /* To jump out of the loop. */
    }
  }
}

trg_t *trg_init(void)
{
  trg_t *trg = (trg_t *)malloc(sizeof(trg_t));
  trg->c = 0;
  trg->v = NULL; /* memory must be allocated before filling data into the table */
  trg->cmax = 0;
  trg->extra_header_text[0] = 0;
  return trg;
}

void trg_free(trg_t *trg)
{
   if( trg != NULL ) {
     free(trg->v);
     free(trg);
   }
}


/* bubblesort the trigger table */
void trg_sort(trg_t *trg)
{
  int i, nswap;
  trgentry_t tmp;

  if(trg->c==0) {
    return;
  }

  do {
    nswap = 0;
    for (i = 0; i < trg->c - 1; i++) {
      if (trg->v[i].sample > trg->v[i+1].sample) {
        tmp = trg->v[i]; trg->v[i] = trg->v[i+1]; trg->v[i+1] = tmp;
        nswap++;
      }
    }
  } while (nswap);
}

trg_t *trg_file_read(FILE *f, double period)
{
  trg_t *trg = trg_init();

  trgentry_t trgin;
  int status;

 fseek(f, 0, SEEK_SET);
  if (trg_header_read(trg, f, period) != TRG_OK) {
    trg_free(trg);
    return NULL;
  }

  do {
    status = trg_line_read(f, period, &trgin);
    if (status == TRG_ERR) {
      trg_free(trg);
      return NULL;
    }
    if (status == TRG_OK) {
      /* obtain space in blocks of 256 entries */
      while (trg->c >= trg->cmax) {
        trg->v = (trgentry_t *)
          realloc(trg->v, (size_t) (trg->cmax + 256) * sizeof(trgentry_t));
        trg->cmax +=  256;
      }
      (trg->v)[trg->c] = trgin;
      (trg->c)++;
    }
  } while (status == TRG_OK);

  trg_sort(trg);

  return trg;
}

trg_t *trg_file_read_unchecked(FILE *f, double *period, short *chanc)
{
  trg_t *trg = trg_init();

  trgentry_t trgin;
  int status;

  if (trg_header_read_unchecked(trg, f, period, chanc) != TRG_OK) {
    trg_free(trg);
    return NULL;
  }

  do {
    status = trg_line_read(f, *period, &trgin);
    if (status == TRG_ERR) {
      trg_free(trg);
      return NULL;
    }
    if (status == TRG_OK) {
      /* obtain space in blocks of 256 entries */
      while (trg->c >= trg->cmax) {
        trg->v = (trgentry_t *)
          realloc(trg->v, (size_t) (trg->cmax + 256) * sizeof(trgentry_t));
        trg->cmax += 256;
      }
      (trg->v)[trg->c] = trgin;
      (trg->c)++;
    }
  } while (status == TRG_OK);

  trg_sort(trg);

  return trg;
}

#undef TRG_OK
#undef TRG_EOF
#undef TRG_ERR

int trg_set(trg_t *trg, quint64 sample, const char *code)
{
  int r = 0;
  int i;
  uint64_t   c    = trg->c;
  trgentry_t *v   = trg->v;
  uint64_t   cmax = trg->cmax;



  /*
    locate the insert index
    (append is most freq. case - start search at end of list)
  */
  i = c;
  while (i > 0 && v[i-1].sample >= sample) i--;

  /* avoid double triggers (multiple triggers at same position are allowed) */
  if ( !(   i < c
         && v[i].sample == sample
         && !strcasecmp(v[i].code, code)) )
  {
    /* make sure that there is some space left in the table */
    while (c >= cmax) {
      v = extend(v, cmax, trgentry_t, 256);
      cmax += 256;
    }
    /* insert in the middle of list? - shift */
    if (i < c) {
      memmove( &(v[i + 1]), &(v[i]), (size_t) (c - i) * sizeof(trgentry_t));
    }

    /* fill in the data */
    v[i].sample = sample;
    strncpy(v[i].code, code, TRG_CODE_LENGTH);
    v[i].code[TRG_CODE_LENGTH] = '\0';
    v[i].cls_code = '\0';
    c++;
    r++;
  }
  trg->v    = v;
  trg->c    = c;
  trg->cmax = cmax;


  return r;
}

/* exact copy of trg_set, but also setting 'cls' member */
int trg_set_cls(trg_t *trg, quint64 sample, const char *code, const char cls)
{
  int r = 0;
  int i;
  int        c    = trg->c;
  trgentry_t *v   = trg->v;
  int        cmax = trg->cmax;



  /*
    locate the insert index
    (append is most freq. case - start search at end of list)
  */
  i = c;
  while (i > 0 && v[i-1].sample >= sample) i--;

  /* avoid double triggers (multiple triggers at same position are allowed) */
  if ( !(   i < c
         && v[i].sample == sample
         && !strcasecmp(v[i].code, code)) )
  {
    /* make sure that there is some space left in the table */
    while (c >= cmax) {
      v = extend(v, cmax, trgentry_t, 256);
      cmax += 256;
    }
    /* insert in the middle of list? - shift */
    if (i < c) {
      memmove( &(v[i + 1]), &(v[i]), (size_t) (c - i) * sizeof(trgentry_t));
    }

    /* fill in the data */
    v[i].sample = sample;
    strncpy(v[i].code, code, TRG_CODE_LENGTH);
    v[i].code[TRG_CODE_LENGTH] = '\0';
    v[i].cls_code = cls;
    c++;
    r++;
  }
  trg->v    = v;
  trg->c    = c;
  trg->cmax = cmax;


  return r;
}

int trg_set_EEP20(trg_t *trg, quint64 sample, unsigned short flag)
{
  int r = 0;
  char code[TRG_CODE_LENGTH + 1];
  unsigned short val = flag & 0xff;

  if (val > 0) {
    sprintf(code, "%d", val);
    r += trg_set(trg, sample, code);
  }
  if (flag & EEP20_DCRESET) {
    r += trg_set(trg, sample, TRG_DCRESET);
  }
  if (flag & EEP20_DISCONT) {
    r += trg_set(trg, sample, TRG_DISCONT);
  }

  return r;
}

int trg_get_c (trg_t *trg)
{
  return trg->c;
}

char *trg_get  (trg_t *trg, int i, uint64_t *sample)
{
  *sample = trg->v[i].sample;
  return trg->v[i].code;
}

char *trg_get_cls  (trg_t *trg, int i, uint64_t *sample, char *cls)
{
  *sample = trg->v[i].sample;
  *cls = trg->v[i].cls_code;
  return trg->v[i].code;
}

int trg_group_match(char *code, trgcode_t *grpv, short grpc)
{
  short i;

  for (i = 0; i < grpc; i++) {
    if (!strcasecmp(code, grpv[i]))
      return 1;
  }
  return 0;
}

int trg_discont_epoch(trg_t *trg, uint64_t start, uint64_t length)
{
  int i = 0;

  while (i < trg->c && trg->v[i].sample < start) i++;

  while (i < trg->c && trg->v[i].sample < start + length) {
    if (TRG_IS_DCRESET(trg->v[i].code) || TRG_IS_DISCONT(trg->v[i].code))
      return 1;
    i++;
  }

  return 0;
}


int trg_seek(trg_t *trg, uint64_t sample, const char *code, char direction)
{
  int i = 0;
  int lasti = -1;

  while (i < trg->c) {
    /* interesting trigger ? */
    if (!strcasecmp(trg->v[i].code, code)) {
      /* found next trigger - ready */
      if (trg->v[i].sample > sample) {
        break;
      }
      /* note last trigger - maybe it's the previous one we look for */
      else if (trg->v[i].sample < sample) {
        lasti = i;
      }
    }
    i++;
  }

  /* forward finds next or out of table (-1) which must be handled in call */
  if (direction)
    return i < trg->c ? i : -1;
  else
    return lasti >= 0 ? lasti : -1;
}

int trg_group_seek(trg_t *trg, uint64_t sample, trgcode_t  *grpv, int grpc, char direction)
{
  int i = 0;
  int lasti = -1;
  int t, found;

  while (i < trg->c) {
    /* interesting trigger ? */
    found = 0;
    for (t = 0; t < grpc && !found; t++)
      found = !strcasecmp(trg->v[i].code, grpv[t]);

    if (found) {
      /* found next trigger - ready */
      if (trg->v[i].sample > sample) {
        break;
      }
      /* note last trigger - maybe it's the previous one we look for */
      else if (trg->v[i].sample < sample) {
        lasti = i;
      }
    }
    i++;
  }

  /* forward finds next or out of table (-1) which must be handled in call */
  if (direction)
    return i < trg->c ? i : -1;
  else
    return lasti >= 0 ? lasti : -1;
}

/* look for triggers at position sample, remove them, update trigger table,
   return number of deleted triggers */

int trg_clear(trg_t *trg, quint64 sample, const char *code)
{
  quint64 *trgc = &(trg->c);
  trgentry_t **trgv = &(trg->v);
  quint64 *trgcmax = &(trg->cmax);

  int r = 0;
  int i = 0;

  while (i < *trgc && (*trgv)[i].sample < sample) i++;

  while (   i < *trgc
         && (*trgv)[i].sample == sample
         && !strcasecmp((*trgv)[i].code, code) )
  {
    if (!TRG_IS_DCRESET((*trgv)[i].code) && !TRG_IS_DISCONT((*trgv)[i].code)) {
      if (i < *trgc - 1)
        memmove( &(*trgv)[i], &(*trgv)[i+1], (size_t) (*trgc - i) * sizeof(trgentry_t));
      (*trgc)--; r++;
    }

    i++;
  }
  *trgv = (trgentry_t *) realloc(*trgv, (size_t) (*trgc) * sizeof(trgentry_t));
  (*trgcmax) = (*trgc);

  return r;
}
