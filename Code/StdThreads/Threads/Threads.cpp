#include <date_time>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>


struct ObjFunc
{
	ObjFunc(const std::string& message)
		: m_message(message)
	{
	}

	void operator () ()
	{
		for( int i = 0; i < 10; ++i )
		{
			print();
			std::this_thread::sleep(std::seconds(1));
		}
	}

	void print()
	{
		m_mtx.lock();
		std::lock_guard<std::mutex> lg(m_mtx);
		std::cout << m_message << std::endl;
	}

private:
	static std::mutex m_mtx;
	const std::string m_message;
};

std::mutex ObjFunc::m_mtx;


int main()
{
	ObjFunc func1("Hello, C++ developers!");
	ObjFunc func2("Welcome to this C++ event!");
	ObjFunc func3("Take your C++ seats!");

	std::cout << "Wait, please..." << std::endl;
	std::this_thread::sleep(std::seconds(5));

	std::thread thr1(func1);
	std::thread thr2(func2);
	std::thread thr3(func3);

	thr1.join();
	thr2.join();
	thr3.join();

	std::cout << "Thank you!" << std::endl;
	std::cin.get();
}
