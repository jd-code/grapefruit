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
 * grapefruit@disjunkt.com  -  http://grapefruit.disjunkt.com/
 */


#ifndef   GRAPEFRUIT_H_INC
#warning agrume.h should not be included as is. grapefruit.h should be included instead
#else  // GRAPEFRUIT_H_INC
#ifdef	    GRAPEFRUIT_H_GLOBINST
#define	    AGTD_H_GLOBINST
#endif //   GRAPEFRUIT_H_GLOBINST
#endif // GRAPEFRUIT_H_INC

#ifndef AGTD_H_INC
#define AGTD_H_INC

#include <vector>

#include <matcalc/matcalc.h>

namespace grapefruit
{
    class TDObj;
    class TDCompound;
    class Mvmt;

    // --------------- agrume related globals ------------------------------------------------------

    GRAPEFRUIT_H_SCOPE list <TDObj *> td_displayed;		//!< the ordered list of currently displayed TDs
    GRAPEFRUIT_H_SCOPE vector <TDObj *> td_evented;		//!< the list of currently eventizabled TDs
    GRAPEFRUIT_H_SCOPE GLfloat convert2Dto3Ddx, convert2Dto3Ddy;//!< the stored dx,dy factors for 2D-3D coordinates conversion utilities

////////#ifdef	AGTD_H_GLOBINST
////////    list <TDObj *> td_displayed;   
////////    vector <TDObj *> td_evented;   
////////    GLfloat convert2Dto3Ddx, convert2Dto3Ddy;
////////#else
////////    extern list <TDObj *> td_displayed;			//!< the ordered list of currently displayed TDs
////////    extern vector <TDObj *> td_evented;			//!< the list of currently eventizabled TDs
////////    extern GLfloat convert2Dto3Ddx, convert2Dto3Ddy;	//!< the stored dx,dy factors for 2D-3D coordinates conversion utilities
////////#endif	// AGTD_H_GLOBINST

#define GRTD_UNDEF 65000
    
    // ----------------------------- TDObj ---------------------------------------------------------
    //
    //! a 3D object with facilities for placing conveniently and also tikkling with the mouse
    //! \nosubgrouping

    class TDObj : public virtual MGrabber
    {
	private:
	    size_t id_evented;			    //!< our id in the list of evented TDs, or GRTD_UNDEF if not evented
	    list<TDObj *>::iterator id_displayed;   //!< our id in the list of displayed TDs, or GRTD_UNDEF if not displayed

	    TDCompound * ptdparent;		    //!< a pointer to a TD to which we are linked, our coordinates are relative to it
	    list<TDObj*>::iterator id_ltdparent;    //!< our id in our parent's list ; valid only if we have a parent

	    double posxorigin, posyorigin;

	    list<list<Mvmt*>::iterator> lmvmtid;    //!< our list of active Mvmts, expessed in regard with the global list lmvmt

	public:
	    bool isglobalactive;    //!< is the TD's renderclickable zone method called globally or locally ? 
	    Vector3 pos;	    //!< TD's position
	    Matrix4 rm;		    //!< TD's rotation matrix (OpenGL 4x4 matrix)
	    double scale;	    //!< TD's scaling factor
	    bool isgrabbable;	    //!< is the TD grab-active ?

	    // bool isselected;
	    

	    virtual ~TDObj (void);

	    inline TDObj (void) : MGrabber ()
		{   rm.setident ();
		    id_evented = GRTD_UNDEF,
		    id_displayed = td_displayed.end();
		    // isselected = false;
		    scale = 1.0;
		    isgrabbable = false;
		    isglobalactive = true;
		    ptdparent = NULL;
		}

	    // ----------- testing various status --------------------------------------------------
	    //
	    /** @name TDObj status methods
	     *  testing various status
	     */
	    //@{

	    //! returns weither the TD is already shown or not ?
	    inline bool isshown (void)
		{   return (id_displayed != td_displayed.end());
		}

	    //! returns weither the TD is already evented (active) ?
	    inline bool isactivated (void)
		{   return (id_evented != GRTD_UNDEF);
		}
	    //@}
	    
	    // ----------- drawing, displaying activating methods ----------------------------------
	    //
	    //! @name TDObj drawing, displaying activating methods
	    //@{
	    
	    //! the method that sends to the OpenGL rendering device the current shape of the TD
	    virtual void render (void) = 0;

	    //! the method that sends to the OpenGL device the sensitive zone of the TD
	    virtual void renderclickablezone (void) = 0;


	    //! adds the TD to the list of displayed TDs and displays it on top of the others
	    void show (void);

	    //! adds the TD to the list of displayed TDs and inserts it on top of the given TDObj
	    void show (TDObj const & td);

	    //! removes the TD from the list of displayed TDs
	    void hide (void);

	    //! adds the TD to the list of evented TDs
	    virtual void activate (bool isglobalactive = true);

	    //! removes the TD from the list of evented TDs
	    virtual void desactivate (void);
	private:
	    //! this is the same method as hide() excepts it can be called from destructor because it doesn't call
	    //! any virtual method. it is private because slower than hide()
	    void hideindestructor (void);

	    //! this one is called from the regular public hide() and from the private hideindestructor(), the common code is here
	    void hide (GLfloat diam);
	public:

	    //@}


	    // ----------- size reporting methods --------------------------------------------------
	    //
	    //! @name TDObj size reporting methods
	    //@{

	    //! reports TD's nearest lower-left box point and TD's size for each axis direction (after scaling and projection)
	    virtual void proj_size (Vector3 &offset, Vector3 &size) = 0;


	    //! reports TD's biggest diameter (relatively from the center of rotation)
	    virtual GLfloat diameter (void) = 0;
	    //@}

	    
	    // ----------- motion methods ----------------------------------------------------------
	    //
	    /** @name TDObj motion methods
	     *  \brief moving and scaling the object into rendering space
	     *  @{
	     */

	    //! sets the TD's position
	    inline void setpos (const Vector3 &v)
		{   pos = v;
		}
	    //! sets the TD's position
	    inline void setpos (GLfloat x, GLfloat y, GLfloat z)
		{   pos.x = x, pos.y = y, pos.z = z;
		}

	    //! sets the TD's (x,y) position, leaving thz z position untouched
	    inline void setxypos (GLfloat x, GLfloat y)
		{   pos.x = x, pos.y = y;
		}
	    //! sets the TD's (x,y) position, leaving the z position untouched (the z component of the Vector3 parameter is ignored)
	    inline void setxypos (const Vector3 &v)
		{   pos.x = v.x, pos.y = v.y;
		}
	    
	    //! translates the TD into space using the given vector
	    inline void translate (const Vector3 &v)
		{   pos += v;
		}
	    //! translates the TD into space with the given vector
	    inline void translate (GLfloat x, GLfloat y, GLfloat z)
		{   pos.x += x, pos.y += y, pos.z += z;
		}

	    //! rotates the TD's along an axis given by vector "v", of an angle "angle"
	    void rotate (Vector3 const & v, double angle);

	    //! updates the scaling
	    void setscale (double s)
		{   scale = s;
		}
	    //@}
	    
	    // ----------- vertical placement management -------------------------------------------
	    //
	    /** @name TDObj vertical placement management
	     *  z-position of TDs are ordered ; distances between TDs are set according to diameter()
	     *  when they are added to displayed-TDs list.<br>
	     *  They are also rendered according to their ordered positions (from bottom) for
	     *  transparencies purpose...
	     */
	    //@{

	    //! pushes the said TD on top of the list, move all the other farther according to diameter()
	    void pushtotop (void)
		{   // we move all TDs from top to us down our own diameter (we make room at the top)
		    list<TDObj *>::iterator li,
					    begin = td_displayed.begin();
		    // if we're already at top : nothing to do
		    if (id_displayed == begin)
			return;
		    GLfloat diam = diameter ();
		    for (li = begin ; li != id_displayed ; li++)
			(*li)->pos.z += diam;
		    // we move the TD to the top of the list
		    td_displayed.push_front (*id_displayed);
		    td_displayed.erase (id_displayed);
		    pos.z = -1.0 + diameter () / 2.0;	    // the initial top-z value should be tunable JDJDJDJD
		}
	    //@}

	    // ----------- miscelaneous ------------------------------------------------------------
	    //
	    /** @name TDObj miscelaneous
	     *  testing various status
	     */
	    //@{

	    //	virtual void gotselected (void);
	    //	virtual void gotdeselected (void);
	    virtual void gotclicked (SDL_Event const & event) {};

	    virtual int startgrabber (SDL_Event const & event);
	    virtual int endgrabber (SDL_Event const & event);
	    virtual void grabberchange (SDL_Event const & event);

	    virtual const string & gettdname (void);
	    //@}

	friend class TDCompound;
	friend class TDMenu;	    //!< TDMenu is friend because we need access to the id_evented
	friend class Mvmt;	    //!< Mvmt accesses to lmvmtid
    };


    // ----------------------------- ACPTD ---------------------------------------------------------
    //
    // Delivers binding Action to TDObj
    //

    class ACpTD : public Action
    {
	protected:
	    TDObj * ptd;
	public:
	    virtual ~ACpTD(void) {}
	    ACpTD (TDObj * p)
		{   ptd = p;
		}
    };

    // ----------------------------- TDObjSHable ---------------------------------------------------
    //
    // Delivers hide, show and toggle actions for any TDObj
    //


    //! Action for hiding and desactivating some TDObj

    class ACTDHide : public ACpTD
    {
	public:
	    virtual ~ACTDHide(void) {}
	    ACTDHide (TDObj * p) : ACpTD (p) {}

	    virtual void doit (void)
		{   if (ptd->isactivated ())
			ptd->desactivate ();
		    if (ptd->isshown ())
			ptd->hide ();
		}
	    virtual string getacname (void)
		{   string name(ptd->gettdname());
		    name += "->ACTDHide";
		    return name;
		}
    };

    //! Action for showing and activating some TDObj

    class ACTDShow : public ACpTD
    {
	public:
	    virtual ~ACTDShow(void) {}
	    ACTDShow (TDObj * p) : ACpTD (p) {}

	    virtual void doit (void)
		{   if (!ptd->isshown ())
			ptd->show ();
		    if (!ptd->isactivated ())
			ptd->activate ();
		}
	    virtual string getacname (void)
		{   string name(ptd->gettdname());
		    name += "->ACTDShow";
		    return name;
		}
    };

    //! Action for toggling some TDObj

    class ACTDToggle : public ACpTD
    {
	public:
	    virtual ~ACTDToggle(void) {}
	    ACTDToggle (TDObj * p) : ACpTD (p) {}

	    virtual void doit (void)
		{   if (ptd->isshown ()) {
			if (ptd->isactivated ())
			    ptd->desactivate ();
			ptd->hide ();
		    } else {
			ptd->show ();
			if (!ptd->isactivated ())
			    ptd->activate ();
		    }
		}
	    virtual string getacname (void)
		{   string name(ptd->gettdname());
		    name += "->ACTDToggle";
		    return name;
		}
    };

    // ----------------------------- TDObjSHable ---------------------------------------------------
    //
    //! Delivers hide and show actions for any TDObj
    //! \nosubgrouping
    
    class TDObjSHable : virtual public TDObj
    {	
	public:

	    // ----------- Set of bound Action for triggering show/hide and co ---------------------
	    //
	    /** @name TDObjSHable specificities
	     * Set of Action bound to TDObj for triggering show/hide and co
	     */
	    //@{
	    
	    ACTDHide actdhide;	    //!< the Action that hides and desactivates the TD
	    ACTDShow actdshow;	    //!< the Action that shows and sactivates the TD
	    ACTDToggle actdtoggle;  //!< the action that toggles the TD

	    TDObjSHable (void) : actdhide (this), actdshow(this), actdtoggle(this) {}
	    ~TDObjSHable (void) {}

	    //! OBSOLETE: returns the Action that hides and desactivates the TD
	    Action * getactdhide (void)
		{   return &actdhide;
		}
	    //! OBSOLETE: returns the Action that shows and sactivates the TD
	    Action * getactdshow (void)
		{   return &actdshow;
		}
	    //! OBSOLETE: returns the action that toggles the TD
	    Action * getactdtoggle (void)
		{   return &actdtoggle;
		}
	    //@}
    };
    
                
    // ----------------------------- TDCompound ----------------------------------------------------
    //
    //! compound of TDObj(s)
    //! \nosubgrouping

    class TDCompound : virtual public TDObj
    {           
	public: 
	    /** @name TDCompound specificities
	     * several TDObj grouped into one "super" TDObj
	     */
	    //@{

	    list <TDObj*> ltd;						    //!< the list of contained TDObj
	    virtual ~TDCompound (void);
	    TDCompound (void) : TDObj() {}

	    virtual bool push_back (TDObj & td);			    //!< adds a TDObj to the set
	    virtual bool erase (list<TDObj*>::iterator id_ltdparent);	    //!< is supposed to remove an element
	    //@}

	    virtual void render (void);
	    virtual void renderclickablezone (void);

	    virtual void activate (bool isglobalactive = true);
	    virtual void desactivate (void);
	    
	    virtual void proj_size (Vector3 &offset, Vector3 &size) = 0;
	    virtual GLfloat diameter (void) = 0;

	    virtual const string & gettdname (void);

    };      


    // ----------------------------- Main rendering functions and related --------------------------
    //
    //


    //! renders (displays) all the TDs listed to be displayed
    void TDrender (void);

    //! returns the top evented TD located at screen coord xe, ye, or NULL
    EventCB * TDrenderclickablezone (int xe, int ye);

    //! roughly converts (x,y) window-coordinate to (x,y,z) 3D space coordinates (with z undefined yet...)
    void convert2Dto3D (int xe, int ye, GLfloat &x, GLfloat &y, GLfloat &dx = convert2Dto3Ddx, GLfloat &dy = convert2Dto3Ddy);
    
    //! this Action dumps the ordered content of td_displayed for debugging purpose
    class ACDump_td_displayed : public Action
    {	public:
	    virtual ~ACDump_td_displayed () {}
	    virtual void doit ();
	    virtual string getacname (void);
    };

    //! this Action takes one displayed TD and puts it on top of another displayed TD.
    //! it is intended for testing purpose (say with mixed GL rendering states, for example)
    class ACScramble_td_displayed : public Action
    {	public:
	    virtual ~ACScramble_td_displayed () {}
	    virtual void doit ();
	    virtual string getacname (void);
    };

    // ----------------------------- size calculation utils ----------------------------------------
    //
    //


    //! modifies two given vectors min and max so that they bound the tightest box around a successive set of vectors v
    inline void boxminimax (Vector3 &min, Vector3 &max, Vector3 const &v)
    {
	if (v.x < min.x) min.x = v.x;
	if (v.y < min.y) min.y = v.y;
	if (v.z < min.z) min.z = v.z;
	if (v.x > max.x) max.x = v.x;
	if (v.y > max.y) max.y = v.y;
	if (v.z > max.z) max.z = v.z;
    }

    inline void proj_size (Vector3 &offset, Vector3 &size, vector<Vector3> const & vv,
			      Vector3 const & trans,    //!< offset de translation
			      Vector3 const & scale,    //!< diagonale d'homotetie
			      Matrix4 const & rm	//!< matrice de rotation
			     )
    {
	vector<Vector3>::const_iterator vi = vv.begin();
	Vector3 min, max,
		v;

	v = (*vi) + trans;
	v.x *= scale.x;
	v.y *= scale.y;
	v.z *= scale.z;
	v = rm * v;

	min = v;
	max = v;

	vi++;

	for ( ; vi!=vv.end() ; vi++) {
	    v = (*vi) + trans;
	    v.x *= scale.x;
	    v.y *= scale.y;
	    v.z *= scale.z;
	    v = rm * v;

	    boxminimax (min, max, v);
	}
	offset = min;
	size = max - min;
    }

}


#endif // AGRUME_H_INC

