/** \file
 * Resource Archive - buildres tool - config file parsing.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	PARSE_CFG_H
#define	PARSE_CFG_H	1

typedef struct s_SUBRESOURCE	SUBRESOURCE;
typedef struct s_RESOURCE	RESOURCE;
typedef struct s_RESLIST	RESLIST;

struct s_SUBRESOURCE {
	unsigned long	sid;
	char		*name;
	unsigned long	row;
	unsigned long	col;
	SUBRESOURCE	*next;
	RESOURCE	*parent;
};

typedef struct s_RESDATA_FILE {
	char		*fname;
	char		*fpath;
} RESDATA_FILE;

typedef struct s_RESDATA_RGB {
	unsigned long	rgb[3];
} RESDATA_RGB;

typedef struct s_RESDATA_RGBA {
	unsigned long	rgba[4];
} RESDATA_RGBA;

typedef struct s_RESDATA_XPM {
	char		*fname;
	unsigned long	cnt;
	char		**data;
	unsigned long	len;
	char		*xpm;
} RESDATA_XPM;

struct s_RESOURCE {
	arc_type_t		type;
	unsigned long		id;
	char			*name;
	union {
		RESDATA_FILE	file;
		RESDATA_RGB	rgb;
		RESDATA_RGBA	rgba;
		RESDATA_XPM	xpm;
	}			data;
	struct s_RESOURCE	*link;
	unsigned long		meta[2];
	RESOURCE		*next;
	unsigned long		sub_cnt;
	SUBRESOURCE		*sub_list;
	SUBRESOURCE		*sub_tail;
	RESLIST			*parent;
};

struct s_RESLIST {
	unsigned long	count;
	unsigned long	sub_count;
	RESOURCE	*list;
	RESOURCE	*tail;
};

extern RESLIST *parse_cfg (char *);

#endif	/* PARSE_CFG_H */

