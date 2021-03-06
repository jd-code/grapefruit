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
#warning tdstring.h should not be included as is. grapefruit.h should be included instead
#else  // GRAPEFRUIT_H_INC
#ifdef	    GRAPEFRUIT_H_GLOBINST
#define	    AGTD_H_GLOBINST
#endif //   GRAPEFRUIT_H_GLOBINST
#endif // GRAPEFRUIT_H_INC

#ifndef TDSTRING_H_HEADER
#define TDSTRING_H_HEADER

namespace grapefruit
{

    //! a string embedded in some TDObj
    class TDString : virtual public TDObj
    {
	/** @name TDString specificities
	 *  a string embedded in a TDObj
	 */
	//@{
	    string s;	    //!< the string content itself
	    double h;	    //!< JDJDJDJD to be better documented ! is it the heith of the string or what ?
	    double width;   //!< the line and point width

	    double sh, sw;

	    vector<Vector3> vv; //!< the list of points defining some convex-envelope of our object (here : a box)

	    Vector4 color;	//!< the color of the text itself (default : white);

	public:
	    virtual ~TDString (void) {}
	    TDString (const string &s, double h = 1.0);

	    void update (const string &s);

	    //! sets the color of the TDString
	    inline void setcolor (Vector4 const & color)
		{   TDString::color = color;
		}

	    //! sets the width (float) used for rendering the TDString
	    inline void setwidth (double width)
		{   TDString::width = width;
		}
	//@}

	    virtual void render (void);
	    virtual void renderclickablezone (void);

	    virtual void proj_size (Vector3 &offset, Vector3 &size);
	    virtual GLfloat diameter (void);

	    virtual void gotclicked (SDL_Event const &event);
	    virtual const string & gettdname (void);
    };

} // namespace grapefruit
    
#endif //  TDSTRING_H_HEADER

