/* 
 * $Id: graphprim.h,v 1.4 2004/03/14 00:24:55 jd Exp $
 * GrapeFruit Copyright (C) 2002,2003 Cristelle Barillon & Jean-Daniel Pauget
 * a whole set of graphical utilities for SDL
 *
 * grapefruit@disjunkt.com  -  http://grapefruit.disjunkt.com/
 * 
 * This program is free software; you can redistribute it and/or
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 * you can also try the web at http://www.gnu.org/
 *
 */


#ifndef GRAPHPRIM_H_INC
#define GRAPHPRIM_H_INC

#include <list>

#include "SDL.h"

namespace grapefruit
{
    using namespace std ;

    //! a 2-dimensions cartesian point
    class point
    {   public:
	    int x, y;
	    inline point (int x, int y) { point::x = x, point::y = y; }
    };

    //! a list of 2-dimensions cartesian points
    class Lpoint : public list<point>
    {	public:
	    void inline push (int x, int y) { point p(x,y); push_back (p); }
    };

    void polyline (SDL_Surface *Sdest, Lpoint & l, Uint32 pixel, bool isclose = false);
    void polyfill (SDL_Surface *Sdest, Lpoint & l, Uint32 pixel);
}

#endif // GRAPHPRIM_H_INC

/*
 * $Log: graphprim.h,v $
 * Revision 1.4  2004/03/14 00:24:55  jd
 * documentation updates
 *
 * Revision 1.3  2003/05/15 22:43:55  jd
 * tuned polyline
 *
 * Revision 1.2  2003/04/22 16:39:28  jd
 * small libtool revision info tuning
 *
 * Revision 1.1.1.1  2003/04/22 14:04:34  jd
 * librarisation of grapfruit
 *
 *
 */
