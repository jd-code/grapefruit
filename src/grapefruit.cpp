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

#include <math.h>

#include <vector>

#define GRAPEFRUIT_H_GLOBINST
#include "grapefruit.h"
#undef GRAPEFRUIT_H_GLOBINST
#include "config.h"

// JDJDJDJD definitions ci-dessous à normaliser...
#ifndef M_PI
# define M_E		2.7182818284590452354	/* e */
# define M_LOG2E	1.4426950408889634074	/* log_2 e */
# define M_LOG10E	0.43429448190325182765	/* log_10 e */
# define M_LN2		0.69314718055994530942	/* log_e 2 */
# define M_LN10		2.30258509299404568402	/* log_e 10 */
# define M_PI		3.14159265358979323846	/* pi */
# define M_PI_2		1.57079632679489661923	/* pi/2 */
# define M_PI_4		0.78539816339744830962	/* pi/4 */
# define M_1_PI		0.31830988618379067154	/* 1/pi */
# define M_2_PI		0.63661977236758134308	/* 2/pi */
# define M_2_SQRTPI	1.12837916709551257390	/* 2/sqrt(pi) */
# define M_SQRT2	1.41421356237309504880	/* sqrt(2) */
#endif // M_PI

namespace grapefruit
{

// returns the release-name of this module
const char * get_grapefruit_version (void)
{
#ifdef BZOUTOTALOCAL
    return ("Total-Local® " PACKAGE "-" VERSION " $Id$");
#else
    return (PACKAGE "-" VERSION " $Id$");
#endif
}

//! --------------- shadow generator -------------------------------------------
//
//
//

SDL_Surface * shadowgen (SDL_Surface * Ssrc, int larg, int div)
{
    SDL_Surface *Sdest = NULL;

    {
	Sdest = Create32bSurface (SDL_SRCALPHA | SDL_HWSURFACE, Ssrc->w, Ssrc->h);
	SDL_SetAlpha    (Sdest, SDL_SRCALPHA, 255);
	SDL_SetColorKey (Sdest, 0, 0);
    }

    Uint32 white  = SDL_MapRGBA(Sdest->format, 0xFF, 0xFF, 0xFF, 0xFF);
    
    SDL_Rect rect;
    rect.x = 0, rect.y = 0, rect.w = Ssrc->w, rect.h = Ssrc->h;
    SDL_FillRect (Sdest,   &rect, white);

    ////SDL_Surface *Stemp = ShadowAlpha (SDL_SRCALPHA | SDL_HWSURFACE, Ssrc, 256/25);
    SDL_Surface *Stemp = ShadowAlpha (SDL_SRCALPHA | SDL_HWSURFACE, Ssrc, 256/div);
    {	int i, j;
	SDL_Rect rdst;

	for (i=1 ; i<larg ; i++)
	    for (j=1 ; j<larg ; j++) {
		rdst.x = i, rdst.y = j;
		SDL_BlitSurface(Stemp, NULL, Sdest, &rdst);
	    }
    }
    BrightToAlpha (Sdest);
    ShadowBlit (Ssrc, Sdest);
//    SDL_BlitSurface(Ssrc, NULL, Sdest, NULL); JDJDJDJD JDJDJDJD apparament c'est inutile ? Shadowblit le fait deja ?
    
    SDL_FreeSurface (Stemp);
    
    return Sdest;
}

// --------------- Currant ----------------------------------------------------

Currant::Currant (RGBAColor bg, RGBAColor sr, RGBAColor txt)
{   Currant::bg = bg,
    Currant::sr = sr,
    Currant::txt = txt;
    agrume = NULL;
}

void Currant::buildtext (int w, int h, const string & s)
{
    text = s;

    SDL_Rect rect;
    SDL_Surface *Smask = NULL;

    Smask = Create32bSurface (SDL_SRCALPHA | SDL_HWSURFACE, w, h);
    SDL_SetAlpha    (Smask, SDL_SRCALPHA, 255);
    SDL_SetColorKey (Smask, 0, 0);

    size_t l = s.size();
    int xt = (w-6)/2-l*4;
    string temp(s);
    temp += '\0';

    Uint32 TransBl = SDL_MapRGBA (Smask->format, 0, 0, 0, 0);	// transparent black

    SDL_FillRect (Smask,   &rect, TransBl);			// erase
//  SDLF_putstr (Smask, xt+1, (h-6)/2-8+1, txtlo, s.c_str());   // text shadow
    SDLF_putstr (Smask, xt, (h-6)/2-8, txt, s.c_str());		// text itself
    rect.x = 0, rect.y = 0, rect.w = w-6, rect.h = 1;		// top
    SDL_FillRect (Smask, &rect, sr);
    rect.x = 0, rect.y = h-6, rect.w = w-6, rect.h = 1;		// bottom
    SDL_FillRect (Smask, &rect, sr);
    rect.x = 0, rect.y = 0, rect.w = 1, rect.h = h-6;		// left
    SDL_FillRect (Smask, &rect, sr);
    rect.x = w-6, rect.y = 0, rect.w = 1, rect.h = h-5;		// right
    SDL_FillRect (Smask, &rect, sr);
    
    Sup = shadowgen (Smask, 6, 10);
    
    rect.x = 0, rect.y = 0, rect.w = w, rect.h = h;
    SDL_FillRect (Smask,   &rect, TransBl);			// erase
//  SDLF_putstr (Smask, xt+1, (h-6)/2-8+1, txtlo, s.c_str());   // text shadow
    SDLF_putstr (Smask, xt+1, (h-6)/2-8+1, txt, s.c_str());	// text itself
    rect.x = 1, rect.y = 1, rect.w = w-6, rect.h = 1;		// top
    SDL_FillRect (Smask, &rect, sr);
    rect.x = 1, rect.y = h-5, rect.w = w-6, rect.h = 1;		// bottom
    SDL_FillRect (Smask, &rect, sr);
    rect.x = 1, rect.y = 1, rect.w = 1, rect.h = h-6;		// left
    SDL_FillRect (Smask, &rect, sr);
    rect.x = w-5, rect.y = 1, rect.w = 1, rect.h = h-5;		// right
    SDL_FillRect (Smask, &rect, sr);
    
    Sdown = shadowgen (Smask, 3, 2);
}

void Currant::draw (bool isdown)
{
    // JDJDJD clipping memorizing method
    SDL_Rect save;
    SDL_GetClipRect (agrume->S, &save);
    SDL_SetClipRect (agrume->S, &initialcliprect);

    agrume->BlitTo (rdst, Sunder);
    agrume->BlitTo (rdst, isdown ? Sdown : Sup);
    agrume->AlphaMaxBlitTo (rdst, isdown ? Sdown : Sup);

    // JDJDJD clipping memorizing method
    SDL_SetClipRect (agrume->S, &save);
}

void Currant::show (Agrume &agrume, int x, int y)
{
    Currant::agrume = &agrume;
    rdst.x = x, rdst.y = y;
    rdst.w = Sdown->w, rdst.h = Sdown->h;

    // JDJDJD clipping memorizing method
    SDL_GetClipRect(agrume.S, &initialcliprect);
    
    Sunder = agrume.CreateMimicSurface (SDL_HWSURFACE, Sdown->w, Sdown->h); // we copy the button's underground
    SDL_SetAlpha (Sunder, 0, 255);
    SDL_SetColorKey (Sunder, 0, 0);
    agrume.BlitFrom (rdst, Sunder);
    agrume.addrect (rdst, (PEventCB)(button));
}






// --------------- Text -------------------------------------------------------
//
// Skeleton-class for building simple texts

Text::Text (int w, int h, const string & s, RGBAColor txt)
{
    Text::txt = (Uint32)txt;
    Text::rdst.w = w,
    Text::rdst.h = h;
    Text::s = s;
    Sunder = NULL;
}

void Text::activate (Agrume &agrume, int x, int y)
{   Text::agrume = &agrume;
    Text::rdst.x = x,
    Text::rdst.y = y;

    if (Sunder != NULL)
	SDL_FreeSurface (Sunder);
    Sunder = agrume.CreateMimicSurface (SDL_HWSURFACE, rdst.w, rdst.h); // we copy the text's underground
    SDL_SetAlpha (Sunder, 0, 255);
    SDL_SetColorKey (Sunder, 0, 0);
    agrume.BlitFrom (rdst, Sunder);


    // int xt = (rdst.w-6)/2-l*4;
    int xt = 0;

    // JDJDJD clipping memorizing method
    SDL_Rect save;
    SDL_GetClipRect (agrume.S, &save);
    SDL_SetClipRect (agrume.S, &rdst);

    agrume.putstr (rdst.x + xt+1, rdst.y + (rdst.h-6)/2-8+1, s.c_str(), txt);

    // JDJDJD clipping memorizing method
    SDL_SetClipRect (agrume.S, &save);
}

void Text::settext (const string & s)
{
    Text::s = s;
    agrume->BlitTo (rdst, Sunder);

    // int xt = (rdst.w-6)/2-l*4;
    int xt = 0;

    // JDJDJD clipping memorizing method
    SDL_Rect save;
    SDL_GetClipRect (agrume->S, &save);
    SDL_SetClipRect (agrume->S, &rdst);

    agrume->putstr (rdst.x + xt+1, rdst.y + (rdst.h-6)/2-8+1, s.c_str(), txt);

    // JDJDJD clipping memorizing method
    SDL_SetClipRect (agrume->S, &save);
}

Text::~Text (void)
{   
    if (Sunder != NULL)
	SDL_FreeSurface (Sunder);
}


// --------------- Button -----------------------------------------------------
//
// Skeleton-class for building simple pressed buttons
// a "Button" appears as a frozen control

Button::Button (int w, int h, const string & s, ButtonStyle & style, bool value)
{
    Pvalue = &ourvalue;
    ispressed = value;
    ourvalue = value;
    Button::style = style.link (this);
    Button::style->buildtext (w, h, s);
}
Button::Button (int w, int h, const string & s, ButtonStyle & style, bool *Pvalue)
{
    Button::Pvalue = Pvalue;
    ispressed = *Button::Pvalue;
    Button::style = style.link (this);
    Button::style->buildtext (w, h, s);
}

Button::~Button ()  // JDJDJDJD incomplete ! need to be furnished !!!
{
    
}

void Button::refresh ()
{
    style->draw (ispressed);
}

void Button::activate (Agrume &agrume, int x, int y)
{
    style->show (agrume, x, y);
    refresh ();
}


// --------------- PushButton -------------------------------------------------
//
// a button that doesn't stay pressed

int PushButton::MOUSEBUTTONDOWN (SDL_Event const & event)
{
#ifdef DEBUGGRPFRT
    bzouzerr << "PushButton::MOUSEBUTTONDOWN (" << thestring << ")" << endl ;
#endif

    ispressed = true;
    *Pvalue = true;
    refresh ();
    valuechange ();
    return 1;
}

int PushButton::MOUSEBUTTONUP (SDL_Event const & event)
{
#ifdef DEBUGGRPFRT
    bzouzerr << "PushButton::MOUSEBUTTONUP (" << thestring << ")" << endl ;
#endif

    ispressed = false;
    *Pvalue = false;
    refresh ();
    valuechange ();
    return 1;
}

int PushButton::LOOSEMOUSEBUTTONFOCUS (SDL_Event const & event)
{
#ifdef DEBUGGRPFRT
    bzouzerr << "PushButton::LOOSEMOUSEBUTTONFOCUS (" << thestring << ")" << endl ;
#endif

    ispressed = false;
    *Pvalue = false;
    refresh ();
    valuechange ();
    return 1;
}

int PushButton::MOUSEOUT (SDL_Event const & event)
{
#ifdef DEBUGGRPFRT
    bzouzerr << "PushButton::MOUSEOUT (" << thestring << ")" << endl ;
#endif
    if (hasmousebuttonfocus ()) {
	ispressed = false;
	*Pvalue = false;
	refresh ();
	valuechange ();
    }
    return 1;
}

int PushButton::MOUSEIN (SDL_Event const & event)
{
#ifdef DEBUGGRPFRT
    bzouzerr << "PushButton::MOUSEIN (" << thestring << ")" << endl ;
#endif
    if (hasmousebuttonfocus ()) {
	ispressed = true;
	*Pvalue = true;
	refresh ();
	valuechange ();
    }
    return 1;
}


// --------------- SwitchButton -----------------------------------------------
//
// a button that toggles its state each time it's pressed

int SwitchButton::MOUSEBUTTONDOWN (SDL_Event const & event)
{
#ifdef DEBUGGRPFRT
    bzouzerr << "SwitchButton::MOUSEBUTTONDOWN (" << thestring << ")" << endl ;
#endif

    ispressed = true;
    refresh ();
    return 1;
}

int SwitchButton::MOUSEBUTTONUP (SDL_Event const & event)
{
#ifdef DEBUGGRPFRT
    bzouzerr << "SwitchButton::MOUSEBUTTONUP (" << thestring << ")" << endl ;
#endif

    *Pvalue = ! (*Pvalue);
    ispressed = *Pvalue;
    refresh ();
    valuechange ();
    return 1;
}

int SwitchButton::LOOSEMOUSEBUTTONFOCUS (SDL_Event const & event)
{
#ifdef DEBUGGRPFRT
    bzouzerr << "SwitchButton::LOOSEMOUSEBUTTONFOCUS (" << thestring << ")" << endl ;
#endif

    ispressed = *Pvalue;
    refresh ();
    return 1;
}

int SwitchButton::MOUSEOUT (SDL_Event const & event)
{
#ifdef DEBUGGRPFRT
    bzouzerr << "SwitchButton::MOUSEOUT (" << thestring << ")" << endl ;
#endif
    if (hasmousebuttonfocus ()) {
	ispressed = *Pvalue;
	refresh ();
    }
    return 1;
}

int SwitchButton::MOUSEIN (SDL_Event const & event)
{
#ifdef DEBUGGRPFRT
    bzouzerr << "SwitchButton::MOUSEIN (" << thestring << ")" << endl ;
#endif
    if (hasmousebuttonfocus ()) {
	ispressed = ! (*Pvalue);
	refresh ();
    }
    return 1;
}


// --------------- OnePressButton ---------------------------------------------
//
// a button that stay down after being pressed

int OnePressButton::MOUSEBUTTONUP (SDL_Event const & event)
{
#ifdef DEBUGGRPFRT
    bzouzerr << "OnePressButton::MOUSEIN (" << thestring << ")" << endl ;
#endif
    SwitchButton::MOUSEBUTTONUP (event);
    *Pvalue = true; //! (*Pvalue);
    ispressed = true;
    refresh();
    valuechange ();
    return 1;
}



// -------------- RadioValue / RadioButton ------------------------------------
//
// a set of buttons where only one can be pressed at a time
//

RadioValue::RadioValue ()
{   Pvalue = &ourvalue;
    *Pvalue = -1;
    curselected = NULL;
    nbmembers = 0;
}

RadioValue::RadioValue (int * Pvalue)
{   RadioValue::Pvalue = Pvalue;
    *Pvalue = -1;
    curselected = NULL;
    nbmembers = 0;
}

void RadioValue::setvalue (int v)
{   if (v < nbmembers) {
	*Pvalue = v;
	if (curselected != NULL) {
	    *curselected->Pvalue = false;
	     curselected->ispressed = false;
	     curselected->refresh ();
	}

	LRadioButton::iterator li = lbutton.begin ();
	int i=0;
	while (i != v) li++, i++;
	*(*li)->Pvalue = true;
	 (*li)->ispressed = true;
	 (*li)->refresh ();
	curselected = (*li);
    }
}

RadioButton * RadioValue::radiobutton (int w, int h, const string & s, ButtonStyle & style)
{   RadioButton * r = new RadioButton (w, h, s, style, this, nbmembers);
    nbmembers ++;
    lbutton.push_back (r);
    return r;
}


// --------------- cyclit -----------------------------------------------------
//
// generates a set of bitmaps for rotating-buttons
//

typedef SDL_Surface * PSDL_Surface;

PSDL_Surface rotit [257];

void initrotit (void)
{
    int wd = 16+3*6;
    int sur = 3;
    int sur2 = sur * sur;
    int w = wd * sur;

    SDL_Surface *Sprework = Create32bSurface (SDL_SRCALPHA | SDL_HWSURFACE, w, w);
    SDL_SetAlpha    (Sprework, SDL_SRCALPHA, 255);
    SDL_SetColorKey (Sprework, 0, 0);

    // JDJDJDJD a reporter dans le style
    Uint32 fg = SDL_MapRGBA(Sprework->format, 128,  128, 255, 255);	// bleu lumineux
//    Uint32 fg = SDL_MapRGBA(Sprework->format, 0xBB,  0, 0,  0xFF);	// rouge sombre
//    Uint32 fg = SDL_MapRGBA(Sprework->format, 0,  60, 128,  0xFF);
//    Uint32 bg = SDL_MapRGBA(Sprework->format, 128,  128,    128,    0x0);
    Uint32 bg = SDL_MapRGBA(Sprework->format, 0,  0,    0,    0x0);

    int i, j;
    long w2 = (w-sur*6) >> 1;
    long rr1 = (long)((w2*0.7) * (w2*0.7));
    long r2  = (long) (w2*0.6);
    long rr2 = (long)((w2*0.6) * (w2*0.6));
    long r3  = (long) (w2*1.0);
    long d1  = (long) (w2*0.2);

    {
	for (i=0 ; i<w ; i++) {
	    for (j=0 ; j<w ; j++) {
		long r = (i-w2)*(i-w2) + (j-w2)*(j-w2);
		if ((rr2 <= r) && (r <rr1))
		    SDLF_putpixel (Sprework, i, j, fg);
		else
		    SDLF_putpixel (Sprework, i, j, bg);
	    }
	}
    }

    SDL_Surface *Swork =  Create32bSurface (SDL_SRCALPHA | SDL_HWSURFACE, w, w);
    SDL_SetAlpha    (Swork, SDL_SRCALPHA, 255);
    SDL_SetColorKey (Swork, 0, 0);

    SDL_Surface *Sfinal = Create32bSurface (SDL_SRCALPHA | SDL_HWSURFACE, wd, wd);
    SDL_SetAlpha    (Sfinal, SDL_SRCALPHA, 255);
    SDL_SetColorKey (Sfinal, 0, 0);

    int ia;
    for (ia=0 ; ia<257 ; ia++) {

	{   SDL_Rect rdst;
	    rdst.x = 0, rdst.y = 0;
	    SDL_BlitSurface (Sprework, NULL, Swork, &rdst);
	}
	
	memcpy (Swork->pixels, Sprework->pixels, Sprework->pitch * Sprework->h);
	
	{
	    double angle = (3.0*M_PI/4.0) + ia * (3.0*M_PI/2.0)/256.0;
	    double ca = cos (angle);
	    double sa = sin (angle);

	    Lpoint polyg;
	    polyg.push ((int)(w2 + ca * r3),		(int)(w2 + sa * r3));
	    polyg.push ((int)(w2 + ca * r2 - sa * d1),	(int)(w2 + sa * r2 + ca * d1));
	    polyg.push ((int)(w2 + ca * r2 + sa * d1),	(int)(w2 + sa * r2 - ca * d1));
	    polyfill (Swork, polyg, fg);
	}
	

	{	vector <Uint32 *> v, vv;
	    int i, j, k, l;

	    for (i=0 ; i<sur ; i++) {
		v.push_back ((Uint32 *)( ((char *)Swork->pixels) + Swork->pitch * i));
		vv.push_back ((Uint32 *)( ((char *)Swork->pixels) + Swork->pitch * i));
	    }
	    
	    for (i=0 ; i<wd ; i++) {
		for (j=0 ; j<wd ; j++) {
		    int r = 0, g = 0, b = 0, a = 0, n = 0;
		    Uint8 rr, gg, bb, aa;

		    for (l=0 ; l<sur ; l++) {
			for (k=0 ; k<sur ; k++) {
			    SDL_GetRGBA (*(v[k]++), Swork->format, &rr, &gg, &bb, &aa);
			    if (aa != 0)
				r += rr, g += gg, b += bb, n++;
			    a += aa;
			}
		    }

		    if (n != 0)
			r /= n,  g /= n,  b /= n;
		    a /= sur2;
		    SDLF_putpixel (Sfinal, j, i, SDL_MapRGBA(Swork->format, r, g, b, a));
		}
		for (k=0 ; k<sur ; k++) {
		    vv[k] = (Uint32*) (((char *)vv[k]) + Swork->pitch * sur);
		    v[k] = vv[k];
		}
	    }
	}
	
	rotit[ia] = shadowgen(Sfinal, 6, 10);
    
        // JDJDJDJD pour controler le resultat
        //	{   SDL_Rect rdst;
        //	    rdst.x = xd+16, rdst.y = yd+16;
        //	    SDL_BlitSurface (Swork, NULL, Sdest, &rdst);
        //	    // SDL_BlitSurface (Sfinal, NULL, Sdest, &rdst);
        //	    rdst.x = xd, rdst.y = yd;
        //	    SDL_BlitSurface (rotit[ia], NULL, Sdest, &rdst);
        //	    SDL_UpdateRect (Sdest, rdst.x, rdst.y, 100, 100);
        //	}
    }

    SDL_FreeSurface (Swork);
    SDL_FreeSurface (Sfinal);
    SDL_FreeSurface (Sprework);
}


// --------------- RotButton --------------------------------------------------
//
// Skeleton-class for building rotating buttons

RotButton::RotButton (int value) : MGrabber ()
{   
    Pvalue = &ourvalue;
    *Pvalue = value;
    if	    (*Pvalue > 256) *Pvalue = 256;
    else if (*Pvalue < 0)   *Pvalue = 0;
    if (value != *Pvalue) valuechange ();

    if (firstuse) {
	initrotit ();
	firstuse = false;
    }

    warpcoordvalid = false;
    modebefore = SDL_GRAB_QUERY;
}

RotButton::RotButton (int * Pvalue) : MGrabber ()
{   
    int value = *Pvalue;
    RotButton::Pvalue = Pvalue;
    if	    (*Pvalue > 256) *Pvalue = 256;
    else if (*Pvalue < 0)   *Pvalue = 0;
    if (value != *Pvalue) valuechange ();

    if (firstuse) {
	initrotit ();
	firstuse = false;
    }
    warpcoordvalid = false;
    modebefore = SDL_GRAB_QUERY;
}

void RotButton::draw (int v)
{
    agrume->BlitTo (rdst, Sunder);

    if	    (v > 256) v = 256;
    else if (v < 0)   v = 0;

    agrume->BlitTo (rdst, rotit [v]);
    agrume->AlphaMaxBlitTo (rdst, rotit [v]);
}

void RotButton::show (Agrume & agrume, int x, int y)
{
    RotButton::agrume = &agrume;
    rdst.x = x, rdst.y = y;
    rdst.w = rotit[0]->w, rdst.h = rotit[0]->h;

    Sunder = agrume.CreateMimicSurface (SDL_HWSURFACE, rdst.w, rdst.h); // we copy the button's underground
    SDL_SetAlpha (Sunder, 0, 255);
    SDL_SetColorKey (Sunder, 0, 0);
    agrume.BlitFrom (rdst, Sunder);
}

void RotButton::activate (Agrume & agrume, int x, int y)
{
    show (agrume, x, y);
    agrume.addrect (rdst, (PEventCB)(this));

    refresh ();
}

void RotButton::refresh ()
{
    draw (*Pvalue);
}

void RotButton::grabberchange (SDL_Event const & event)
{
    int value = *Pvalue;
    *Pvalue -= event.motion.yrel;
    if	    (*Pvalue > 256) *Pvalue = 256;
    else if (*Pvalue < 0)   *Pvalue = 0;
    refresh ();
    if (value != *Pvalue) valuechange ();
}

////	int RotButton::MOUSEMOTION (SDL_Event const & event)
////	{   
////	#ifdef JDDEBUGMOTION
////	    bzouzerr << "debug motion : " << event.motion.x << ":" << event.motion.y << "   "
////				      << event.motion.xrel << ":" << event.motion.yrel << endl ;
////	#endif
////	
////	    if ((event.motion.xrel != 0) && (event.motion.yrel !=0)) {
////	#ifdef BUILD_FOR_WIN32
////		if (event.motion.yrel < 50) {   // JDJDJDJD with windows we have a huge drift at starting grabbing the mouse
////						// we may in the future is the trick of avoiding treating mouse warps this way ?
////	#endif
////	//	    SDL_WarpMouse(xorigin, yorigin);
////		    int value = *Pvalue;
////		    *Pvalue -= event.motion.yrel;
////		    if	    (*Pvalue > 256) *Pvalue = 256;
////		    else if (*Pvalue < 0)   *Pvalue = 0;
////		    sdx += event.motion.xrel,
////		    sdy += event.motion.yrel;
////		    refresh ();
////		    if (value != *Pvalue) valuechange ();
////	#ifdef BUILD_FOR_WIN32
////		}
////	#endif
////	    }
////	    return 0;
////	}
////	
////	int RotButton::LOOSEMOUSEBUTTONFOCUS (SDL_Event const & event)
////	{
////	// bzouzerr << "RotButton::MOUSEBUTTONUP modebofore=" << ((modebefore==SDL_GRAB_ON) ? "SDL_GRAB_ON" : "pas SDL_GRAB_ON") << endl ;
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
////	    warpcoordvalid = false;
////	    ownergrabmouse = NULL;
////	
////	    return 0;
////	}
////	
////	int RotButton::MOUSEBUTTONDOWN (SDL_Event const & event)
////	{
////	    xorigin = event.button.x;
////	    yorigin = event.button.y;
////	    sdx = 0, sdy = 0;
////	    warpcoordvalid = true;
////	
////	    if (modebefore == SDL_GRAB_QUERY)
////		modebefore = SDL_WM_GrabInput (SDL_GRAB_QUERY);
////	
////	// bzouzerr << "RotButton::MOUSEBUTTONDOWN SDL_GRAB_ON" << endl ;
////	    SDL_WM_GrabInput (SDL_GRAB_ON);
////	    SDL_ShowCursor (SDL_DISABLE);
////	    ownergrabmouse = (PEventCB)this;
////	    return 0;
////	}
////	
////	int RotButton::MOUSEBUTTONUP (SDL_Event const & event)
////	{
////	// bzouzerr << "RotButton::MOUSEBUTTONUP modebofore=" << ((modebefore==SDL_GRAB_ON) ? "SDL_GRAB_ON" : "pas SDL_GRAB_ON") << endl ;
////	    SDL_WM_GrabInput (modebefore);
////	    modebefore = SDL_GRAB_QUERY;
////	    SDL_ShowCursor(SDL_ENABLE);
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

RotButton::~RotButton ()
{   // JDJDJDJD  a completer ?????!!!!!
}

}

