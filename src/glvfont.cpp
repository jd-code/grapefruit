
#include "grapefruit.h"

using namespace grapefruit ;

namespace grapefruit
{

    /** @defgroup doxgr_glvfont font rendering functions
     * @warning This API might change a lot still.
     * @brief Those functions and symbols are used to render vector-defined fonts with Open-GL.
     *
     * Externally, only grapefruit::render_string, grapefruit::compute_string_size and grapefruit::loadglvfont are publicly available, all others are for internal use only.
     * The font description is stored in a set of tables of integers, one per char. a table of 256 integer pointers stores the entry point for each char.
     * for rendering chars or computing char-sizes, there are two functions : one that parse the GLMarks description on the fly, another that
     * use a stored display-list or explicitely stored offsets.
     * 
     * @{
     */

//! The description tokens for glvfonts.
    
typedef enum {	ENDLIST		= -100,	    //!< end of the description
		END	    	= -101,	    //!< end of some GL-Object
		LINE_STRIP  	= -102,	    //!< starts a LINE_STRIP GL-object (open), is followed by couples of coordinates until END
		LINE_LOOP   	= -103,	    //!< starts a LINE_LOOP GL-object (closed), is followed by couples of coordinates until END
		LINES	    	= -104,	    //!< some GL lines, is followed by couples of coordinates until END
		POINTS	    	= -105,	    //!< some GL points, is followed by couples of coordinates until END
                                                
		CHAR_STEP   	= -200,	    //!< the step of the char (where to put the next following char)
		CHAR_REWIND 	= -201,	    //!< triggers the offsets to go back to the beginning of the line
		LINE_FEED   	= -202,	    //!< triggers the offseast to go one line down
                                                
		CHAR_WIDTH	= -203,	    //!< DOES NOT work yet ! is supposed to give the char witdth when it is not equal to CHAR_STEP
                                                
		CALL_LIST_REL	= -204,	    //!< calls another chars description, usefull for creating composed chars. the biggest of all offsets will be kept
	     } GLMarks;

    //! @}





int 

  /* \0*/ glfb_000[] = { CHAR_STEP, 5, LINE_LOOP, 1,2, 1,6, 5,6, 5,2, END, LINE_LOOP, 2,3, 2,5, 4,5, 4,3, END, ENDLIST },
  /* \n*/ glfb_010[] = { CHAR_REWIND, LINE_FEED, -9, ENDLIST },

  /*   */ glfb_032[] = { CHAR_STEP, 3, ENDLIST },
  /* ! */ glfb_033[] = { CHAR_STEP, 1, LINE_STRIP, 1,8, 1,2, END, POINTS, 1,1, END, ENDLIST },
  /* " */ glfb_034[] = { CHAR_STEP, 3, LINES, 1,8, 1,6, 2,8, 2,6, END, ENDLIST },
  /* # */ glfb_035[] = { CHAR_STEP, 5, LINES, 2,8, 2,2, 4,8, 4,2, 1,3, 5,3, 1,7, 5,7, END, ENDLIST },
  /* $ */ glfb_036[] = { CALL_LIST_REL, 'S', LINES, 3,1, 3,9, END, ENDLIST },
  /* % */ glfb_037[] = { CHAR_STEP, 5, LINE_LOOP, 1,7, 2,6, 3,7, 2,8, END, LINE_LOOP, 4,2, 5,3, 4,4, 3,3, END, LINES, 1,2, 5,8, END, ENDLIST },
  /* & */ glfb_038[] = { CHAR_STEP, 5, LINE_STRIP, 5,2, 1,6, 1,7, 2,8, 3,7, 3,6, 1,4, 1,3, 2,2, 3,2, 5,4, END, ENDLIST },
  /* ' */ glfb_039[] = { CHAR_STEP, 2, LINES, 1,8, 1,7, END, ENDLIST },
  /* ( */ glfb_040[] = { CHAR_STEP, 2, LINE_STRIP, 2,2, 1,3, 1,7, 2,8, END, ENDLIST },
  /* ) */ glfb_041[] = { CHAR_STEP, 2, LINE_STRIP, 1,2, 2,3, 2,7, 1,8, END, ENDLIST },
  /* * */ glfb_042[] = { CHAR_STEP, 5, LINE_STRIP, 1,5, 5,5, END, LINE_STRIP, 2,3, 4,7, END, LINE_STRIP, 2,7, 4,3, END, ENDLIST },
  /* + */ glfb_043[] = { CHAR_STEP, 5, LINE_STRIP, 1,5, 5,5, END, LINE_STRIP, 3,3, 3,7, END, ENDLIST },
  /* , */ glfb_044[] = { CHAR_STEP, 2, LINE_STRIP, 2,3, 2,2, 1,1, END, ENDLIST },
  /* - */ glfb_045[] = { CHAR_STEP, 4, LINE_STRIP, 1,5, 4,5, END, ENDLIST },
  /* . */ glfb_046[] = { CHAR_STEP, 2, LINES, 1,3, 1,2, END, ENDLIST },
  /* / */ glfb_047[] = { CHAR_STEP, 3, LINE_STRIP, 1,2, 3,8, END, ENDLIST },
  /* 0 */ glfb_048[] = { CHAR_STEP, 5, LINE_STRIP, 5,7, 4,8, 2,8, 1,7, 1,3, 2,2, 4,2, 5,3, 5,7, 1,3, END, ENDLIST },
  /* 1 */ glfb_049[] = { CHAR_STEP, 5, LINE_STRIP, 1,2, 5,2, END, LINE_STRIP, 1,6, 3,8, 3,2, END, ENDLIST },
  /* 2 */ glfb_050[] = { CHAR_STEP, 5, LINE_STRIP, 1,7, 2,8, 4,8, 5,7, 5,6, 1,2, 5,2, END, ENDLIST },
  /* 3 */ glfb_051[] = { CHAR_STEP, 5, LINE_STRIP, 1,8, 5,8, 3,5, 4,5, 5,4, 5,3, 4,2, 2,2, 1,3, END, ENDLIST },
  /* 4 */ glfb_052[] = { CHAR_STEP, 5, LINE_STRIP, 4,2, 4,8, 1,4, 5,4, END, ENDLIST },
  /* 5 */ glfb_053[] = { CHAR_STEP, 5, LINE_STRIP, 1,3, 2,2, 4,2, 5,3, 5,5, 4,6, 1,6, 1,8, 5,8, END, ENDLIST },
  /* 6 */ glfb_054[] = { CHAR_STEP, 5, LINE_STRIP, 4,8, 2,8, 1,7, 1,3, 2,2, 4,2, 5,3, 5,5, 4,6, 2,6, 1,5, END, ENDLIST },
  /* 7 */ glfb_055[] = { CHAR_STEP, 5, LINE_STRIP, 1,8, 5,8, 2,2, END, ENDLIST },
  /* 8 */ glfb_056[] = { CHAR_STEP, 5, LINE_STRIP, 2,6, 1,7, 2,8, 4,8, 5,7, 4,6, 2,6, 1,5, 1,3, 2,2, 4,2, 5,3, 5,5, 4,6, END, ENDLIST },
  /* 9 */ glfb_057[] = { CHAR_STEP, 5, LINE_STRIP, 5,5, 4,4, 2,4, 1,5, 1,7, 2,8, 4,8, 5,7, 5,3, 4,2, 2,2, END, ENDLIST },
  /* : */ glfb_058[] = { CHAR_STEP, 2, LINES, 2,6, 2,5, 2,3, 2,2, END, ENDLIST },
  /* ; */ glfb_059[] = { CHAR_STEP, 2, LINES, 2,6, 2,5, END, LINE_STRIP, 2,3, 2,2, 1,1, END, ENDLIST },
  /* < */ glfb_060[] = { CHAR_STEP, 4, LINE_STRIP, 4,8, 1,5, 4,2, END, ENDLIST },
  /* = */ glfb_061[] = { CHAR_STEP, 4, LINE_STRIP, 1,4, 4,4, END, LINE_STRIP, 1,6, 4,6, END, ENDLIST },
  /* > */ glfb_062[] = { CHAR_STEP, 4, LINE_STRIP, 1,8, 4,5, 1,2, END, ENDLIST },
  /* ? */ glfb_063[] = { CHAR_STEP, 5, LINE_STRIP, 1,7, 2,8, 4,8, 5,7, 5,6, 3,3, 3,2, END, POINTS, 3,1, END, ENDLIST },

//  /* @ */ glfb_064[] = { CHAR_STEP, 6, LINE_STRIP, 1,4, 2,3, 3,3, 4,4, 4,5, 3,6, 2,6, 1,5, 1,3, 2,2, 4,2, 5,3, 5,6, 4,7, 2,7, 1,6, END, ENDLIST },
//  /* @ */ glfb_064[] = { CHAR_STEP, 7, LINES, 6,6, 6,4, END, LINE_STRIP, 6,5, 5,6, 3,6, 2,5, 2,4, 3,3, 5,3, 6,4, 7,3, 7,7, 6,8, 2,8, 1,7, 1,3, 2,2, 6,2, END, ENDLIST },
//  /* @ */ glfb_064[] = { CHAR_STEP, 7, LINES, 6,5, 6,3, END, LINE_STRIP, 6,4, 5,5, 3,5, 2,4, 2,3, 3,2, 5,2, 6,3, 7,2, 7,6, 6,7, 2,7, 1,6, 1,2, 2,1, 6,1, END, ENDLIST },
  /* @ */ glfb_064[] = { CHAR_STEP, 7, LINES, 5,5, 5,3, END, LINE_STRIP, 5,4, 4,5, 3,5, 2,4, 2,3, 3,2, 4,2, 5,3, 6,2, 6,6, 5,7, 2,7, 1,6, 1,2, 2,1, 6,1, END, ENDLIST },
  /* A */ glfb_065[] = { CHAR_STEP, 7, LINE_STRIP, 1,2, 4,8, 7,2, END, LINE_STRIP, 2,4, 6,4, END, ENDLIST },
  /* B */ glfb_066[] = { CHAR_STEP, 5, LINE_STRIP, 1,2, 4,2, 5,3, 5,4, 4,5, 1,5, END, LINE_STRIP, 1,2, 1,8, END, LINE_STRIP, 1,8, 4,8, 5,7, 5,6, 4,5, END, ENDLIST },
  /* C */ glfb_067[] = { CHAR_STEP, 6, LINE_STRIP, 6,7, 5,8, 2,8, 1,7, 1,3, 2,2, 5,2, 6,3, END, ENDLIST },
  /* D */ glfb_068[] = { CHAR_STEP, 5, LINE_STRIP, 1,8, 4,8, 5,7, 5,3, 4,2, 1,2, END, LINE_STRIP, 1,2, 1,8, END, ENDLIST },
  /* E */ glfb_069[] = { CHAR_STEP, 5, LINE_STRIP, 5,2, 1,2, 1,8, 5,8, END, LINE_STRIP, 1,5, 3,5, END, ENDLIST },
  /* F */ glfb_070[] = { CHAR_STEP, 5, LINE_STRIP, 1,2, 1,8, 5,8, END, LINE_STRIP, 1,5, 3,5, END, ENDLIST },
  /* G */ glfb_071[] = { CHAR_STEP, 6, LINE_STRIP, 6,7, 5,8, 2,8, 1,7, 1,3, 2,2, 5,2, 6,3, 6,5, 5,5, END, ENDLIST },
  /* H */ glfb_072[] = { CHAR_STEP, 5, LINE_STRIP, 1,2, 1,8, END, LINE_STRIP, 1,5, 5,5, END, LINE_STRIP, 5,2, 5,8, END, ENDLIST },
  /* I */ glfb_073[] = { CHAR_STEP, 1, LINE_STRIP, 1,2, 1,8, END, ENDLIST },
  /* J */ glfb_074[] = { CHAR_STEP, 5, LINE_STRIP, 1,5, 1,3, 2,2, 4,2, 5,3, 5,8, END, ENDLIST },
  /* K */ glfb_075[] = { CHAR_STEP, 4, LINE_STRIP, 1,2, 1,8, END, LINE_STRIP, 4,2, 1,5, 4,8, END, ENDLIST },
  /* L */ glfb_076[] = { CHAR_STEP, 5, LINE_STRIP, 5,2, 1,2, 1,8, END, ENDLIST },
  /* M */ glfb_077[] = { CHAR_STEP, 7, LINE_STRIP, 1,2, 1,8, 4,5, 7,8, 7,2, END, ENDLIST },
  /* N */ glfb_078[] = { CHAR_STEP, 5, LINE_STRIP, 1,2, 1,8, 5,2, 5,8, END, ENDLIST },
  /* O */ glfb_079[] = { CHAR_STEP, 6, LINE_LOOP, 6,7, 5,8, 2,8, 1,7, 1,3, 2,2, 5,2, 6,3, END, ENDLIST },
  /* P */ glfb_080[] = { CHAR_STEP, 5, LINE_STRIP, 1,2, 1,8, END, LINE_STRIP, 1,8, 4,8, 5,7, 5,6, 4,5, 1,5, END, ENDLIST },
  /* Q */ glfb_081[] = { CHAR_STEP, 6, LINE_STRIP, 6,3, 6,7, 5,8, 2,8, 1,7, 1,3, 2,2, 5,2, 6,3, 4,5, END, ENDLIST },
  /* R */ glfb_082[] = { CHAR_STEP, 5, LINE_STRIP, 1,2, 1,8, END, LINE_STRIP, 1,8, 4,8, 5,7, 5,6, 4,5, 1,5, END, LINE_STRIP, 5,2, 2,5, END, ENDLIST },
  /* S */ glfb_083[] = { CHAR_STEP, 6, LINE_STRIP, 1,3, 2,2, 5,2, 6,3, 6,4, 5,5, 2,5, 1,6, 1,7, 2,8, 5,8, 6,7, END, ENDLIST },
  /* T */ glfb_084[] = { CHAR_STEP, 5, LINE_STRIP, 1,8, 5,8, END, LINE_STRIP, 3,8, 3,2, END, ENDLIST },
  /* U */ glfb_085[] = { CHAR_STEP, 5, LINE_STRIP, 1,8, 1,3, 2,2, 4,2, 5,3, 5,8, END, ENDLIST },
  /* V */ glfb_086[] = { CHAR_STEP, 5, LINE_STRIP, 1,8, 3,2, 5,8, END, ENDLIST },
  /* W */ glfb_087[] = { CHAR_STEP, 9, LINE_STRIP, 1,8, 3,2, 5,8, 7,2, 9,8, END, ENDLIST },
  /* X */ glfb_088[] = { CHAR_STEP, 5, LINE_STRIP, 1,8, 5,2, END, LINE_STRIP, 5,8, 1,2, END, ENDLIST },
  /* Y */ glfb_089[] = { CHAR_STEP, 5, LINE_STRIP, 1,8, 3,5, 5,8, END, LINE_STRIP, 3,5, 3,2, END, ENDLIST },
  /* Z */ glfb_090[] = { CHAR_STEP, 5, LINE_STRIP, 1,8, 5,8, 1,2, 5,2, END, ENDLIST },
  /* [ */ glfb_091[] = { CHAR_STEP, 3, LINE_STRIP, 3,8, 1,8, 1,2, 3,2, END, ENDLIST },
  /* \ */ glfb_092[] = { CHAR_STEP, 3, LINE_STRIP, 3,2, 1,8, END, ENDLIST },
  /* ] */ glfb_093[] = { CHAR_STEP, 3, LINE_STRIP, 1,8, 3,8, 3,2, 1,2, END, ENDLIST },
  /* ^ */ glfb_094[] = { CHAR_STEP, 5, LINE_STRIP, 1,7, 3,8, 5,7, END, ENDLIST },
  /* _ */ glfb_095[] = { CHAR_STEP, 5, LINE_STRIP, 1,0, 5,0, END, ENDLIST },

  /* ` */ glfb_096[] = { CHAR_STEP, 2, LINE_STRIP, 1,8, 2,7, END, ENDLIST },
  /* a */ glfb_097[] = { CHAR_STEP, 5, LINE_STRIP, 2,6, 4,6, 5,5, 5,3, 4,2, 2,2, 1,3, 2,4, 4,4, 5,3, 5,2, END, ENDLIST },
  /* b */ glfb_098[] = { CHAR_STEP, 5, LINE_STRIP, 1,8, 1,3, 2,2, 4,2, 5,3, 5,5, 4,6, 2,6, 1,5, END, ENDLIST },
  /* c */ glfb_099[] = { CHAR_STEP, 5, LINE_STRIP, 5,3, 4,2, 2,2, 1,3, 1,5, 2,6, 4,6, 5,5, END, ENDLIST },
  /* d */ glfb_100[] = { CHAR_STEP, 5, LINE_STRIP, 5,8, 5,3, 4,2, 2,2, 1,3, 1,5, 2,6, 4,6, 5,5, 5,2, END, ENDLIST },
  /* e */ glfb_101[] = { CHAR_STEP, 5, LINE_STRIP, 5,3, 4,2, 2,2, 1,3, 1,5, 2,6, 4,6, 5,5, 5,4, 1,4, END, ENDLIST },
// JDJDJDJD jensuisla JENSUILA
//  /* f */ glfb_102[] = { CHAR_STEP, 2, CHAR_WIDTH, 5, LINE_STRIP, 1,2, 1,7, 2,8, 3,8, END, LINE_STRIP, 1,5, 2,5, END, ENDLIST },
  /* f */ glfb_102[] = { CHAR_STEP, 2, LINE_STRIP, 1,2, 1,7, 2,8, 3,8, END, LINE_STRIP, 1,5, 2,5, END, ENDLIST },
  /* g */ glfb_103[] = { CHAR_STEP, 5, LINE_STRIP, 5,3, 4,2, 2,2, 1,3, 1,5, 2,6, 4,6, 5,5, 5,1, 4,0, 2,0, END, LINE_STRIP, 5,5, 5,6, END, ENDLIST },
  /* h */ glfb_104[] = { CHAR_STEP, 5, LINE_STRIP, 1,2, 1,8, END, LINE_STRIP, 5,2, 5,5, 4,6, 2,6, 1,5, END, ENDLIST },
  /* i */ glfb_105[] = { CHAR_STEP, 2, LINE_STRIP, 2,2, 1,3, 1,6, END, POINTS, 1,7, END, ENDLIST },
  /* j */ glfb_106[] = { CHAR_STEP, 2, LINE_STRIP, 0,0, 1,0, 2,1, 2,6, END, POINTS, 2,7, END, ENDLIST },
  /* k */ glfb_107[] = { CHAR_STEP, 3, LINE_STRIP, 1,2, 1,8, END, LINE_STRIP, 3,2, 1,4, 3,6, END, ENDLIST },
  /* l */ glfb_108[] = { CHAR_STEP, 2, LINE_STRIP, 2,2, 1,3, 1,8, END, ENDLIST },
  /* m */ glfb_109[] = { CHAR_STEP, 9, LINE_STRIP, 1,2, 1,6, END, LINE_STRIP, 1,5, 2,6, 4,6, 5,5, 5,2, END, LINE_STRIP, 5,5, 6,6, 8,6, 9,5, 9,2, END, ENDLIST },
  /* n */ glfb_110[] = { CHAR_STEP, 5, LINE_STRIP, 1,2, 1,6, END, LINE_STRIP, 1,5, 2,6, 4,6, 5,5, 5,2, END, ENDLIST },
  /* o */ glfb_111[] = { CHAR_STEP, 5, LINE_STRIP, 5,3, 4,2, 2,2, 1,3, 1,5, 2,6, 4,6, 5,5, 5,3, END, ENDLIST },
  /* p */ glfb_112[] = { CHAR_STEP, 5, LINE_STRIP, 1,6, 1,0, END, LINE_STRIP, 1,5, 2,6, 4,6, 5,5, 5,3, 4,2, 2,2, 1,3, END, ENDLIST },
  /* q */ glfb_113[] = { CHAR_STEP, 5, LINE_STRIP, 5,6, 5,0, END, LINE_STRIP, 5,3, 4,2, 2,2, 1,3, 1,5, 2,6, 4,6, 5,5, END, ENDLIST },
  /* r */ glfb_114[] = { CHAR_STEP, 3, LINE_STRIP, 1,2, 1,6, END, LINE_STRIP, 1,5, 2,6, 3,6, END, ENDLIST },
  /* s */ glfb_115[] = { CHAR_STEP, 5, LINE_STRIP, 1,3, 2,2, 4,2, 5,3, 4,4, 2,4, 1,5, 2,6, 4,6, 5,5, END, ENDLIST },
  /* t */ glfb_116[] = { CHAR_STEP, 2, LINE_STRIP, 2,2, 1,3, 1,8, END, LINE_STRIP, 1,7, 2,7, END, ENDLIST },
  /* u */ glfb_117[] = { CHAR_STEP, 5, LINE_STRIP, 1,6, 1,3, 2,2, 4,2, 5,3, 5,6, END, ENDLIST },
  /* v */ glfb_118[] = { CHAR_STEP, 5, LINE_STRIP, 1,6, 3,2, 5,6, END, ENDLIST },
  /* w */ glfb_119[] = { CHAR_STEP, 9, LINE_STRIP, 1,6, 3,2, 5,6, 7,2, 9,6, END, ENDLIST },
  /* x */ glfb_120[] = { CHAR_STEP, 5, LINE_STRIP, 1,6, 5,2, END, LINE_STRIP, 5,6, 1,2, END, ENDLIST },
  /* y */ glfb_121[] = { CHAR_STEP, 5, LINE_STRIP, 1,6, 1,3, 2,2, 4,2, 5,3, END, LINE_STRIP, 5,6, 5,1, 4,0, 2,0, END, ENDLIST },
  /* z */ glfb_122[] = { CHAR_STEP, 5, LINE_STRIP, 1,6, 5,6, 1,2, 5,2, END, ENDLIST },
  /* { */ glfb_123[] = { CHAR_STEP, 3, LINE_STRIP, 3,0, 2,1, 2,3, 1,4, 2,5, 2,7, 3,8, END, ENDLIST },
  /* | */ glfb_124[] = { CHAR_STEP, 3, LINES, 2,0, 2,8, END, ENDLIST },
  /* } */ glfb_125[] = { CHAR_STEP, 3, LINE_STRIP, 1,0, 2,1, 2,3, 3,4, 2,5, 2,7, 1,8, END, ENDLIST },
  /* ~ */ glfb_126[] = { CHAR_STEP, 5, LINE_STRIP, 1,5, 2,6, 4,4, 5,5, END, ENDLIST },

  /*  */ glfb_160[] = { CHAR_STEP, 3, LINES, 2,0, 2,8, END, ENDLIST },
  /* ¡ */ glfb_161[] = { CHAR_STEP, 1, LINE_STRIP, 1,7, 1,2, END, POINTS, 1,8, END, ENDLIST },
  /* ¢ */ glfb_162[] = { CALL_LIST_REL, 'c', LINES, 3,1, 3,7, END, ENDLIST },
  /* £ */ glfb_163[] = { CHAR_STEP, 5, LINE_STRIP, 5,2, 4,3, 1,3, 1,2, 2,2, 2,7, 3,8, 5,8, END, LINES, 2,5, 4,5, END, ENDLIST },
  /* ¤ */ glfb_164[] = { CHAR_STEP, 5, LINE_STRIP, 5,3, 4,2, 2,2, 1,4, 1,6, 2,8, 4,8, 5,7, END, LINES, 1,4, 3,4, 1,5, 3,5, END, ENDLIST },
  /* ¥ */ glfb_165[] = { CALL_LIST_REL, 'Y', LINES, 2,5, 4,5, 2,4, 4,4, END, ENDLIST },
  /* ¦ */ glfb_166[] = { CALL_LIST_REL, 'S', LINE_STRIP, 3,9, 4,8, 5,9, END, ENDLIST },
  /* § */ glfb_167[] = { CHAR_STEP, 3, LINE_STRIP, 1,3, 2,2, 3,3, 1,5, 2,6, END, LINE_STRIP, 3,7, 2,8, 1,7, 3,5, 2,4, END, ENDLIST },
  /* ¨ */ glfb_168[] = { CALL_LIST_REL, 's', LINE_STRIP, 2,7, 3,6, 4,7, END, ENDLIST },
  /* © */ glfb_169[] = { CALL_LIST_REL, 'O', LINE_STRIP, 5,4, 4,3, 3,3, 2,4, 2,6, 3,7, 4,7, 5,6, END, ENDLIST },
  /* ª */ glfb_170[] = { ENDLIST },
  /* « */ glfb_171[] = { ENDLIST },
  /* ¬ */ glfb_172[] = { ENDLIST },
  /* ­ */ glfb_173[] = { ENDLIST },
  /* ® */ glfb_174[] = { ENDLIST },
  /* ¯ */ glfb_175[] = { ENDLIST },
  /* ° */ glfb_176[] = { ENDLIST },
  /* ± */ glfb_177[] = { ENDLIST },
  /* ² */ glfb_178[] = { ENDLIST },
  /* ³ */ glfb_179[] = { ENDLIST },
  /* ´ */ glfb_180[] = { ENDLIST },
  /* µ */ glfb_181[] = { ENDLIST },
  /* ¶ */ glfb_182[] = { ENDLIST },
  /* · */ glfb_183[] = { ENDLIST },
  /* ¸ */ glfb_184[] = { ENDLIST },
  /* ¹ */ glfb_185[] = { ENDLIST },
  /* º */ glfb_186[] = { ENDLIST },
  /* » */ glfb_187[] = { ENDLIST },
  /* ¼ */ glfb_188[] = { ENDLIST },
  /* ½ */ glfb_189[] = { ENDLIST },
  /* ¾ */ glfb_190[] = { ENDLIST },
  /* ¿ */ glfb_191[] = { ENDLIST },
  /* À */ glfb_192[] = { ENDLIST },
  /* Á */ glfb_193[] = { ENDLIST },
  /* Â */ glfb_194[] = { ENDLIST },
  /* Ã */ glfb_195[] = { ENDLIST },
  /* Ä */ glfb_196[] = { ENDLIST },
  /* Å */ glfb_197[] = { ENDLIST },
  /* Æ */ glfb_198[] = { ENDLIST },
  /* Ç */ glfb_199[] = { ENDLIST },
  /* È */ glfb_200[] = { ENDLIST },
  /* É */ glfb_201[] = { ENDLIST },
  /* Ê */ glfb_202[] = { ENDLIST },
  /* Ë */ glfb_203[] = { ENDLIST },
  /* Ì */ glfb_204[] = { ENDLIST },
  /* Í */ glfb_205[] = { ENDLIST },
  /* Î */ glfb_206[] = { ENDLIST },
  /* Ï */ glfb_207[] = { ENDLIST },
  /* Ð */ glfb_208[] = { ENDLIST },
  /* Ñ */ glfb_209[] = { ENDLIST },
  /* Ò */ glfb_210[] = { ENDLIST },
  /* Ó */ glfb_211[] = { ENDLIST },
  /* Ô */ glfb_212[] = { ENDLIST },
  /* Õ */ glfb_213[] = { ENDLIST },
  /* Ö */ glfb_214[] = { ENDLIST },
  /* × */ glfb_215[] = { ENDLIST },
  /* Ø */ glfb_216[] = { ENDLIST },
  /* Ù */ glfb_217[] = { ENDLIST },
  /* Ú */ glfb_218[] = { ENDLIST },
  /* Û */ glfb_219[] = { ENDLIST },
  /* Ü */ glfb_220[] = { ENDLIST },
  /* Ý */ glfb_221[] = { ENDLIST },
  /* Þ */ glfb_222[] = { ENDLIST },
  /* ß */ glfb_223[] = { ENDLIST },
  /* à */ glfb_224[] = { ENDLIST },
  /* á */ glfb_225[] = { ENDLIST },
  /* â */ glfb_226[] = { ENDLIST },
  /* ã */ glfb_227[] = { ENDLIST },
  /* ä */ glfb_228[] = { ENDLIST },
  /* å */ glfb_229[] = { ENDLIST },
  /* æ */ glfb_230[] = { ENDLIST },
  /* ç */ glfb_231[] = { ENDLIST },
  /* è */ glfb_232[] = { ENDLIST },
  /* é */ glfb_233[] = { ENDLIST },
  /* ê */ glfb_234[] = { ENDLIST },
  /* ë */ glfb_235[] = { ENDLIST },
  /* ì */ glfb_236[] = { ENDLIST },
  /* í */ glfb_237[] = { ENDLIST },
  /* î */ glfb_238[] = { ENDLIST },
  /* ï */ glfb_239[] = { ENDLIST },
  /* ð */ glfb_240[] = { ENDLIST },
  /* ñ */ glfb_241[] = { ENDLIST },
  /* ò */ glfb_242[] = { ENDLIST },
  /* ó */ glfb_243[] = { ENDLIST },
  /* ô */ glfb_244[] = { ENDLIST },
  /* õ */ glfb_245[] = { ENDLIST },
  /* ö */ glfb_246[] = { ENDLIST },
  /* ÷ */ glfb_247[] = { ENDLIST },
  /* ø */ glfb_248[] = { ENDLIST },
  /* ù */ glfb_249[] = { ENDLIST },
  /* ú */ glfb_250[] = { ENDLIST },
  /* û */ glfb_251[] = { ENDLIST },
  /* ü */ glfb_252[] = { ENDLIST },
  /* ý */ glfb_253[] = { ENDLIST },
  /* þ */ glfb_254[] = { ENDLIST },
  /* ÿ */ glfb_255[] = { ENDLIST };


int *glfb[] = {
    &glfb_000[0], NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,        
    NULL,         NULL,         &glfb_010[0], NULL,         &glfb_010[0], NULL,         NULL,         NULL,        
    NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,        
    NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,        
    &glfb_032[0], &glfb_033[0], &glfb_034[0], &glfb_035[0], &glfb_036[0], &glfb_037[0], &glfb_038[0], &glfb_039[0],
    &glfb_040[0], &glfb_041[0], &glfb_042[0], &glfb_043[0], &glfb_044[0], &glfb_045[0], &glfb_046[0], &glfb_047[0],
    &glfb_048[0], &glfb_049[0], &glfb_050[0], &glfb_051[0], &glfb_052[0], &glfb_053[0], &glfb_054[0], &glfb_055[0],
    &glfb_056[0], &glfb_057[0], &glfb_058[0], &glfb_059[0], &glfb_060[0], &glfb_061[0], &glfb_062[0], &glfb_063[0],
    &glfb_064[0], &glfb_065[0], &glfb_066[0], &glfb_067[0], &glfb_068[0], &glfb_069[0], &glfb_070[0], &glfb_071[0],
    &glfb_072[0], &glfb_073[0], &glfb_074[0], &glfb_075[0], &glfb_076[0], &glfb_077[0], &glfb_078[0], &glfb_079[0],
    &glfb_080[0], &glfb_081[0], &glfb_082[0], &glfb_083[0], &glfb_084[0], &glfb_085[0], &glfb_086[0], &glfb_087[0],
    &glfb_088[0], &glfb_089[0], &glfb_090[0], &glfb_091[0], &glfb_092[0], &glfb_093[0], &glfb_094[0], &glfb_095[0],
    &glfb_096[0], &glfb_097[0], &glfb_098[0], &glfb_099[0], &glfb_100[0], &glfb_101[0], &glfb_102[0], &glfb_103[0],
    &glfb_104[0], &glfb_105[0], &glfb_106[0], &glfb_107[0], &glfb_108[0], &glfb_109[0], &glfb_110[0], &glfb_111[0],
    &glfb_112[0], &glfb_113[0], &glfb_114[0], &glfb_115[0], &glfb_116[0], &glfb_117[0], &glfb_118[0], &glfb_119[0],
    &glfb_120[0], &glfb_121[0], &glfb_122[0], &glfb_123[0], &glfb_124[0], &glfb_125[0], &glfb_126[0], NULL,        
    NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,        
    NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,        
    NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,        
    NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,         NULL,        
    &glfb_160[0], &glfb_161[0], &glfb_162[0], &glfb_163[0], &glfb_164[0], &glfb_165[0], &glfb_166[0], &glfb_167[0],
    &glfb_168[0], &glfb_169[0], &glfb_170[0], &glfb_171[0], &glfb_172[0], &glfb_173[0], &glfb_174[0], &glfb_175[0],
    &glfb_176[0], &glfb_177[0], &glfb_178[0], &glfb_179[0], &glfb_180[0], &glfb_181[0], &glfb_182[0], &glfb_183[0],
    &glfb_184[0], &glfb_185[0], &glfb_186[0], &glfb_187[0], &glfb_188[0], &glfb_189[0], &glfb_190[0], &glfb_191[0],
    &glfb_192[0], &glfb_193[0], &glfb_194[0], &glfb_195[0], &glfb_196[0], &glfb_197[0], &glfb_198[0], &glfb_199[0],
    &glfb_200[0], &glfb_201[0], &glfb_202[0], &glfb_203[0], &glfb_204[0], &glfb_205[0], &glfb_206[0], &glfb_207[0],
    &glfb_208[0], &glfb_209[0], &glfb_210[0], &glfb_211[0], &glfb_212[0], &glfb_213[0], &glfb_214[0], &glfb_215[0],
    &glfb_216[0], &glfb_217[0], &glfb_218[0], &glfb_219[0], &glfb_220[0], &glfb_221[0], &glfb_222[0], &glfb_223[0],
    &glfb_224[0], &glfb_225[0], &glfb_226[0], &glfb_227[0], &glfb_228[0], &glfb_229[0], &glfb_230[0], &glfb_231[0],
    &glfb_232[0], &glfb_233[0], &glfb_234[0], &glfb_235[0], &glfb_236[0], &glfb_237[0], &glfb_238[0], &glfb_239[0],
    &glfb_240[0], &glfb_241[0], &glfb_242[0], &glfb_243[0], &glfb_244[0], &glfb_245[0], &glfb_246[0], &glfb_247[0],
    &glfb_248[0], &glfb_249[0], &glfb_250[0], &glfb_251[0], &glfb_252[0], &glfb_253[0], &glfb_254[0], &glfb_255[0],
    NULL
} ;

    /** \addtogroup doxgr_glvfont font rendering functions
     * @{
     */

// --------------------------------------------------------------------------------------------------------------------------
// non exported globals, used only via the exported functions of this module

GLint dl_glvfont = 0;			    //!< if non-null, the first GL-handle of the display-lists for chars
bool dl_isvalid = false;		    //!< is grapefruit::dl_glvfont a valid handle ? grapefruit::loadglvfont can (re-)validate it.

int glvxoff[256],			    //!< the horizontal offsets for each chars
    glvyoff [256];			    //!< the vertical offsets for each chars
bool  glvoff_isvalid = false;		    //!< are the offsets stored in glvxoff and glvyoff valid ? grapefruit::loadglvfont can (re-)validate it.



//! renders the char c and updates accordingly the horizontal and vertical offsets, doesn't use any DL

void render_char_from_table (int c, double &xoff, double &yoff)
{
    GLMarks * p = (GLMarks *) (glfb [c]);   // JDJDJDJD some controls about overflow are missing here !

    double  xstep = 0,
	    ystep = 0,
	    tx, ty;

    if (p == NULL)
	p = (GLMarks *) (glfb[0]);

    while (*p != ENDLIST) {
	switch (*p) {
	    case CALL_LIST_REL:
		p++;
		tx = 0, ty = 0;
		render_char_from_table (*p, tx, ty);
		if (tx > xstep)
		    xstep = tx;
		if (ty > ystep)
		    ystep = ty;
		break;

	    case CHAR_STEP:
		p++;
		xstep += *p;
		break;
		
	    case CHAR_REWIND:
		// xstep -= xoff;
		break;
		
	    case LINE_FEED:
		p++;
		ystep += *p;
		break;
		
	    case LINES:
		p++;
		glBegin (GL_LINES);
		while (*p >= 0) {
		    glVertex2f (*p, *(p+1));
		    p += 2;
		}
		if (*p != END)
		    bzouzerr << "missing END nopcode in glfb[" << c << "]" << endl ;
		glEnd();
		break;

	    case POINTS:
		p++;
		glBegin (GL_POINTS);
		while (*p >= 0) {
		    glVertex2f (*p, *(p+1));
		    p += 2;
		}
		if (*p != END)
		    bzouzerr << "missing END nopcode in glfb[" << c << "]" << endl ;
		glEnd();
		break;

	    case LINE_LOOP:
		p++;
		glBegin (GL_LINE_LOOP);
		while (*p >= 0) {
		    glVertex2f (*p, *(p+1));
		    p += 2;
		}
		if (*p != END)
		    bzouzerr << "missing END nopcode in glfb[" << c << "]" << endl ;
		glEnd();
		break;

	    case LINE_STRIP:
		p++;
		glBegin (GL_LINE_STRIP);
		while (*p >= 0) {
		    glVertex2f (*p, *(p+1));
		    p += 2;
		}
		if (*p != END)
		    bzouzerr << "missing END nopcode in glfb[" << c << "]" << endl ;
		glEnd();
		break;

	    case END:
		bzouzerr << "unexpected END nopcode in glfb[" << c << "]" << endl ;
		break;
		
	    case ENDLIST:
		bzouzerr << "unexpected ENDLIST nopcode in glfb[" << c << "]" << endl ;
		break;

	    default:
		bzouzerr << "unknown opcode " << *p << " in glfb[" << c << "]" << endl ;
		break;
	}
	p++;
    }

    xoff += xstep, yoff += ystep;
}


//! computes and updates the horizontal and vertical offsets associated with char c, doesn't use any DL nor any cached datas

void compute_char_size_from_table (int c, double &xoff, double &yoff)
{
    GLMarks * p = (GLMarks *) (glfb [c]);   // JDJDJDJD some controls about overflow are missing here !

    double  xstep = 0,
	    ystep = 0,
	    tx, ty;

    if (p == NULL)
	p = (GLMarks *) (glfb[0]);

    while (*p != ENDLIST) {
	switch (*p) {
	    case CALL_LIST_REL:
		p++;
		tx = 0, ty = 0;
		compute_char_size_from_table (*p, tx, ty);
		if (tx > xstep)
		    xstep = tx;
		if (ty > ystep)
		    ystep = ty;
		break;

	    case CHAR_STEP:
		p++;
		xstep += *p;
		break;
		
	    case CHAR_REWIND:
		// xstep -= xoff;
		break;
		
	    case LINE_FEED:
		p++;
		ystep += *p;
		break;
		
	    case LINES:
		p++;
		// glBegin (GL_LINES);
		while (*p >= 0) {
		    // glVertex2f (*p, *(p+1));
		    p += 2;
		}
		if (*p != END)
		    bzouzerr << "missing END nopcode in glfb[" << c << "]" << endl ;
		// glEnd();
		break;

	    case POINTS:
		p++;
		// glBegin (GL_POINTS);
		while (*p >= 0) {
		    // glVertex2f (*p, *(p+1));
		    p += 2;
		}
		if (*p != END)
		    bzouzerr << "missing END nopcode in glfb[" << c << "]" << endl ;
		// glEnd();
		break;

	    case LINE_LOOP:
		p++;
		// glBegin (GL_LINE_LOOP);
		while (*p >= 0) {
		    // glVertex2f (*p, *(p+1));
		    p += 2;
		}
		if (*p != END)
		    bzouzerr << "missing END nopcode in glfb[" << c << "]" << endl ;
		// glEnd();
		break;

	    case LINE_STRIP:
		p++;
		// glBegin (GL_LINE_STRIP);
		while (*p >= 0) {
		    glVertex2f (*p, *(p+1));
		    p += 2;
		}
		if (*p != END)
		    bzouzerr << "missing END nopcode in glfb[" << c << "]" << endl ;
		// glEnd();
		break;

	    case END:
		bzouzerr << "unexpected END nopcode in glfb[" << c << "]" << endl ;
		break;
		
	    case ENDLIST:
		bzouzerr << "unexpected ENDLIST nopcode in glfb[" << c << "]" << endl ;
		break;

	    default:
		bzouzerr << "unknown opcode " << *p << " in glfb[" << c << "]" << endl ;
		break;
	}
	p++;
    }

    xoff += xstep, yoff += ystep;
}


//! renders the char c and updates accordingly the horizontal and vertical offsets, uses DL if already stored via grapefruit::loadglvfont

void render_char (int c, double &xoff, double &yoff)
{
    glPushMatrix();
    glTranslatef (xoff, yoff, 0);
    if (dl_isvalid) {
	glCallList (dl_glvfont + c);	    // JDJDJDJD some controls about overflow are missing here !
	xoff += glvxoff[c],
	yoff += glvyoff[c];
    } else {
	render_char_from_table (c, xoff, yoff);
    }
    glPopMatrix();

    if (c == 10)    // CHAR_REWIND
	xoff = 0;
}


//! computes and updates the horizontal and vertical offsets associated with char c, may use cached datas already loaded with grapefruit::loadglvfont, doesn't use any DL though.

void compute_char_size (int c, double &xoff, double &yoff)
{
    if (glvoff_isvalid) {
	xoff += glvxoff[c],
	yoff += glvyoff[c];
    } else {
	compute_char_size_from_table (c, xoff, yoff);
    }

    if (c == 10)    // CHAR_REWIND
	xoff = 0;
}


//! renders the chars with glv-fonts, may use DLs if previously loaded by grapefruit::loadglvfont.

Vector3 render_string (const string &s, double addstep)
{
    string::const_iterator si;
    double  xoff = 0,
	    yoff = 0;

    render_char (10, xoff, yoff);   // line-feed prepended...
    for (si=s.begin() ; si!=s.end() ; si++) {
	render_char (((int)*si) & 0xff, xoff, yoff);
	xoff += addstep;
    }
    return Vector3(xoff, yoff, 0.0);
}


//! computes the size that a rendered string would occupy (x is assumed horizontal and y vertical, writing from left to righ, bup to bottom), may use cached datas if grapefruit::loadglvfont was called once before.

Vector3 compute_string_size (const string &s, double addstep)
{
    string::const_iterator si;
    double  xoff = 0.0,
	    yoff = 0.0,
	    mxoff = 0.0,
	    myoff = 0.0;

    compute_char_size (10, xoff, yoff);   // line-feed prepended...
    for (si=s.begin() ; si!=s.end() ; si++) {
	compute_char_size (((int)*si) & 0xff, xoff, yoff);
	if (xoff > mxoff)
	    mxoff = xoff;
	if (yoff < myoff)
	    myoff = yoff;
	xoff += addstep;
    }
    return Vector3(mxoff, myoff, 0.0);
}


//! loads or updates display-lists (DLs starting at grapefruit::dl_glvfont) used for rendering the glv-font, also updates the offsets (grapefruit::glvxoff and grapefruit::glvyoff) associated with each chars.

int loadglvfont (void)
{
    dl_isvalid = false;
    dl_glvfont = glGenLists (256);		    // JDJDJDJD we should test the result here

    int i;
    double xoff , yoff;
    for (i=0 ; i<256 ; i++) {
	xoff = 0.0, yoff = 0.0;
	glNewList (dl_glvfont+i, GL_COMPILE);
	render_char_from_table (i, xoff, yoff);
	glEndList ();
	glvxoff[i] = (int)xoff;
	glvyoff[i] = (int)yoff;
    }
    dl_isvalid = true;
    glvoff_isvalid = true;

    return 0;
}

    //! @}
}

