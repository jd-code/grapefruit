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

namespace grapefruit
{



// ------------------------- Scene ----------------------------------------------------------------
// 

Scene::Scene (void)
{
}

void Scene::render (void)
{
    list <TDObj *>::iterator li;
    for (li=td_displayed.begin() ; li!=td_displayed.end() ; li++) {
	TDObj & td = **li;
	glPushMatrix ();
	glTranslatef (td.pos.x, td.pos.y, td.pos.z);
	glMultMatrixf (&td.rm.m[0][0]);
	glScalef (td.scale, td.scale, td.scale);
	td.render ();
	glPopMatrix ();
    }
}


void Scene::renderclickablezone (void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();              // from grapefruit's redrawer

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
}

// ------------------------- View -----------------------------------------------------------------
// 

void View::show (void)
{
    if (id_displayed != view_displayed.end()) {
	bzouzerr << "attempt to show an already shown view ? skipped" << endl;
	return;
    }
    id_displayed = view_displayed.insert (view_displayed.end(), this);
}


void View::show (View const & v)
{
    if (id_displayed != view_displayed.end()) {
	bzouzerr << "attempt to show an already shown view ? skipped" << endl;
	return;
    }
    id_displayed = view_displayed.insert (v.id_displayed, this);
}


void View::show_above (View const & v)
{
    if (id_displayed != view_displayed.end()) {
	bzouzerr << "attempt to show an already shown view ? skipped" << endl;
	return;
    }
    list <View *>::iterator li = v.id_displayed;
    if (li != view_displayed.end())
	li++;
    id_displayed = view_displayed.insert (li, this);
}


void View::hide (void)
{
    if (id_displayed == view_displayed.end()) {
	bzouzerr << "attempt to hide an already hidden view ? skipped" << endl;
	return;
    }
    view_displayed.erase (id_displayed);
    id_displayed = view_displayed.end();
}


void View::activate (void)
{
    if (id_evented != view_evented.end()) {
	bzouzerr << "attempt to activate an already activated view ? skipped" << endl;
	return;
    }
    id_evented = view_evented.insert (view_evented.end(), this);
}


void View::activate (View const & v)
{
    if (id_evented != view_evented.end()) {
	bzouzerr << "attempt to activate an already activated view ? skipped" << endl;
	return;
    }
    id_evented = view_evented.insert (v.id_evented, this);
}


void View::activate_above (View const & v)
{
    if (id_evented != view_evented.end()) {
	bzouzerr << "attempt to activate an already activated view ? skipped" << endl;
	return;
    }
    list <View *>::iterator li = v.id_evented;
    if (li != view_evented.end())
	li++;
    id_evented = view_evented.insert (li, this);
}


void View::desactivate (void)
{
    if (id_evented == view_evented.end()) {
	bzouzerr << "attempt to desactivate an already desactivated view ? skipped" << endl;
	return;
    }
    view_evented.erase (id_evented);
    id_evented = view_evented.end();
}


void ViewOrtho::setsizes (GLint screen_x, GLint screen_y, GLsizei screen_w, GLsizei screen_h,
	       GLdouble scene_left,	GLdouble scene_right,
	       GLdouble scene_bottom,	GLdouble scene_top,  
	       GLdouble scene_near,	GLdouble scene_far)
{
    if ((screen_w == 0) || (screen_h == 0)) {
	bzouzerr << "uncomputable ratio : some size is null ! screen_w=" << screen_w << " screen_h=" << screen_h << endl;
	return;
    }

    GLdouble scene_w = scene_right - scene_left,
	     scene_h = scene_top - scene_bottom;

    if ((scene_w == 0) || (scene_h == 0)) {
	bzouzerr << "uncomputable ratio : some size is null ! scene_w=" << scene_w << " scene_h=" << scene_h << endl;
	return;
    }

    x = screen_x,  y = screen_y,  w = screen_w,  h = screen_h;

    GLdouble screen_ratio = (GLdouble)screen_w / (GLdouble)screen_h,
	     scene_ratio = scene_w / scene_h;

    if (screen_ratio > scene_ratio) {	// we adjust according to the scene height
	GLdouble ratio = screen_h / scene_h;
	    left = scene_left + (scene_w/2.0) - (0.5*screen_w/ratio);
	   right = scene_left + (scene_w/2.0) + (0.5*screen_w/ratio);
	  bottom = scene_bottom;
	     top = scene_top;
	near_val = scene_near;
	 far_val = scene_far;
    } else {				// we adjust according to the scene width
	GLdouble ratio = screen_w / scene_w;
	    left = scene_left;
	   right = scene_right;
	  bottom = scene_bottom + (scene_h/2.0) - (0.5*screen_h/ratio);
	     top = scene_bottom + (scene_h/2.0) + (0.5*screen_h/ratio);
	near_val = scene_near;
	 far_val = scene_far;
    }
}

void ViewOrtho::render (void)
{
    glViewport (x, y, w, h);

    glMatrixMode ( GL_PROJECTION ); // from grapefruit's redrawer
    glPushMatrix();                 // from grapefruit's redrawer
    glLoadIdentity ();              // from grapefruit's redrawer
    glOrtho (left, right, bottom, top, near_val, far_val);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();              // from grapefruit's redrawer

    scene.render();

    glPopMatrix ();                 // from grapefruit's redrawer
}

EventCB * ViewOrtho::renderclickablezone (int xe, int ye)
{
    int i;

    GLuint selectBuf[512];
    GLint hits;

    glSelectBuffer (512, selectBuf);
    glRenderMode (GL_SELECT);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();

    GLfloat x, y, dx, dy;
    convert2Dto3D (xe,ye , x,y , dx,dy);

bzouzerr << " xe=" << xe << " ye=" << ye
	 << "   x=" << x << " y=" << y << "   dx=" << dx << " dy=" << dy
	 << endl;
    
    glOrtho (x, x+dx, y, y+dy, near_val, far_val);         // we use an orthogonal projection

    glInitNames();
    glPushName(GRTD_UNDEF);

    scene.renderclickablezone();
    
    hits = glRenderMode (GL_RENDER);

bzouzerr << "hits=" << hits << endl ;
    
    if (hits == 0)
	return NULL;

    // JDJDJDJD le cas hits == -1 reste à traiter !

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

    return scene.td_evented [name];
}


void ViewOrtho::convert2Dto3D (int xe, int ye, GLfloat &x, GLfloat &y, GLfloat &dx, GLfloat &dy)
{
    dx = (right-left)/w;
    dy = (top-bottom)/h;
     x = xe * dx + left;
     y = top - ye * dy;	    // JDJDJDJD a approfondir !!!
    
    ////    double windows_h = (GLfloat) screen->h / (GLfloat) screen->w;

    ////    if (windows_h > 1.0) {
    ////        dx = 2.0 / (GLfloat) screen->w ;
    ////        dy = -2.0 * windows_h / (GLfloat) screen->h ;

    ////        // x = 2.0 * (GLfloat) xe / (GLfloat) screen->w - 1.0 ;
    ////        // y = -2.0 * windows_h * (GLfloat) ye / (GLfloat) screen->h + windows_h ;
    ////        x = (GLfloat) xe * dx - 1.0 ;
    ////        y = (GLfloat) ye * dy + windows_h ;
    ////    } else {
    ////        windows_h = 1.0 / windows_h;
    ////        dx = 2.0 * windows_h / (GLfloat) screen->w ;
    ////        dy = -2.0 / (GLfloat) screen->h ;
    ////        // x = 2.0 * windows_h * (GLfloat) xe / (GLfloat) screen->w - windows_h ;
    ////        // y = -2.0 * (GLfloat) ye / (GLfloat) screen->h + 1.0 ;
    ////        x = (GLfloat) xe * dx - windows_h ;
    ////        y = (GLfloat) ye * dy + 1.0 ;
    ////    }
}

// ------------------------- TDObj ----------------------------------------------------------------
// 

TDObj::~TDObj (void)
{   
    if (id_displayed != td_notdispd)
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

const string & TDObj::gettdname (void)
{   static string name ("undefined");
    return name;
}

void TDObj::show (void)
{
    if (id_displayed == td_notdispd) {
	id_displayed = td_displayed.insert (td_displayed.end(), this);
	GLfloat diam = diameter ();
	list<TDObj *>::iterator li;
    int i=0;
	for (li = td_displayed.begin() ; li != id_displayed ; li++, i++)    // JDJDJDJD we may reach END !!!!!
	    (*li)->pos.z -= diam;
	// pos.z = -1.0 + diam / 2.0;	    // the initial top-z value should be tunable JDJDJDJD
	pos.z = -15.0 - diam / 2.0;	    // the initial top-z value should be tunable JDJDJDJD
    } else
	bzouzerr << "called when already displayed ?" << endl ;
}

void TDObj::show (TDObj const & td)
{
    if (id_displayed == td_notdispd) {
	list<TDObj *>::iterator  ni = td.id_displayed;
	ni ++;
	id_displayed = td_displayed.insert (ni, this);

	GLfloat diam = diameter ();
	list<TDObj *>::iterator li;
	for (li = td_displayed.begin() ; li != id_displayed ; li++)
	    (*li)->pos.z -= diam;
	pos.z = td.pos.z + diam / 2.0;	// just on top of td
    } else
	bzouzerr << "called when already displayed ?" << endl ;
}

void TDObj::hide (GLfloat diam)
{  
    // we move all TDs from us to bottom  closer to top the size of our diameter
    list<TDObj *>::iterator li;
    for (li = td_displayed.begin() ; li != id_displayed ; li++)
	(*li)->pos.z += diam;
    td_displayed.erase (id_displayed);
    id_displayed = td_notdispd;
}

void TDObj::hide (void)
{  
    if (id_displayed != td_notdispd) {
	hide (diameter ());
    } else
	bzouzerr << "called when not already displayed ?" << endl ;
}

void TDObj::hideindestructor (void)
{  
    if (id_displayed != td_notdispd) {
	if (id_displayed == td_displayed.begin()) {
	    hide (0.0); // we're at the bottom of the list, diameter doesn't matter
	    return;
	}

	list<TDObj *>::iterator pi = id_displayed;
	pi--;

	hide (  2.0*(pos.z - (*pi)->pos.z) - (*pi)->diameter()  );
    } else
	bzouzerr << "called when not already displayed ?" << endl ;
}

void TDObj::activate (bool isglobalactive)
{
    TDObj::isglobalactive = isglobalactive;
    if (id_evented == GRTD_UNDEF) {
	id_evented = td_evented.size();
	td_evented.push_back (this);
     } else
	bzouzerr << "called when already displayed ?" << endl ;
}

void TDObj::desactivate (void)
{  
    if (id_evented != GRTD_UNDEF) {
	size_t size = td_evented.size();
	if (id_evented != size - 1) {	// we move the last TD from the vector to the current position
	// if (size > 1)  	// we move the last TD from the vector to the current position
	    TDObj * last = td_evented [size - 1];
	    td_evented [id_evented] = last;
	    last->id_evented = id_evented;
	}
	td_evented.pop_back();
	id_evented = GRTD_UNDEF;
    } else
	bzouzerr << "called when not already displayed ?" << endl ;
}

void TDObj::rotate (Vector3 const & v, double angle)
{
     rm = rm * Matrix4 (v, angle);
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




// ------------------------- TDCompound -----------------------------------------------------------
// 

TDCompound::~TDCompound (void)
{
    if (id_displayed != td_notdispd)
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

const string & TDCompound::gettdname (void)
{   static string name ("tdcompound");
    return name;
}


// ------------------------- global TDObj methods -------------------------------------------------
// 

////	void OldTDrender (void)
////	{
////	    Mvmt::hippity ();		// we take care of subscribed Mvmt
////	
////	    double windows_h = (GLfloat) screen->h / (GLfloat) screen->w;
////	    glMatrixMode ( GL_PROJECTION ); // from grapefruit's redrawer
////	    glPushMatrix();                 // from grapefruit's redrawer
////	    glLoadIdentity ();              // from grapefruit's redrawer
////	
////	    if (windows_h > 1.0)                                            // the screen is taller than wide (??)
////		glOrtho (-1.0, 1.0, -windows_h, windows_h, 15.0, 60.0);     // we use an adjusted orthogonal projection
////		// glFrustum (-1.0, 1.0, -windows_h, windows_h, 5.0, 60.0);     // we use an adjusted orthogonal projection
////	    else {                                                          // the screen is wider than tall
////		windows_h = 1.0 / windows_h;
////		//glOrtho (-windows_h, windows_h, -1.0, 1.0, 600.0, 0.0);     // we use an orthogonal projection
////		glOrtho (-windows_h, windows_h, -1.0, 1.0, 15.0, 60.0);     // we use an orthogonal projection
////		// glFrustum (-windows_h, windows_h, -1.0, 1.0, 5.0, 60.0);     // we use an orthogonal projection
////	    }
////	
////	
////	    glMatrixMode(GL_MODELVIEW);
////	    glLoadIdentity ();              // from grapefruit's redrawer
////	
////	    list <TDObj *>::iterator li;
////	    for (li=td_displayed.begin() ; li!=td_displayed.end() ; li++) {
////		TDObj & td = **li;
////		glPushMatrix ();
////		glTranslatef (td.pos.x, td.pos.y, td.pos.z);
////		glMultMatrixf (&td.rm.m[0][0]);
////		glScalef (td.scale, td.scale, td.scale);
////		td.render ();
////		glPopMatrix ();
////	    }
////	
////	    glPopMatrix ();                 // from grapefruit's redrawer
////	}


void TDrender (void)
{
    Mvmt::hippity ();		// we take care of subscribed Mvmt

    list <View *>::iterator li;

    for (li=view_displayed.begin() ; li!=view_displayed.end() ; li++)
	(*li)->render();
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
    list <View *>::iterator li;
    EventCB * ev = NULL;

    for (li=view_evented.begin() ; li!=view_evented.end() ; li++) {
	ev = (*li)->renderclickablezone (xe, ye);
	if (ev != NULL)
	    return (ev);
    }
    return NULL;
}

// ------------------------- ACDump_td_displayed --------------------------------------------------
// 

void ACDump_td_displayed::doit (void)
{   
    list <View *>::iterator lj;
    list <TDObj *>::iterator li;

    bool thereisaprevious = false;
    GLfloat zprev = 0.0;
    
    cerr << "--------------------------dump of td_displayed------------------------------" << endl ;
    for (lj=view_displayed.begin() ; lj!=view_displayed.end() ; lj++) {
	cerr << "view :" << endl ;
	list <TDObj *> &td_displayed = (*lj)->scene.td_displayed;
	for (li=td_displayed.begin() ; li!=td_displayed.end() ; li++) {
	    cerr << (*li)->pos << " diam =" << setw (10) << (*li)->diameter() << " " << (*li)->gettdname();
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
	cerr << "------------- " << td_displayed.size() << " elements" << endl << endl;
    }
}


string ACDump_td_displayed::getacname (void)
{   string name("ACDump_td_displayed");
    return name;
}
 


// ------------------------- ACScramble_td_displayed ----------------------------------------------
// 

void ACScramble_td_displayed::doit (void)
{
    list <View *>::iterator lj;

    for (lj=view_displayed.begin() ; lj!=view_displayed.end() ; lj++) {
	list <TDObj *> &td_displayed = (*lj)->scene.td_displayed;

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
}


string ACScramble_td_displayed::getacname (void)
{   string name("ACScramble_td_displayed");
    return name;
}

}   // grapefruit namespace's end...

