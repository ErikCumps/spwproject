/** \file
 * The SPWaW war cabinet - strategic map - hex position object.
 *
 * Copyright (C) 2012-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	STRATMAP_HEXPOS_H
#define	STRATMAP_HEXPOS_H	1

/*! This class represents the position of a hex in a grid */
class SmapHexPos
{
public:
	SmapHexPos	(void);
	~SmapHexPos	(void);

public:
	/*! Set the grid position in x and y coordinates */
	SmapHexPos &	set	(int posx, int posy);

	/*! Calculate the grid array index of this hex in a grid with the specified width and height */
	inline int
	calculate_idx (int width, int height)
	{
		if (!d.has_idx) {
			idx = (((x < 0)||(x >= width)||(y < 0)||(y >= height)) ? -1 : y*width+x);
			d.has_idx = true;
		}
		return (idx);
	}

	inline bool operator== (const SmapHexPos &rhs) { return ((x == rhs.x) && (y == rhs.y)); }
	inline bool operator!=(const SmapHexPos &rhs) { return ((x != rhs.x) || (y != rhs.y)); }

public:
	int	x;	/*!< grid x coordinate	*/
	int	y;	/*!< grid y coordinate	*/
	int	idx;	/*!< grid array index	*/

private:
	struct s_data {
		bool	has_idx;
	} d;
};

#endif	/* STRATMAP_HEXPOS_H */