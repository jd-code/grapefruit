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
			    cinetic.cpp						    \
			    td.cpp glvfont.cpp tdstring.cpp menu.cpp		    \
			    tdfps.cpp						    \
			    glreinit.cpp					    \
			    graphprim.cpp					    \
			    graphutils.c jeuchar.c				    \
			    testdll.c

## grapefruit_LDADD = @GL_LIBS@

## headers to be installed
pkginclude_HEADERS =	include/grapefruit/agrume.h		\
			include/grapefruit/grapecolor.h		\
			include/grapefruit/grapefruit.h		\
			include/grapefruit/grapekey.h  		\
			include/grapefruit/glreinit.h		\
			include/grapefruit/graperoute.h		\
			include/grapefruit/cinetic.h		\
			include/grapefruit/td.h			\
			include/grapefruit/glvfont.h		\
			include/grapefruit/glcomp.h		\
			include/grapefruit/tdstring.h		\
			include/grapefruit/menu.h		\
			include/grapefruit/tdfps.h		\
			include/grapefruit/graphprim.h		\
			include/grapefruit/graphutils.h		\
			include/grapefruit/jeuchar.h


