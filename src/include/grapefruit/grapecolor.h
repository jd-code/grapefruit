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
#warning grapecolor.h should not be included as is. grapefruit.h should be included instead
#else  // GRAPEFRUIT_H_INC
#ifdef	    GRAPEFRUIT_H_GLOBINST
#define	    GRAPECOLOR_H_GLOBINST
#endif //   GRAPEFRUIT_H_GLOBINST
#endif // GRAPEFRUIT_H_INC

#ifndef GRAPECOLOR_H_INC
#define GRAPECOLOR_H_INC

namespace grapefruit
{
    // using namespace std ;

    // --------------- DefaultPixelFormat -----------------------------------------
    //
    // grapefruit's default SDL pixel format.
    //

    //! sets the grapefruit's default SDL pixel format at starting grapefruit.
    inline SDL_PixelFormat initDefaultPixelFormat ()
    {   SDL_PixelFormat f;

	// JDJDJDJD JDJDJDJD il n'est pas clair qu'une globale initialisee comme ça
	// puisse fonctionner avant que SDLinit ait été appelée ???
	SDL_Surface *temp = Create32bSurface (SDL_SRCALPHA, 10, 10);
	SDL_SetAlpha    (temp, SDL_SRCALPHA, 255);
	SDL_SetColorKey (temp, 0, 0);
	f = *(temp->format);
	SDL_FreeSurface (temp);
	
	// JDJD cette methode a l'inconvenient de ne pas mettre la partie alpha a une valeur correcte
	//	elle a été remplacée par le code ci-dessus (qui a peut-être d'autres inconvénients...)
	//
	//		f.palette       = NULL,
	//		f.BitsPerPixel  = 32,
	//		f.BytesPerPixel = 4,
	//	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	//		f.Rmask  = 0xff000000, f.Gmask  = 0x00ff0000, f.Bmask  = 0x0000ff00, f.Amask  = 0x000000ff,
	//	#else
	//		f.Rmask  = 0x000000ff, f.Gmask  = 0x0000ff00, f.Bmask  = 0x00ff0000, f.Amask  = 0xff000000,
	//	#endif
	//		f.Rshift =         24, f.Gshift =         16, f.Bshift =          8, f.Ashift =          0,
	//		f.Rloss  =          0, f.Gloss  =          0, f.Bloss  =          0, f.Aloss  =          0,
	//		f.colorkey = 0,
	//		f.alpha = 255;

	return f;
    }


    
#ifndef   GRAPECOLOR_H_GLOBINST
    GRAPEFRUIT_H_SCOPE SDL_PixelFormat DefaultPixelFormat;
#else
    //! grapefruit's default SDL pixel format.
    GRAPEFRUIT_H_SCOPE SDL_PixelFormat DefaultPixelFormat = initDefaultPixelFormat ();
#endif // GRAPECOLOR_H_GLOBINST

////////#ifndef   GRAPECOLOR_H_GLOBINST
////////    extern SDL_PixelFormat DefaultPixelFormat;
////////#else
////////    //! grapefruit's default SDL pixel format.
////////    SDL_PixelFormat DefaultPixelFormat = initDefaultPixelFormat ();
////////#endif // GRAPECOLOR_H_GLOBINST


    // --------------- RGBColor/RGBAColor -----------------------------------------
    //
    //! Utilities for color manipulations
    //

    class RGBColor
    {   
	public:
	    Uint8 r,g,b;

	    RGBColor (Uint32 c = 0)
	    {   RGBColor::r = (c & 0xff0000) >> 16;
		RGBColor::g = (c & 0x00ff00) >>  8;
		RGBColor::b = (c & 0x0000ff);
	    }
	    RGBColor (Uint8 r, Uint8 g, Uint8 b)
	    {   RGBColor::r =r;
		RGBColor::g =g;
		RGBColor::b =b;
	    }
    };

#define RGB_BLACK 	0xff000000UL
#define RGB_WHITE	0xffffffffUL
#define RGB_RED   	0xffff0000UL
#define RGB_GREEN 	0xff00ff00UL
#define RGB_BLUE  	0xff0000ffUL
#define RGB_YELLOW	0xffffff00UL
#define RGB_CYAN  	0xff00ffffUL
#define RGB_MAUVE 	0xffff00ffUL

#define RGB_DARKRED   	0xffaa0000UL
#define RGB_DARKGREEN 	0xff00aa00UL
#define RGB_DARKBLUE  	0xff0000aaUL
#define RGB_DARKYELLOW	0xffaaaa00UL
#define RGB_DARKCYAN  	0xff00aaaaUL
#define RGB_DARKMAUVE 	0xffaa00aaUL

    //! An utility class for defining and manipulating RGBA colors
    class RGBAColor
    {
	public:
	    Uint8 r,g,b,a;
	    bool isnttranslated;
	    Uint32 val;

	    RGBAColor (Uint32 c = 0)	//!< this one is machine-independent (not related to SDL_BYTEORDER)
	    {   RGBAColor::r = (c & 0x00ff0000) >> 16;
		RGBAColor::g = (c & 0x0000ff00) >>  8;
		RGBAColor::b = (c & 0x000000ff);
		RGBAColor::a = (c & 0xff000000) >> 24;
		isnttranslated = true;
	    }
	    RGBAColor (Uint8 r, Uint8 g, Uint8 b, Uint8 a=255U)
	    {   RGBAColor::r = r;
		RGBAColor::g = g;
		RGBAColor::b = b;
		RGBAColor::a = a;
		isnttranslated = true;
	    }
	    RGBAColor (RGBColor const & c, Uint8 a=255U)
	    {   RGBAColor::r = c.r;
		RGBAColor::g = c.g;
		RGBAColor::b = c.b;
		RGBAColor::a = a;
		isnttranslated = true;
	    }
	    Uint32 SDL_MapRGBA (SDL_PixelFormat *format = &DefaultPixelFormat)
	    {	return ::SDL_MapRGBA (format, r, g, b, a);
	    }
	    operator Uint32()
	    {
		if (isnttranslated) {
// cerr << "RGBAColor::Uint32 translation..." << endl ;
		    val = ::SDL_MapRGBA (&DefaultPixelFormat, r, g, b, a);
		    isnttranslated = false;
		}
// cerr << "RGBAColor::Uint32 " << setbase (16)
//      << (Uint16) r << ":" << (Uint16) g << ":" << (Uint16) b << "-" << (Uint16) a << " = "
//      << val << endl ;
		return val;
	    }
    };
#define RGBA_TRBLACK 0x0UL

// JDJDJDJD some old function that need to become members of the previous classes
//
//	static inline int tolow (int i, int n)
//	{   int nn;
//	    for (nn=0 ; nn<n ; nn++)
//		i = (  (i << 1) ) / 3;
//	    return i;
//	}
//	static inline int tohi (int i, int n)
//	{   int nn;
//	    for (nn=0 ; nn<n ; nn++)
//		i = i + ((255-i) >> 2);
//	    return i;
//	}
//	
//	Uint32 tolow (SDL_PixelFormat *format, RGBColor & c, Uint8 alpha, int n = 1)
//	{   return SDL_MapRGBA (format,
//				tolow (c.r, n),
//				tolow (c.g, n),
//				tolow (c.b, n),
//				alpha);
//	}
//	
//	Uint32 tohi (SDL_PixelFormat *format, RGBColor & c, Uint8 alpha, int n = 1)
//	{   return SDL_MapRGBA (format,
//				tohi (c.r, n),
//				tohi (c.g, n),
//				tohi (c.b, n),
//				alpha);
//	}

// #define TRANSP_DEF ((Uint32)GL_SRC_ALPHA + (((Uint32)GL_SRC_COLOR) << 16))
// #define TRANSP_DEF ((Uint32)GL_DST_ALPHA + (((Uint32)GL_ONE_MINUS_SRC_ALPHA) << 16))
// #define TRANSP_DEF ((Uint32)GL_SRC_COLOR + (((Uint32)GL_ONE_MINUS_DST_ALPHA) << 16))
#define TRANSP_DEFB ((Uint32)GL_SRC_COLOR + (((Uint32)GL_ONE_MINUS_SRC_ALPHA) << 16))
#define TRANSP_DEF ((Uint32)GL_ONE + (((Uint32)GL_ONE_MINUS_SRC_ALPHA) << 16))

    //! a utility class for setting the OpenGL transparency function
    class TranspFunc
    {
	public:
	    int transpS,	//!< OpenGL transparency function for source
		transpD;	//!< OpenGL transparency function for destination

	    TranspFunc (Uint32 combined = TRANSP_DEF)
	    {   transpS = (int)(combined & 0xFFFF);
		transpD = (int)((combined & 0xFFFF0000) >> 16);
	    }
	    TranspFunc (int transpS, int transpD)
	    {	TranspFunc::transpS = transpS,
		TranspFunc::transpD = transpD;
	    }
    };

}

#endif // GRAPECOLOR_H_INC

/*
 * $Log: grapecolor.h,v $
 * Revision 1.5  2004/05/03 22:02:50  jd
 * big merge for mingw/msys and cross-mingw
 *
 * Revision 1.4  2004/03/14 00:24:55  jd
 * documentation updates
 *
 * Revision 1.3  2003/09/06 22:17:16  jd
 * first release of TDs
 *
 * Revision 1.2  2003/05/15 22:42:40  jd
 * added TranspFunc : transparancy functions
 *
 * Revision 1.1.1.1  2003/04/22 14:04:34  jd
 * librarisation of grapfruit
 *
 *
 */
