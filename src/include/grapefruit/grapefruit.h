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

#ifndef GRAPEFRUIT_H_INC
#define GRAPEFRUIT_H_INC

#include <iostream>
#include <iomanip>
#include <string>
#include <map>

#include <GL/gl.h>

#ifdef _WIN32
#   ifdef DLL_EXPORT
#	ifdef GRAPEFRUIT_H_GLOBINST
#	    define GRAPEFRUIT_H_SCOPE __declspec(dllexport)
#	else
#	    define GRAPEFRUIT_H_SCOPE extern
#	endif
#   else
#	ifdef LIBGRAPEFRUIT_DLL_IMPORT
#	    define GRAPEFRUIT_H_SCOPE extern __declspec(dllimport)
#	else
// JDJDJDJD still some study needed here :
//     objects seem to need being defined once only via GRAPEFRUIT_H_GLOBINST selection rather than behaving
//     exclusively via GRAPEFRUIT_H_SCOPE ???
#	    ifdef GRAPEFRUIT_H_GLOBINST
#		define GRAPEFRUIT_H_SCOPE
#	    else
#		define GRAPEFRUIT_H_SCOPE extern
#	    endif
#	endif
#   endif
#endif
#ifndef GRAPEFRUIT_H_SCOPE
#   ifdef GRAPEFRUIT_H_GLOBINST
#	define GRAPEFRUIT_H_SCOPE
#   else
#	define GRAPEFRUIT_H_SCOPE extern
#   endif
#endif

#include "graphutils.h"
#include "graphprim.h"

#include "grapecolor.h"
#include "grapekey.h"
#include "glreinit.h"
#include "graperoute.h"
#include "td.h"
#include "cinetic.h"
#include "glcomp.h"
#include "glvfont.h"
#include "tdstring.h"
#include "tdfps.h"
#include "menu.h"
#include "agrume.h"

#ifndef bzouzerr
#define bzouzerr cerr << __FILE__  << ':'  << __LINE__ << ':' << __PRETTY_FUNCTION__ << ':'
#endif

//! Everything about graphic interaction facilities, buttons, shadows, popups...
/*! Grapefruit is lacking a lot of stuff right now, but it is intended to take
 *  care of all incoming interaction from the user, mainly by catching and treating
 *  every SDL signals...
 */
namespace grapefruit
{
    using namespace std ;

    //! returns the release-name of this module
    const char * get_grapefruit_version (void);

    // --------------- RotButton --------------------------------------------------
    //
    //! Skeleton-class for building rotating buttons

    class RotButton : public virtual MGrabber
    {
	private:
	    SDL_Rect rdst;
	    Agrume *agrume;
	    SDL_Surface *Sunder;

	    static bool firstuse;

	    ////    bool warpcoordvalid;	//!< are the saved coord valid for mouse position restauration ?
	    ////    int xorigin,		//!< the original x-coord of the mouse just before we make it disappear
	    ////        yorigin;		//!< the original y-coord of the mouse just before we make it disappear
	    ////    int sdx,			//!< the total x-shift of the mouse during the grab (for restoring at the end)
	    ////        sdy;			//!< the total y-shift of the mouse during the grab (for restoring at the end)
	    ////    SDL_GrabMode modebefore;	//!< what was the mode of the mouse before we take it

	    void draw (int value);
	    void show (Agrume & agrume, int x, int y);

	protected:
	    int ourvalue;
	    int *Pvalue;

	public:
	    RotButton (int value = 0);
	    RotButton (int* Pvalue);
	    virtual ~RotButton ();

	    void activate (Agrume & agrume, int x, int y);
	    virtual void valuechange (void) {};
	    virtual void grabberchange (SDL_Event const & event);
	    void refresh ();

	    ////    virtual int MOUSEMOTION	    	(SDL_Event const & event);
	    ////    virtual int MOUSEBUTTONDOWN	    	(SDL_Event const & event);
	    ////    virtual int MOUSEBUTTONUP   	(SDL_Event const & event);
	    ////    virtual int LOOSEMOUSEBUTTONFOCUS   (SDL_Event const & event);
    };
    

    // --------------- ButtonStyle ------------------------------------------------
    //
    // Some button-style skeleton
    //

    class Button;
    
    //! Some button-style skeleton

    class ButtonStyle
    {
	protected:
	    Button *button; // JDJDJDJD ??
	public:
	    inline ButtonStyle () { button = NULL; }
	    inline ButtonStyle (Button * b) { button = b; }
	    virtual inline ButtonStyle* link (Button * b) { return new ButtonStyle (b); }

	    virtual inline void draw (bool isdown) {}
	    virtual inline void show (Agrume &agrume, int x, int y) {}
	    virtual inline void buildtext (int w, int h, const string & s) {}

	    virtual ~ButtonStyle () {}
    };

    // --------------- Currant ----------------------------------------------------
    //
    //! Skeleton-class for building objects behaving the grapefruit way

    class Currant : public virtual ButtonStyle
    {
	private:
	    //! the destination agrume
	    Agrume *agrume;

	    //! the clipping rectangle of the agrume when showed
	    SDL_Rect initialcliprect;

	    RGBAColor bg, sr, txt;

	    //! ourselves when pressed down
	    SDL_Surface *Sdown,
	    //! ourselves when up
			*Sup;
	    //! backup of the surface on which we are
	    SDL_Surface *Sunder;

	    //! the text to be be displayed
	    string text;

	    SDL_Rect rdst;

	public:

	    Currant (RGBAColor bg = RGBA_TRBLACK, RGBAColor sr = RGB_DARKRED, RGBAColor txt = RGB_WHITE);
	    virtual inline ButtonStyle* link (Button * b)
	    {	Currant *r = new Currant (*this);
		r->button = b;
		return r;
	    }

	    virtual void draw (bool isdown);
	    virtual void show (Agrume &agrume, int x, int y);
	    virtual void buildtext (int w, int h, const string & s);
    };


    // --------------- Text -------------------------------------------------------
    //
    //! Skeleton-class for building simple texts

    class Text
    {
	    SDL_Rect rdst;
	    Agrume *agrume;
	    SDL_Surface *Sunder;
	    Uint32 txt;
	    int w, h;

	    string s;
	public:
	    Text (int w, int h, const string & s, RGBAColor txt);
	    virtual ~Text (void);
	    virtual void activate (Agrume &agrume, int x, int y);
	    void settext (const string & s);
    };
    

    // --------------- Button -----------------------------------------------------
    //
    //! Skeleton-class for building simple pressed buttons
    /*! a "Button" appears as a frozen control
     */

    class Button : public virtual EventCB  // , private Currant
    {
	protected:
	    ButtonStyle * style;
	    bool ispressed;
	    bool ourvalue;
	    bool *Pvalue;

	    bool ischown;
	    
	public:
	    string thestring;   // JDJDJDJD line to be removed or moved to text button only

	    Button (int w, int h, const string & s, ButtonStyle & style, bool value = false);
	    Button (int w, int h, const string & s, ButtonStyle & style, bool* Pvalue);
	    virtual ~Button ();

	    inline void setPvalue (bool* Pvalue)    // JDJDJDJD we should think about handling a NULL Pvalue ?
		{   bool temp = *Button::Pvalue;
		    Button::Pvalue = Pvalue;
		    if (temp != *Pvalue) {
			ispressed = *Pvalue;	    // JDJDJDJD we have to decide if call valuechange...
			refresh ();
		    }
		}
	    virtual void valuechange (void) {};
	    virtual void activate (Agrume &agrume, int x, int y);
	    virtual void refresh ();

	    inline bool hasmousebuttonfocus (void) {
		return (mousebuttonfocus == (PEventCB)this);
	    }
    };


    // --------------- PushButton -------------------------------------------------
    //
    //! a button that doesn't stay pressed

    class PushButton : public Button
    {
	private:
	    PushButton (int w, int h, const string & s, ButtonStyle & style, bool value)    // JDJD the bool value is ignored
		: Button (w, h, s, style, false) {}	                // in case of PushButton this constructor is depleted
	public:
	    PushButton (int w, int h, const string & s, ButtonStyle & style)
		: Button (w, h, s, style, false) {}
	    PushButton (int w, int h, const string & s, ButtonStyle & style, bool* Pvalue)
		: Button (w, h, s, style, Pvalue) {}
	    
	    virtual int MOUSEOUT		(SDL_Event const & event);
	    virtual int MOUSEIN			(SDL_Event const & event);
	    virtual int MOUSEBUTTONDOWN	    	(SDL_Event const & event);
	    virtual int MOUSEBUTTONUP   	(SDL_Event const & event);
	    virtual int LOOSEMOUSEBUTTONFOCUS   (SDL_Event const & event);
    };


    // --------------- SwitchButton -----------------------------------------------
    //
    //! a button that toggles its state each time it's pressed

    class SwitchButton : public Button
    {
	public:
	    SwitchButton (int w, int h, const string & s, ButtonStyle & style, bool value = false)
		: Button (w, h, s, style, value) {}
	    SwitchButton (int w, int h, const string & s, ButtonStyle & style, bool *Pvalue)
		: Button (w, h, s, style, Pvalue) {}
	    virtual int MOUSEOUT		(SDL_Event const & event);
	    virtual int MOUSEIN			(SDL_Event const & event);
	    virtual int MOUSEBUTTONDOWN	    	(SDL_Event const & event);
	    virtual int MOUSEBUTTONUP	    	(SDL_Event const & event);
	    virtual int LOOSEMOUSEBUTTONFOCUS   (SDL_Event const & event);
    };
    
    // --------------- AgrumeStyle::CloseButton -----------------------------------
    //
    //! a button for closing agrumes


    class AgrumeStyle::CloseButton : public SwitchButton
    {	private:
	    Agrume * agrume;
	public:
	    CloseButton (int w, int h, const string & s, ButtonStyle & style, Agrume* agrume)
		: SwitchButton (w, h, s, style, false)
		{   CloseButton::agrume = agrume;
		}
	    virtual void valuechange (void)
		{   agrume->hide ();
		    ispressed = false;
		    ourvalue = false;
		    refresh ();
		}
    };


    // -------------- OnePressButton -------------------------------------------------
    //
    //! a button that can be pressed only once (eg for exiting)

    class OnePressButton : public SwitchButton
    {
	public:
	    OnePressButton (int w, int h, const string & s, ButtonStyle & style, bool value = false)
		: SwitchButton (w, h, s, style, value) {}
	    OnePressButton (int w, int h, const string & s, ButtonStyle & style, bool* Pvalue)
		: SwitchButton (w, h, s, style, Pvalue) {}
	    virtual int MOUSEBUTTONUP	    (SDL_Event const & event);
    };


    // -------------- RadioValue / RadioButton ---------------------------------------
    //
    // a set of buttons where only one can be pressed at a time

    class RadioButton;
    typedef list<RadioButton*> LRadioButton;

    class RadioValue
    {

	protected:
	    int *Pvalue;			//!< current value (the selected radio)
	    int ourvalue;			//!< this value isn't used when Pvalue is supplied at construction
	    LRadioButton lbutton;		//!< the list of buttons
	    RadioButton * curselected;		//!< the currently selected button (or end if not)
	    int nbmembers;			//!< the total number of members

	public: 
	    RadioValue ();
	    RadioValue (int * Pvalue1);
	    void setvalue (int v);
	    RadioButton * radiobutton (int w, int h, const string & s, ButtonStyle & style);
	    virtual void valuechange (void) {};
	    virtual ~RadioValue () {}	// JDJDJDJD devrait supprimer ses différents membres dans la liste

	friend class RadioButton;
    };

    //! a set of buttons where only one can be pressed at a time

    class RadioButton : public virtual SwitchButton
    {
	    int order;			//!< our own order
	    RadioValue *radio;		//!< the radiovalue we're member of

	public:
	    RadioButton (int w, int h, const string & s, ButtonStyle & style, RadioValue *radio, int order)
		: SwitchButton  (w, h, s, style, false)
	    {	RadioButton::order = order;
		RadioButton::radio = radio;
	    }
	    virtual void valuechange (void)
	    {   
		if (radio->curselected != NULL) {
		    RadioButton & curselected = *radio->curselected;
		    if (&curselected != this) {
			*curselected.Pvalue = false;
			curselected.ispressed = false;
			curselected.refresh ();
		    } else {    // a radiobutton cannot dselect itself
			*Pvalue = true;
			ispressed = true;
			refresh ();
			return; // in such a case the value doesn't change
		    }
		}
		*radio->Pvalue = order;
		 radio->valuechange ();
		 radio->curselected = this;
	    }
	    
	friend class RadioValue;
    };

// --------------- some globals --------------------------------------------------

    
#ifndef   GRAPEFRUIT_H_GLOBINST

	GRAPEFRUIT_H_SCOPE SDL_Surface *screen;

#else //  GRAPEFRUIT_H_GLOBINST

	GRAPEFRUIT_H_SCOPE SDL_Surface *screen = NULL;
	
	//! is this the first use of RotButton
	GRAPEFRUIT_H_SCOPE bool RotButton::firstuse = true;

	
#endif // GRAPEFRUIT_H_GLOBINST

}



#endif // GRAPEFRUIT_H_INC

