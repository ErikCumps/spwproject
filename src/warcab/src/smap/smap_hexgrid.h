/** \file
 * The SPWaW war cabinet - strategic map - hex grid object.
 *
 * Copyright (C) 2012-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	STRATMAP_HEXGRID_H
#define	STRATMAP_HEXGRID_H	1

#include "smap_hex.h"

/*! This class represents the hex grid of a strategic map */
class SmapHexGrid
{
public:
	SmapHexGrid	(void);
	~SmapHexGrid	(void);

public:
	/*! Setup a grid for a map of the given width and height.
	 * Any previous grid will be destroyed first!
	 */
	void	setup	(int width, int height);

	/*! Setup a grid, based on another grid.
	 * Any previous grid will be destroyed first!
	 */
	void	setup	(SmapHexGrid &grid);

	/*! Cleanup a grid, releasing allocated memory */
	void	cleanup	(void);

public:
	int	width;		/*!< The width of the grid			*/
	int	height;		/*!< The height of the grid			*/
	bool	dotted;		/*!< Render grid with dotted frontlines?	*/
	SmapHex	*map;		/*!< The array containing the hexes of the grid	*/

public:
	/*! Determine the grid array index of the hex with the specified hex grid coordinates */
	inline int
	grid2idx (int x, int y)
	{
		return (((x < 0)||(x >= width)||(y < 0)||(y >= height)) ? -1 : y*width+x);
	}

	/*! Determine the grid array index of the the specified SmapHexPos */
	inline int
	grid2idx (SmapHexPos &pos)
	{
		return (pos.calculate_idx (width, height));
	}

	/*! Obtain the grid hex at the specified hex grid coordinates */
	inline SmapHex *
	grid2hex (int x, int y)
	{
		int idx = grid2idx (x, y);
		if (idx >= 0) {
			return (&(map[idx]));
		} else {
			return (&(illegal_hex));
		}
	}

	/*! Obtain the grid hex at the specified SmapHexPos */
	inline SmapHex *
	grid2hex (SmapHexPos &pos)
	{
		int idx = grid2idx(pos);
		if (idx >= 0) {
			return (&(map[idx]));
		} else {
			return (&(illegal_hex));
		}
	}

	/*! Convert a SmapHexPos into grid coordinates */
	inline void
	grid2coord (SmapHexPos &pos, int &cx, int &cy)
	{
		cx = 2*pos.x + (pos.y % 2);
		cy = 2*pos.y;
	}

	/*! Determine the SmaphexPos of the hex East of the specified SmapHexPos */
	inline bool
	hexEE (SmapHexPos &pos, SmapHexPos &npos)
	{
		if (grid2idx (npos.set (pos.x + 1, pos.y)) < 0) npos.set (-1, -1);
		return (npos.idx != -1);
	}

	/*! Determine the SmaphexPos of the hex SouthEast of the specified SmapHexPos */
	inline bool
	hexSE (SmapHexPos &pos, SmapHexPos &npos)
	{
		if (grid2idx (npos.set (pos.x + (pos.y % 2), pos.y + 1)) < 0) npos.set (-1, -1);
		return (npos.idx != -1);
	}

	/*! Determine the SmaphexPos of the hex SouthWest of the specified SmapHexPos */
	inline bool
	hexSW (SmapHexPos &pos, SmapHexPos &npos)
	{
		if (grid2idx (npos.set (pos.x - ((pos.y+1) % 2), pos.y + 1)) < 0) npos.set (-1, -1);
		return (npos.idx != -1);
	}

	/*! Determine the SmaphexPos of the hex West of the specified SmapHexPos */
	inline bool
	hexWW (SmapHexPos &pos, SmapHexPos &npos)
	{
		if (grid2idx (npos.set (pos.x - 1, pos.y)) < 0) npos.set (-1, -1);
		return (npos.idx != -1);
	}

	/*! Determine the SmaphexPos of the hex NorthWest of the specified SmapHexPos */
	inline bool
	hexNW (SmapHexPos &pos, SmapHexPos &npos)
	{
		if (grid2idx (npos.set (pos.x - ((pos.y+1) % 2), pos.y - 1)) < 0) npos.set (-1, -1);
		return (npos.idx != -1);
	}

	/*! Determine the SmaphexPos of the hex NorthEast of the specified SmapHexPos */
	inline bool
	hexNE (SmapHexPos &pos, SmapHexPos &npos)
	{
		if (grid2idx (npos.set (pos.x + (pos.y % 2), pos.y - 1)) < 0) npos.set (-1, -1);
		return (npos.idx != -1);
	}

private:
	SmapHex	illegal_hex;	/*!< This default, invalid, SmapHex object is returned when an actual SmapHex can not be found */
};

#endif	/* STRATMAP_HEXGRID_H */