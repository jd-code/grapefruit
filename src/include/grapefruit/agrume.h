/* 
 * $Id: agrume.h,v 1.7 2004/05/03 22:02:50 jd Exp $
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
#warning agrume.h should not be included as is. grapefruit.h should be included instead
#else  // GRAPEFRUIT_H_INC
#ifdef	    GRAPEFRUIT_H_GLOBINST
#define	    AGRUME_H_GLOBINST
#endif //   GRAPEFRUIT_H_GLOBINST
#endif // GRAPEFRUIT_H_INC

#ifndef AGRUME_H_INC
#define AGRUME_H_INC

namespace grapefruit
{
    // using namespace std ;

    // --------------- AgrumeStyle ------------------------------------------------
    //
    //! some window-style skeleton
    //

    class AgrumeStyle
    {
	protected:
	    Agrume *agrume;
	    class CloseButton;
	    CloseButton * closebutton;
	public:
	    inline AgrumeStyle () { agrume = NULL; closebutton = NULL; }
	    inline AgrumeStyle (Agrume * a) { agrume = a; closebutton = NULL; }
	    virtual inline AgrumeStyle* link (Agrume * a) { return new AgrumeStyle (a); }
	    virtual inline void fill_bg (void) {}
	    virtual inline void dispose_decoration (void) {}
	    virtual ~AgrumeStyle () {}
    };

    //! the default style profided for Agrume
    class StyleDef : public virtual AgrumeStyle
    {
	    RGBAColor	bg, //!< the background base color
			fg, //!< the text foreground main color
			dc; //!< the decorations color
	    TranspFunc	tf; //!< the transparency functions
	    bool hasclose, hascaption;
	    CaptionZone *captzone;
	public:
	    StyleDef (RGBAColor bg = RGBA_TRBLACK, RGBAColor fg = RGB_WHITE, bool hasclose = true, bool hascaption = true, TranspFunc tf = TRANSP_DEF);
	    StyleDef (RGBAColor bg, RGBAColor fg, RGBAColor dc, bool hasclose = true, bool hascaption = true, TranspFunc tf = TRANSP_DEF);

	    inline StyleDef () { agrume = NULL; captzone = NULL; closebutton = NULL; }
	    inline StyleDef (Agrume * a) { agrume = a; captzone = NULL; closebutton = NULL; }
	    virtual inline AgrumeStyle* link (Agrume * a)
	    {	StyleDef *r = new StyleDef (*this);
		r->agrume = a;
		return r;
	    }
	    virtual void fill_bg (void);
	    virtual void dispose_decoration (void);
	    virtual ~StyleDef ();
    };
    
    // --------------- Agrume -----------------------------------------------------
    //
    // some window
    //

    typedef Agrume * PAgrume;
    typedef list<PAgrume> LPAgrume;
    
    //! some window
    class Agrume : public ClicableZone
    {
	private:
	    bool isshown,
		 toredraw;

	    AgrumeStyle * style;

	    Agrume (Agrume &);		//!< no copy-constructor for Agrume (use reference transmission instead)
	    Agrume& operator=(Agrume&);	//!< not allowed !

	    static LPAgrume agrumes;
	    static LPAgrume shownagrumes;

	    // GL part	JDJDJDJD to be �ifdefed� !!!!
	    GLuint gltexid;
	    GLfloat wf, hf;

	public:
	    int x, y, w, h, // JDJDJDJD temporairement publique...
		w2, h2;

	    TranspFunc tf; //!< the transparency functions

	    SDL_Surface *S;

	    Agrume (int x, int y, int w, int h, AgrumeStyle & style, PEventCB defcb=NULL);
	    ~Agrume (void)
		{   if (style != NULL)
			delete (style);
		}

	    void hide (void);
	    void show (void);
	    void refresh (int x, int y, int w, int h);
	    void refresh (void);

	    void BlitTo (SDL_Rect & rdst, SDL_Surface * Ssource);
	    void AlphaMaxBlitTo (SDL_Rect & rdst, SDL_Surface * Ssource);
	    void BlitFrom (SDL_Rect & rdst, SDL_Surface * Sdest);

	    void rectangle (SDL_Rect & rdst, Uint32 sdlcolor);

	    inline void clear (Uint32 sdlcolor)
	    {	SDL_Rect r;
		r.x = 0, r.y = 0, r.w = w, r.h = h;
		rectangle (r, sdlcolor);
	    }
	    inline void polyfill (Lpoint & l, Uint32 pixel)
	    {	grapefruit::polyfill (S, l, pixel);
	    }
	    inline void polyline (Lpoint & l, Uint32 pixel, bool isclosed = false)
	    {	grapefruit::polyline (S, l, pixel, isclosed);
	    }
	    inline void line (int x1, int y1, int x2, int y2, Uint32 pixel)
	    {	grapefruit::line (S, x1, y1, x2, y2, pixel);
	    }
	    inline void putstr (int x, int y, const char *text, Uint32 pixel)
	    {	SDLF_putstr (S, x, y, pixel, text);
	    }

	    inline SDL_Surface * CreateMimicSurface (Uint32 flags, int width, int height)
	    {	return SDL_CreateMimicSurface (flags, width, height, S);
	    }

	friend void reloader (void);
	friend void redrawer (int ww, int wh);
	friend PEventCB getcb (int x, int y);
	friend class AgrumeStyle;
    };

	// --------------- agrume related globals -------------------------------------

#ifdef   AGRUME_H_GLOBINST
	//! the list of all agrumes
	GRAPEFRUIT_H_SCOPE LPAgrume Agrume::agrumes;

	//! the list of shown agrumes
	GRAPEFRUIT_H_SCOPE LPAgrume Agrume::shownagrumes;
#endif

////////#ifdef   AGRUME_H_GLOBINST
////////	// --------------- agrume related globals -------------------------------------
////////
////////	//! the list of all agrumes
////////	LPAgrume Agrume::agrumes;
////////
////////	//! the list of shown agrumes
////////	LPAgrume Agrume::shownagrumes;
////////#endif

}

#endif // AGRUME_H_INC

/*
 * $Log: agrume.h,v $
 * Revision 1.7  2004/05/03 22:02:50  jd
 * big merge for mingw/msys and cross-mingw
 *
 * Revision 1.6  2004/03/14 00:24:55  jd
 * documentation updates
 *
 * Revision 1.5  2003/09/06 22:17:16  jd
 * first release of TDs
 *
 * Revision 1.4  2003/06/04 22:41:52  jd
 * enabled some close buttons for agrume
 *
 * Revision 1.3  2003/05/15 22:41:12  jd
 * added transparency functions
 * tuned polyline
 * tuned local include / vs sys include
 *
 * Revision 1.2  2003/05/11 21:53:50  jd
 * updtaes and debug after merging against genrythm
 *
 * Revision 1.1.1.1  2003/04/22 14:04:34  jd
 * librarisation of grapfruit
 *
 *
 */
 

