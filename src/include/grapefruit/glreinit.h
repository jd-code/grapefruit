/* 
 * $Id$
 * GrapeFruit Copyright (C) 2002,2003,2004 Cristelle Barillon & Jean-Daniel Pauget
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
#warning glreinit.h should not be included as is. grapefruit.h should be included instead
#else  // GRAPEFRUIT_H_INC
#ifdef	    GRAPEFRUIT_H_GLOBINST
#define	    AGGLREINIT_H_GLOBINST
#endif //   GRAPEFRUIT_H_GLOBINST
#endif // GRAPEFRUIT_H_INC

#ifndef AGGLREINIT_H_INC
#define AGGLREINIT_H_INC

namespace grapefruit
{

    int glreinit (void);

    class GLReInit
    {
	public:
	    virtual int perform_reinit (void) = 0;
	    void subscribe (void);
	    void unsubscribe (void);
	private:
	    static list <GLReInit *> lglreinit;

	friend int glreinit (void);
    };

#ifdef AGGLREINIT_H_GLOBINST
    GRAPEFRUIT_H_SCOPE list <GLReInit *> GLReInit::lglreinit;
#endif

}

#endif // AGGLREINIT_H_INC

