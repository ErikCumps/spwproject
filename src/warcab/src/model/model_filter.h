/** \file
 * The SPWaW war cabinet - data model handling - filters (EXPERIMENTAL).
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	MODEL_FILTER_H
#define	MODEL_FILTER_H	1

typedef unsigned int	MDL_MFID;

typedef struct s_MDL_MF {
	MDL_MFID	id;
	char		*name;
} MDL_MF;

typedef struct s_MDL_MFLST {
	int	cnt;
	MDL_MF	*list;
} MDL_MFLST;

typedef unsigned int	MDL_MF_TGTID;

typedef struct s_MDL_MF_TGT {
	MDL_MF_TGTID	id;
	char		*name;
} MDL_MF_TGT;

typedef struct s_MDL_MF_TGTLST {
	MDL_MFID	filter;
	int		size;
	int		cnt;
	MDL_MF_TGT	*list;
} MDL_MF_TGTLST;

extern void	MDL_MF_TGTLST_new	(MDL_MF_TGTLST **list);
extern void	MDL_MF_TGTLST_free	(MDL_MF_TGTLST **list);
extern void	MDL_MF_TGTLST_clear	(MDL_MF_TGTLST *list);
extern void	MDL_MF_TGTLST_add	(MDL_MF_TGTLST *list, MDL_MF_TGTID id, char *name);

//////////////////////////////////////////////////////////////////////

typedef struct s_xMDL_FILTER_TARGET {
	char			*name;
	QVariant		*value;
} xMDL_FILTER_TARGET;

typedef struct s_xMDL_FILTER_LIST {
	unsigned int		size;
	unsigned int		used;
	xMDL_FILTER_TARGET	*data;
} xMDL_FILTER_LIST;

extern void	xMDL_FILTER_new		(xMDL_FILTER_LIST **list);
extern void	xMDL_FILTER_free	(xMDL_FILTER_LIST **list);
extern void	xMDL_FILTER_clear	(xMDL_FILTER_LIST *list);
extern void	xMDL_FILTER_add		(xMDL_FILTER_LIST *list, char *name, const QVariant &value);

#endif	/* MODEL_FILTER_H */

