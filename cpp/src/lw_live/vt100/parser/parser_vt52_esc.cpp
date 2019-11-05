#include "parser_vt52_esc.h"
#include "../terminal.h"


parser_vt52_esc::parser_vt52_esc()
{
	
}

parser_vt52_esc::~parser_vt52_esc()
{

}

void parser_vt52_esc::parse()
{
	get_parser()->set_termstate(TOPLEVEL);
	cursor old_curs = get_screen()->get_cursor();
	char ch = get_char();
	
	switch (ch) {
	case 'A':
		get_control()->move_cursor(old_curs.row - 1, old_curs.col, 1);
		break;
	case 'B':
		get_control()->move_cursor(old_curs.row + 1, old_curs.col, 1);
		break;
	case 'C':
		get_control()->move_cursor(old_curs.row, old_curs.col + 1, 1);
		break;
	case 'D':
		get_control()->move_cursor(old_curs.row, old_curs.col - 1, 1);
		break;
	case 'F':
		//Charset(CSET_LINEDRW);
		break;
	case 'G':
		//Charset(CSET_ASCII);
		break;
	case 'H':
		get_control()->move_cursor(0, 0, 0);
		break;
	case 'I':
	{
		if (old_curs.row == 0)
		{
			margin marg;
			marg.top = 0;
			marg.bottom = old_curs.row - 1;
			m_term->scroll(marg, -1, true);
		} else if (old_curs.row > 0)
		{
			get_screen()->set_curs_row(old_curs.row - 1);
		}
		get_screen()->set_wrap_next(false);
		break;
	}
	case 'J':
		get_control()->erase_lots(false, false, true);
		break;
	case 'K':
		get_control()->erase_lots(true, false, true);
		break;
	case 'Y':
		get_parser()->set_termstate(VT52_Y1);
		break;
	case 'Z':
		//Identify();
		break;
	case '=':
		//KeypadApp(true);
		break;
	case '>':
		//KeypadApp(false);
		break;
	case '<':
		get_conf()->set_vt52mode(false);
		break;
#ifdef VT52_PLUS
	case 'E':
		get_control()->move_cursor(0, 0, 0);
		get_control()->erase_lots(false, false, true);
		break;
	case 'L':
		m_term->inset_lines(1);
		break;
	case 'M':
		m_term->delete_lines(1);
		break;
	case 'b':
		get_parser()->set_termstate(VT52_FG);
		break;
	case 'c':
		get_parser()->set_termstate(VT52_BG);
		break;
	case 'd':
		get_control()->erase_lots(false, true, false);
		break;
	case 'e':
		get_conf()->set_cursor_on(true);
		break;
	case 'f':
		get_conf()->set_cursor_on(false);
		break;
		/* case 'j': Save cursor position - broken on ST */
		/* case 'k': Restore cursor position */
	case 'l':
		get_control()->erase_lots(true, true, true);
		get_screen()->set_curs_col(0);
		get_screen()->set_wrap_next(false);
		break;
	case 'o':
		get_control()->erase_lots(true, true, false);
		break;
	case 'p':
		//SetAttr(ATTR_REVERSE);
		break;
	case 'q':
		//RemoveAttr(ATTR_REVERSE);
		break;
	case 'v':	       /* wrap Autowrap on - Wyse style */
		get_screen()->set_wrap(true);
		break;
	case 'w':	       /* Autowrap off */
		get_screen()->set_wrap(false);
		break;
	case 'R':
		//ResetEraseChar();
		break;
	case 'S':
		//SetAttr(ATTR_UNDER);
		break;
	case 'W':
		//RemoveAttr(ATTR_UNDER);
		break;
	case 'U':
		//Bold(true);
	case 'T':
		//Bold(false);
		break;
#endif
	}
}
