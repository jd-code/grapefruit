/* 
 * $Id: td.cpp,v 1.17 2004/05/03 22:02:50 jd Exp $
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

namespace grapefruit
{

TDObj::~TDObj (void)
{   
    if (id_displayed != td_displayed.end())
	hideindestructor ();

    if (mouseflyover == this)	// JDJDJDJD maybe we should check that mousebuttonfocus and ownergrabmouse need the same treatment ?
	mouseflyover = NULL;
    
    if (id_evented != GRTD_UNDEF)
	desactivate ();
    if (ptdparent != NULL) {
	bzouzerr << "WARNING - we have here some TDObj that is destroyed while still in some parent's list" << endl ;
	ptdparent->erase (id_ltdparent);
    }
}

void TDObj::show (void)
{
    if (id_displayed == td_displayed.end()) {
	td_displayed.push_back (this);
	id_displayed = td_displayed.end();
	id_displayed --;
	GLfloat diam = diameter ();
	list<TDObj *>::iterator li;
    int i=0;
	for (li = td_displayed.begin() ; li != id_displayed ; li++, i++)
	    (*li)->pos.z -= diam;
	// pos.z = -1.0 + diam / 2.0;	    // the initial top-z value should be tunable JDJDJDJD
	pos.z = -15.0 - diam / 2.0;	    // the initial top-z value should be tunable JDJDJDJD
    } else
	bzouzerr << "TDObj::show : called when already displayed ?" << endl ;
}

void TDObj::show (TDObj const & td)
{
    if (id_displayed == td_displayed.end()) {
	list<TDObj *>::iterator  ni = td.id_displayed;
	ni ++;
	id_displayed = td_displayed.insert (ni, this);

	GLfloat diam = diameter ();
	list<TDObj *>::iterator li;
	for (li = td_displayed.begin() ; li != id_displayed ; li++)
	    (*li)->pos.z -= diam;
	pos.z = td.pos.z + diam / 2.0;	// just on top of td
    } else
	bzouzerr << "TDObj::show : called when already displayed ?" << endl ;
}

void TDObj::hide (GLfloat diam)
{  
    // we move all TDs from us to bottom  closer to top the size of our diameter
    list<TDObj *>::iterator li;
    for (li = td_displayed.begin() ; li != id_displayed ; li++)
	(*li)->pos.z += diam;
    td_displayed.erase (id_displayed);
    id_displayed = td_displayed.end();
}

void TDObj::hide (void)
{  
    if (id_displayed != td_displayed.end()) {
	hide (diameter ());
    } else
	bzouzerr << "TDObj::hide : called when not already displayed ?" << endl ;
}

void TDObj::hideindestructor (void)
{  
    if (id_displayed != td_displayed.end()) {
	if (id_displayed == td_displayed.begin()) {
	    hide (0.0); // we're at the bottom of the list, diameter doesn't matter
	    return;
	}

	list<TDObj *>::iterator pi = id_displayed;
	pi--;

	hide (  2.0*(pos.z - (*pi)->pos.z) - (*pi)->diameter()  );
    } else
	bzouzerr << "TDObj::hide : called when not already displayed ?" << endl ;
}

void TDObj::activate (bool isglobalactive)
{
    TDObj::isglobalactive = isglobalactive;
    if (id_evented == GRTD_UNDEF) {
	id_evented = td_evented.size();
	td_evented.push_back (this);
     } else
	bzouzerr << "TDObj::activate : called when already displayed ?" << endl ;
}

void TDObj::desactivate (void)
{  
    if (id_evented != GRTD_UNDEF) {
	size_t size = td_evented.size();
	if (id_evented != size - 1) {	// we move the last TD from the vector to the current position
	// if (size > 1) {	// we move the last TD from the vector to the current position
	    TDObj * last = td_evented [size - 1];
	    td_evented [id_evented] = last;
	    last->id_evented = id_evented;
	}
	td_evented.pop_back();
	id_evented = GRTD_UNDEF;
    } else
	bzouzerr << "TDObj::desactivate : called when not already displayed ?" << endl ;
}

void TDObj::rotate (Vector3 const & v, double angle)
{
     rm = rm * Matrix4 (v, angle);
}


// ------------------------- TDCompound -----------------------------------------------------------
// 

TDCompound::~TDCompound (void)
{
    if (id_displayed != td_displayed.end())
	hideindestructor ();

    if (id_evented != GRTD_UNDEF)
	desactivate ();

    list <TDObj*> :: iterator li;

    for (li=ltd.begin() ; li!=ltd.end() ; li++)
        (*li)->ptdparent = NULL;
}


bool TDCompound::push_back (TDObj & td)
{
    if (td.ptdparent == NULL) {
        ltd.push_back (&td);
        td.ptdparent = this;
	td.id_ltdparent = --ltd.end();
        return true; 
    } else {
	bzouzerr << "WARNING - we have here a TD that is already registered to some other parent,"
		 << "who wants to register to some (other ?) parent" << endl ;
        return false;   
    }
}       

bool TDCompound::erase (list<TDObj*>::iterator id_ltdparent)
{
    TDObj & td = **id_ltdparent;

    if (td.ptdparent != this) {	    // JDJD this is a rather nasty test - could well be supressed for performance ?
	bzouzerr << "ERROR - we have here some TDObj that want to be erased from a parent that's not his ! - skipped" << endl ;
	return false;
    }

    if (td.id_evented != GRTD_UNDEF)
	td.desactivate();

    td.ptdparent = NULL;
    ltd.erase (id_ltdparent);

    return true;
}

void TDCompound::render (void)
{                       
    list <TDObj*> :: iterator li;
    for (li=ltd.begin() ; li!=ltd.end() ; li++) {
        TDObj & td = **li;
        glPushMatrix ();
        glTranslatef (td.pos.x, td.pos.y, td.pos.z);
        glMultMatrixf (&td.rm.m[0][0]);
        glScalef (td.scale, td.scale, td.scale);
        td.render ();
        glPopMatrix ();
    }
}

void TDCompound::renderclickablezone (void)
{
    list <TDObj*> :: iterator li;

    for (li=ltd.begin() ; li!=ltd.end() ; li++) {
        TDObj & td = **li;
        glPushMatrix ();
        glTranslatef (td.pos.x, td.pos.y, td.pos.z);
        glMultMatrixf (&td.rm.m[0][0]);
        glScalef (td.scale, td.scale, td.scale);
        glLoadName (td.id_evented);
        td.renderclickablezone ();
        glPopMatrix ();
    }
}

void TDCompound::activate (bool isglobalactive)
{
    TDObj::activate (isglobalactive);

    list <TDObj*> :: iterator li;
    for (li=ltd.begin() ; li!=ltd.end() ; li++)
        (*li)->activate (false);
}

void TDCompound::desactivate ()
{
    list <TDObj*> :: iterator li;

    TDObj::desactivate ();
    
    for (li=ltd.begin() ; li!=ltd.end() ; li++)
        (*li)->desactivate ();
}


// ------------------------- global TDObj methods -------------------------------------------------
// 

void TDrender (void)
{
    list <TDObj *>::iterator li;

    double windows_h = (GLfloat) screen->h / (GLfloat) screen->w;
    glMatrixMode ( GL_PROJECTION ); // from grapefruit's redrawer
    glPushMatrix();                 // from grapefruit's redrawer
    glLoadIdentity ();              // from grapefruit's redrawer

    if (windows_h > 1.0)                                            // the screen is taller than wide (??)
	glOrtho (-1.0, 1.0, -windows_h, windows_h, 15.0, 60.0);     // we use an adjusted orthogonal projection
	// glFrustum (-1.0, 1.0, -windows_h, windows_h, 5.0, 60.0);     // we use an adjusted orthogonal projection
    else {                                                          // the screen is wider than tall
	windows_h = 1.0 / windows_h;
	//glOrtho (-windows_h, windows_h, -1.0, 1.0, 600.0, 0.0);     // we use an orthogonal projection
	glOrtho (-windows_h, windows_h, -1.0, 1.0, 15.0, 60.0);     // we use an orthogonal projection
	// glFrustum (-windows_h, windows_h, -1.0, 1.0, 5.0, 60.0);     // we use an orthogonal projection
    }

glMatrixMode(GL_MODELVIEW);
glLoadIdentity ();              // from grapefruit's redrawer
    
    for (li=td_displayed.begin() ; li!=td_displayed.end() ; li++) {
	TDObj & td = **li;
	glPushMatrix ();
	glTranslatef (td.pos.x, td.pos.y, td.pos.z);
	glMultMatrixf (&td.rm.m[0][0]);
	glScalef (td.scale, td.scale, td.scale);
	td.render ();
	glPopMatrix ();
    }

    glPopMatrix ();                 // from grapefruit's redrawer
}


// void convert2Dto3D (int xe, int ye, GLfloat &x, GLfloat &y, GLfloat &dx = convert2Dto3Ddx, GLfloat &dy = convert2Dto3Ddy)
void convert2Dto3D (int xe, int ye, GLfloat &x, GLfloat &y, GLfloat &dx, GLfloat &dy)
{
    double windows_h = (GLfloat) screen->h / (GLfloat) screen->w;

    if (windows_h > 1.0) {
	dx = 2.0 / (GLfloat) screen->w ;
	dy = -2.0 * windows_h / (GLfloat) screen->h ;

	// x = 2.0 * (GLfloat) xe / (GLfloat) screen->w - 1.0 ;
	// y = -2.0 * windows_h * (GLfloat) ye / (GLfloat) screen->h + windows_h ;
	x = (GLfloat) xe * dx - 1.0 ;
	y = (GLfloat) ye * dy + windows_h ;
    } else {
	windows_h = 1.0 / windows_h;
	dx = 2.0 * windows_h / (GLfloat) screen->w ;
	dy = -2.0 / (GLfloat) screen->h ;
	// x = 2.0 * windows_h * (GLfloat) xe / (GLfloat) screen->w - windows_h ;
	// y = -2.0 * (GLfloat) ye / (GLfloat) screen->h + 1.0 ;
	x = (GLfloat) xe * dx - windows_h ;
	y = (GLfloat) ye * dy + 1.0 ;
    }
}

EventCB * TDrenderclickablezone (int xe, int ye)
{
    GLuint selectBuf[512];
    GLint hits;

    glSelectBuffer (512, selectBuf);
    glRenderMode (GL_SELECT);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();

    GLfloat x, y, dx, dy;
    convert2Dto3D (xe,ye , x,y , dx,dy);

    glOrtho (x, x+dx, y, y+dy, 15.0, 60.0);         // we use an orthogonal projection

    glInitNames();
    glPushName(GRTD_UNDEF);

glMatrixMode(GL_MODELVIEW);
glLoadIdentity ();              // from grapefruit's redrawer

//STUPIDROT glMultMatrixf (&mVue.m[0][0]);

    vector <TDObj *>::iterator li;
    int i;
    for (li=td_evented.begin(), i=0 ; li!=td_evented.end() ; li++, i++) {
	TDObj & td = **li;
	if (td.isglobalactive) { 
	    glPushMatrix ();
	    glTranslatef (td.pos.x, td.pos.y, td.pos.z);
	    glMultMatrixf (&td.rm.m[0][0]);
	    glScalef (td.scale, td.scale, td.scale);
	    glLoadName (i);
	    td.renderclickablezone ();
	    glPopMatrix ();
	}
    }

    hits = glRenderMode (GL_RENDER);

    if (hits == 0)
	return NULL;

    // JDJDJDJD le cas hits == -1 reste � traiter !

    GLuint nbnames,
	    *ptr = selectBuf;
    unsigned int maxz = *(ptr + 1);
    unsigned int name = *(ptr + 3);

    for (i = 0; i < hits; i++) {           /* for each hit */
	nbnames = *ptr++;
	if (*ptr < maxz) {
	    maxz = *ptr++;
	    ptr++;
	    name = *ptr;
	    ptr += nbnames;
	} else {
	    ptr += nbnames + 2;
	}
    }

    return td_evented [name];
}

// TDObj * lastselected = NULL;

//  void TDObj::gotselected (void)
//  {   if (lastselected != NULL)
//  	lastselected->gotdeselected ();
//      lastselected = this;
//      isselected = true;
//  }
//  
//  void TDObj::gotdeselected (void)
//  {
//      lastselected = NULL;
//      isselected = false;
//  }

void TDObj::grabberchange (SDL_Event const & event)
{
    GLfloat x, y, dx, dy;
    convert2Dto3D (0,0 , x,y , dx,dy);
    if (isgrabbable) {
	pos.x += event.motion.xrel * dx;
	pos.y += event.motion.yrel * dy;
    }
}

int TDObj::startgrabber (SDL_Event const & event)
{   
    posxorigin = pos.x;
    posyorigin = pos.y;
    return MGrabber::startgrabber (event);  // JDJDJDJD la souris disparait au click, ce serait mieux de ne la faire disparaitre
					    // que si l'on est grabbable
}

int TDObj::endgrabber (SDL_Event const & event)
{   
    if (abs (sdx) + abs (sdy) < 5) {
	if (isgrabbable) {
	    pos.x = posxorigin;
	    pos.y = posyorigin;
	}
	//  if (isselected)
	//      gotdeselected ();
	//  else
	//      gotselected ();
	int r = MGrabber::endgrabber (event);
	gotclicked (event); // JDJDJDJD ce serait bien de tester si le relachement a lieu sur l'objet lui-meme pour valider les clicks
	return r;
    }
    return MGrabber::endgrabber (event);
}

void ACDump_td_displayed::doit (void)
{   list <TDObj *>::iterator li;

    bool thereisaprevious = false;
    GLfloat zprev = 0.0;
    
    cerr << "--------------------------dump of td_displayed------------------------------" << endl ;
    for (li=td_displayed.begin() ; li!=td_displayed.end() ; li++) {
	cerr << (*li)->pos << " diam =" << setw (10) << (*li)->diameter() ;
	if (thereisaprevious) {
	    if ((*li)->pos.z < zprev)
		cerr << " order mismatch !" << endl ;
	    else if ((*li)->pos.z == zprev)
		cerr << " z are equals !" << endl ;
	    else
		cerr << endl;
	} else {
	    thereisaprevious = true;
	    cerr << endl;
	}
	zprev = (*li)->pos.z;
    }
    cerr << "------------- " << td_displayed.size() << " elements" << endl ;
}

//! returns a random integer in the set :  [1;max]      [CB]
inline int randint ( int max) { return 1+(int) ((double)max * rand()/(RAND_MAX+1.0));}

void ACScramble_td_displayed::doit (void)
{
    int size = td_displayed.size();
    if (size <= 1)
	return ;
    int r = (int) ((double)size * rand()/(RAND_MAX+1.0));
    int i;
    list <TDObj *>::iterator li;
    for (i=0, li=td_displayed.begin() ; (i<r) && (li!=td_displayed.end()) ; li++, i++);
    TDObj *p = *li;
    p->hide();
    size --;
    r = (int) ((double)size * rand()/(RAND_MAX+1.0));
    for (i=0, li=td_displayed.begin() ; (i<r) && (li!=td_displayed.end()) ; li++, i++);
    p->show(**li);
}

}   // grapefruit namespace's end...

/*
 * $Log: td.cpp,v $
 * Revision 1.17  2004/05/03 22:02:50  jd
 * big merge for mingw/msys and cross-mingw
 *
 * Revision 1.16  2004/03/13 16:36:05  cristelle
 * ONESHOT staytype added and small adjustments in deletions vs actions ordering
 *
 * Revision 1.15  2004/03/03 23:13:10  jd
 * major changes ! td_displayed ordering debbugged, TDrender cleaned accordingly,
 * clickablezone rendering idem
 *
 * Revision 1.14  2004/03/02 00:04:22  jd
 * "pure virtual method called" bug corrected for TDCompound
 *
 * Revision 1.13  2004/03/01 18:05:01  jd
 * corrected "pure virtual method called" bug (paradigm : never call a virtual function in destructors)
 *
 * Revision 1.12  2004/02/19 00:39:12  jd
 * added automated z-location management
 *
 * Revision 1.11  2004/01/13 23:49:02  jd
 * corrected TDCompound desactivation's crash bug
 *
 * Revision 1.10  2003/10/24 21:36:09  jd
 * ultimate cleaning for double checking compound members destroyin-process
 *
 * Revision 1.9  2003/10/24 20:57:04  jd
 * librarization of TDCompound
 * debugging of TDObj hide and desactivate
 *
 * Revision 1.8  2003/10/23 13:48:58  cristelle
 * TDObj has been tuned for TDCompound
 *
 * Revision 1.7  2003/10/20 13:30:05  jd
 * added TD size related methods
 * numerous TDs updates
 *
 * Revision 1.6  2003/09/29 22:28:36  jd
 * corrected isometric projection for TDs (still needed for Agrumes)
 *
 * Revision 1.5  2003/09/25 13:15:04  jd
 * small tunings with grabbing and TDs
 *
 * Revision 1.4  2003/09/17 20:56:58  jd
 * updates for gcc-3.3.1
 * MGrabber object added (maybe temporarily)
 *
 * Revision 1.3  2003/09/15 15:54:37  jd
 * attempt to get rid of first move in grabbing/dragging
 *
 * Revision 1.2  2003/09/11 23:49:06  jd
 * corrected slow-motion bug in various grabbing code
 * tuned big-motion squiz handling specific to win32 platforms
 * added grabbing code for TDs (first release)
 * added config.h
 *
 * Revision 1.1  2003/09/10 13:47:39  jd
 * added first raw versions of TDs
 *
 *
 */
