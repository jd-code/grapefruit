/* 
 * $Id$
 * GrapeFruit Copyright (C) 2002,2003 Cristelle Barillon & Jean-Daniel Pauget
 * a whole set of graphical utilities for SDL
 *
 * grapefruit@disjunkt.com  -  http://grapefruit.disjunkt.com/
 * 
 * GrapeFruit program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with GrapeFruit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 * you can also try the web at http://www.gnu.org/
 *
 * grapefruit@disjunkt.com  -  http://grapefruit.disjunkt.com/
 */

#ifndef   GRAPEFRUIT_H_INC
#warning tdfps.h should not be included as is. grapefruit.h should be included instead
#else  // GRAPEFRUIT_H_INC
#ifdef	    GRAPEFRUIT_H_GLOBINST
#define	    AGTDFPS_H_GLOBINST
#endif //   GRAPEFRUIT_H_GLOBINST
#endif // GRAPEFRUIT_H_INC

#ifndef AGTDFPS_H_HEADER
#define AGTDFPS_H_HEADER

namespace grapefruit
{
    //! this TD displays the number of render call per seconds (fps) and an average for the last nbmoy values

    class TDfps : public TDString, public TDObjSHable
    {
	    Uint32 lasttime;	//!< the last time render was called

	    int nbmoy;		//!< the number of values used for average (moyenne) calculation
	    Uint32 *lt;		//!< the array for the last nbmoy time-intervals
	    int ilt;		//!< the ring-array index
	    int nbval;		//!< number of valid measure stored in the array
	    Uint32 sdt;		//!< the sum of the last nbmoy time intervals

	public:
	    virtual ~TDfps(void);
	    TDfps (int nbmoy=10);
	    int setnbmoy (int nbmoy);	//!< updates the number of nbmoy values used for average

	    virtual void render (void);
	    virtual const string & gettdname (void);
    };

}   // namespace grapefruit

#endif	// AGMENU_H_HEADER

