/* 
 * $Id$
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
 * grapefruit@disjunkt.com  -  http://grapefruit.disjunkt.com/
 */

#include <math.h>

#include "grapefruit.h"
#include <bzouz/bzouz.h>

namespace grapefruit
{
    TDfps::TDfps (int nbmoy) : TDString ("")
    {	lasttime = 0xffffffff;
	lt = NULL;
	setnbmoy (nbmoy);
	setcolor (Vector4(1.0,0.5,0.0,0.0));	// orange
	setwidth (20.0);			// thick !
    }

    TDfps::~TDfps (void)
    {	
	if (lt != NULL)
	    delete[] lt;
	lt = NULL;
    }

    int TDfps::setnbmoy (int nbmoy)
    {	
	if (lt != NULL)
	    delete[] lt;

	lt = new Uint32 [nbmoy];
	if (lt == NULL) {
	    bzouzerr << " could not allocate for " << nbmoy << " Uint32 ! skipping" << endl;
	    nbmoy = 0;
	} else {
	    int i;
	    for (i=0 ; i<nbmoy ; i++)
		lt[i] = 0;
	}
	TDfps::nbmoy = nbmoy;
	ilt = 0;
	sdt = 0;
	nbval = 0;

	return TDfps::nbmoy;
    }
    
    void TDfps::render (void)
    {
	Uint32 curtime = SDL_GetTicks();
	Uint32 dt = curtime-lasttime;

	string s;

	if (dt == 0)
	     s = "max";
	else
	    s = itos (floor(1000.0/dt));

	if (lt != NULL) {
	    s += "/s\n";
	    sdt += dt;
	    sdt -= lt[ilt];
	    lt[ilt] = dt;
	    ilt ++; if (ilt == nbmoy) ilt = 0;
	    if (nbval < nbmoy) nbval++;

	    if (sdt == 0)
		s += "max";
	    else
		s += itos (floor((nbval*1000.0) / sdt));
	}

	s += "/s";
	update (s);
	TDString::render ();
	lasttime = curtime;
    }


    const string & TDfps::gettdname (void)
    {
static const string tdfpsname ("TDfps") ;
	return tdfpsname;
    }

}   // namespace grapefruit
