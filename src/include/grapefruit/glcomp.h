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
#warning glcomp.h should not be included as is. grapefruit.h should be included instead
#else  // GRAPEFRUIT_H_INC
#ifdef	    GRAPEFRUIT_H_GLOBINST
#define	    GLCOMP_H_INSTGLOB
#endif //   GRAPEFRUIT_H_GLOBINST
#endif // GRAPEFRUIT_H_INC

#ifndef GLCOMP_H_HEADER
#define GLCOMP_H_HEADER

#include <vector>

#include "SDL.h"

#include <matcalc/matcalc.h>

namespace grapefruit
{

#ifdef GLCOMP_H_INSTGLOB

    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_BLACK	  (0.0, 0.0, 0.0, 1.0);
    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_TRANSPBLACK (0.0, 0.0, 0.0, 0.0);
    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_WHITE	  (1.0, 1.0, 1.0, 1.0);

    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_RED	  (1.0, 0.0, 0.0, 1.0);
    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_GREEN	  (0.0, 1.0, 0.0, 1.0);
    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_BLUE	  (0.0, 0.0, 1.0, 1.0);
    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_YELLOW	  (1.0, 1.0, 0.0, 1.0);
    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_CYAN	  (0.0, 1.0, 1.0, 1.0);
    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_MAUVE	  (1.0, 0.0, 1.0, 1.0);

    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_DARKRED	  (0.5, 0.0, 0.0, 1.0);
    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_DARKGREEN	  (0.0, 0.5, 0.0, 1.0);
    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_DARKBLUE	  (0.0, 0.0, 0.5, 1.0);
    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_DARKYELLOW  (0.5, 0.5, 0.0, 1.0);
    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_DARKCYAN	  (0.0, 0.5, 0.5, 1.0);
    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_DARKMAUVE	  (0.5, 0.0, 0.5, 1.0);

    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_ORANGE	  (1.0, 5.0, 0.0, 1.0);

    GRAPEFRUIT_H_SCOPE GLint dl_circle;

#else // GLCOMP_H_INSTGLOB

    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_BLACK;
    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_TRANSPBLACK;
    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_WHITE;

    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_RED;
    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_GREEN;
    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_BLUE;
    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_YELLOW;
    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_CYAN;
    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_MAUVE;

    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_DARKRED;
    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_DARKGREEN;
    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_DARKBLUE;
    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_DARKYELLOW;
    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_DARKCYAN;
    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_DARKMAUVE;

    GRAPEFRUIT_H_SCOPE Vector4 GLRGBA_ORANGE;

    GRAPEFRUIT_H_SCOPE GLint dl_circle;

#endif // GLCOMP_H_INSTGLOB



    inline void init_circle (void)
    {
	dl_circle = glGenLists(1);
	glNewList (dl_circle, GL_COMPILE);
	    glNormal3f (0.0, 0.0, 0.0);	// a zero normal vector for all the following...
	    int i;
	    double a;
	    for (i=0 ; i<128 ; i++) {
		a = 2.0 * M_PI * i / 128.0;
		glVertex3f (cos (a), sin(a), 0.0);
	    }
	    glEnd ();
	glEndList ();
    }

    inline void render_circle (void)
    {   glCallList (dl_circle);
    }


}   // namespace grapefruit

#endif // GLCOMP_H_HEADER

