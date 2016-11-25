/** \file
 * The SPWaW war cabinet - strategic map - rendering.
 *
 * Copyright (C) 2012-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	STRATMAP_RENDERER_H
#define	STRATMAP_RENDERER_H	1

#include "smap_hexgrid.h"
#include "smap_renderdata.h"

/*! Hex position in renderer coordinates */
typedef struct s_SMAP_RENDERER_HEX {
	int		posx;		/*!< The x pixel coordinate in renderer space		*/
	int		posy;		/*!< The y pixel coordinate in renderer space		*/
} SMAP_RENDERER_HEX;

/*! Hex grid with positions in renderer coordinates */
typedef struct s_SMAP_RENDERER_GRID {
	int			width;	/*!< The width of the grid				*/
	int			height;	/*!< The height of the grid				*/
	SMAP_RENDERER_HEX	*map;	/*!< The array containing the hex positions of the grid	*/
} SMAP_RENDERER_GRID;

/*! This class renders the strategic map for diaplay by the SmapWidget */
class SmapRenderer
{
public:
	/*! Creates a renderer using the specified render data descriptor */
	SmapRenderer	(SMAP_RENDERDATA &renderdata);
	~SmapRenderer	(void);

public:
	/*! Prepares the renderer for rendering the specified strategic map hex grid */
	void		forGrid	(int marginx, int marginy, SmapHexGrid &smap);

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
		QPixmap *frontline;	/*!< The frtonline(s)							*/
		QPixmap	*dots;		/*!< The unit dots							*/
	} layer;			/*!< The layers of the rendered map					*/

private:
	struct s_data {
		int	pm_width;	/*!< The width (in pixels) of the QPixmaps used for rendering		*/
		int	pm_height;	/*!< The height (in pixels) of the QPixmaps used for rendering		*/

		int	hex_side;	/*!< The height (in pixels) of a hex east/west edge			*/
		int	hex_top;	/*!< The height (in pixels) above/below a hex east/west edge		*/

		int	hmove_x;	/*!< The X step (in pixels) for moving horizontally to a neighbour hex	*/
		int	hmove_y;	/*!< The Y step (in pixels) for moving horizontally to a neighbour hex	*/
		int	vmove_x;	/*!< The X step (in pixels) for moving vertically to a neighbour hex	*/
		int	vmove_y;	/*!< The Y step (in pixels) for moving vertically to a neighbour hex	*/
	
		struct s_pm {
			QPixmap	border;					/*!< Hex border				*/
			QPixmap	selection;				/*!< Hex selection indicator		*/
			QPixmap	hexmask[SMAP_HM_CNT];			/*!< Hex render masks			*/
			QPixmap	transparent;				/*!< Transparency mask			*/
			QPixmap	height[SMAP_HH_CNT*SMAP_HM_CNT];	/*!< Height map colorfields		*/
			QPixmap	water[SMAP_HM_CNT];			/*!< Water feature colorfields		*/
			QPixmap	bridge[SMAP_HRD_CNT];			/*!< Bridge feature sections		*/
			QPixmap	road1[SMAP_HRD_CNT];			/*!< Primary road feature sections	*/
			QPixmap	road2[SMAP_HRD_CNT];			/*!< Secondary road feature sections	*/
			QPixmap	railr[SMAP_HRD_CNT];			/*!< Reailroad feature sections		*/
			QPixmap	inf_blue[SMAP_HM_CNT];			/*!< Blue influence colorfields		*/
			QPixmap	inf_red[SMAP_HM_CNT];			/*!< Red influence colorfields		*/
			QPixmap	inf_neutral[SMAP_HM_CNT];		/*!< Neutral influence colorfields	*/
			QPixmap	bluedot[16];				/*!< Blue unit indicators		*/
			QPixmap	reddot[16];				/*!< Red unit indicators		*/
			QPixmap	vh_blue;				/*!< Victory hex claimed by blue player	*/ 
			QPixmap vh_red;					/*!< Victory hex claimed by blue player	*/
			QPixmap vh_neutral;				/*!< Victory hex, not yet claimed	*/
			QPixmap	frontline[SMAP_HRD_CNT];		/*!< Frontline edges			*/
		} pm;			/*!< The QPixmaps used for rendering					*/
	
		SmapHexGrid		*hgrid;		/*!< The strategic map hex grid to render		*/
		SMAP_RENDERER_GRID	rgrid;		/*!< The hex grid in renderer coordinates		*/
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

	/*! Returns the height map hex render mask index for the specified height and mask index */
	inline int
	hthm2idx (int h, int m)
	{
		return (h*SMAP_HM_CNT+m);
	}
};

#endif	/* STRATMAP_RENDERER_H */
