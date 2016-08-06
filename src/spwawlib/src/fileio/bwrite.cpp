#include "stdafx.h"
#include "common/internal.h"

bool
bwrite (int fd, char *b, int len)
{
	int	rc;

	rc = write (fd, b, len);

#if	IOLOGGING
	log ("write(fd=%d, len=%d) = %d\n", fd, len, rc);
#endif	/* IOLOGGING */

	if (rc == -1) ERROR1 ("failed to write to file (errno=%lu)", errno);

	if (rc != len) ERROR2 ("failed to write to file (wanted=%lu, written=%lu)", len, rc);
	return (true);
}
