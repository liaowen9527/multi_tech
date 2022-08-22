#include "std/LWException.h"
#include <iostream>

int main(void)
{
	try
	{
		LWException ex;
		ex.SetErrorCode(1, "aaa");
		throw ex;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "..." << std::endl;
	}
}