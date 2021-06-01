// AnyWave
// Copyright (C) 2013-2021  INS UMR 1106
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#ifndef __VAL_H__
#define __VAL_H__

#include <stdarg.h>

/* maximum length of a structured value name */
#define VAL_MAXNAME 1024

/* the set of distinct value types */
enum {
    VAL_TYPE_VAL    = 1<<0,
    VAL_TYPE_PTR    = 1<<1,
    VAL_TYPE_CHAR   = 1<<2,
    VAL_TYPE_SHORT  = 1<<3,
    VAL_TYPE_INT    = 1<<4,
    VAL_TYPE_LONG   = 1<<5,
    VAL_TYPE_FLOAT  = 1<<6,
    VAL_TYPE_DOUBLE = 1<<7
};

/* the set of return codes */
typedef enum {
    VAL_OK,        /* everything ok */
    VAL_ERR_ARG,   /* error: invalid argument */
    VAL_ERR_USE,   /* error: invalid use */
    VAL_ERR_MEM,   /* error: no more memory */
    VAL_ERR_HSH,   /* error: hash table problem */
    VAL_ERR_INT,   /* error: internal error */
    VAL_ERR_SYS    /* error: system error (see errno) */
} val_rc_t;

/* the opaque data structure and type */
struct val_s;
typedef struct val_s val_t;

/* function type for use with val_apply() */
typedef val_rc_t (*val_cb_t)(void *, const char *, int, const char *, void *);

/* unique library identifier */
extern const char val_id[];

/* set of API functions */
val_rc_t val_create  (val_t **);
val_rc_t val_destroy (val_t *);
val_rc_t val_reg     (val_t *, const char *, int, const char *, void *);
val_rc_t val_unreg   (val_t *, const char *);
val_rc_t val_query   (val_t *, const char *, int *, char **, void **);
val_rc_t val_set     (val_t *, const char *, ...);
val_rc_t val_get     (val_t *, const char *, ...);
val_rc_t val_vset    (val_t *, const char *, va_list);
val_rc_t val_vget    (val_t *, const char *, va_list);
val_rc_t val_apply   (val_t *, const char *, int, val_cb_t, void *);

#endif /* __VAL_H__ */

