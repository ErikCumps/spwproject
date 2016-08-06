#include "stdafx.h"
#include <spwawlib_api.h>
#include "gamefile/gamefile.h"
#include "common/internal.h"

SPWAW_ERROR
load_from_game (GAMEDATA *src, SPWAW_SNAPSHOT *dst)
{
	SPWAW_ERROR	rc;
	STRTAB		*stab = NULL;
	UNIT_LIST	ul1, ul2;

	CNULLARG (src); CNULLARG (dst);
	stab = (STRTAB *)(dst->stab);

	dst->raw.game.cmt.title = azstrstab (src->cmt.title, stab);
	dst->raw.game.cmt.mapsrc = azstrstab (src->cmt.mapsrc, stab);

	rc = sec38_save_snapshot (src, dst, stab);	ROE ("sec38_save_snapshot()");
	rc = sec39_save_snapshot (src, dst, stab);	ROE ("sec39_save_snapshot()");
	rc = sec37_save_snapshot (src, dst, stab);	ROE ("sec37_save_snapshot()");
	rc = sec08_save_snapshot (src, dst, stab);	ROE ("sec08_save_snapshot()");
	rc = sec14_save_snapshot (src, dst, stab);	ROE ("sec14_save_snapshot()");

	memset (&ul1, 0, sizeof (ul1));
	memset (&ul2, 0, sizeof (ul2));

	rc = sec35_save_snapshot (src, dst, stab);		ROE ("sec35_save_snapshot()");
	rc = sec01_save_snapshot (src, dst, stab, ul1, ul2);	ROE ("sec01_save_snapshot()");
	rc = sec34_save_snapshot (src, dst, stab, ul1, ul2);	ROE ("sec34_save_snapshot()");
	rc = sec17_save_snapshot (src, dst, stab, ul1, ul2);	ROE ("sec17_save_snapshot()");

	rc = sec26_save_snapshot (src, dst, stab);	ROE ("sec26_save_snapshot()");

	return (SPWERR_OK);
}
