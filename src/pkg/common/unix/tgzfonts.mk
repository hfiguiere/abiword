#! gmake

## AbiSource Applications
## Copyright (C) 1998 AbiSource, Inc.
##
## This program is free software; you can redistribute it and/or
## modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2
## of the License, or (at your option) any later version.
## 
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  
## 02111-1307, USA.

##################################################################
# THIS MAKEFILE IS A STANDARD SET OF INSTALL TARGETS AND WILL BE 
# INCLUDED BY ACTUAL PLATFORM PKG MAKEFILES.  REFERENCES TO SHARED
# (COMMON) RESOURCES SHOULD BE DONE FROM A LOCATION "INSIDE" THE
# INCLUDING MAKEFILE.  BE SURE TO INCLUDE THIS MAKEFILE AFTER 
# YOU HAVE ALREADY INCLUDED THE ABI_DEFS.MK AND ABI_RULES.MK.
##################################################################	

##################################################################
## tgzfonts -- the Unix fonts package in .tar.gz format.  this
##			package is kind of an oddball since it
##			only commonly builds .tar.gz for all Unix
##			platforms (no deb, rpm, slp, etc.)

# get the package names we'll use to build with
ABI_DEPTH=../..
include $(ABI_DEPTH)/pkg/common/unix/allfonts.mk

font_files = 	README \
	  	INSTALL \
		readme.abw \
		COPYING \
		install.sh \
		fonts

tgz_fonts:
	@echo "* Building .tar.gz package [fonts] ..."
	@$(subst xxxx,$(DIST),$(VERIFY_DIRECTORY))
	@$(subst xxxx,$(DIST)/$(ABI_FONTS_PKGBASENAME),$(VERIFY_DIRECTORY))
	(cd $(OUTDIR)/AbiSuite/fonts; tar cf - $(font_files))| (cd $(DIST)/$(ABI_FONTS_PKGBASENAME); tar xf -)
	(cd $(DIST); tar cf - $(ABI_FONTS_PKGBASENAME) | gzip - - > $(ABI_FONTS_PKGBASENAME).tar.gz)
	(cd $(DIST); rm -rf $(ABI_FONTS_PKGBASENAME))
