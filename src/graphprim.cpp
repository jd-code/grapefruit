/* 
 * $Id: graphprim.cpp,v 1.4 2004/03/14 00:24:55 jd Exp $
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

#include <map>
#include <iostream>

#include "grapefruit.h"
#include "graphprim.h"

namespace grapefruit
{
    using namespace std ;

    //! draws the sides of a polygon, opened or closed accordingly to "isclosed"
void polyline (SDL_Surface *Sdest, Lpoint & l, Uint32 pixel, bool isclosed)
{
    Lpoint::iterator li1 = l.begin();
    Lpoint::iterator li2 = li1;
    li2++;
    
    for (li1=l.begin() ; li2!=l.end() ; li1++, li2++) {
	line (Sdest, li1->x, li1->y, li2->x, li2->y, pixel);
    }
    li2=l.begin();
    if (isclosed)
	line (Sdest, li1->x, li1->y, li2->x, li2->y, pixel);
}

    //! draws a filled polygon
void polyfill (SDL_Surface *Sdest, Lpoint & l, Uint32 pixel)
{   // JDJDJDJD à finir : les pointes ne sont pas belles essentiellement car l'algorithme des droites
    // n'optimise pas les extremités des droites...

    map <int, list<int> > scts;

    Lpoint::iterator li1;
    Lpoint::iterator li2;
    
    int maxy = Sdest->h;

    for (li1=l.begin() ; li1!=l.end() ; li1++) {
	int x1, y1, x2, y2;
	li2 = li1;
	li2++;
	if (li2 == l.end()) li2=l.begin();

line (Sdest, li1->x, li1->y, li2->x, li2->y, pixel);
	
	if (li2->y == li1->y) {	    // horizontal line
	    scts[li1->y].push_back (li1->x);
	    scts[li1->y].push_back (li2->x);
	} else {		    // diagonal or vertical line
	    if (li2->y > li1->y)
		x1 = li1->x, y1 = li1->y, x2 = li2->x, y2 = li2->y;
	    else
		x2 = li1->x, y2 = li1->y, x1 = li2->x, y1 = li2->y;

	    int x, y;
	    double p = (x2-x1) / (double)(y2-y1);

	    for (y=y1 ; y!=y2 ; y++) {
		x = x1 + (int) ((double)(y - y1) * p);
		if ((y>=0) && (y<maxy))
		{	scts[y].push_back (x);
		}
	    }
	}
    }

    map <int, list<int> >::iterator si;
    list<int>::iterator li;
    SDL_Rect rect; 

    for (si=scts.begin() ; si!=scts.end() ; si++) {
	list<int> &l = si->second;
	l.sort ();
	rect.y = si->first;
	rect.h = 1;

	for (li=l.begin() ; li!=l.end() ; li++) {
	    rect.x = *li;
	    li++;
	    if (li == l.end()) break;
	    rect.w = *li - rect.x + 1;
	    SDL_FillRect (Sdest, &rect, pixel);
	}
	if ((l.size() % 2) != 0)
	    bzouzerr << "---------------------- on a une liste avec nombre impair de sommets ? y=" << si->first << " nb=" << l.size() << endl ;
    }
}


} // namespace grapefruit

/*
 * $Log: graphprim.cpp,v $
 * Revision 1.4  2004/03/14 00:24:55  jd
 * documentation updates
 *
 * Revision 1.3  2003/09/11 23:49:06  jd
 * corrected slow-motion bug in various grabbing code
 * tuned big-motion squiz handling specific to win32 platforms
 * added grabbing code for TDs (first release)
 * added config.h
 *
 * Revision 1.2  2003/05/15 22:44:39  jd
 * tuned polyline
 * tuned includes (sys vs local)
 *
 * Revision 1.1.1.1  2003/04/22 14:04:34  jd
 * librarisation of grapfruit
 *
 * Revision 1.3  2003/04/19 14:58:59  jd
 * first set of cleaning
 *
 */ 
