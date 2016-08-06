/** \file
 * Resource Archive - API implementation.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	ARCHIVE_H
#define	ARCHIVE_H	1

#include <resarchv.h>
#include <system/independence.h>
#include <ad_hash.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

struct arc_item_ps;
typedef struct arc_item_ps arc_item_pt;

struct arc_item_ps {
	arc_item_t	item;
	unsigned long	nsize;
	char		*file;
	arc_item_pt	*link;
	arc_item_pt	*next;
};

struct arc_ps;
typedef struct arc_ps arc_pt;

struct arc_ps {
	arc_t		arc;
	arc_item_pt	*items;
	arc_item_pt	*tail;
	ADhash_t	idhash;
	ADhash_t	fnhash;
	int		can_add;
	void		*map;
};

#ifdef	SYS_CC_MSC
#pragma pack(push, 1)
#endif  /* SYS_CC_MSC */

typedef struct s_header_t {
	unsigned char	magic[8];	/* magic string 			*/
	unsigned long	version;	/* version 				*/
	unsigned long	size;		/* header size 				*/
	unsigned long	count;		/* item count 				*/
	unsigned long	st_items;	/* start of items section 		*/
	unsigned long	sz_items;	/* size of items section 		*/
	unsigned long	st_names;	/* start of names section 		*/
	unsigned long	sz_names;	/* size of names section 		*/
	unsigned long	st_data;	/* start of data section 		*/
	unsigned long	sz_data;	/* size of data section 		*/
	unsigned long	sigused;	/* signature enabled flag 		*/ /*! \todo Turn this into an options field */
	unsigned char	sig[8];		/* signature 				*/
} header_t PACKED;

typedef struct s_item_t {
	unsigned long	type;		/* item type 				*/
	unsigned long	id;		/* item ID 				*/
	unsigned long	name;		/* name pointer (from start of file) 	*/
	unsigned long	pos;		/* item position (from start fo file) 	*/
	unsigned long	size;		/* item size 				*/
} item_t PACKED;

typedef struct s_name_t {
	unsigned long	size;		/* name length 				*/
	char		name[1];	/* name (zero-length names not allowed)	*/
} name_t PACKED;

#ifdef	SYS_CC_MSC
#pragma	pack(pop)
#endif	/* SYS_CC_MSC */

#endif	/* ARCHIVE_H */
