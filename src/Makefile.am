## 
## $Id$
## GrapeFruit Copyright (C) 2002 Cristelle Barillon, Jean-Daniel PAUGET
## A whole set of bouncing balls
## 
## grapefruit@disjunkt.com  -  http://grapefruit.disjunkt.com/
## 
## This file is part of GrapeFruit.
## 
## GrapeFruit is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
## 
## GrapeFruit is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with GrapeFruit; if not, write to the Free Software
## Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
## 
## you can also try the web at http://www.gnu.org/
## 


lib_LTLIBRARIES = libgrapefruit.la

libgrapefruit_la_LDFLAGS = -version-info @lt_major@:@lt_revision@:@lt_age@ @GL_LIBS@ @WIN32LDFLAF@
##			   -no-undefined \
##			   -Xlinker --export-all-symbols \


libgrapefruit_la_SOURCES =  grapefruit.cpp grapekey.cpp graperoute.cpp agrume.cpp   \
			    td.cpp glvfont.cpp tdstring.cpp menu.cpp		    \
			    graphprim.cpp					    \
			    graphutils.c jeuchar.c				    \
			    testdll.c

## grapefruit_LDADD = @GL_LIBS@

## headers to be installed
pkginclude_HEADERS =	include/grapefruit/agrume.h		\
			include/grapefruit/grapecolor.h		\
			include/grapefruit/grapefruit.h		\
			include/grapefruit/grapekey.h  		\
			include/grapefruit/graperoute.h		\
			include/grapefruit/td.h			\
			include/grapefruit/glvfont.h		\
			include/grapefruit/glcomp.h		\
			include/grapefruit/tdstring.h		\
			include/grapefruit/menu.h		\
			include/grapefruit/graphprim.h		\
			include/grapefruit/graphutils.h		\
			include/grapefruit/jeuchar.h


## 
##  $Log: Makefile.am,v $
##  Revision 1.6  2004/05/03 22:02:50  jd
##  big merge for mingw/msys and cross-mingw
##
##  Revision 1.5  2004/01/20 15:40:02  jd
##  massive librarization around menu, stdstrings etc
##
##  Revision 1.4  2003/12/02 00:08:49  jd
##  fisrt draft of keymapping
##
##  Revision 1.3  2003/09/06 22:17:16  jd
##  first release of TDs
##
##  Revision 1.2  2003/04/22 16:39:28  jd
##  small libtool revision info tuning
##
##  Revision 1.1.1.1  2003/04/22 14:04:34  jd
##  librarisation of grapfruit
##
## 
