#! gmake

##################################################################
##################################################################
## abi_defs.mk --  Makefile definitions for building AbiSource software.
## This is a makefile include.  It should be included after ABI_DEPTH
## is set and before any other declarations.
##
## The general structure of an AbiSource Makefile should be:
##
##        #! gmake
##        ABI_DEPTH=<your depth in source tree from abi/src>
##        include $(ABI_DEPTH)/config/abi_defs.mk
##        <local declarations>
##        include $(ABI_DEPTH)/config/abi_rules.mk
##        <local rules>
##
##################################################################
##################################################################


##################################################################
##################################################################
## Interlude into NSPR makefile system.
## 
## Map ABI_DEPTH (set in makefile that included us) into NSPR's
## MOD_DEPTH, include their config.mk, and fix up any paths as
## necessary.

MOD_DEPTH=$(ABI_DEPTH)/other/nsprpub
include $(MOD_DEPTH)/config/config.mk


##################################################################
##################################################################
## Define ABI_ symbols to help with cross-platform (xp) and
## platform-dependent directory naming.

ifeq ($(OS_ARCH),WINNT)
ABI_NATIVE=	win
else
ABI_NATIVE=	unix
endif

##################################################################
##################################################################
## Define AbiSoftware version

ABI_VERSION=	0_0


##################################################################
##################################################################
## Help for the loader.  In the makefile which builds the program,
## the following three variables:
##
##    ABI_APPLIBS should be for ABI_ versioned things in $(DIST)/lib
##    ABI_OTHLIBS should be for MOD_ versioned things in $(DIST)/lib (from abi/src/other)
##    ABI_LIBS should be for the X11 libraries and the like

ifeq ($(OS_ARCH),WINNT)
EXTRA_LIBS=	$(addprefix $(DIST)/lib/lib,$(addsuffix $(ABI_VERSION)_s.lib,$(ABI_APPLIBS)))	\
		$(addprefix $(DIST)/lib/lib,$(addsuffix $(MOD_VERSION)_s.lib,$(ABI_OTHLIBS)))	\
		$(addsuffix .lib,$(ABI_LIBS))
else
EXTRA_LIBS=	-L$(DIST)/lib 							\
		$(addprefix -l,$(addsuffix $(ABI_VERSION),$(ABI_APPLIBS)))	\
		$(addprefix -l,$(addsuffix $(MOD_VERSION),$(ABI_OTHLIBS)))	\
		$(addprefix -l,$(ABI_LIBS))
endif

##################################################################
##################################################################
