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


#ifndef   GRAPEFRUIT_H_INC
#warning key.h should not be included as is. grapefruit.h should be included instead
#else  // GRAPEFRUIT_H_INC
#ifdef	    GRAPEFRUIT_H_GLOBINST
#define	    GRAPEKEY_H_GLOBINST
#endif //   GRAPEFRUIT_H_GLOBINST
#endif // GRAPEFRUIT_H_INC

#ifndef GRAPEKEY_H_INC
#define GRAPEKEY_H_INC

// #ifndef KMOD_ALL
// #define KMOD_ANY    0xfffe  //!< grapefruit extension to SDLMod : equals any of the possible modifiers
// #else
// #error	cannot over-define KMOD_ANY, this must be dig out...
// #endif // KMOD_ALL

namespace grapefruit
{
    // --------------- Action -----------------------------------------------------
    //
    //! canvas for every and any performable action in our software

    class Action
    {
	public:
	    virtual ~Action (void) {}
	    virtual void doit (void) = 0;
    };


    
    // --------------- GrapeKeyMapKey ---------------------------------------------
    //
    //! storing key-mapping to actions

    class GrapeKeyMapKey
    {
	    //! this maps unicode chars prior to any other keymap
	    map <Uint16, Action*> map_unicode;
	    //! this maps SDL scan codes without consideration of modifiers (ALT/CTRL etc...)
	    map <SDLKey, Action*> map_sdlkey;
	    //! in last resort, this maps SDL scan codes WITH consideration of modifiers (ALT/CTRL etc...)
	    map <Uint32, Action*> map_sdlkeymod;

	public:
	    GrapeKeyMapKey ();
	    //! adds a unicode mapping to the keymap
	    void map_unicode_action (Uint16 unicode, Action & a);
	    //! adds a SDL scancode mapping without consideration of modifiers (ALT/CTRL etc...) to the keymap
	    void map_sdlkey_action (SDLKey k, Action & a);
	    //! adds a SDL scancode mapping WITH consideration of modifiers (ALT/CTRL etc...) to the keymap
	    void map_sdlkey_action (SDLKey k, SDLMod mod, Action & a);
	    //! this looks for any matching keymap and triggers the bound action if any or return false
	    bool trigger_action (SDL_Event const & event);
    };
}

#endif // GRAPEKEY_H_INC

