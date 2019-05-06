#pragma once

#include <qbytearray.h>
#include "fif_types.h"
#include "fif_id.h"
#include "fif_file.h"

//
//   The magic hexadecimal values
//
#define IS_MATRIX               0xFFFF0000          /**< Is Matrix encoding. ffff0000 */
#define MATRIX_CODING_DENSE     0x00004000          /**< MATRIX_CODING_DENSE encoding. 4000 */
#define MATRIX_CODING_CCS       0x00004010          /**< MATRIX_CODING_CCS encoding. 4010 */
#define MATRIX_CODING_RCS       0x00004020          /**< MATRIX_CODING_RCS encoding. 4020 */
#define DATA_TYPE               0x0000FFFF          /**< DATA_TYPE encoding 0000ffff */

//////
// WE ASSUME ALL PLATFORMES ARE LITTLE ENDIAN NATIVE (Windows x64, Linux amd64 and MacOS).
#define NATIVE_ENDIAN    FIFFV_LITTLE_ENDIAN

class fifTag : public QByteArray
{
public:
	fifTag();
	fifTag(const fifTag& tag);
	virtual ~fifTag();

	static void convert_tag_data(fifTag *tag, int from_endian, int to_endian);
	fiff_int_t getMatrixCoding() const;
	static fiff_int_t fiff_type_fundamental(fiff_int_t type) { return type & FIFFTS_FS_MASK; }
	static fiff_int_t fiff_type_base(fiff_int_t type) { return type & FIFFTS_BASE_MASK; }
	static fiff_int_t fiff_type_matrix_coding(fiff_int_t type) { return type & FIFFTS_MC_MASK; }
	static void convert_matrix_from_file_data(fifTag *tag);
	static void convert_matrix_to_file_data(fifTag *tag);

	bool isMatrix() const;
	fiff_int_t getType() const;
	QString getInfo() const;
	fifId toFifID() const;

	fiff_int_t  kind;       /**< Tag number.
							 *   This defines the meaning of the item */
	fiff_int_t  type;       /**< Data type.
							 *   This defines the representation of the data. */
							 //    fiff_int_t  size;       /**< Size of the data.
							 //                             *   The size is given in bytes and defines the
							 //                             *   total size of the data. */
	fiff_int_t  next;       /**< Pointer to the next object.
							 *   Zero if the object follows
							 *   sequentially in file.
							 *   Negative at the end of file */
};
