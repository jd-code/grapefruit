

#include "grapefruit.h"
#include "glcomp.h"

#define SHOWBBOX false
using namespace grapefruit ;

namespace grapefruit
{

// ------------------------- TDMenuItem -----------------------------------------------------------
//

void TDMenuItem::gotclicked (SDL_Event const &event) 
{
    if (ptdmenu == NULL) {
	bzouzerr << " we have here a ptdmenu that is NULL ????" << endl ;
	paclick->doit();
    }
    else switch (ptdmenu->stayType) {
	case TDMenu::ONECLICK:
	    ptdmenu->desactivate ();
	    ptdmenu->hide ();
	    paclick->doit();
	    break;

	case TDMenu::ONESHOT:
	    ptdmenu->desactivate ();
	    ptdmenu->hide ();
	    paclick->doit();
	    delete (ptdmenu);
	    // JDJDJDJD ceci est mis en stand-by jusqu'à ce qu'on en ait de nouveau besoin
	    // committodelete (ptdmenu);
	    break;

	default:
	    paclick->doit();
    }
}
//void TDMenuItem::gotclicked (SDL_Event const &event) 
//{
//    if (ptdmenu != NULL) {
//	if (ptdmenu->stayType == TDMenu::ONECLICK) {
//	    ptdmenu->desactivate ();
//	    ptdmenu->hide ();
//	}
//	if (ptdmenu->stayType == TDMenu::ONESHOT) {
//	    ptdmenu->desactivate ();
//	    ptdmenu->hide ();
//	    delete (ptdmenu);
//	}
//    } else
//	bzouzerr << " we have here a ptdmenu that is NULL ????" << endl ;
//    paclick->doit();
//}

// ------------------------- TDMenu ---------------------------------------------------------------
//
//

TDMenu::TDMenu (const string &s, MenuViewType viewType, MenuStayType stayType, int numcol) : TDCompound ()
{
    TDMenu::s = s;
    TDMenu::viewType = viewType;
    TDMenu::stayType = stayType;
    TDMenu::numcol = numcol;
    nbelem = 0;
    xpadding = 0.03;
    ypadding = 0.015;
    vv.insert (vv.end(), 4, Vector3());
    // JDJDJDJD to be removed iif the previous line is ok...
    //	vv.push_back(Vector3(0.0, 0.0, 0.0));
    //	vv.push_back(Vector3(0.0, 0.0, 0.0));
    //	vv.push_back(Vector3(0.0, 0.0, 0.0));
    //	vv.push_back(Vector3(0.0, 0.0, 0.0));
}

TDMenu::TDMenu (const string &s) : TDCompound ()
{
    TDMenu::s = s;
    TDMenu::viewType = INLINE;
    TDMenu::stayType = STICKY;
    numcol = 1;
    nbelem = 0;
    xpadding = 0.03;
    ypadding = 0.015;
    vv.insert (vv.end(), 4, Vector3());
}

TDMenu::TDMenu () : TDCompound ()
{
    TDMenu::s = "--no_name--";
    TDMenu::viewType = INLINE;
    TDMenu::stayType = STICKY;
    numcol = 1;
    nbelem = 0;
    xpadding = 0.03;
    ypadding = 0.015;
    vv.insert (vv.end(), 4, Vector3());
}


void TDMenu::render (void)
{
    TDCompound::render();
    if (SHOW_CONVEX_ENVELOPP) {
	glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,    (GLfloat *) GLRGBA_BLACK );
	glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR,		    (GLfloat *) GLRGBA_BLACK );
	glMateriali  (GL_FRONT_AND_BACK, GL_SHININESS, 128); 
	glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION,		    (GLfloat *) GLRGBA_CYAN);
	glShadeModel (GL_FLAT);

	glEnable (GL_POINT_SMOOTH);
	glEnable (GL_LINE_SMOOTH);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE);
	glLineWidth (1.0);
	glPointSize (1.0);
	glDepthMask (GL_FALSE);

	vector<Vector3>::iterator vi;
	int i;

	glBegin (GL_LINE_LOOP);
	for (vi=vv.begin(),i=0 ; (vi!=vv.end()) && (i<4) ; vi++, i++)
	    glVertex3f (vi->x, vi->y, vi->z);
	glEnd ();

	glDepthMask (GL_TRUE);
	glDisable (GL_LINE_SMOOTH);
	glBlendFunc (GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);
    }
}

void TDMenu::compute_all_pos () 
{
    Vector3 locoffset, locsize,  newpos;
    list< TDObj * >::iterator ltdi;
    vector<GLfloat>::iterator maxsizesi;
    int i,j;
   
    sizesum.x = 0; sizesum.y = 0;
    for (maxsizesi=maxlinesizes.begin() ; maxsizesi!=maxlinesizes.end() ; maxsizesi++)
	sizesum.y += *maxsizesi;
    sizesum.y += (maxlinesizes.size() -1)* ypadding;
    for (maxsizesi=maxcolsizes.begin() ; maxsizesi!=maxcolsizes.end() ; maxsizesi++)
	sizesum.x += *maxsizesi;
    sizesum.x += (maxcolsizes.size() -1)* xpadding;
    
    
    for (ltdi=TDCompound::ltd.begin(), i=0 ; (ltdi!=TDCompound::ltd.end())  ;  i++, ltdi++) {
	newpos.x = -sizesum.x / 2.0 ; newpos.y = sizesum.y / 2.0;
	(*ltdi)->proj_size (locoffset, locsize);
	for (j=0; j< i%numcol ; j ++) 
	    newpos.x += maxcolsizes [j] + xpadding;
	// newpos.x += locsize.x / 2.0; //CBCBCB is it always this way?
	newpos.x -= locoffset.x; // JDJDJDJD is this valid ?
	for (j=0; j<= i/numcol ; j++)
	    newpos.y -= maxlinesizes [j] + ypadding;
	//newpos.y -= locsize.y/2.0;   //CBCBCB is it always this way?
	newpos.y -= locoffset.y - ypadding; // JDJDJDJD is this valid ?
	(*ltdi)->setpos (newpos); 
    }
   
    vv [0].x = - sizesum.x / 2.0; vv [1].x = vv [0].x;
    vv [0].y = - sizesum.y / 2.0; vv [3].y = vv [0].y;
    vv [1].y =   sizesum.y / 2.0; vv [2].y = vv [1].y;
    vv [2].x =   sizesum.x / 2.0; vv [3].x = vv [2].x;


}

void TDMenu::change_viewtype (MenuViewType newviewtype, int numcol)
{
    list< TDObj * >::iterator ltdi;
    Vector3 locoffset, locsize;
    int loccolnum, linenum;
    int i;
    if ((viewType != newviewtype) || ((viewType==INARRAY) && (TDMenu::numcol!= numcol))) {
	if (newviewtype == INLINE) 
	    TDMenu::numcol = nbelem+1;
	else	
	    TDMenu::numcol = numcol;

	if (!maxcolsizes.empty())
	    maxcolsizes.erase(maxcolsizes.begin(), maxcolsizes.end());
	if (!maxlinesizes.empty())
	    maxlinesizes.erase(maxlinesizes.begin(), maxlinesizes.end());
	// recompute maxcolsize and maxlinesize
	for (ltdi=TDCompound::ltd.begin(), i=0; (ltdi!=TDCompound::ltd.end())  ;i++, ltdi++) {
	    loccolnum = i % TDMenu::numcol;
	    linenum   = i / TDMenu::numcol;
	    (*ltdi)->proj_size (locoffset, locsize);
	    
	    // we update the maxcolsize (maximum column sizes) vector 
	    if (((int)maxcolsizes.size() < TDMenu::numcol) )  
		maxcolsizes.push_back (fabs(locsize.x));
	     else
		if ( maxcolsizes[loccolnum]<fabs(locsize.x))
		    maxcolsizes[loccolnum]=fabs(locsize.x);
	    // we update the maxlinesize (maximum line sizes) vector
	    if (loccolnum == 0)
		maxlinesizes.push_back (fabs(locsize.y));
	     else
		if ( maxlinesizes[linenum] < fabs(locsize.y)) 
		    maxlinesizes[linenum] = fabs(locsize.y);
     
	}
	viewType = newviewtype;
	compute_all_pos ();
    }
    else 
	return;
}

void TDMenu::change_padding (Vector3 padding)
{
     if ((padding.x == xpadding) && (padding.y == ypadding))
	return;

     xpadding = padding.x;
     ypadding = padding.y;

     compute_all_pos ();
}

bool TDMenu::push_back (TDObj & td)
{
    Vector3 locoffset, locsize;
    int loccolnum;
    int linenum;

    if (TDCompound::push_back (td)) {
	
	loccolnum = nbelem % numcol;
	linenum   = nbelem / numcol;
	td.proj_size (locoffset, locsize);

	// we update the maxcolsize (maximum column sizes) vector 
	if (((int)maxcolsizes.size() < numcol) )  
	    maxcolsizes.push_back (fabs(locsize.x));
	 else
	    if ( maxcolsizes[loccolnum]<fabs(locsize.x))
		maxcolsizes[loccolnum]=fabs(locsize.x);
	
	// we update the maxlinesize (maximum line sizes) vector
	if (loccolnum == 0)
	    maxlinesizes.push_back (fabs(locsize.y));
	 else
	    if ( maxlinesizes[linenum] < fabs(locsize.y)) 
		maxlinesizes[linenum] = fabs(locsize.y);
	
	compute_all_pos ();
	nbelem++; 
	if (viewType == INLINE) 
		numcol=nbelem+1;

	return true;
    }
    return false;
}

bool TDMenu::push_back (TDMenuItem & td)
{
    if (TDMenu::push_back (*(TDObj *)(&td) )) {	// JDJDJDJD il y a probablement mieux que cette cascade de cast a...
						// ...faire pour appeler l'autre methode ???
	td.ptdmenu = this;
	return true ;
    } else
	return false ;
}

const string & TDMenu::gettdname (void)
{   static string name ("tdmenu");
    return name;
}

}   // grapefruit namespace's end...

