#include "parser_c0.h"
#include "../terminal.h"

parser_c0::parser_c0()
{
	
}

parser_c0::~parser_c0()
{

}

void parser_c0::parse()
{
	char c = get_char();
	switch (c)
	{
	case '\005':             /* ENQ: terminal type query */
		//m_ldisc->QueryType2();
		break;
	case '\007':            /* BEL: Bell */
		//m_ctrlTerm->Bell();
		break;
	case '\b':              /* BS: Back space */
		get_control()->backspace();
		break;
	case '\016':            /* LS1: Locking-shift one */
		//m_ctrlTerm->LockShift(1);
		break;
	case '\017':            /* LS0: Locking-shift zero */
		//m_ctrlTerm->LockShift(0);
		break;
	case '\033':            /* ESC: Escape */
		get_control()->into_esc();
		break;
	case '\r':            /* CR: Carriage return */
		get_control()->cr();
		break;
	case '\014':            /* FF: Form feed */
		//m_ctrlTerm->FormFeed();
		break;
	case '\013':            /* VT: Line tabulation */
		//m_ctrlTerm->LineTab();
		break;
	case '\n':            /* LF: Line feed */
		get_control()->lf();
		break;
	case '\t':              /* HT: Character tabulation */
		get_control()->tab();
		break;
	default:
		break;
	}
}

