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


#include "grapefruit.h"

using namespace grapefruit ;

#include "glcomp.h"
////////#include "glvfont.h"
////////#include "tdstring.h"



namespace grapefruit
{

#define SMOOTHFONT   true	// JDJDJDJD this should be some global parameter read from some conf file

    TDString::TDString (const string &s, double h) : TDObj ()
    {
	// TDString::h = 2.0 * h / screen->h;
	TDString::h = h / 9.0;
	TDString::s = s;
	Vector3 v = compute_string_size (s, 0.5);
	sw = v.x, sh = v.y;
	vra = 0.0;
	angrot = 0.0;
	isgrabbable = true;
	color = GLRGBA_WHITE;

	// building the convex-envelope (here : a box)
	vv.push_back(Vector3(0.0, 0.0, 0.0));
	vv.push_back(Vector3(0.0, sh,  0.0));
	vv.push_back(Vector3(sw,  sh,  0.0));
	vv.push_back(Vector3(sw,  0.0, 0.0));
    }

    const string & TDString::gettdname (void)
    {	static string name ("tdstring");
	return name;
    }

    void TDString::update (const string &s)
    {
	TDString::s = s;
	Vector3 v = compute_string_size (s, 0.5);
	sw = v.x, sh = v.y;

	// building the convex-envelope (here : a box)
	vv.push_back(Vector3(0.0, 0.0, 0.0));
	vv.push_back(Vector3(0.0, sh,  0.0));
	vv.push_back(Vector3(sw,  sh,  0.0));
	vv.push_back(Vector3(sw,  0.0, 0.0));
    }
    
    void TDString::gotclicked (SDL_Event const &event)
    {	// vra = 2.0*M_PI / 80.0;
	if (event.type == SDL_MOUSEBUTTONUP) {
	    GLfloat x, y;
	    grapefruit::convert2Dto3D  (event.button.x, event.button.y, x, y);
	    if (x - pos.x > 0.0)
		vra = 2.0*M_PI / 75.0;
	    else
		vra = -2.0*M_PI / 75.0;
	}
    }

    void TDString::render (void)
    {	
	// JDJD bad cinematic code
	if (vra != 0.0) {
	    // rotate (Vector3 (0.0, 1.0, 0.0), vra);
	    Vector3 v(0.1, 0.8, 0.2);
	    v /= v.norm();
	    rotate (v, vra);
	    angrot += vra;

	    if ((angrot >= 6.0 * M_PI) || (angrot <= -6.0 * M_PI)) {
		vra = 0.0;
		angrot = 0.0;
	    }
	}
	// JDJD end of bad cinematic code*

	glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,    (GLfloat *) GLRGBA_BLACK );
	glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR,		    (GLfloat *) GLRGBA_BLACK );
	glMateriali  (GL_FRONT_AND_BACK, GL_SHININESS, 128); 
	glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION,		    (GLfloat *) color);

	glShadeModel(GL_FLAT);
	
	glPushMatrix();

	double hh = h;	// JDJDJDJD beuark
	//  double hh;
	//  if (pos.x + sw*h > 1.0)
	//      hh = (1.0-pos.x)/sw;
	//  else
	//      hh = h;

	glScalef (hh, hh, 1.0);
	glTranslatef (-0.5 * sw, -0.5 * sh, 0.0);

	if (SMOOTHFONT) {
	    glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
	    // glHint (GL_LINE_SMOOTH_HINT, GL_FASTEST);
	    glEnable (GL_POINT_SMOOTH);
	    glEnable (GL_LINE_SMOOTH);
	    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	    // glBlendFunc (GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);  // JDJD blend
	    // glBlendFunc (GL_ONE_MINUS_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);  // JDJD blend
	}
	glLineWidth (1.2);
	glPointSize (1.2);
	glDepthMask (GL_FALSE);
	render_string (s, 0.5);
	// glDisable(GL_LINE_SMOOTH);
	glDepthMask (GL_TRUE);
	glPopMatrix();
	glLineWidth (1);
	if (SMOOTHFONT) {
	    glDisable (GL_POINT_SMOOTH);
	    glDisable (GL_LINE_SMOOTH);
	}
	
    }
    void TDString::renderclickablezone (void)
    {
	double hh = h;	// JDJDJDJD beuark
	//  double hh;
	//  if (pos.x + sw*h > 1.0)
	//      hh = (1.0-pos.x)/sw;
	//  else
	//      hh = h;

	glScalef (hh, hh, 1.0);
	glTranslatef (-0.5 * sw, -0.5 * sh, 0.0);

	glBegin (GL_POLYGON);
	glVertex2f (0.0, 0.0);
	glVertex2f (0.0, sh);
	glVertex2f (sw,  sh);
	glVertex2f (sw,  0.0);
	glEnd ();
    }

    void TDString::proj_size (Vector3 &offset, Vector3 &size)
    {
	vector<Vector3>::iterator vi = vv.begin();
	Vector3 min, max,
		v;

	v = (*vi) + Vector3(-0.5 * sw, -0.5 * sh, 0.0);
	v.x *= h;
	v.y *= h;
	v.z *= 1.0;
	v = rm * v;

	min = v;
	max = v;

	vi++;

	for ( ; vi!=vv.end() ; vi++) {
	    v = (*vi) + Vector3(-0.5 * sw, -0.5 * sh, 0.0);
	    v.x *= h;
	    v.y *= h;
	    v.z *= 1.0;
	    v = rm * v;
	    //	v += Vector3(-0.5 * sw, -0.5 * sh, 0.0);
	    //	v.x = vi->x * h;
	    //	v.y = vi->y * h;
	    //	v.z = vi->z * 1.0;
	    //	v = rm * v;

	    if (v.x < min.x) min.x = v.x;
	    if (v.y < min.y) min.y = v.y;
	    if (v.z < min.z) min.z = v.z;
	    if (v.x > max.x) max.x = v.x;
	    if (v.y > max.y) max.y = v.y;
	    if (v.z > max.z) max.z = v.z;
	}
	offset = min;
	size = max - min;

	{   Vector3 aoffset, asize;
	    ::proj_size (	aoffset, asize, vv,
			Vector3 (-0.5 * sw, -0.5 * sh, 0.0),
			Vector3 (h, h, 1.0),
			rm
		      );

	    if ((fabs(aoffset.norm() - offset.norm()) > 0.00001)) {
		bzouzerr << " erreur de taille : " << aoffset.norm() - offset.norm()
			 << " / " << offset.norm()
		         << " = " << 100.0 * (aoffset.norm() - offset.norm())/ offset.norm() << " %" << endl ;
	    }
	}
    }

    GLfloat TDString::diameter (void)
    {	return sqrt(sw*sw + sh*sh) * h;
    }

};

