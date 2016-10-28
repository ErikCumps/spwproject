/** \file
 * The SPWaW war cabinet - data model handling - sorting.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	MODEL_SORT_H
#define	MODEL_SORT_H	1

#include "model.h"

typedef int (*MDL_CMP) (const void *a, const void *b);

typedef enum e_MDL_CMPTYPE {
	MDL_CMPINT,
	MDL_CMPSTR,
	MDL_CMPDBL,
	MDL_CMPUID,
} MDL_CMPTYPE;

typedef struct s_MDL_CMPDEFEL {
	MDL_CMPTYPE	type;
	int		col;
	int		rev;
} MDL_CMPDEFEL;

typedef struct s_MDL_CMPDEF {
	int		ID;
	int		cnt;
	MDL_CMPDEFEL	def[2];
} MDL_CMPDEF;

typedef union u_MDL_CMPDATA {
	int		i;
	char		*s;
	double		d;
	unsigned int	u;
} MDL_CMPDATA;

extern int	MDL_stdcmp_GMO	(const void *a, const void *b, MDL_CMPTYPE type, int col, bool rev);
extern int	MDL_stdcmp_GMR	(const void *a, const void *b, MDL_CMPTYPE type, int col, bool rev);

extern int	MDL_stdcmp_GMO	(const void *a, const void *b, MDL_CMPDEF *def);
extern int	MDL_stdcmp_GMR	(const void *a, const void *b, MDL_CMPDEF *def);

#define	MDL_PACK_UID(uir_)		(((uir_->snap->data.FMID & 0xFF) << 8) | (uir_->snap->data.FSID & 0xFF))
#define	MDL_UNPACK_UID(p_,fid_,fsid_)	do { fid_ = (p_ >> 8) & 0xFF; fsid_ = p_ & 0xFF; } while (0)

#endif	/* MODEL_SORT_H */
