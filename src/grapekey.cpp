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
 */

#include "grapefruit.h"

namespace grapefruit
{

// --------------- Action -----------------------------------------------------
//
//! canvas for every and any performable action in our software

    Action::Action (void)
    {	handle = n_occur++;
	mactions[handle] = this;
    }

    Action::~Action (void)
    {	mactions.erase(handle);
    }

// -------------- ActionPool --------------------------------------------------
//
//! pool of Actions, with cross-existence checking.

    void ActionPool::doit (void)
    {
	  map<long, Action *>::iterator	mi,
					me = Action::mactions.end();	// JDJDJDJD could be factorised
		   list<long>::iterator	li = l.begin(),
					le = l.end();

	while (li!=le) {
	    mi = Action::mactions.find(*li);
	    if (mi != me) {
		mi->second->doit();
		li++;
	    } else {
		if (warnaboutmissingActions)
		    bzouzerr << "warning: attempt to call a missing Action" << endl;
		list<long>::iterator lj = li;
		li++;
		l.erase (lj);
	    }
	}
    }

    ActionPool & ActionPool::operator += (const Action & a)
    {
	l.push_back (a.handle);
	return *this;
    }
    
// --------------- GrapeKeyMapKey ---------------------------------------------
//
//! match some keymaps to some Actions objects.
//

    GrapeKeyMapKey::GrapeKeyMapKey (void)
    {
    }

    void GrapeKeyMapKey::map_unicode_action (Uint16 unicode, Action & a)
    {
	map_unicode [unicode] += a;
    }

    void GrapeKeyMapKey::map_sdlkey_action (SDLKey k, Action & a)
    {
	map_sdlkey [k] += a;
    }

    void GrapeKeyMapKey::map_sdlkey_action (SDLKey k, SDLMod mod, Action & a)
    {
	map_sdlkeymod [ (mod << 16) | k ] += a;
    }

    bool GrapeKeyMapKey::trigger_action (SDL_Event const & event)
    {
	{   map<Uint16, ActionPool>::iterator mi = map_unicode.find (event.key.keysym.unicode);
	    if (mi!= map_unicode.end()) {
		mi->second.doit();
		return true;
	    }
	}
	{   map<SDLKey, ActionPool>::iterator mi = map_sdlkey.find (event.key.keysym.sym);
	    if (mi!= map_sdlkey.end()) {
		mi->second.doit();
		return true;
	    }
	}
	{   map<Uint32, ActionPool>::iterator mi = map_sdlkeymod.find ((event.key.keysym.mod << 16) | event.key.keysym.sym);
	    if (mi!= map_sdlkeymod.end()) {
		mi->second.doit();
		return true;
	    }
	}
	return false;
    }
}

