/* 
 * $Id: graphutils.h,v 1.1 2003/04/22 14:04:34 jd Exp $
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

#ifndef GRAPHUTILS_H_INC
#define GRAPHUTILS_H_INC

#include "SDL.h"

#ifdef __cplusplus
extern "C" {
namespace grapefruit {
#endif


void SDLF_putpixel (SDL_Surface *surface, int x, int y, Uint32 pixel);
void line (SDL_Surface *Sdest, int x1, int y1, int x2, int y2, Uint32 pixel);
void SDLF_putstr (SDL_Surface *surface, int x, int y, Uint32 pixel, const char *texte);
void SDLF_putstrA (SDL_Surface *surface, int x, int y, Uint32 pixel, const char *texte);

SDL_Surface *SDL_CreateMimicSurface(Uint32 flags, int width, int height, SDL_Surface const * from);
SDL_Surface *Create32bSurface (Uint32 flags, int width, int height);

void MultAlpha (SDL_Surface *s, Uint32 m);
SDL_Surface *ShadowAlpha (Uint32 flags, SDL_Surface const * from, Uint32 m);
void BrightToAlpha (SDL_Surface *s);
void AlphaMaxBlit (SDL_Surface const * from, SDL_Surface *dest, SDL_Rect * rdst);
void ShadowBlit (SDL_Surface const * from, SDL_Surface *dest);

#ifdef __cplusplus
}   /* closing the 'namespace grapefruit {' above... */
}   /* closing the 'extern "C" {' above... */
#endif

#endif /* GRAPHUTILS_H_INC */

/*
 * $Log: graphutils.h,v $
 * Revision 1.1  2003/04/22 14:04:34  jd
 * Initial revision
 *
 * Revision 1.4  2003/04/11 22:57:32  jd
 * miscs about gcc 2.95 -> 3.2
 * clipping added in various graphical primitives (to be enhanced in polyxxx)
 * agrume extensions
 *
 * Revision 1.3  2003/04/08 23:02:36  jd
 * agrume enhancements
 *
 * Revision 1.2  2003/04/03 00:02:04  jd
 * added AlphaMaxBlit : mixing two alpha layers
 *
 * Revision 1.1  2003/03/26 15:04:16  jd
 * rought merge with grapefruit
 * first transparencies
 * first orthogonal projections
 * first texture
 *
 * Revision 1.5  2003/03/18 23:25:54  jd
 * some line capabilities added
 *
 * Revision 1.4  2003/02/12 23:58:32  jd
 * added copy of underlying of buttons
 *
 * Revision 1.3  2003/02/12 17:17:39  jd
 * dirty ci for first transparent buttons
 *
 * Revision 1.2  2003/02/06 00:36:54  jd
 * split of grapefruit.cpp to population.cpp/h
 * addition of grapefruit.cpp/h
 * first use of buttons
 * some alternative fitnesses tests by JD
 *
 * Revision 1.1  2003/01/18 12:17:55  jd
 * merged genetics + SonGe files
 * first release with sounds
 * cleanup for cvs
 *
 * Revision 1.1  2002/12/16 23:25:52  jd
 * addition of graphutils, embryo of a small graph lib
 *
 */
