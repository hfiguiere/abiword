This file contains instructions on building, using and developing
AbiWord with bidi-rectional support and is maintained by Tomas Frydrych
<tomas@frydrych.uklinux.net> to whom comments, suggestions, and bug 
fixes of the BiDi features should be directed.

Enabling BiDi during build
--------------------------
use the normal procedure for building AW, but set 
ABI_OPT_BIDI_ENABLED=1 on the make command line.

If you want the default direction to be hardwired to RTL, you can 
further set ABI_OPT_BIDI_RTL_DOMINANT=1. It is also possible to
change default direction at runtime, see below.


Using BiDi
----------
The Bi-Directional extension uses two properties, a character
property 'dir' (direction) and paragraph property 'dom-dir' 
(dominant direction).

The BiDi works in two modes, Unicode mode and user mode. In
Unicode mode the 'direction' property is transparent to the user,
being determined for each character on the basis of its Unicode
value. In user mode the character property 'direction' behaves
as any other, i.e., the user has to explicitely format text for
direction. The Unicode mode is the prefered mode, and mixing the
two modes in creation of a single document is not advised (the 
manual mode might be discontinued in the future).

The mode is selected on the 'Other' tab of the 'Preferences'
dialogue, while direction and dominant direction can be
specified using the 'Font' and 'Paragraph' dialogues or the
two buttons provided on the formating toolbar.

The 'Other' tab also allows to change the default direction (
however, this change will only take place when you create a
new document or restart AbiWord).


TECHNICAL NOTES
===============

The BiDi Problem
----------------

Let's say that the user inputs a string 'abcd XYZ UVW klm' where
small letters represent text that is from left to right (ltr) while
capitals represent text that is right to left (rtl), and that the
overall direction of the document is ltr. The visual representation
of the string on the screen will need to be

  'abcd WVU ZYX klm'

that is, the visual order is different than the order in which the
text was input, the logical order. The algorithm that is used to
work out the visual ordering is straight forward, but cannot be
applied to an arbitrary segment of the text. Rather it has to take into
account always an entire line, and this causes problem for software
that does not draw text in whole lines, such as AbiWord.

A chunk of text which has uniform attributes, such as font or colour,
is in AbiWord terminology called run. In BiDi mode, text in a run
has to be also of consistent direction, with three possible options:
ltr, rtl, and neutral. The last of these applies basically to whitespace,
which derives its actual direction from the context. The direction of
the run is worked out either automatically from the Unicode value of a
character, or is indicated by the user if in manual mode.

The heart of BiDi in AbiWord is built into fp_Line class (guess why?).
Each run of text stores its direction, and this is used by the function
fp_Line::_createMapOfRuns() to calculate the order of the runs belonging
to the particular line. The order is stored in an array that is used
to translate logical coordinances to visual ones and vice versa. Any
operations on the actual text buffer have to use logical order, while
any operations on the screen have to use visual order.

In addition to direction being the property of any chunk of text, each
paragraph has also a new property, in AW called dom-dir (dominant 
direction). This can be either ltr or rtl. The string of text we used 
in the illustration earlier ('abcd XYZ UVW klm') will look differently,
if the overall direction is ltr, say an English document with a Hebrew 
quote in it ('abcd WVU ZYX klm') or rtl, such as Hebrew document with 
an English quote ('klm WVU ZYX abcd'). This paragraph property is in AW
always explicitely specified by the user, we do not use any heuristic 
algorithm that would try to guess (there is really no reliable way to 
guess ...).

This really is all there is to BiDi, just sometimes it makes life more
complicated than this introduction might lead you to believe. For instance
the insertion point can appear on two places on the screen at the same time,
because the the visual position of the next character to be input is the
function of the direction of this character, which we do not know, since
it has not been typed in yet :-).


Developing BiDi
---------------
As of the moment, the BiDi support is experimental and is not 
enabled for the officially distributed binaries. All code that
is BiDi specific must be located inside #ifdef BIDI_ENABLED 
construct. However, we want to share all possible code with the
non-BiDi version, even if that means more shorter #ifdef's; longer
#ifdef/#else sections should be avoided as much as possible.

UT_uint32 foo1;
UT_uint32 foo2;
#ifdef BIDI_ENABLED
UT_uint32 foo3;
#endif

is preferable to

#ifdef BIDI_ENABLED
UT_uint32 foo1;
UT_uint32 foo2;
UT_uint32 foo3;
#else
UT_uint32 foo1;
UT_uint32 foo2;
#endif

because the latter creates two independent branches, and any
changes in the non-BiDi one will not be carried over into the
BiDi branch -- we want to take advantage of any general bug
fixes that the 'non-BiDi people' will do in 'their' code, i.e.,
if someone changes foo1 to UT_sint32, this would not make it
into the BiDi build in the latter version, and what is worse,
we would not know about it at all, since the CVS would just
silently merge the change. 

Any new code should be 'littered' with asserts, because these
are great help in tracing bugs. In particular, put an assert in
before dereferencing any pointers; virtually all SIGSEGVs I have
experienced with BiDi in AW had to do with dereferencing 0.

