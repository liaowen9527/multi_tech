#pragma once

/* putty term state
default		VT52	TOPLEVEL
0x1b		no		SEEN_ESC
0x1b[		no		SEEN_CSI
0x1b]		no		SEEN_OSC
0x1b]W		no		SEEN_OSC_W
====================DO_CTRLS, it is a 38 line =======================
0x1b]P		no		SEEN_OSC_P
====================OSC_STRING, if SEEN_OSC_W or SEEN_OSC, and c is not num and some chars, into OSC_STRING
====================OSC_MAYBE_ST, if c == '\033' && state == OSC_STRING, into OSC_MAYBE_ST
0x1b		yes		VT52_ESC
0x1bY		yes		VT52_Y1
====================VT52_Y2, if state == VT52_Y1, input any char, into VT52_Y2
0x1bb		yes		VT52_FG
0x1bc		yes		VT52_BG
*/

enum termstate {
	TOPLEVEL = 0,
	SEEN_ESC,
	SEEN_CSI,
	SEEN_OSC,
	SEEN_OSC_W,

	DO_CTRLS,

	SEEN_OSC_P,
	OSC_STRING, OSC_MAYBE_ST,
	VT52_ESC,
	VT52_Y1,
	VT52_Y2,
	VT52_FG,
	VT52_BG
};

