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

#include <ctype.h>

#include "grapefruit.h"

namespace grapefruit
{

// --------------- ClicableZone -----------------------------------------------
//
//! match some bitmap-zones to EventCB objects
//

ClicableZone::ClicableZone (int w, int h, PEventCB defcb)
{
    ClicableZone::wz = w;
    ClicableZone::hz = h;
    ClicableZone::defcb = defcb;
    zone [0] = defcb;
}

void ClicableZone::addrect (int x, int y, int w, int h, PEventCB cb)
{
    int i;

    for (i=y ; i<y+h ; i++) {
	ZoneMap::iterator zi = zone.lower_bound (x+i*wz);	// the first equal or greater than our start ("next")
	ZoneMap::iterator li = zone.upper_bound (x+i*wz + w);   // the first strictly greater than our end

	PEventCB temp;			// we put appart the element that precedes our end
	if (li != zone.begin ()) {	// so that after we can patch back the zone after us and before the "next"
	    ZoneMap::iterator t = li;
	    t--;
	    temp = t->second;
	} else
	    temp = defcb;

	ZoneMap::iterator ti;		// we erase the points we overlap
	while (zi != li) {
	    ti = zi;
	    zi ++;
	    zone.erase (ti);
	}

	zone [x+i*wz] = cb;		// we add ourselves
	zone [x+i*wz + w] = temp;	// and perform the patch described above
    }
}

PEventCB ClicableZone::getcb (int x, int y)
{   ZoneMap::iterator zi = zone.upper_bound (x+y*wz);
    if (zi == zone.begin())
	return defcb;
    zi--;
	return zi->second;
}

void ClicableZone::empty (void)
{   zone.erase (zone.begin(), zone.end());
    zone [0] = defcb;
}

// --------------- Deletions Bouzin -------------------------------------------
//
// we take care of schedulled deletions that could not occur during events treatments
//

bool therearedeletions = false;

//! the list of commited to deletion TDObj, they'll be destroyed by commitdeletions
list <TDObj *> tdtobedeleted;

//! schedule the C++ deletion of a given TDObj for when things are quiets regarding events
void committodelete (TDObj * p)
{   tdtobedeleted.push_back (p);
    therearedeletions = true;
}

//! commit the C++ deletions that where scheduled by committodelete
void commitdeletions (void)
{   list <TDObj *>::iterator li,
		begin = tdtobedeleted.begin(),
		end =   tdtobedeleted.end();
    for (li=begin ; li!=end ; li++) {
	delete (*li);
    }
    tdtobedeleted.erase (begin, end);
    therearedeletions = false;
}


// --------------- GrapeRouteEvent --------------------------------------------
//
//! the event router, will call the appropriate member functions...
//

int GrapeRouteEvent (SDL_Event const & event)
{
    PEventCB &mouseflyover	= EventCB::mouseflyover;
    PEventCB &mousebuttonfocus	= EventCB::mousebuttonfocus;
    PEventCB &ownergrabmouse	= EventCB::ownergrabmouse;

    int r = 0;
    PEventCB temp = NULL;

    switch (event.type) {

	case SDL_ACTIVEEVENT:
	    break;

	case SDL_KEYDOWN: 
	    if (global_keydownhandler != NULL) {
		if (!global_keydownhandler (event)) {
		    cout << "[unmapped char pressed]" ;
		    if (isprint (event.key.keysym.unicode))
			cout << " [" << (char)(event.key.keysym.unicode) << "] " ;
		    else
			cout << " [" << event.key.keysym.unicode << "] " ;

		    cout << " scancode=" << (int)event.key.keysym.scancode
			 << " sdlsym=" << event.key.keysym.sym
			 << " mod=" << event.key.keysym.mod
			 << " unicode=" << event.key.keysym.unicode
			 << endl;
			 ; 
		}
	    }
	    //	if ((event.key.keysym.unicode < 0xFF) && (event.key.keysym.unicode > 0)) {
	    //	    cout << (char)(event.key.keysym.unicode) << flush ;
	    //	    switch (event.key.keysym.unicode) {
	    //	        // JDJDJDJD ajouter gestion du focus clavier
	    //	        case '-':
	    //	        case 'w':
	    //	        default:
	    //	    	break;
	    //	    }
	    //	} else {
	    //	    cout << "[unknown char pressed]"			// JDJDJDJD à enlever...
	    //	         << " scancode=" << event.key.keysym.scancode
	    //	         << " sdlsym=" << event.key.keysym.sym
	    //	         << " mod=" << event.key.keysym.mod
	    //	         << " unicode=" << event.key.keysym.unicode
	    //	         << endl;
	    //	         ; 
	    //	}
	    break;

	case SDL_KEYUP:
	    break;

	case SDL_MOUSEMOTION:
	    if (ownergrabmouse != NULL) {
		ownergrabmouse->MOUSEMOTION (event);
	    } else {
		temp = getcb (event.button.x, event.button.y);
		if (mouseflyover != temp) {
		    if (mouseflyover != NULL)
			r += mouseflyover->MOUSEOUT (event);
		    mouseflyover = temp;
		    if (mouseflyover != NULL)
			r += mouseflyover->MOUSEIN (event);
		}
	    }
	    break;

	case SDL_MOUSEBUTTONDOWN:
	    temp = getcb (event.button.x, event.button.y);
	    if (temp != mousebuttonfocus) {
		if (mousebuttonfocus != NULL) {
		    r += mousebuttonfocus->LOOSEMOUSEBUTTONFOCUS (event);
		}
	    }
	    mousebuttonfocus = temp;
	    if (temp != NULL) {
		r += temp->MOUSEBUTTONDOWN (event);
	    }
	    break;

	case SDL_MOUSEBUTTONUP:
	    temp = getcb (event.button.x, event.button.y);
	    if (temp != mousebuttonfocus) {
		if (mousebuttonfocus != NULL)
		    r += mousebuttonfocus->LOOSEMOUSEBUTTONFOCUS (event);
	    }
	    else if (temp != NULL) {
		r += temp->MOUSEBUTTONUP (event);
	    }
	    mousebuttonfocus = NULL;
	    break;

	case SDL_JOYAXISMOTION     : break;
	case SDL_JOYBALLMOTION     : break;
	case SDL_JOYHATMOTION      : break;
	case SDL_JOYBUTTONUP       : break;
	case SDL_JOYBUTTONDOWN     : break;
	case SDL_QUIT              : break;
	case SDL_SYSWMEVENT        : break;
	case SDL_VIDEORESIZE       : break;
	case SDL_VIDEOEXPOSE       : break;
	case SDL_USEREVENT         : break;
	default                    : bzouzerr << "unknown event" << endl ;
    }

// JDJDJDJD ceci est mis en stand-by jusqu'à ce qu'on en ait de nouveau besoin
//    if (therearedeletions) {
//	commitdeletions ();
//    }
    
    return r;
}


// -- dragging ---  dragging --- dragging --- dragging --- dragging --- dragging --- dragging --- dragging ---
//
// initialy took from CaptionZone

MGrabber::MGrabber (void)
{
    warpcoordvalid = false;
    modebefore = SDL_GRAB_QUERY;
}

int MGrabber::MOUSEBUTTONDOWN (SDL_Event const & event)
{   return startgrabber (event);
}

int MGrabber::MOUSEMOTION (SDL_Event const & event)
{   return movegrabber(event);
}

int MGrabber::MOUSEBUTTONUP (SDL_Event const & event)
{   return endgrabber (event);
}

int MGrabber::LOOSEMOUSEBUTTONFOCUS (SDL_Event const & event)    // maybe RotButton should inherit this part ???
{   return endgrabber (event);
}

int MGrabber::movegrabber (SDL_Event const & event)
{   
#ifdef JDDEBUGMOTION
    bzouzerr << "debug motion : " << event.motion.x << ":" << event.motion.y << "   "
			      << event.motion.xrel << ":" << event.motion.yrel << endl ;
#endif
    if (firstmove) {
	firstmove = false;
	return 0;
    }
    
    if ((event.motion.xrel != 0) || (event.motion.yrel != 0)) {
#ifdef BUILD_FOR_WIN32
	if ((event.motion.yrel < 50) && (event.motion.xrel < 50)){   // JDJDJDJD with win32 we have a huge drift at starting
								     // grabbing the mouse - we may in the future use the trick
								     // of avoiding treating mouse warps this way ?
#endif
	// if ((event.motion.yrel > 100) || (event.motion.xrel > 100)){
	if (	((event.motion.yrel > 100) || (event.motion.xrel > 100))
	     && ((event.motion.x == screen->w - 1) || (event.motion.y == screen->h - 1))
	   ) {
	    cerr.setf (ios::left);
	    cerr << "big motion  type=" << setw(6) << hex << (Uint16) event.motion.type
		 << " state=" << setw(6) << (Uint16) event.motion.state << dec
		 << " x=" << setw(6) << event.motion.x
		 << " y=" << setw(6) << event.motion.y
		 << " xrel=" << setw(6) << event.motion.xrel
		 << " yrel=" << setw(6) << event.motion.yrel << endl ;
/*
 * jd en 1280 x 1024
 *
 * big motion  type=4      state=1      x=493    y=899    xrel=285    yrel=726   
 * big motion  type=4      state=1      x=869    y=899    xrel=473    yrel=783   
 * big motion  type=4      state=1      x=1199   y=899    xrel=950    yrel=634   
 * big motion  type=4      state=1      x=1199   y=899    xrel=1127   yrel=621   
 * big motion  type=4      state=1      x=552    y=899    xrel=209    yrel=748   
 *
 * big motion  type=4      state=1      x=551    y=840    xrel=184    yrel=318   <---
 * big motion  type=4      state=1      x=575    y=899    xrel=219    yrel=881   
 * big motion  type=4      state=1      x=1199   y=899    xrel=504    yrel=723   
 * big motion  type=4      state=1      x=1199   y=899    xrel=973    yrel=819   
 * big motion  type=4      state=1      x=1031   y=899    xrel=548    yrel=479   
 *
 */
	} else {
//	    SDL_WarpMouse(xorigin, yorigin);

	    sdx += event.motion.xrel,
	    sdy += event.motion.yrel;
	    grabberchange (event);
	}
#ifdef BUILD_FOR_WIN32
	}
#endif
    }
    return 0;
}

int MGrabber::startgrabber (SDL_Event const & event)
{
    xorigin = event.button.x;
    yorigin = event.button.y;
    sdx = 0, sdy = 0;
    firstmove = true;
    warpcoordvalid = true;

    if (modebefore == SDL_GRAB_QUERY)
	modebefore = SDL_WM_GrabInput (SDL_GRAB_QUERY);

// bzouzerr << " SDL_GRAB_ON" << endl ;
    SDL_WM_GrabInput (SDL_GRAB_ON);
    SDL_ShowCursor (SDL_DISABLE);
    ownergrabmouse = (PEventCB)this;
    return 0;
}


int MGrabber::endgrabber (SDL_Event const & event)
{
// bzouzerr << " modebofore=" << ((modebefore==SDL_GRAB_ON) ? "SDL_GRAB_ON" : "pas SDL_GRAB_ON") << endl ;
    SDL_WM_GrabInput (modebefore);
    modebefore = SDL_GRAB_QUERY;
    SDL_ShowCursor(SDL_ENABLE);	// we display the cursor before moving it ot it might bug....
    if (warpcoordvalid) {
	xorigin += sdx, yorigin += sdy;
	if (xorigin < 0)
	    xorigin = 0;
	else if (xorigin > screen->w)
	    xorigin = screen->w -1;
	
	if (yorigin < 0)
	    yorigin = 0;
	else if (yorigin > screen->h)
	    yorigin = screen->h -1;
	
	SDL_WarpMouse(xorigin, yorigin);
    }
    warpcoordvalid = false;
    ownergrabmouse = NULL;
    return 0;
}


// --------------- CaptionZone ------------------------------------------------
//
/*! handler that capts (grab and hide) the mouse, tracks its move to the
 *  virtual function valuechange, and frees it at button release
 */

CaptionZone::CaptionZone (int* x, int *y) : MGrabber ()
{   Px = x, Py = y;

////    warpcoordvalid = false;
////    modebefore = SDL_GRAB_QUERY;
}

void CaptionZone::activate (Agrume & agrume, SDL_Rect const & rdst)
{   agrume.addrect (rdst, (PEventCB)this);
}

void CaptionZone::grabberchange (SDL_Event const & event)
{
    *Px += event.motion.xrel,
    *Py += event.motion.yrel;
    valuechange ();
}

////	int CaptionZone::MOUSEMOTION (SDL_Event const & event)
////	{   
////	#ifdef JDDEBUGMOTION
////	    bzouzerr << "debug motion : " << event.motion.x << ":" << event.motion.y << "   "
////				      << event.motion.xrel << ":" << event.motion.yrel << endl ;
////	#endif
////	
////	    if ((event.motion.xrel != 0) || (event.motion.yrel !=0)) {
////	#ifdef BUILD_FOR_WIN32
////		if ((event.motion.yrel < 50) && (event.motion.xrel < 50)){   // JDJDJDJD with win32 we have a huge drift at starting
////									     // grabbing the mouse - we may in the future use the trick
////									     // of avoiding treating mouse warps this way ?
////	#endif
////	//	    SDL_WarpMouse(xorigin, yorigin);
////		    *Px += event.motion.xrel,
////		    *Py += event.motion.yrel;
////		    sdx += event.motion.xrel,
////		    sdy += event.motion.yrel;
////		    valuechange ();
////	#ifdef BUILD_FOR_WIN32
////		}
////	#endif
////	    }
////	    return 0;
////	}
////	
////	int CaptionZone::LOOSEMOUSEBUTTONFOCUS (SDL_Event const & event)    // maybe RotButton should inherit this part ???
////	{
////	// bzouzerr << "modebofore=" << ((modebefore==SDL_GRAB_ON) ? "SDL_GRAB_ON" : "pas SDL_GRAB_ON") << endl ;
////	    SDL_WM_GrabInput (modebefore);
////	    modebefore = SDL_GRAB_QUERY;
////	    SDL_ShowCursor (SDL_ENABLE);
////	    if (warpcoordvalid) {		    // we move after showing or it might bug
////		xorigin += sdx, yorigin += sdy;
////		if (xorigin < 0)
////		    xorigin = 0;
////		else if (xorigin > screen->w)
////		    xorigin = screen->w -1;
////		
////		if (yorigin < 0)
////		    yorigin = 0;
////		else if (yorigin > screen->h)
////		    yorigin = screen->h -1;
////		
////		SDL_WarpMouse(xorigin, yorigin);
////	    }
////	    ownergrabmouse = NULL;
////	    warpcoordvalid = false;
////	
////	    return 0;
////	}
////	
////	int CaptionZone::MOUSEBUTTONDOWN (SDL_Event const & event)
////	{
////	    xorigin = event.button.x;
////	    yorigin = event.button.y;
////	    sdx = 0, sdy = 0;
////	    warpcoordvalid = true;
////	
////	    if (modebefore == SDL_GRAB_QUERY)
////		modebefore = SDL_WM_GrabInput (SDL_GRAB_QUERY);
////	
////	// bzouzerr << "SDL_GRAB_ON" << endl ;
////	    SDL_WM_GrabInput (SDL_GRAB_ON);
////	    SDL_ShowCursor (SDL_DISABLE);
////	    ownergrabmouse = (PEventCB)this;
////	    return 0;
////	}
////	
////	int CaptionZone::MOUSEBUTTONUP (SDL_Event const & event)
////	{
////	// bzouzerr << "modebofore=" << ((modebefore==SDL_GRAB_ON) ? "SDL_GRAB_ON" : "pas SDL_GRAB_ON") << endl ;
////	    SDL_WM_GrabInput (modebefore);
////	    modebefore = SDL_GRAB_QUERY;
////	    SDL_ShowCursor(SDL_ENABLE);	// we display the cursor before moving it ot it might bug....
////	    if (warpcoordvalid) {
////		xorigin += sdx, yorigin += sdy;
////		if (xorigin < 0)
////		    xorigin = 0;
////		else if (xorigin > screen->w)
////		    xorigin = screen->w -1;
////		
////		if (yorigin < 0)
////		    yorigin = 0;
////		else if (yorigin > screen->h)
////		    yorigin = screen->h -1;
////		
////		SDL_WarpMouse(xorigin, yorigin);
////	    }
////	    warpcoordvalid = false;
////	    ownergrabmouse = NULL;
////	    return 0;
////	}

CaptionZone::~CaptionZone (void)
{   // JDJDJDJD emptiness to be checked...
}

}

