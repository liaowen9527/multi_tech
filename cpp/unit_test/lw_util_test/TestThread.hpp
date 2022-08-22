#include <iostream>

#include "lw_util/thread/thread_wrapper.h"


class TestThread : public thread_wrapper
{
public:
	TestThread() {}
	~TestThread() {}

public:
	void Test()
	{
		std::cout << "begin thread." << std::endl;

		//sleep 3s
		run("test", 2, true, 3000);

		//sleep 4s
		std::this_thread::sleep_for(std::chrono::seconds(4));
		resume();

		//will print after 7 second
		std::this_thread::sleep_for(std::chrono::seconds(4));

		quit(true);

		std::cout << "quit thread." << std::endl;
	}
	
protected:
	virtual void inner_thread_func()
	{
		std::cout << "thread_id:" << std::this_thread::get_id() << std::endl;
	}
	
};