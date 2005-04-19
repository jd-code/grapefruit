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

namespace grapefruit
{
    // --------------- Action -----------------------------------------------------
    //
    //! canvas for every and any performable action in our software.

    class Action
    {
     static map<long, Action *> mactions;   //!< the map of all still existing actions, identified by their "unique-in-history" handle.
     static long n_occur;		    //!< global counter for Actions' handles creations.
	    long handle;		    //!< the "unique-in-history" handle matching the Action.
	public:

	    virtual ~Action (void);
	    Action (void);
	    virtual void doit (void) = 0;   //!< the virtual action-code to be performed, by itself.

	    virtual string getacname (void);
	    static void dump_mactions (ostream &cout);
	    long gethandle (void)
		{   return handle;
		}

	friend class ActionPool;
    };

    // -------------- ActionPool --------------------------------------------------
    //
    //! pool of Actions, with cross-existence checking.

    
    class ActionPool
    {
	private:
	    list<long> l;				    //!< the list of Action to trigger.
	public:
     static bool warnaboutmissingActions;		    //!< must we warn about attempt to use Action that were deleted (default yes)
     static bool debug_pool;				    //!< do we have the debuging of ActionPool performed at each "doit" call ?
	    ActionPool (void) {}
	    void doit (void);				    //!< carefully checks each Action existence before triggering them.
	    ~ActionPool (void) {}			    //!< naturally performs each Action one after the other
	    ActionPool & operator += (const Action & a);    //!< adds an Action at the end of the pool.
    };

#ifdef GRAPEKEY_H_GLOBINST
	GRAPEFRUIT_H_SCOPE map<long, Action *> Action::mactions;
	GRAPEFRUIT_H_SCOPE long Action::n_occur = 0;
	GRAPEFRUIT_H_SCOPE bool ActionPool::warnaboutmissingActions = true;
	GRAPEFRUIT_H_SCOPE bool ActionPool::debug_pool = false;
#endif // GRAPEKEY_H_GLOBINST
    
    // --------------- GrapeKeyMapKey ---------------------------------------------
    //
    //! storing key-mapping to actions

    class GrapeKeyMapKey
    {
	    //! this maps unicode chars prior to any other keymap
	    map <Uint16, ActionPool> map_unicode;
	    //! this maps SDL scan codes without consideration of modifiers (ALT/CTRL etc...)
	    map <SDLKey, ActionPool> map_sdlkey;
	    //! in last resort, this maps SDL scan codes WITH consideration of modifiers (ALT/CTRL etc...)
	    map <Uint32, ActionPool> map_sdlkeymod;

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

