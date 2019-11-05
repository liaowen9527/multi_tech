#pragma once

#define VT52_PLUS
#define CL_ANSIMIN	0x0001	       /* Codes in all ANSI like terminals. */
#define CL_VT100	0x0002	       /* VT100 */
#define CL_VT100AVO	0x0004	       /* VT100 +AVO; 132x24 (not 132x14) & attrs */
#define CL_VT102	0x0008	       /* VT102 */
#define CL_VT220	0x0010	       /* VT220 */
#define CL_VT320	0x0020	       /* VT320 */
#define CL_VT420	0x0040	       /* VT420 */
#define CL_VT510	0x0080	       /* VT510, NB VT510 includes ANSI */
#define CL_VT340TEXT	0x0100	       /* VT340 extensions that appear in the VT420 */
#define CL_SCOANSI	0x1000	       /* SCOANSI not in ANSIMIN. */
#define CL_ANSI		0x2000	       /* ANSI ECMA-48 not in the VT100..VT420 */
#define CL_OTHER	0x4000	       /* Others, Xterm, linux, putty, dunno, etc */

#define ANSIMIN	0x0001	       /* Codes in all ANSI like terminals. */
#define VT100	0x0002	       /* VT100 */
#define VT100AVO	0x0004	       /* VT100 +AVO; 132x24 (not 132x14) & attrs */
#define VT102	0x0008	       /* VT102 */
#define VT220	0x0010	       /* VT220 */
#define VT320	0x0020	       /* VT320 */
#define VT420	0x0040	       /* VT420 */
#define VT510	0x0080	       /* VT510, NB VT510 includes ANSI */
#define VT340TEXT	0x0100	       /* VT340 extensions that appear in the VT420 */
#define SCOANSI	0x1000	       /* SCOANSI not in ANSIMIN. */
#define ANSI		0x2000	       /* ANSI ECMA-48 not in the VT100..VT420 */
#define OTHER	0x4000	       /* Others, Xterm, linux, putty, dunno, etc */

#define TM_VT100	(CL_ANSIMIN|CL_VT100)
#define TM_VT100AVO	(TM_VT100|CL_VT100AVO)
#define TM_VT102	(TM_VT100AVO|CL_VT102)
#define TM_VT220	(TM_VT102|CL_VT220)
#define TM_VTXXX	(TM_VT220|CL_VT340TEXT|CL_VT510|CL_VT420|CL_VT320)
#define TM_SCOANSI	(CL_ANSIMIN|CL_SCOANSI)

#define TM_PUTTY	(0xFFFF)



namespace vt_type
{
	inline bool is_compat(int vt_1, int vt_2)
	{
#pragma warning( disable : 4554 ) 
		return vt_1 & vt_2 == 0;
	}
}
