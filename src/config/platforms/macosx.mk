#! gmake

## AbiSource Program Utilities
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

##############################################################################
## MacOSX platform defines (grafted from FreeBSD)
## Modified for MacOS X by Hubert Figuiere <hfiguiere@teaser.fr>
##############################################################################

##################################################################
##################################################################
## The main makefile and/or this file requires that OS_ARCH be set
## to something to describe which chip that this OS is running on.
## This can be used to change which tools are used and/or which
## compiler/loader options are used.  It will probably also be used
## in constructing the name object file destination directory.

#OS_ARCH		:= $(shell uname -m | sed "s/\//-/" )
#We should change this at some point in time to the generic x86/ppc
OS_ARCH		:= $(shell uname -p | sed "s/\//-/" | \
	           sed -e s/powerpc/ppc/)

# These are (probably) optional for your platform.
PPC_ARCH_FLAGS		=

# Define tools
CC		= cc
CCC		= CC
RANLIB		= ranlib
REZ		= /Developer/Tools/Rez
RESMERGER	= /Developer/Tools/ResMerger

# Suffixes
OBJ_SUFFIX	= o
LIB_SUFFIX	= a
DLL_SUFFIX	= so
AR		= ar cr $@

# Compiler flags
ifeq ($(ABI_OPT_DEBUG),1)
OPTIMIZER	= -g -Wall
DEFINES		= -DDEBUG -UNDEBUG
OBJ_DIR_SFX	= DBG
else
OPTIMIZER	= -O2 -Wall
DEFINES		=
OBJ_DIR_SFX	= OBJ
endif

# Includes
OS_INCLUDES	= -I$(ABI_ROOT)/../libiconv/include \
        -I$(ABI_ROOT)/../libpng \
		-I/usr/local/include \
		-I/Developer/Headers/FlatCarbon 
G++INCLUDES		= -I$(ABI_ROOT)/../libiconv/include -I/usr/include/g++

# Compiler flags
PLATFORM_FLAGS		= -fpascal-strings -DUSE_CARBON_EVENT -DCARBON_ON_MACH_O=1 -DXP_MAC_TARGET_MACOSX -DXP_MAC_TARGET_CARBON -DXP_MAC_TARGET_QUARTZ -DNO_SYS_ERRLIST $(OS_INCLUDES)
#PORT_FLAGS		= -D_POSIX_SOURCE -D_BSD_SOURCE -DHAVE_STRERROR -D_XOPEN_SOURCE -D__USE_XOPEN_EXTENDED
OS_CFLAGS		= $(DSO_CFLAGS) $(PLATFORM_FLAGS) $(PORT_FLAGS)

# Architecture-specific flags
ifeq ($(OS_ARCH), x86)
PLATFORM_FLAGS		+= $(X86_ARCH_FLAGS)
OS_ENDIAN		= LittleEndian32
endif

ifeq ($(OS_ARCH), ppc)
PLATFORM_FLAGS		+= $(PPC_ARCH_FLAGS)
OS_ENDIAN		= BigEndian32
endif

LDFLAGS			= -framework Carbon -framework CoreServices \
				-framework ApplicationServices

# Actually pspell is only available statically, so a the needed support
# for static linkage.
ifeq ($(ABI_OPT_PSPELL),1)
LDFLAGS      += -lpspell-modules -lstdc++
endif

# Shared library flags
#MKSHLIB		= $(LD) $(DSO_LDOPTS) -soname $(@:$(OBJDIR)/%.so=%.so)
MKSHLIB			= $(LD) $(DSO_LDOPTS) -soname $(@:$(OBJDIR)/%.so=%.so)

ABI_NATIVE	= mac
ABI_FE		= Mac

# Rez flags
ABI_MAC_REZ_INCS= -i $(ABI_ROOT)/src/af/xap/mac
ABI_MAC_REZ_OPTS= $(ABI_MAC_REZ_INCS) -d REZ_CARBON -F Carbon -F HIToolbox -useDF

##################################################################
## ABIPKGDIR defines the directory containing the Makefile to use to
## build a set of distribution archives (.deb, .rpm, .tgz, .exe, etc)
## This is relative to $(ABI_ROOT)/src/pkg

ABIPKGDIR	= macosx

# End of macos defs
