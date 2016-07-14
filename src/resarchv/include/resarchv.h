/** \file
 * Resource Archive - API header file.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	RESARCHV_H
#define	RESARCHV_H	1

/* Support C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/*! Archive signature data type */
typedef unsigned char arc_sig_t[8];

/*! Archive object */
typedef struct {
	char		*name;		/*!< archive filename			*/
	unsigned long	count;		/*!< archive item count			*/
	unsigned char	sigused;	/*!< archive signature enabled flag	*/
	arc_sig_t	sig;		/*!< archive signature			*/
} arc_t;

/*! Archive item type */
typedef enum {
	arc_BIN = 0,	/*!< binary data of unspecified type	*/
	arc_IMG,	/*!< image				*/
	arc_IMGCOL,	/*!< image collection			*/
	arc_RGB,	/*!< RGB color triplet			*/
	arc_RGBA,	/*!< RGBA color quadlet			*/
	arc_XPM,	/*!< XPM resource			*/
	arc_TYPE_LIMIT = arc_XPM	/*!< \internal final code */
} arc_type_t;

/*! Archive item type lookup function
 *
 * \param type	archive item type
 * \return	Pointer to const string with archive item type name
 */
extern const char *	arc_type_name	(arc_type_t type);

/*! Archive item type description lookup function
 *
 * \param type	archive item type
 * \return	Pointer to const string with archive item type description
 */
extern const char *	arc_type_lookup	(arc_type_t type);

/*! Archive item object */
typedef struct {
	unsigned long	id;	/*!< item ID */
	char		*name;	/*!< item name */
	arc_type_t	type;	/*!< item type */
	unsigned long	pos;	/*!< item position in archive file */
	unsigned long	size;	/*!< item size */
	void		*data;	/*!< pointer to item data (NULL if not loaded) */
} arc_item_t;

/*! Create new empty archive
 * \param filename	name for new resource archive file
 * \param sig		pointer to signature for archive
 * \return		pointer to new archive object
 *
 * \note The signature will be disabled if \c sig is \c NULL
 */
extern arc_t *		arc_new		(char *filename, arc_sig_t *sig);

/*! Add item to archive, extracting data from file
 * \param arc	pointer to archive object
 * \param type	new item type
 * \param id	new item id
 * \param name	new item name
 * \param file  name of file containing new item data
 * \return	operation result code
 * \retval 0	failure
 * \retval 1	success
 *
 * \note This is only allowed for new archives that have not yet been stored to file!
 */
extern int		arc_add_file	(arc_t *arc, arc_type_t type, unsigned long id, char *name, char *file);

/*! Add item to archive, data obtained from buffer
 * \param arc	pointer to archive object
 * \param type	new item type
 * \param id	new item id
 * \param name	new item name
 * \param data	data for new item
 * \param len	length of data for new item
 * \return	operation result code
 * \retval 0	failure
 * \retval 1	success
 *
 * \note This is only allowed for new archives that have not yet been stored to file!
 */
extern int		arc_add_data	(arc_t *arc, arc_type_t type, unsigned long id, char *name, void *data, unsigned int len);

/*! Add item link to archive
 * \param arc	pointer to archive object
 * \param type	new item type
 * \param id	new item id
 * \param name	new item name
 * \param link	new item link id
 * \return	operation result code
 * \retval 0	failure
 * \retval 1	success
 *
 * \note This is only allowed for new archives that have not yet been stored to file!
 */
extern int		arc_add_link	(arc_t *arc, arc_type_t type, unsigned long id, char *name, unsigned long link);


/*! Store archive to file
 * \param arc	pointer to archive object
 * \return	operation result code
 * \retval 0	failure
 * \retval 1	success
 */
extern int		arc_store	(arc_t *arc);

/*! Load archive from file
 * \param filename	name of resource archive file
 * \param sig		pointer to signature for checking
 * \return		pointer to archive object
 *
 * \note Signature will not be checked if \c sig is \c NULL or if
 * resource archive file has no signature.
 */
extern arc_t *		arc_load	(char *filename, arc_sig_t *sig);

/*! Access next archive item
 * \param arc	pointer to archive object
 * \param prev	pointer to previous archive item object
 * \return	pointer to next archive item object
 *
 * \note Returns a pointer to the first item if \c prev is \c NULL.
 */
extern arc_item_t *	arc_next	(arc_t *arc, arc_item_t *prev);

/*! Access archive item by item id
 * \param arc	pointer to archive object
 * \param id	item id
 * \return	pointer to archive item object
 *
 * \note Returns \c NULL if item not found.
 */
extern arc_item_t *	arc_byid	(arc_t *arc, unsigned long id);

/*! Close open archive
 * \param arc	pointer to archive object
 */
extern void		arc_close	(arc_t *arc);


/* Support C++ compilers */
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif	/* RESARCHV_H */
