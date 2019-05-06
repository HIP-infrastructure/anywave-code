#include "fif_tag.h"
#include "fif_direntry.h"
#include "fif_ch_info.h"
#include "fif_dig_point.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Fonctions de conversion BigEndian LitleEndian
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
inline qint32 swap_int(qint32 source)
{
	unsigned char *csource = (unsigned char *)(&source);
	qint32 result;
	unsigned char *cresult = (unsigned char *)(&result);

	cresult[0] = csource[3];
	cresult[1] = csource[2];
	cresult[2] = csource[1];
	cresult[3] = csource[0];
	return (result);
}

inline qint16 swap_short(qint16 source)
{
	unsigned char *csource = (unsigned char *)(&source);
	qint16 result;
	unsigned char *cresult = (unsigned char *)(&result);

	cresult[0] = csource[1];
	cresult[1] = csource[0];
	return (result);
}

inline void swap_intp(qint32 *source)
{
	unsigned char *csource = (unsigned char *)(source);

	unsigned char c;

	c = csource[3];
	csource[3] = csource[0];
	csource[0] = c;
	c = csource[2];
	csource[2] = csource[1];
	csource[1] = c;
}

inline qint64 swap_long(qint64 source)
{
	unsigned char *csource = (unsigned char *)(&source);
	qint64    result;
	unsigned char *cresult = (unsigned char *)(&result);

	cresult[0] = csource[7];
	cresult[1] = csource[6];
	cresult[2] = csource[5];
	cresult[3] = csource[4];
	cresult[4] = csource[3];
	cresult[5] = csource[2];
	cresult[6] = csource[1];
	cresult[7] = csource[0];
	return (result);
}

inline void swap_longp(qint64 *source)
{
	unsigned char *csource = (unsigned char *)(source);
	unsigned char c;

	c = csource[0];
	csource[0] = csource[7];
	csource[7] = c;

	c = csource[1];
	csource[1] = csource[6];
	csource[6] = c;

	c = csource[2];
	csource[2] = csource[5];
	csource[5] = c;

	c = csource[3];
	csource[3] = csource[4];
	csource[4] = c;
}

inline float swap_float(float source)
{
	unsigned char *csource = (unsigned char *)(&source);
	float result;
	unsigned char *cresult = (unsigned char *)(&result);

	cresult[0] = csource[3];
	cresult[1] = csource[2];
	cresult[2] = csource[1];
	cresult[3] = csource[0];
	return (result);
}

inline void swap_floatp(float *source)

{
	unsigned char *csource = (unsigned char *)(source);
	unsigned char c;

	c = csource[3];
	csource[3] = csource[0];
	csource[0] = c;
	c = csource[2];
	csource[2] = csource[1];
	csource[1] = c;
}

inline void swap_doublep(double *source)
{
	unsigned char *csource = (unsigned char *)(source);
	unsigned char c;

	c = csource[7];
	csource[7] = csource[0];
	csource[0] = c;

	c = csource[6];
	csource[6] = csource[1];
	csource[1] = c;

	c = csource[5];
	csource[5] = csource[2];
	csource[2] = c;

	c = csource[4];
	csource[4] = csource[3];
	csource[3] = c;
}

fifTag::fifTag() : QByteArray()
{
	kind = type = next = 0;
}

fifTag::fifTag(const fifTag& copy) : QByteArray(copy.data(), copy.size())
{
	kind = copy.kind;
	type = copy.type;
	next = copy.next;
}

fiff_int_t fifTag::getMatrixCoding() const
{
	return IS_MATRIX & this->type;
}


bool fifTag::isMatrix() const
{
	return getMatrixCoding() != 0;
}

void fifTag::convert_matrix_from_file_data(fifTag *tag)
{
/*
 * Assumes that the input is in the non-native byte order and needs to be swapped to the other one
 */
	int ndim;
	int k;
	int *dimp, *data, kind, np, nz;
	float *fdata;
	double *ddata;
	unsigned int tsize = tag->size();

	if (fiff_type_fundamental(tag->type) != FIFFTS_FS_MATRIX)
		return;
	if (tag->data() == NULL)
		return;
	if (tsize < sizeof(fiff_int_t))
		return;

	dimp = ((fiff_int_t *)((tag->data()) + tag->size() - sizeof(fiff_int_t)));
	swap_intp(dimp);
	ndim = *dimp;
	if (fiff_type_matrix_coding(tag->type) == FIFFTS_MC_DENSE) {
		if (tsize < (ndim + 1) * sizeof(fiff_int_t))
			return;
		dimp = dimp - ndim;
		for (k = 0, np = 1; k < ndim; k++) {
			swap_intp(dimp + k);
			np = np * dimp[k];
		}
	}
	else {
		if (tsize < (ndim + 2) * sizeof(fiff_int_t))
			return;
		if (ndim > 2)       /* Not quite sure what to do */
			return;
		dimp = dimp - ndim - 1;
		for (k = 0; k < ndim + 1; k++)
			swap_intp(dimp + k);
		nz = dimp[0];
		if (fiff_type_matrix_coding(tag->type) == FIFFTS_MC_CCS)
			np = nz + dimp[2] + 1; /* nz + n + 1 */
		else if (fiff_type_matrix_coding(tag->type) == FIFFTS_MC_RCS)
			np = nz + dimp[1] + 1; /* nz + m + 1 */
		else
			return;     /* Don't know what to do */
		/*
		 * Take care of the indices
		*/
		for (data = (int *)(tag->data()) + nz, k = 0; k < np; k++)
			swap_intp(data + k);
		np = nz;
	}
	/*
	 * Now convert data...
	 */
	kind = fiff_type_base(tag->type);
	if (kind == FIFFT_INT) {
#if defined(_OPENMP)
#pragma omp parallel for
#endif
		for (data = (int *)(tag->data()), k = 0; k < np; k++)
			swap_intp(data + k);
	}
	else if (kind == FIFFT_FLOAT) {
#if defined(_OPENMP)
#pragma omp parallel for
#endif
		for (fdata = (float *)(tag->data()), k = 0; k < np; k++)
			swap_floatp(fdata + k);
	}
	else if (kind == FIFFT_DOUBLE) {
#if defined(_OPENMP)
#pragma omp parallel for
#endif
		for (ddata = (double *)(tag->data()), k = 0; k < np; k++)
			swap_doublep(ddata + k);
	}
}

void fifTag::convert_matrix_to_file_data(fifTag *tag)
{
/*
 * Assumes that the input is in the NATIVE_ENDIAN byte order and needs to be swapped to the other one
 */
	int ndim;
	int k;
	int *dimp, *data, kind, np;
	float *fdata;
	double *ddata;
	unsigned int tsize = tag->size();

	if (fiff_type_fundamental(tag->type) != FIFFTS_FS_MATRIX)
		return;
	if (tag->data() == NULL)
		return;
	if (tsize < sizeof(fiff_int_t))
		return;

	dimp = ((fiff_int_t *)(((char *)tag->data()) + tag->size() - sizeof(fiff_int_t)));
	ndim = *dimp;
	swap_intp(dimp);

	if (fiff_type_matrix_coding(tag->type) == FIFFTS_MC_DENSE) {
		if (tsize < (ndim + 1) * sizeof(fiff_int_t))
			return;
		dimp = dimp - ndim;
		for (k = 0, np = 1; k < ndim; k++) {
			np = np * dimp[k];
			swap_intp(dimp + k);
		}
	}
	else {
		if (tsize < (ndim + 2) * sizeof(fiff_int_t))
			return;
		if (ndim > 2)		/* Not quite sure what to do */
			return;
		dimp = dimp - ndim - 1;
		if (fiff_type_matrix_coding(tag->type) == FIFFTS_MC_CCS)
			np = dimp[0] + dimp[2] + 1; /* nz + n + 1 */
		else if (fiff_type_matrix_coding(tag->type) == FIFFTS_MC_RCS)
			np = dimp[0] + dimp[1] + 1; /* nz + m + 1 */
		else
			return;			/* Don't know what to do */
		for (k = 0; k < ndim + 1; k++)
			swap_intp(dimp + k);
	}
	/*
	* Now convert data...
	*/
	kind = fiff_type_base(tag->type);
	if (kind == FIFFT_INT) {
#if defined(_OPENMP)
#pragma omp parallel for
#endif
		for (data = (int *)(tag->data()), k = 0; k < np; k++)
			swap_intp(data + k);
	}
	else if (kind == FIFFT_FLOAT) {
#if defined(_OPENMP)
#pragma omp parallel for
#endif
		for (fdata = (float *)(tag->data()), k = 0; k < np; k++)
			swap_floatp(fdata + k);
	}
	else if (kind == FIFFT_DOUBLE) {
#if defined(_OPENMP)
#pragma omp parallel for
#endif
		for (ddata = (double *)(tag->data()), k = 0; k < np; k++)
			swap_doublep(ddata + k);
	}
	else if (kind == FIFFT_COMPLEX_FLOAT) {
#if defined(_OPENMP)
#pragma omp parallel for
#endif
		for (fdata = (float *)(tag->data()), k = 0; k < 2 * np; k++)
			swap_floatp(fdata + k);
	}
	else if (kind == FIFFT_COMPLEX_DOUBLE) {
#if defined(_OPENMP)
#pragma omp parallel for
#endif
		for (ddata = (double *)(tag->data()), k = 0; k < 2 * np; k++)
			swap_doublep(ddata + k);
	}
}

void fifTag::convert_tag_data(fifTag *tag, int from_endian, int to_endian)
{
	int np, k, r;
	char *offset;
	fiff_int_t *ithis;
	fiff_short_t *sthis;
	fiff_long_t *lthis;
	float *fthis;
	double *dthis;
	fiffDataRef drthis;

	if (tag->data() == NULL || tag->size() == 0)
		return;

	if (from_endian == FIFFV_NATIVE_ENDIAN)
		from_endian = NATIVE_ENDIAN;
	if (to_endian == FIFFV_NATIVE_ENDIAN)
		to_endian = NATIVE_ENDIAN;

	if (fiff_type_fundamental(tag->type) == FIFFTS_FS_MATRIX) {
		if (from_endian == NATIVE_ENDIAN)
			convert_matrix_to_file_data(tag);
		else
			convert_matrix_from_file_data(tag);
		return;
	}
	switch (tag->type) {

	case FIFFT_INT:
	case FIFFT_JULIAN:
	case FIFFT_UINT:
		np = tag->size() / sizeof(fiff_int_t);
		for (ithis = (fiff_int_t *)tag->data(), k = 0; k < np; k++, ithis++)
			swap_intp(ithis);
		break;

	case FIFFT_LONG:
	case FIFFT_ULONG:
		np = tag->size() / sizeof(fiff_long_t);
		for (lthis = (fiff_long_t *)tag->data(), k = 0; k < np; k++, lthis++)
			swap_longp(lthis);
		break;

	case FIFFT_SHORT:
	case FIFFT_DAU_PACK16:
	case FIFFT_USHORT:
		np = tag->size() / sizeof(fiff_short_t);
		for (sthis = (fiff_short_t *)tag->data(), k = 0; k < np; k++, sthis++)
			*sthis = swap_short(*sthis);
		break;

	case FIFFT_FLOAT:
	case FIFFT_COMPLEX_FLOAT:
		np = tag->size() / sizeof(fiff_float_t);
		for (fthis = (fiff_float_t *)tag->data(), k = 0; k < np; k++, fthis++)
			swap_floatp(fthis);
		break;

	case FIFFT_DOUBLE:
	case FIFFT_COMPLEX_DOUBLE:
		np = tag->size() / sizeof(fiff_double_t);
		for (dthis = (fiff_double_t *)tag->data(), k = 0; k < np; k++, dthis++)
			swap_doublep(dthis);
		break;

	case FIFFT_OLD_PACK:
		fthis = (float *)tag->data();
		/*
		 * Offset and scale...
		 */
		swap_floatp(fthis + 0);
		swap_floatp(fthis + 1);
		sthis = (short *)(fthis + 2);
		np = (tag->size() - 2 * sizeof(float)) / sizeof(short);
		for (k = 0; k < np; k++, sthis++)
			*sthis = swap_short(*sthis);
		break;

	case FIFFT_DIR_ENTRY_STRUCT:
		np = tag->size() / fifDirEntry::storageSize();
		for (k = 0; k < np; k++) {
			offset = (char*)tag->data() + k * fifDirEntry::storageSize();
			ithis = (fiff_int_t*)offset;
			ithis[0] = swap_int(ithis[0]);//kind
			ithis[1] = swap_int(ithis[1]);//type
			ithis[2] = swap_int(ithis[2]);//size
			ithis[3] = swap_int(ithis[3]);//pos
		}
		break;

	case FIFFT_ID_STRUCT:
		np = tag->size() / fifId::storageSize();
		for (k = 0; k < np; k++) {
			offset = (char*)tag->data() + k * fifId::storageSize();
			ithis = (fiff_int_t*)offset;
			ithis[0] = swap_int(ithis[0]);//version
			ithis[1] = swap_int(ithis[1]);//machid[0]
			ithis[2] = swap_int(ithis[2]);//machid[1]
			ithis[3] = swap_int(ithis[3]);//time.secs
			ithis[4] = swap_int(ithis[4]);//time.usecs
		}
		break;

	case FIFFT_CH_INFO_STRUCT:
		np = tag->size() / fifChInfo::storageSize();
		for (k = 0; k < np; k++) {
			offset = (char*)tag->data() + k * fifChInfo::storageSize();
			ithis = (fiff_int_t*)offset;
			fthis = (float*)offset;

			ithis[0] = swap_int(ithis[0]);  //scanno
			ithis[1] = swap_int(ithis[1]);  //logno
			ithis[2] = swap_int(ithis[2]);  //kind
			swap_floatp(&fthis[3]);         //range
			swap_floatp(&fthis[4]);         //cal
			ithis[5] = swap_int(ithis[5]);  //coil_type
			for (r = 0; r < 12; ++r)
				swap_floatp(&fthis[6 + r]);   //loc
			ithis[18] = swap_int(ithis[18]);//unit
			ithis[19] = swap_int(ithis[19]);//unit_mul
		}

		break;

	case FIFFT_CH_POS_STRUCT:
		np = tag->size() / fifChPos::storageSize();
		for (k = 0; k < np; ++k)
		{
			offset = (char*)tag->data() + k * fifChPos::storageSize();
			ithis = (fiff_int_t*)offset;
			fthis = (float*)offset;

			ithis[0] = swap_int(ithis[0]);    //coil_type
			for (r = 0; r < 12; r++)
				swap_floatp(&fthis[1 + r]);    //r0, ex, ey, ez
		}

		break;

	case FIFFT_DIG_POINT_STRUCT:
		np = tag->size() / fifDigPoint::storageSize();

		for (k = 0; k < np; k++) {
			offset = tag->data() + k * fifDigPoint::storageSize();
			ithis = (fiff_int_t*)offset;
			fthis = (float*)offset;

			ithis[0] = swap_int(ithis[0]);//kind
			ithis[1] = swap_int(ithis[1]);//ident

			for (r = 0; r < 3; ++r)
				swap_floatp(&fthis[2 + r]);        //r
		}
		break;

	case FIFFT_COORD_TRANS_STRUCT:
		np = tag->size() / FiffCoordTrans::storageSize();

		for (k = 0; k < np; ++k)
		{
			offset = tag->data() + k * FiffCoordTrans::storageSize();
			ithis = (fiff_int_t*)offset;
			fthis = (float*)offset;

			ithis[0] = swap_int(ithis[0]);
			ithis[1] = swap_int(ithis[1]);

			for (r = 0; r < 24; ++r)
				swap_floatp(&fthis[2 + r]);
		}
		break;

	case FIFFT_DATA_REF_STRUCT:
		np = tag->size() / sizeof(fiffDataRefRec);
		for (drthis = (fiffDataRef)tag->data(), k = 0; k < np; k++, drthis++) {
			drthis->type = swap_int(drthis->type);
			drthis->endian = swap_int(drthis->endian);
			drthis->size = swap_long(drthis->size);
			drthis->offset = swap_long(drthis->offset);
		}
		break;

	default:
		break;
	}
}

fiff_int_t fifTag::getType() const
{
	if (isMatrix())
	{
		return DATA_TYPE & this->type;
	}
	else
	{
		return this->type;
	}
}