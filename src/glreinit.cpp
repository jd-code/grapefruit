/* 
 * $Id$
 * GrapeFruit Copyright (C) 2002,2003,2004 Cristelle Barillon & Jean-Daniel Pauget
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

#include "grapefruit.h"

using namespace grapefruit ;

namespace grapefruit
{

void GLReInit::subscribe (void)
{
    lglreinit.push_back (this);
}

void GLReInit::unsubscribe (void)
{
    lglreinit.remove (this);
}

int glreinit (void)
{   list <GLReInit *>::iterator li;
    for (li=GLReInit::lglreinit.begin() ; li!=GLReInit::lglreinit.end() ; li++)
	(*li)->perform_reinit();
    return 0;
}

};

