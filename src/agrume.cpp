/* 
 * $Id: agrume.cpp,v 1.9 2004/03/14 00:24:55 jd Exp $
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

// --------------- AgrumeStyle ------------------------------------------------
//
// some window-style skeleton
//

StyleDef::StyleDef (RGBAColor bg, RGBAColor, RGBAColor dc, bool hasclose, bool hascaption, TranspFunc tf)
{   StyleDef::bg = bg;
    StyleDef::fg = fg;
    StyleDef::dc = dc;
    StyleDef::tf = tf;
    StyleDef::hasclose = hasclose,
    StyleDef::hascaption = hascaption;
}

StyleDef::StyleDef (RGBAColor bg, RGBAColor fg, bool hasclose, bool hascaption, TranspFunc tf)
{   StyleDef::bg = bg;
    StyleDef::fg = fg;
    StyleDef::dc = fg;
    StyleDef::tf = tf;
    StyleDef::hasclose = hasclose,
    StyleDef::hascaption = hascaption;
}

void StyleDef::fill_bg (void)
{   agrume->clear(bg.SDL_MapRGBA (agrume->S->format));
}

StyleDef::~StyleDef (void)
{   if (closebutton != NULL) {
	delete (closebutton);
	cerr << "deleting some closebutton" << endl ;
			     }
}

//! this is for our closebutton's style... it could be better ? JDJDJDJD
static Currant bstylecurrant;

void StyleDef::dispose_decoration (void)
{
    Uint32 dc = StyleDef::dc.SDL_MapRGBA(agrume->S->format);
    int xcap = 2, wcap = agrume->w - 5;
    SDL_Rect cliprect;
    cliprect.x = 0, cliprect.y = 0, cliprect.w = agrume->w, cliprect.h = agrume->h;

    agrume->tf = tf;
    
    if (hasclose) {
	SDL_Rect r;
	r.x=2, r.y=2, r.h=1, r.w=12;
	agrume->rectangle (r, dc);
	r.h=12, r.w=1;
	agrume->rectangle (r, dc);
	r.x=13;
	agrume->rectangle (r, dc);
	r.x=2, r.y=13, r.h=1, r.w=12;

	agrume->rectangle (r, dc);  
	closebutton = new CloseButton (10,10,"", bstylecurrant, agrume);
	closebutton->activate (*agrume, 2, 2 );
	
	xcap += 16;
	wcap -= 16;
	cliprect.y = 16;
	cliprect.h = agrume->h - 16;
    }
    if (hascaption) {
	SDL_Rect r;
	r.x=xcap, r.y=2, r.h=1, r.w=wcap;
	agrume->rectangle (r, dc);
	r.y=7;
	agrume->rectangle (r, dc);
	r.y=12;
	agrume->rectangle (r, dc);

	captzone = new CaptionZone (&agrume->x, &agrume->y);
	if (captzone == NULL)
	    bzouzerr << "StyleDef::dispose_decoration : could not allocate a CaptionZone ?" << endl ;
	else {
	    r.y = 2, r.h = 12-2;
	    captzone->activate (*agrume, r);
	}
	cliprect.y = 16;
	cliprect.h = agrume->h - 16;
    }
    // cliprect.x += 20, cliprect.y += 10, cliprect.w -= 40, cliprect.h -= 20; // JDJDJDJD pour tester

    // JDJDJD la ligne ci dessous est suprimée à fin de test pour voir...
    SDL_SetClipRect(agrume->S, &cliprect);
}

// --------------- Agrume -----------------------------------------------------
//
// some window
//

#define AGRUMEDEBUG // let's verbose debug the agrumes...

Agrume::Agrume (int x, int y, int w, int h, AgrumeStyle & style, PEventCB defcb)
{
    Agrume::x = x,
    Agrume::y = y,
    Agrume::w = w,
    Agrume::h = h;
    tf = TRANSP_DEF;
    // tf.transpS = GL_SRC_ALPHA, tf.transpD = GL_SRC_COLOR;
    w2 = 1;
    while (w2 < w)  w2 <<= 1;	// we're looking for the first power of 2 greater than w and h
    h2 = 1;
    while (h2 < h)  h2 <<= 1;
    wf = (double)w / w2,
    hf = (double)h / h2;


    S = Create32bSurface (SDL_HWSURFACE, w2, h2);
    SDL_SetAlpha    (S, 0, 255);
    SDL_SetColorKey (S, 0, 0);

    Agrume::style = style.link (this);
    Agrume::style->fill_bg ();
    Agrume::style->dispose_decoration ();
    isshown = false, toredraw = true;

    gltexid = 0;
}

void Agrume::show (void)
{
    if (!isshown) {
	shownagrumes.push_back (this);
	isshown = true;
    }
#ifdef AGRUMEDEBUG
    else {
	bzouzerr << "Agrume::show warning : shown called though the agrume's already shown ?" << endl ;
    }
#endif
}

void Agrume::hide (void)
{
    if (isshown) {
	isshown = false;

	LPAgrume::iterator bi;
	for (bi=shownagrumes.begin() ; bi!=shownagrumes.end() ; bi++) {
	    if (*bi == this) {
		shownagrumes.erase (bi);    // JDJDJD we should test if upward baskets are to be refreshed
		return;			    // JDJDJDJD we don't test multiple presence of ourselves in the list
	    }
	}
	bzouzerr << "Agrume::hide internal error : could not find ourselves in shown list ???" << endl ;
    }
#ifdef AGRUMEDEBUG
    else {
	bzouzerr << "Agrume::hide warning : hide called though the agrume's already hidden ?" << endl ;
    }
#endif
}

void Agrume::refresh (void)
{
    toredraw = true;
}

void Agrume::BlitTo (SDL_Rect & rdst, SDL_Surface * Ssource)
{
    SDL_BlitSurface (Ssource, NULL, S, &rdst);
    refresh ();
}

void Agrume::AlphaMaxBlitTo (SDL_Rect & rdst, SDL_Surface * Ssource)
{
    AlphaMaxBlit (Ssource, S, &rdst);
    refresh ();
}

void Agrume::BlitFrom (SDL_Rect & rdst, SDL_Surface * Sdest)
{
    if (SDL_BlitSurface (S, &rdst, Sdest, NULL) != 0)
	bzouzerr << "Agrume::BlitFrom SDL_BlitSurface returned an error" << endl ;
}

void Agrume::rectangle (SDL_Rect & rdst, Uint32 sdlcolor)
{   SDL_FillRect (S, &rdst, sdlcolor);
    refresh ();
}

//! reloads the textures that maps the set of displayed Agrume
void reloader (void)
{
    LPAgrume::iterator bi;

    for (bi=Agrume::shownagrumes.begin() ; bi!=Agrume::shownagrumes.end() ; bi++) {
	Agrume & b = **bi;

	if (b.toredraw) {
	    if (b.gltexid == 0) {	// we need to allocate a new texture
		glGenTextures (1, &b.gltexid);
	    }
	    glBindTexture(GL_TEXTURE_2D, b.gltexid);
	    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	// JDJDJDJD a verifier plateforme par plateforme ???
#ifdef VERBOSEAGRUME
bzouzerr << "upload de la texture no " << b.gltexid << " : " << b.w2 << "x" << b.h2 << " (" << b.w << "x" << b.h << ")" << endl ;
#endif
	    glTexImage2D(GL_TEXTURE_2D, 0, 4,		// level 0 texture , 4 colors components
		b.w2, b.h2, 0,				// w2 x h2 , border=0
		GL_RGBA, GL_UNSIGNED_BYTE, 
		b.S->pixels);
	    
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	    b.toredraw = false;
	}
    }
}

//! the function that redraws the set of Agrume
void redrawer (int ww, int wh)  // JDJDJDJD ww, wh are the size of the main GL output
{
    glMatrixMode ( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity ();
    glOrtho (0, ww, wh, 0,  5.0, 60.0);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    // glBlendFunc(GL_SRC_COLOR, GL_DST_COLOR);
    // glBlendFunc(GL_SRC_COLOR, GL_DST_COLOR);
    // glBlendFunc(GL_SRC_COLOR, GL_SRC_ALPHA);	// avec replace c'est quasi pas mal...
    glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    GLfloat color[4];
    color[0] = 0.0, color[1] = 0.8, color[2] = 0.9, color[3] = 0.2;
    // color[0] = 0.0, color[1] = 0.0, color[2] = 0.0, color[3] = 0.0;
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color);

    LPAgrume::iterator bi;
    GLfloat p = 8.0;

    for (bi=Agrume::shownagrumes.begin() ; bi!=Agrume::shownagrumes.end() ; bi++, p+=1.0) {
	Agrume & b = **bi;
   
	glBindTexture(GL_TEXTURE_2D, b.gltexid);
	glBlendFunc(b.tf.transpS, b.tf.transpD);
	glBegin(GL_QUADS);

	glTexCoord2f(0.0,  0.0);	glVertex3f(b.x,	    b.y,     p);
	glTexCoord2f(0.0,  b.hf);	glVertex3f(b.x,	    b.y+b.h, p);
	glTexCoord2f(b.wf, b.hf);	glVertex3f(b.x+b.w, b.y+b.h, p);
	glTexCoord2f(b.wf, 0.0);	glVertex3f(b.x+b.w, b.y,     p);

	glEnd();
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix ();
}

//! the global function for matching some evented objects at (x,y) on the screen
PEventCB getcb (int x, int y)
{
    LPAgrume::reverse_iterator bi;
    for (bi=Agrume::shownagrumes.rbegin() ; bi!=Agrume::shownagrumes.rend() ; bi++) {
	Agrume &a = **bi;
	if ((x>a.x) && (y>a.y) && (x < (a.x + a.w)) && (y < (a.y + a.h))) {
	    return a.getcb (x-a.x, y-a.y);
	}
    }
    return TDrenderclickablezone (x, y);
    // return NULL;
}

}

/*
 * $Log: agrume.cpp,v $
 * Revision 1.9  2004/03/14 00:24:55  jd
 * documentation updates
 *
 * Revision 1.8  2003/09/11 23:49:06  jd
 * corrected slow-motion bug in various grabbing code
 * tuned big-motion squiz handling specific to win32 platforms
 * added grabbing code for TDs (first release)
 * added config.h
 *
 * Revision 1.7  2003/09/06 22:17:16  jd
 * first release of TDs
 *
 * Revision 1.6  2003/06/04 22:41:52  jd
 * enabled some close buttons for agrume
 *
 * Revision 1.5  2003/05/20 21:42:48  jd
 * corrected bug in texture's vertical size miscalculation
 *
 * Revision 1.4  2003/05/15 22:45:16  jd
 * added transparency functions
 *
 * Revision 1.3  2003/05/11 21:53:49  jd
 * updtaes and debug after merging against genrythm
 *
 * Revision 1.2  2003/04/22 16:39:28  jd
 * small libtool revision info tuning
 *
 * Revision 1.1.1.1  2003/04/22 14:04:34  jd
 * librarisation of grapfruit
 *
 * Revision 1.1.1.1  2003/02/04 16:39:55  jd
 * imported sources
 *
 *
 */

