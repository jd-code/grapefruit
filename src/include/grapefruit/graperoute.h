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
#warning graperoute.h should not be included as is. grapefruit.h should be included instead
#else  // GRAPEFRUIT_H_INC
#ifdef	    GRAPEFRUIT_H_GLOBINST
#define	    GRAPEROUTE_H_GLOBINST
#endif //   GRAPEFRUIT_H_GLOBINST
#endif // GRAPEFRUIT_H_INC

#ifndef GRAPEROUTE_H_INC
#define GRAPEROUTE_H_INC

namespace grapefruit
{
    // using namespace std ;

    // --------------- Deletions Bouzin -------------------------------------------
    //! registers some TD to be deleted during next cool time between two events handling bouzins

    //////// JDJDJDJD ceci est mis en stand-by jusqu'à ce qu'on en ait de nouveau besoin
    ////////class TDObj;
    ////////void committodelete (TDObj * p);
    
    // --------------- GrapeRouteEvent --------------------------------------------
    //! the event router

    int GrapeRouteEvent (SDL_Event const & event);


    // --------------- EventCB : Event Call-Back ----------------------------------
    //
    // Skeleton-class inherited by objects which need to have called-back members
    //

    class EventCB;
    typedef EventCB * PEventCB;

    //! Skeleton-class inherited by objects which need to have called-back members
    class EventCB	// event callback
    {
	protected:
	    static PEventCB mousebuttonfocus;   //!< who has the focus related with the last clickdown from the mouse
	    static PEventCB mouseflyover;	//!< who is currently flought over by the mouse
	    static PEventCB ownergrabmouse;	//!< who has currently grabbed the mouse

	public:
	    virtual ~EventCB (void) {}
	    virtual int MOUSEBUTTONDOWN		(SDL_Event const & event) { return 0; }
	    virtual int MOUSEBUTTONUP		(SDL_Event const & event) { return 0; }
	    virtual int MOUSEMOTION	    	(SDL_Event const & event) { return 0; }

	    virtual int LOOSEMOUSEBUTTONFOCUS   (SDL_Event const & event) { return 0; }
	    virtual int MOUSEOUT		(SDL_Event const & event) { return 0; }
	    virtual int MOUSEIN			(SDL_Event const & event) { return 0; }

	friend int GrapeRouteEvent (SDL_Event const & event);
    };



    // --------------- ClicableZone -----------------------------------------------
    //
    // match some bitmap-zones to EventCB objects
    //

    //! defines a 2D geometrized set of matching zones to EventCB objects
    typedef map<long, PEventCB> ZoneMap;

    //! match some bitmap-zones to EventCB objects
    class ClicableZone
    {
	    ZoneMap zone;
	    int wz, hz;
	    PEventCB defcb;

	public:
	    //! creates some clickable zone
	    ClicableZone (int w=10000, int h=10000, PEventCB defcb=NULL);

	    //! binds some part of the zone to an object
	    void addrect (int x, int y, int w, int h, PEventCB cb);
	    inline void addrect (SDL_Rect const & rect, PEventCB cb)
	    {	addrect (rect.x, rect.y, rect.w, rect.h, cb);
	    }

	    //! retrieves the bound object from coordinates
	    PEventCB getcb (int x, int y);

	    //! empties the zone from all previous bindings
	    void empty (void);
    };



    // --------------- MGrabber ---------------------------------------------------
    //
    //! this class provides the property of grabbing the mouse until release

    class MGrabber : public virtual EventCB
    {	
	protected:
	    bool warpcoordvalid;	//!< are the saved coord valid for mouse position restauration ?
	    bool firstmove;		//!< are we treating the first move after grabbing (that we discard)
	    int xorigin,		//!< the original x-coord of the mouse just before we make it disappear
		yorigin;		//!< the original y-coord of the mouse just before we make it disappear
	    int sdx,			//!< the total x-shift of the mouse during the grab (for restoring at the end)
		sdy;			//!< the total y-shift of the mouse during the grab (for restoring at the end)
	    SDL_GrabMode modebefore;	//!< what was the mode of the mouse before we take it
	
	public:
	    virtual ~MGrabber (void) {}
	    MGrabber (void);
	    virtual void grabberchange (SDL_Event const & event) {};

	    virtual int MOUSEMOTION		    (SDL_Event const & event);
	    virtual int MOUSEBUTTONDOWN		    (SDL_Event const & event);
	    virtual int MOUSEBUTTONUP		    (SDL_Event const & event);
	    virtual int LOOSEMOUSEBUTTONFOCUS	    (SDL_Event const & event);

	    virtual int movegrabber		    (SDL_Event const & event);
	    virtual int startgrabber		    (SDL_Event const & event);
	    virtual int endgrabber		    (SDL_Event const & event);
    };
    
    
    // --------------- CaptionZone ------------------------------------------------
    //
    //

    class Agrume;

    //! handler that capts (grab and hide) the mouse, tracks its move to the
    //! virtual function valuechange, and frees it at button release


    class CaptionZone : public virtual MGrabber
    {
	private:
	    int *Px, *Py;		//!< the coordinate we will modify on the fly (of the object tied to the Caption)

	public:
	    CaptionZone (int* x, int *y);
	    virtual ~CaptionZone ();

	    void activate (Agrume & agrume, SDL_Rect const & rdst);

	    virtual void valuechange (void) {};

	    virtual void grabberchange (SDL_Event const & event);
    };

    //! type for pointer to function that handle some event
    typedef bool (*PEventHandler) (SDL_Event const & event);

	// --------------- focus related globals --------------------------------------

#ifdef   GRAPEROUTE_H_GLOBINST
	//! who has the focus related with the last clickdown from the mouse
	GRAPEFRUIT_H_SCOPE PEventCB EventCB::mousebuttonfocus = NULL;

	//! who is currently flought-over by the mouse
	GRAPEFRUIT_H_SCOPE PEventCB EventCB::mouseflyover = NULL;

	//! who has currently grabbed the mouse
	GRAPEFRUIT_H_SCOPE PEventCB EventCB::ownergrabmouse = NULL;

	//! do we warn about unmapped keys
	GRAPEFRUIT_H_SCOPE bool advertise_unmapped_keys = true;

	//! the global key-down handler (application specific)
	GRAPEFRUIT_H_SCOPE PEventHandler global_keydownhandler = NULL;

	//! the global key-up handler (application specific)
	GRAPEFRUIT_H_SCOPE PEventHandler global_keyuphandler = NULL;
#else
	//! do we warn about unmapped keys
	GRAPEFRUIT_H_SCOPE bool advertise_unmapped_keys;

	//! the global key-down handler (application specific)
	GRAPEFRUIT_H_SCOPE PEventHandler global_keydownhandler;

	//! the global key-up handler (application specific)
	GRAPEFRUIT_H_SCOPE PEventHandler global_keyuphandler;
#endif

}

#endif // GRAPEROUTE_H_INC

