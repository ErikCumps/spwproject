/** \file
 * Resource Archive - API implementation.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "archive.h"
#include <debug_memtrace.h>

static const char *HEADER_MAGIC = "ResArchv";
static const unsigned long HEADER_VERSION = 3;

#define	ALIGN32BITMASK	3
#define	ALIGN32BITSHIFT	2

static const char *arc_type_names[arc_TYPE_LIMIT+1] = {
	"arc_BIN",
	"arc_IMG",
	"arc_IMGCOL",
	"arc_RGB",
	"arc_RGBA",
	"arc_XPM"
};
const char *arc_type_name (arc_type_t type) { return (arc_type_names[type]); }

static const char *arc_type_descs[arc_TYPE_LIMIT+1] = {
	"generic binary resource",
	"image resource",
	"image collection resource",
	"RGB color triplet",
	"RGBA color quadlet",
	"XPM resource"
};
const char *arc_type_lookup (arc_type_t type) { return (arc_type_descs[type]); }

#if	ARCH_LINUX
void	*
mmapfile (int fd, int *et, unsigned long *ec)
{
	struct stat	info;
	int		rc;
	void		*map;

	*et = *ec = errno = 0;

	rc = fstat (fd, &info);
	if (rc == -1) { *et = 1; *ec = errno; return (NULL); }

	map = mmap (NULL, info.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (map == MAP_FAILED) { *et = 1; *ec = errno; return (NULL); }

	return (map);
}
#endif	/* ARCH_LINUX */

#if	ARCH_WINDOWS
void	*
mmapfile (int fd, int *et, unsigned long *ec)
{
	HANDLE	hfd, hmap;
	void	*map;

	*et = *ec = errno = 0;

	hfd = (void *)_get_osfhandle (fd);
	if (hfd == (void *)-1) { *et = 1; *ec = errno; return (NULL); }

	hmap = CreateFileMapping (hfd, NULL, PAGE_READONLY, 0, 0, NULL);
	if (hmap == NULL) { *et = 2; *ec = GetLastError(); return (NULL); }

	map = MapViewOfFile (hmap, FILE_MAP_READ, 0, 0, 0);
	if (map == NULL) { *et = 2; *ec = GetLastError(); return (NULL); }

	return (map);
}
#endif	/* ARCH_WINDOWS */

#if	ARCH_LINUX
void
unmmap (void *map)
{
	if (!map) return;

	// TODO: implement
	return;
}
#endif	/* ARCH_LINUX */

#if	ARCH_WINDOWS
void
unmmap (void *map)
{
	if (!map) return;

	UnmapViewOfFile(map);
}
#endif	/* ARCH_WINDOWS */


#define	IDHASHBUCKETS	211

static INLINE unsigned long
idforkey (void *key)
{
	if (key == NULL) {
		printf ("*** Abort: can not fetch id from NULL data pointer!\n");
		abort ();
	}

	return (*((unsigned long *)key));

}

static unsigned int
id_hash_func (void *key)
{
	return (idforkey (key) % IDHASHBUCKETS);
}

static int
id_hash_match (void *key1, void *key2)
{
	return (idforkey (key1) == idforkey (key2));
}

#define	FNHASHBUCKETS	111
#define	NMFORKEY(key)	((char *)key)

static unsigned int
fn_hash_func (void *key)
{
	char		*p = NMFORKEY (key);
	unsigned int	V = 0, len;

	len = strlen (p);
	while (*p != '\0') {
		V = (V << 1) | (V >> ((sizeof(V)*8) - 1));
		V += *p;
		p++;
	}
	V += len;

        return (V % FNHASHBUCKETS);
}

static int
fn_hash_match (void *key1, void *key2)
{
	return (!strcmp (NMFORKEY (key1), NMFORKEY (key2)));
}


arc_t *
arc_new (char *filename, arc_sig_t *sig)
{
	arc_pt	*arc;

	if (filename == NULL) abort();

	arc = (arc_pt *)malloc (sizeof (arc_pt));
	if (arc == NULL) return (NULL);
	memset (arc, 0, sizeof (arc_pt));

	arc->idhash = ADhash_setup (IDHASHBUCKETS, id_hash_func, id_hash_match, NULL);
	if (arc->idhash == NULL) {
		free (arc);
		return (NULL);
	}

	arc->fnhash = ADhash_setup (FNHASHBUCKETS, fn_hash_func, fn_hash_match, NULL);
	if (arc->fnhash == NULL) {
		ADhash_destroy (arc->idhash);
		free (arc);
		return (NULL);
	}

	arc->arc.name = strdup (filename);

	if (sig != NULL) {
		arc->arc.sigused = 1;
		memcpy (arc->arc.sig, sig, sizeof (arc->arc.sig));
	}

	arc->can_add = 1;

	return ((arc_t *)arc);
}

int
arc_add_file (arc_t *arc, arc_type_t type, unsigned long id, char *name, char *file)
{
	arc_pt		*parc = (arc_pt *)arc;
	struct stat	info;
	arc_item_pt	*item, *link = NULL;

	if ((parc == NULL) || (name == NULL) || (file == NULL)) abort();

	if (!parc->can_add) {
		printf ("ARC: arc_add_file: add not allowed for this archive!\n");
		return (0);
	}
	if (stat (file, &info) == -1) {
		printf ("ARC: arc_add_file: failed to stat file \"%s\"!\n", file);
		return (0);
	}
	if (!S_ISREG (info.st_mode)) {
		printf ("ARC: arc_add_file: file \"%s\" is not a regular file!\n", file);
		return (0);
	}
	if (ADhash_get (parc->idhash, &id) != NULL) {
		printf ("ARC: arc_add_file: duplicate resource id (%lu) detected!\n", id);
		return (0);
	}
	link = (arc_item_pt *)ADhash_get (parc->fnhash, file);

	item = (arc_item_pt *)malloc (sizeof (arc_item_pt));
	if (item == NULL) return (0);
	memset (item, 0, sizeof (arc_item_pt));

	item->item.type	= type;
	item->item.id	= id;
	item->item.pos	= 0;

	item->item.name	= strdup (name);
	item->nsize = strlen (item->item.name);
	if (!link) {
		item->item.size	= info.st_size;
		item->link = NULL;
		item->file = strdup (file);
	} else {
		item->item.size	= 0;
		item->link = link;
		item->file = NULL;
	}

	if (!parc->items) parc->items = item;
	if (parc->tail) parc->tail->next = item;
	parc->tail = item;
	parc->arc.count++;

	ADhash_add (parc->idhash, &(item->item.id), item);
	if (!link) ADhash_add (parc->fnhash, item->file, item);

	return (1);
}

int
arc_add_data (arc_t *arc, arc_type_t type, unsigned long id, char *name, void *data, unsigned int len)
{
	arc_pt		*parc = (arc_pt *)arc;
	arc_item_pt	*item;

	if ((parc == NULL) || (name == NULL) || (data == NULL) || (len == 0)) abort();

	if (!parc->can_add) {
		printf ("ARC: arc_add_data: add not allowed for this archive!\n");
		return (0);
	}
	if (ADhash_get (parc->idhash, &id) != NULL) {
		printf ("ARC: arc_add_data: duplicate resource id (%lu) detected!\n", id);
		return (0);
	}

	item = (arc_item_pt *)malloc (sizeof (arc_item_pt));
	if (item == NULL) return (0);
	memset (item, 0, sizeof (arc_item_pt));

	item->item.data = (void *)malloc (len);
	if (item->item.data == NULL) {
		free (item); return (0);
	}
	memcpy (item->item.data, data, len);

	item->item.type	= type;
	item->item.id	= id;
	item->item.pos	= 0;
	item->item.name	= name ? strdup (name) : strdup ("embedded data");
	item->item.size	= len;
	item->link = NULL;
	item->file = NULL;
	item->nsize = strlen (item->item.name);

	if (!parc->items) parc->items = item;
	if (parc->tail) parc->tail->next = item;
	parc->tail = item;
	parc->arc.count++;

	ADhash_add (parc->idhash, &(item->item.id), item);

	return (1);
}

int
arc_add_link (arc_t *arc, arc_type_t type, unsigned long id, char *name, unsigned long link)
{
	arc_pt		*parc = (arc_pt *)arc;
	arc_item_pt	*item;

	if ((parc == NULL) || (name == NULL) || (link == 0)) abort();

	if (!parc->can_add) {
		printf ("ARC: arc_add_link: add not allowed for this archive!\n");
		return (0);
	}
	if (ADhash_get (parc->idhash, &id) != NULL) {
		printf ("ARC: arc_add_link: duplicate resource id (%lu) detected!\n", id);
		return (0);
	}
	if (ADhash_get (parc->idhash, &link) == NULL) {
		printf ("ARC: arc_add_link: resource link id (%lu) not detected!\n", link);
		return (0);
	}

	item = (arc_item_pt *)malloc (sizeof (arc_item_pt));
	if (item == NULL) return (0);
	memset (item, 0, sizeof (arc_item_pt));

	item->item.type	= type;
	item->item.id	= id;
	item->item.pos	= 0;
	item->item.name	= name ? strdup (name) : strdup ("embedded data");
	item->item.size	= 0;
	item->link = (arc_item_pt *)arc_byid (arc, link);
	item->file = NULL;
	item->nsize = strlen (item->item.name);

	if (!parc->items) parc->items = item;
	if (parc->tail) parc->tail->next = item;
	parc->tail = item;
	parc->arc.count++;

	ADhash_add (parc->idhash, &(item->item.id), item);

	return (1);
}

static INLINE int
iul_min (int a, unsigned long b)
{
	return (((a < 0) || ((unsigned long)a < b)) ? a : b);
}

static void
write_file (int to, char *file, unsigned long size)
{
	int		fd;
	char		buffer[4096];
	unsigned long	todo;
	int		toread, got;

	fd = open (file, O_RDONLY|O_BINARY);
	if (fd == -1) return;

	todo = size;
	while (todo > 0) {
		toread = iul_min (4096, todo);
		got = read (fd, buffer, sizeof (buffer));
		if (got == 0) break;
		write (to, buffer, got);
		todo -= got;
	}

	close (fd);
}

static void
write_data (int to, void *data, unsigned long size)
{
	write (to, data, size);
}

static INLINE unsigned long
align_32bit (unsigned long len)
{
	unsigned long	a;

	a = len;
	if (a & ALIGN32BITMASK) { a = ((len >> ALIGN32BITSHIFT) + 1) << ALIGN32BITSHIFT; }

	//printf ("# align_32bit (len=%lu) = %lu\n", len, a);

	return (a);
}

int
arc_store (arc_t *arc)
{
	arc_pt		*parc = (arc_pt *)arc;
	arc_item_pt	*item;
	header_t	header;
	item_t		*items = NULL;
	item_t		**links = NULL;
	unsigned long	i, j, npos, dpos;
	int		fd;
	name_t		*name;
	char		*names = NULL, *p;

	if (parc == NULL) return (0);

	memset (&header, 0, sizeof (header_t));
	memcpy (&header.magic, HEADER_MAGIC, sizeof (header.magic));
	header.version = HEADER_VERSION;
	header.size    = sizeof (header_t);
	header.count   = parc->arc.count;

	header.sigused = parc->arc.sigused;
	if (header.sigused) {
		memcpy (&(header.sig), parc->arc.sig, sizeof (header.sig));
	}

	header.st_items = header.size;
	header.sz_items = sizeof (item_t) * header.count;
	items = (item_t *)malloc (header.sz_items);
	if (items == NULL) goto handle_error;
	memset (items, 0, header.sz_items);
	links = (item_t **)malloc (sizeof (item_t *) * header.count);
	if (links == NULL) goto handle_error;
	memset (links, 0, sizeof (item_t *) * header.count);

	item = parc->items; i = 0; header.sz_names = 0;
	while (item != NULL) {
		items[i].type = item->item.type;
		items[i].id   = item->item.id;
		links[i] = NULL;

		if (!item->link) {
			items[i].size = item->item.size;
		} else {
			items[i].size = item->link->item.size;
			for (j=0; j<i; j++) {
				if (items[j].id == item->link->item.id) links[i] = &(items[j]);
			}
		}

		header.sz_names += align_32bit (item->nsize + sizeof (items->size));

		item = item->next; i++;
	}
	header.st_names = header.st_items + header.sz_items;

	names = (char *)malloc (header.sz_names);
	if (names == NULL) goto handle_error;
	memset (names, 0, header.sz_names);

	header.st_data = header.st_names + header.sz_names;

	item = parc->items; i = 0; header.sz_data = 0;
	npos = header.st_names; dpos = header.st_data;
	p = names;
	while (item != NULL) {
		if (!item->link) {
			header.sz_data += items[i].size;

			items[i].pos = dpos;
			dpos += items[i].size;
		} else {
			items[i].pos = links[i]->pos;
		}

		items[i].name = npos;
		npos += align_32bit (item->nsize + sizeof (items->size));

		name = (name_t *)p;
		name->size = item->nsize;
		memcpy (name->name, item->item.name, item->nsize);
		p += align_32bit (item->nsize + sizeof (items->size));

		item = item->next; i++;
	}

	fd = open (parc->arc.name, O_RDWR|O_CREAT|O_TRUNC|O_BINARY, 0666);
	if (fd == -1) goto handle_error;

	write (fd, &header, header.size);
	write (fd, items, header.sz_items);
	write (fd, names, header.sz_names);

	item = parc->items;
	while (item != NULL) {
		switch (item->item.type) {
			case arc_BIN:
			case arc_IMG:
			case arc_IMGCOL:
			default:
				write_file (fd, item->file, item->item.size);
				break;
			case arc_RGB:
			case arc_RGBA:
			case arc_XPM:
				write_data (fd, item->item.data, item->item.size);
				break;
		}
		item = item->next;
	}

	close (fd);

	free (names);
	free (links);
	free (items);

	return (1);

handle_error:
	if (names) free (names);
	if (links) free (links);
	if (items) free (items);
	return (0);
}

arc_t *
arc_load (char *filename, arc_sig_t *sig)
{
	arc_pt		*parc;
	int		fd;
	header_t	header;
	int		got;
	int		et;
	unsigned long	ec;
	unsigned long	i;
	arc_item_pt	*item_info, *last;
	item_t		*item_data;
	name_t		*name_data;


	if (filename == NULL) abort();

	fd = open (filename, O_RDONLY|O_BINARY);
	if (fd == -1) return (NULL);

	parc = (arc_pt *)malloc (sizeof (arc_pt));
	if (parc == NULL) { close (fd); return (NULL); }
	memset (parc, 0, sizeof (arc_pt));

	parc->idhash = ADhash_setup (IDHASHBUCKETS, id_hash_func, id_hash_match, NULL);
	if (parc->idhash == NULL) { free (parc); close (fd); return (NULL); }

	parc->fnhash = ADhash_setup (FNHASHBUCKETS, fn_hash_func, fn_hash_match, NULL);
	if (parc->fnhash == NULL) { ADhash_destroy (parc->idhash); free (parc); close (fd); return (NULL); }

	parc->arc.name = strdup (filename);

	memset (&header, 0, sizeof (header));
	got = read (fd, &header, sizeof (header));
	if (got != sizeof (header)) {
		fprintf (stderr, "*** Error: unexpected EOF reading \"%s\"!\n", parc->arc.name);
		free (parc->arc.name);
		free (parc);
		close (fd);
		return (NULL);
	}
	if (header.size != sizeof (header)) {
		fprintf (stderr, "*** Error: header size mismatch: expected %u but got %lu!\n",
				sizeof (header), header.size);
		free (parc->arc.name);
		free (parc);
		close (fd);
		return (NULL);
	}
	if (strncmp ((const char *)header.magic, HEADER_MAGIC, sizeof (header.magic)) != 0) {
		int	bs;
		char	*bf;

		bs = sizeof (header.magic + 1);
		bf = (char *)malloc (bs);
		memset (bf, 0, bs);
		memcpy (bf, header.magic, bs - 1);
		fprintf (stderr, "*** Error: header magic mismatch: expected \"%s\" but got \"%s\"!\n",
				HEADER_MAGIC, bf);
		free (bf);
		free (parc->arc.name);
		free (parc);
		close (fd);
		return (NULL);
	}
	if (header.version > HEADER_VERSION) {
		fprintf (stderr, "*** Error: header version mismatch: expected %lu but got %lu!\n",
				HEADER_VERSION, header.version);
		free (parc->arc.name);
		free (parc);
		close (fd);
		return (NULL);
	}

	parc->arc.count = header.count;

	if (header.sigused && (sig != NULL)) {
		if (memcmp (&(header.sig), sig, sizeof (header.sig)) != 0) {
			fprintf (stderr, "*** Error: archive signature mismatch!\n");
			free (parc->arc.name);
			free (parc);
			close (fd);
			return (NULL);
		}
		parc->arc.sigused = (unsigned char)(header.sigused & 0xff);
		memcpy (&(parc->arc.sig), &(header.sig), sizeof (header.sig));
	}

	parc->map = mmapfile (fd, &et, &ec);
	if (parc->map == NULL) {
		fprintf (stderr, "*** Error: failed to map archive file into memory!\n");
		fprintf (stderr, "*** Error type = %d, code = %lu\n", et, ec);
		free (parc->arc.name);
		free (parc);
		close (fd);
		return (NULL);
	}

	last = NULL;
	item_data = (item_t *)((char *)parc->map + header.st_items);
	for (i = 0; i < parc->arc.count; i++) {

		//printf ("# new item data {id=%lu, name=%lu, pos=%lu, size=%lu }\n",
		//		item_data[i].id, item_data[i].name, item_data[i].pos, item_data[i].size);

		item_info = (arc_item_pt *)malloc (sizeof (arc_item_pt)); if (item_info == NULL) abort();
		memset (item_info, 0, sizeof (arc_item_pt));

		item_info->item.type = item_data[i].type;
		item_info->item.id   = item_data[i].id;
		item_info->item.pos  = item_data[i].pos;
		item_info->item.size = item_data[i].size;
		item_info->item.data = (void *)((char *)parc->map + item_data[i].pos);

		name_data = (name_t *)((char *)parc->map + item_data[i].name);

		//printf ("# new name data {size=%lu, name=...} \n", name_data->size);

		item_info->nsize = name_data->size;
		item_info->item.name = (char *)malloc (item_info->nsize + 1);
		if (item_info->item.name == NULL) abort();
		memset (item_info->item.name, 0, item_info->nsize + 1);
		memcpy (item_info->item.name, name_data->name, item_info->nsize);

		if (last == NULL) parc->items = item_info; else last->next = item_info;
		last = item_info;

		ADhash_add (parc->idhash, &(item_info->item.id), item_info);
		ADhash_add (parc->fnhash, item_info->item.name, item_info->item.name);

		//printf ("# new item {id=%lu, name=\"%s\", nsize=%lu, pos=%lu, size=%lu }\n",
		//		item_info->id, item_info->name, item_info->nsize, item_info->pos, item_info->size);
	}

	close (fd);

	return ((arc_t *)parc);
}

arc_item_t *
arc_next (arc_t *arc, arc_item_t *prev)
{
	arc_pt		*parc = (arc_pt *)arc;
	arc_item_pt	*pprev = (arc_item_pt *)prev;

	if (pprev == NULL) return ((arc_item_t *)parc->items);
	return ((arc_item_t *)pprev->next);
}

arc_item_t *
arc_byid (arc_t *arc, unsigned long id)
{
	arc_pt		*parc = (arc_pt *)arc;
	arc_item_t	*item;

	if (parc == NULL) return (NULL);

	item = (arc_item_t *)ADhash_get (parc->idhash, &id);

	return (item);
}

void
arc_close (arc_t *arc)
{
	arc_pt		*parc = (arc_pt *)arc;
	arc_item_pt	*p, *q;

	if (!arc) return;

	p = parc->items;
	while (p) {
		q = p; p = p->next;
		if (!parc->map) { if (q->item.data) free (q->item.data); }
		if (q->item.name) free (q->item.name);
		if (q->file) free (q->file);
		free (q);
	}
	ADhash_destroy (parc->idhash);
	ADhash_destroy (parc->fnhash);
	if (parc->map) unmmap (parc->map);
	if (parc->arc.name) free (parc->arc.name);
	free (parc);
}