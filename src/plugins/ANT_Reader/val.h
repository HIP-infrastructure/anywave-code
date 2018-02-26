#ifndef VAL_H
#define VAL_H

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

#endif