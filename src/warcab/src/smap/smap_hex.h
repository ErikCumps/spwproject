/** \file
 * The SPWaW war cabinet - strategic map - hex object.
 *
 * Copyright (C) 2012-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	STRATMAP_HEX_H
#define	STRATMAP_HEX_H	1

#include "smap_hexdata.h"
#include "smap_hexpos.h"

/*! This class represents a hex in a strategic map */
class SmapHex
{
public:
	SmapHex		(void);
	~SmapHex	(void);

public:
	/*! Set the grid position from s SmapHexPos */
	void	setPos		(SmapHexPos &pos);

	/*! Set the height of this hex from the raw height */
	void	setHeight	(int raw_height);

	/*! Set the water flag of this hex */
	void	setWater	(bool has_water);

	/*! Set the bridge flag of this hex */
	void	setBridge	(bool has_bridge);

	/*! Set the road connections for this hex */
	void	setRoadConn	(int road1, int road2, int railr, int traml);

	/*! Set the victory hex status of this hex */
	void	setVicHex	(SPWAW_VHSTATUS owner);

	/*! Increment the unit count for this hex */
	void	addUnit		(SMAP_HI influence);

public:
	/*! Does this hex contain a bridge? */
	bool	hasBridge	(void);

	/*! Has this hex one or more roads? */
	bool	hasRoad		(SMAP_HRD dir);

public:
	SmapHexPos	pos;			/*!< The position of this hex in the map grid					*/
	SMAP_HH		height;			/*!< The binned height of this hex						*/
	int		actheight;		/*!< The actual height of this hex (in meters)					*/
	bool		water;			/*!< True if the hex contains water						*/
	bool		bridge;			/*!< True if the hex contains a bridge						*/
	int		conn_road1;		/*!< The primary road connections map for this hex				*/
	int		conn_road2;		/*!< The secondary road connections map for this hex				*/
	int		conn_railr;		/*!< The railroad connections map for this hex					*/
	int		conn_traml;		/*!< The tramline connections map for this hex					*/
	int		conn_bridge;		/*!< The bridge connections map for this hex					*/
	bool		vic_hex;		/*!< True if this hex is a victory hex						*/
	SMAP_HI		vic_hex_owner;		/*!< The owner of the victory hex						*/
	int		unit_cnt_blue;		/*!< The number of blue units in this hex					*/
	int		unit_cnt_red;		/*!< The number of red units in this hex					*/
	int		influence_blue_cnt;	/*!< The number of blue units contributing to the blue influence in this hex	*/
	int		influence_red_cnt;	/*!< The number of red units contributing to the red influence in this hex	*/
	double		influence_blue;		/*!< The blue influence in this hex						*/
	double		influence_red;		/*!< The red influence in this hex						*/
	SMAP_HI		influence;		/*!< The global influence assignment for this hex				*/
	int		frontline;		/*!< The frontline edge map for this hex					*/

private:
	struct s_data {
		bool	ready;
	} d;
};

#endif	/* STRATMAP_HEX_H */