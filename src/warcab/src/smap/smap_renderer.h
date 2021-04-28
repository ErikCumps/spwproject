/** \file
 * The SPWaW war cabinet - strategic map - rendering.
 *
 * Copyright (C) 2012-2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	STRATMAP_RENDERER_H
#define	STRATMAP_RENDERER_H	1

#include "gui/gui_private.h"
#include "smap_hexgrid.h"
#include "smap_renderdata.h"

/*! Hex position in renderer coordinates */
typedef struct s_SMAP_RENDERER_HEX {
	int			posx;		/*!< The x pixel coordinate in renderer space		*/
	int			posy;		/*!< The y pixel coordinate in renderer space		*/
} SMAP_RENDERER_HEX;

/*! Hex grid with positions in renderer coordinates */
typedef struct s_SMAP_RENDERER_GRID {
	int			width;		/*!< The width of the grid				*/
	int			height;		/*!< The height of the grid				*/
	SMAP_RENDERER_HEX	*map;		/*!< The array containing the hex positions of the grid	*/
} SMAP_RENDERER_GRID;

/*! This class renders the strategic map for diaplay by the SmapWidget */
class SmapRenderer
{
public:
	/*! Creates a renderer using the specified render data descriptor */
	SmapRenderer	(SMAP_RENDERDATA &renderdata);
	~SmapRenderer	(void);

public:
	/*! Returns the render data description string */
	const char	*description (void);

	/*! Prepares the renderer for rendering the specified strategic map hex grid */
	void		forGrid	(int marginx, int marginy, SmapHexGrid &smap, SPWAW_SNAP_MAP_DATA *btlmap);

	/*! Selects an apropriate height colorfield */
	void		selectHCF (SMAP_HPMC_TYPE type, SPWAW_GAME_TYPE gametype, SPWAW_TERRAIN terrain);

	/*! Stops rendering and releases all resources */
	void		cleanup	(void);

	/*! Render the map */
	void		render	(void);

	/*! Uses the specified QPainter to render the cursor at the specified position */
	void		render_cursor (QPainter &painter, SmapHexPos &cursor);

	/*! Converts a SmaphexPos to renderer pixel coordinates */
	bool		hex2pos (SmapHexPos &pos, int &px, int &py);

	/*! Determines the hex containing the specified Qpoint and returns its position in
	 * renderer pixel coordinates and map hex coordinates */
	bool		pos2hex	(const QPoint &pos, int &px, int &py, int &hx, int &hy);

public:
	QRect		canvas;		/*!< The canvas	containing the rendered frame				*/
	QRect		frame;		/*!< The frame containing the rendered map				*/
	QRect		map;		/*!< The rendered map							*/

	struct s_layer {
		QPixmap *hmap;		/*!< The height map							*/
		QPixmap	*features;	/*!< The hex features							*/
		QPixmap *vhex;		/*!< The victory hexes and ownership					*/
		QPixmap *influence;	/*!< The influence map							*/
		QPixmap	*grid;		/*!< The hex grid							*/
		QPixmap *frontline;	/*!< The frontline(s)							*/
		QPixmap	*dots;		/*!< The unit dots							*/
	} layer;			/*!< The layers of the rendered map					*/

private:
	struct s_data {
		const char	*desc;	/*!< The description string of the render data				*/

		int	pm_width;	/*!< The width (in pixels) of the QPixmaps used for rendering		*/
		int	pm_height;	/*!< The height (in pixels) of the QPixmaps used for rendering		*/

		int	hex_side;	/*!< The height (in pixels) of a hex east/west edge			*/
		int	hex_top;	/*!< The height (in pixels) above/below a hex east/west edge		*/

		int	hmove_x;	/*!< The X step (in pixels) for moving horizontally to a neighbour hex	*/
		int	hmove_y;	/*!< The Y step (in pixels) for moving horizontally to a neighbour hex	*/
		int	vmove_x;	/*!< The X step (in pixels) for moving vertically to a neighbour hex	*/
		int	vmove_y;	/*!< The Y step (in pixels) for moving vertically to a neighbour hex	*/

		SMAP_RENDERDATA		*rd;	/*!< The QPixmap renderdata					*/

		SmapHexGrid		*hgrid;	/*!< The strategic map hex grid to render			*/
		SMAP_RENDERER_GRID	rgrid;	/*!< The hex grid in renderer coordinates			*/

		GUIVALTRACK (SPWAW_SNAP_MAP_DATA *, btlmap);	/*!< The current battle map data		*/
		GUIVALTRACK(SMAP_RENDERDATA_HPMC *, hpmc);	/*!< The current height pixmap collection	*/
	} d;

private:
	/*! Returns the hex render mask index for the specified hex grid coordinates */
	inline SMAP_HM
	grid2hm (int x, int y)
	{
		SMAP_HM	hm;

		if ((x == (d.rgrid.width-1)) && (y == (d.rgrid.height-1))) {
			hm = SMAP_HM_BR;
		} else if (y == (d.rgrid.height-1)) {
			hm = SMAP_HM_B;
		} else if (x == (d.rgrid.width-1)) {
			if (y % 2)
				hm = SMAP_HM_RO;
			else
				hm = SMAP_HM_RE;
		} else if ((x == 0) && ((y+1) % 2)) {
			hm = SMAP_HM_LE;
		} else {
			hm = SMAP_HM_C;
		}

		return (hm);
	}
};

#endif	/* STRATMAP_RENDERER_H */
