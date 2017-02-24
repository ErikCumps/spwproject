/** \file
 * The SPWaW war cabinet - GUI - simple errors.
 *
 * Copyright (C) 2017 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "../common.h"



/* --- forward declarations --- */



/* --- private types  --- */

typedef struct s_SIMPLE_ERROR {
	SL_ERROR	error;
	MSGBOX_TYPE	type;
	char		*title;
	char		*message;
} SIMPLE_ERROR;



/* --- private macros  --- */



/* --- private variables --- */

static SIMPLE_ERROR simple_errors[] = {
	{ ERR_DOSSIER_INCOMPATIBLE, MSGBOX_ERROR,
		"Failed to load incompatible dossier!",
		"This dossier was created by another version of warcab.\n"
		"\n"
		"Because its file format version is not compatible, it\n"
		"cannot be reliably loaded by this version of warcab.\n"
	},
	{ SL_ERR_NO_ERROR, MSGBOX_ERROR, NULL, NULL }
};



/* --- code --- */

static SIMPLE_ERROR *
lookup_simple_error (SL_ERROR error)
{
	unsigned int	i = 0;

	while (simple_errors[i].error != SL_ERR_NO_ERROR) {
		if (simple_errors[i].error == error)
			return &(simple_errors[i]);
		i++;
	}
	return (NULL);
}

bool
GUI_handle_simple_error (SL_ERROR error)
{
	SIMPLE_ERROR	*p;

	p = lookup_simple_error (error);
	if (p) {
		GUI_msgbox (p->type, p->title, p->message);
	}
	return (p ? true : false);
}
