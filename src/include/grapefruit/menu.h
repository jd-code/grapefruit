/* 
 * $Id$
 * Loopsoids Copyright (C) 2002,2003 Cristelle Barillon & Jean-Daniel Pauget
 * some people say loopsoids creations can tell the future
 *
 * loopsoids@disjunkt.com  -  http://loopsoids.disjunkt.com/
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
#warning menu.h should not be included as is. grapefruit.h should be included instead
#else  // GRAPEFRUIT_H_INC
#ifdef	    GRAPEFRUIT_H_GLOBINST
#define	    AGTD_H_GLOBINST
#endif //   GRAPEFRUIT_H_GLOBINST
#endif // GRAPEFRUIT_H_INC


#ifndef AGMENU_H_HEADER
#define AGMENU_H_HEADER

// JDJDJDJD a enlever...
////////    #include "SDL.h"
////////    #include "qualsample.h"
////////    #include "tdstring.h"
////////    #include "bzouz/bzouz.h"

////////    #include "glcomp.h"	// ::proj_size

#define SHOW_CONVEX_ENVELOPP true

namespace grapefruit
{
    // ------------------------- ActionTest -----------------------------------------------------------
    //
    //! dummy but convenient action that just outputs when called - for tests purpose only

    class ActionTest : public Action
    {
	    int nbclick;
	    bool doreport;
	public:
	    ActionTest (bool doreport = false)
		{	nbclick = 0;
		    ActionTest::doreport = doreport;
		}	
	    virtual void doit (void)
		{
		    if (doreport)
			bzouzerr << "Action!!! (" << nbclick << ")" << endl ;
		    nbclick ++;
		}
    };

    class TDMenu ;

    // ------------------------- TDMenuItem -----------------------------------------------------------
    //
    //! standard template for any menu item 
    //! \nosubgrouping

    class TDMenuItem : public TDObj
    {
	protected:
	    TDMenu * ptdmenu;   //!< pointer to the menu which it belongs to
	    Action * paclick;   //!< pointer to the bound action when clicked
	    Action * pagrab;    //!< pointer to the bound action when grabbed // JDJDJDJD unfinished !

	    virtual void valuechange (SDL_Event const &event){}
	    virtual void gotclicked (SDL_Event const &event);

	public:
	    virtual ~TDMenuItem  (void) {}
	    
	    TDMenuItem (Action & ac) : TDObj ()
		{
		    ptdmenu = NULL;
		    paclick = & ac;
		    pagrab  = & ac;
		}
	    
	    TDMenuItem (Action & aclick, Action & agrab) : TDObj ()
		{
		    ptdmenu = NULL;
		    paclick = & aclick;
		    pagrab  = & agrab ;
		}


	    friend class TDMenu;
    };


    // ------------------------- TDMenuItemString -----------------------------------------------------
    //
    //! standard template for any menu item that is represented by its name 
    //! \nosubgrouping

    class TDMenuItemString : public TDMenuItem
    {
	private:
	    string name;
	    TDString *tds;
	public :
	    virtual ~TDMenuItemString (void)
		{   if (tds != NULL)
			delete (tds);
		}
	    TDMenuItemString (Action & ac, const string & s) : TDMenuItem (ac)
		{
		    name = s;
		    tds = new TDString (s, 0.03);
		    // JDJDJDJD manque le test de nullité ?
		}
	    TDMenuItemString (Action & aclick, Action & agrab, const string & s) : TDMenuItem (aclick, agrab)
		{
		    name = s;
		    tds = new TDString (s, 0.03);
		    // JDJDJDJD manque le test de nullité ?
		}

	    virtual void render (void)
		{   tds->render();
		}
	    virtual void renderclickablezone (void)
		{   tds->renderclickablezone();
		}
	    virtual void proj_size (Vector3 &offset, Vector3 &size) 
		{   tds->proj_size(offset, size);
		}
	    virtual GLfloat diameter (void) 
		{   return tds->diameter();
		}
    };


    // ------------------------- TDMenu ---------------------------------------------------------------
    //
    //! standard template for any menu
    //! \nosubgrouping

    class TDMenu : public TDCompound, public TDObjSHable
    {
	public:
	    typedef enum {  INLINE,
			    INCOLUMN,
			    INARRAY,
	    } MenuViewType;

	    typedef enum {  STICKY,		// stays on the screen
			    ONECLICK,   	// disappears after click
			    ONESHOT		// disappears and is deleted after click
	    } MenuStayType;
	    
	private:
	    string s;			//!< menu's name
	    MenuStayType stayType;		//!< wether the menu stays or not after a selectionor some clock or loosing focus...
	    MenuViewType viewType;		//!< the menu's disposition (column, line, array)
	    GLfloat xpadding;		//!< horizontal space between cells	
	    GLfloat ypadding;		//!< vertical space between cells	
	    int numcol;			//!< maximum number of column we can use according to "viewType"
	    int nbelem;			//!< number of TD-components
	    vector<GLfloat> maxcolsizes;	//!< max column sizes to display correctly in ARRAY viewtype
	    vector<GLfloat> maxlinesizes;	//!< max line size to display correctly in ARRAY viewtype
	    Vector3 sizesum;		//!< more or less the next coordinate in use for positionning the next menu element
	    vector<Vector3> vv;		//!< the list of points defining some convex-envelope of our object (here : a box)

	    void compute_all_pos ();	//!< recomputes the positions of each TD component of the menu

	public:
	    virtual ~TDMenu (void) {}
	    TDMenu ();
	    TDMenu (const string &s);
	    TDMenu (const string &s, MenuViewType viewType, MenuStayType stayType, int numcol=1);

	    bool push_back (TDMenuItem & td);
	    bool push_back (TDObj & td);

	    void change_padding (Vector3 padding);  

	    void change_viewtype (MenuViewType newviewtype, int numcol=1);

	    void change_staytype (MenuStayType newstaytype)
	    {
		TDMenu::stayType = newstaytype;
	    }

	    void change_name (const string & name)
	    {
		s = name;
	    }

	    virtual void render (void);
	    virtual void proj_size (Vector3 &offset, Vector3 &size) 
		{
		    //::grapefruit::proj_size (offset, size, vv, pos, Vector3 (scale, scale, scale), rm);
		    ::grapefruit::proj_size (offset, size, vv, Vector3 (0.0,0.0,0.0) , Vector3 (scale, scale, scale), rm);
		}
	    virtual GLfloat diameter (void) 
		{
		    return sqrt(sizesum.x*sizesum.x + sizesum.y*sizesum.y) * scale; //CBCBCB this formula has NOT been checked!!
		}

	friend class TDMenuItem;
    };
    //CBCBCB what happens if we erase one element of the menu

} // grapefuit's namespace's end

#endif // AGMENU_H_HEADER


/*
 * $Log: menu.h,v $
 * Revision 1.9  2004/03/13 16:36:05  cristelle
 * ONESHOT staytype added and small adjustments in deletions vs actions ordering
 *
 * Revision 1.8  2004/03/01 18:05:02  jd
 * corrected "pure virtual method called" bug (paradigm : never call a virtual function in destructors)
 *
 * Revision 1.7  2004/02/28 15:38:49  jd
 * documentations updates
 *
 * Revision 1.6  2004/02/26 15:27:40  cristelle
 * orthographic changes in menus defines
 *
 * Revision 1.5  2004/02/19 00:39:12  jd
 * added automated z-location management
 *
 * Revision 1.4  2004/02/13 14:07:40  jd
 * moved from inheriting Actions to being enriched with Action-derivated members instead...
 *
 * Revision 1.3  2004/02/10 14:18:00  cristelle
 * added a change viewtype for menu objects.
 *
 * Revision 1.2  2004/01/31 15:59:17  jd
 * introduction of TDObjHideable TDObjShowable
 *
 * Revision 1.1  2004/01/20 15:41:53  jd
 * massive librarization around menu, stdstrings etc
 *
 * Revision 1.11  2004/01/16 15:08:17  cristelle
 * cleaned menu.cpp of redundant code
 * added a point_smooth in circle rendering so that the ticks always appear round
 *
 * Revision 1.10  2004/01/13 23:34:57  jd
 * extension for handling "oneclick" menus, first draft...
 *
 * Revision 1.9  2003/12/09 17:32:01  jd
 * added TDAudio(s) objects family for visually controlling audio structures
 *
 * Revision 1.8  2003/12/02 00:08:03  jd
 * structure for circle playing added
 * librarised class Action (for keymaps)
 *
 * Revision 1.7  2003/11/27 14:28:51  jd
 * updated datas structures for Circle and co.
 *
 * Revision 1.6  2003/11/27 00:32:26  cristelle
 * menu management nice version
 *
 * Revision 1.5  2003/11/26 23:47:03  cristelle
 * intermediate version of menu management = coexistence of old version (all relative positions) and future version (absolute positions)
 *
 * Revision 1.4  2003/11/01 15:18:43  cristelle
 * first nice version of menu
 *
 * Revision 1.3  2003/10/24 20:56:04  jd
 * librarization of TDCompound
 *
 * Revision 1.2  2003/10/23 22:12:37  cristelle
 * first functionnal draft of TDMenu/TDMenuItem/Action
 *
 * Revision 1.1  2003/10/20 14:10:23  cristelle
 * *** empty log message ***
 *
 */
