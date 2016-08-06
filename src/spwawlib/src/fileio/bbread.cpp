#include "stdafx.h"
#include "fileio/fileio.h"
#include "common/internal.h"

BBR *
bbread_init (int fd, long len)
{
	BBR	*bb = NULL;

	bb = safe_malloc (BBR); COOMRET (bb, "BBR", NULL);

	bb->file.fd   = fd;
	bb->file.left = len;
	
	bb->bsize = sizeof (bb->buffer);
	memset (bb->buffer, 0, bb->bsize);
	bb->left = 0;
	bb->idx  = bb->bsize;

	return (bb);
}

bool
bbread (BBR *bb, char *b)
{
	int	todo, done;

	if (!bb || !b) {
		ERROR0 ("unexpected NULL argument");
		return (false);
	}

	if (!bb->left) {
		if (!bb->file.left) {
			ERROR0 ("attempt to read beyond end of file block");
			return (false);
		}
		todo = bb->bsize; if (bb->file.left < todo) todo = bb->file.left;

		done = read (bb->file.fd, bb->buffer, todo);

#if	IOLOGGING
		log ("read(fd=%d, len=%d) = %d\n", bb->file.fd, todo, done);
#endif	/* IOLOGGING */

		if (done == -1) {
			ERROR2 ("failed to read from file (errno=%lu)", bb->file.fd, errno);
			return (false);
		}

		bb->idx = 0;
		bb->left = done;
		bb->file.left -= done;

	}
	if (!bb->left) return (false);

	*b = bb->buffer[bb->idx];
	bb->idx++;
	bb->left--;
	return (true);
}

bool
bbread (BBR *bb, char *b, int len, bool noerr)
{
	bool	rc = true;
	int	left;
	char	*p;
	int	todo, done;

	if (!bb || !b) {
		ERROR0 ("unexpected NULL argument");
		return (false);
	}

	left = len; p = b;
	while (left) {
		if (!bb->left) {
			if (!bb->file.left) {
				ERROR0 ("attempt to read beyond end of file block");
				return (false);
			}
			todo = bb->bsize; if (bb->file.left < todo) todo = bb->file.left;

			done = read (bb->file.fd, bb->buffer, todo);

#if	IOLOGGING
			log ("read(fd=%d, len=%d) = %d\n", bb->file.fd, todo, done);
#endif	/* IOLOGGING */

			if (done == -1) {
				if (noerr) {
					bb->file.left = bb->left = left = 0;
					break;
				} else {
					ERROR1 ("failed to read from file (errno=%lu)", errno);
					return (false);
				}
			}

			bb->idx  = 0;
			bb->left = done;
			bb->file.left -= done;
		}
		if (!bb->left) {
			rc = false;
			break;
		}
		todo = left; if (bb->left < todo) todo = bb->left;
		memcpy (p, &(bb->buffer[bb->idx]), todo);
		bb->idx  += todo;
		bb->left -= todo;
		p    += todo;
		left -= todo;
	}

	return (rc);
}

void
bbread_stop (BBR *bb)
{
	if (!bb) return;

	bb->file.fd = 0;
	free (bb);
}
