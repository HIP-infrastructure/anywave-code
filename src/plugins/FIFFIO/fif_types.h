#pragma once

#include <Eigen/Core>
#include <QPair>
#include <QVariant>

typedef unsigned char        fiff_byte_t;
typedef char                 fiff_char_t;
typedef qint16               fiff_short_t;
typedef quint16              fiff_ushort_t;
typedef qint32               fiff_int_t;
typedef quint32              fiff_uint_t;
typedef qint64               fiff_long_t;
typedef quint64              fiff_ulong_t;
typedef float                fiff_float_t;
typedef double               fiff_double_t;
typedef quint16              fiff_dau_pack13_t;
typedef quint16              fiff_dau_pack14_t;
typedef qint16               fiff_dau_pack16_t;
typedef qint32               fiff_julian_t;

static Eigen::MatrixXd defaultMatrixXd = Eigen::MatrixXd::Constant(1, 1, -1);
const static Eigen::MatrixXd defaultConstMatrixXd(0, 0);
const static Eigen::MatrixXi defaultMatrixXi(0, 0);
const static Eigen::VectorXi defaultVectorXi;
const static Eigen::RowVectorXi defaultRowVectorXi;
const static QPair<QVariant, QVariant> defaultVariantPair;

typedef Eigen::Matrix<qint16, Eigen::Dynamic, Eigen::Dynamic> MatrixDau16;
typedef Eigen::Matrix<short, Eigen::Dynamic, Eigen::Dynamic> MatrixShort;

typedef struct _fiffTimeRec {
	fiff_int_t secs;           /**< GMT time in seconds since epoch */
	fiff_int_t usecs;          /**< Fraction of seconds in microseconds */
} *fiffTime, fiffTimeRec;   /**< Accurate time stamps used in FIFF files.*/

typedef struct _fiff_data_ref {
	fiff_int_t      type;       /**< Type of the data */
	fiff_int_t      endian;     /**< Are the data in the little or big endian byte order */
	fiff_long_t     size;       /**< Size of the data, can be over 2 GB  */
	fiff_long_t     offset;     /**< Offset to the data in the external file  */
} *fiffDataRef, fiffDataRefRec;

//
//typedef struct _fiffTagRec {
//	fiff_int_t  kind;		/**< Tag number.
//					*   This defines the meaning of the item */
//	fiff_int_t  type;		/**< Data type.
//					*   This defines the reperentation of the data. */
//	fiff_int_t  size;		/**< Size of the data.
//					*   The size is given in bytes and defines the
//					*   total size of the data. */
//	fiff_int_t  next;		/**< Pointer to the next object.
//					*   Zero if the object follows
//					*   sequentially in file.
//					*   Negative at the end of file */
//	fiff_data_t *data;		/**< Pointer to the data.
//					*   This point to the data read or to be written. */
//} *fiffTag, fiffTagRec;   /**< FIFF data tag */
//
//typedef struct _fiffIdRec {
//	fiff_int_t version;	   /**< File version */
//	fiff_int_t machid[2];	   /**< Unique machine ID */
//	fiffTimeRec time;	   /**< Time of the ID creation */
//} *fiffId, fiffIdRec;	   /**< This is the file identifier */
//
//typedef fiffIdRec fiff_id_t;
