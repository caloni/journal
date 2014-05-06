#pragma once
#include <exception>
#include <stdexcept>
#include <string>

namespace std
{
	class system_error : public std::exception
	{
	public:
		system_error(const std::string& message)
			: m_message(std::string("system_error: ") + message)
		{
		}

		virtual const char *what() const
		{
			return m_message.c_str();
		}

	private:
		const std::string m_message;
	};
}
