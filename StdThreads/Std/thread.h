#pragma once
#include "date_time.h"
#include "stdexceptex.h"
#include <sstream>

#ifdef _WIN32
#include "threadwin.h"
namespace native = std::win;
#else
#include "threadso.h"
namespace native = std::so;
#endif


namespace std
{

	class thread
	{
	public:
		class id
		{
		public:
			id() { }
		};

		typedef native::native_handle_type native_handle_type;

		thread(thread& rhs)
		{
			m_native_handle = rhs.m_native_handle;
			native::start_native_handle(rhs.m_native_handle);
		}

		template<typename F>
		explicit thread(F f)
		{
			int ret = native::create_thread(m_native_handle, f);

			if( ret != 0 )
				throw_system_error(ret);
		}

		thread& operator = (const thread& rhs)
		{
		}

		~thread()
		{
			detach();
		}

		void detach()
		{
			if( joinable() )
				native::close_native_handle(m_native_handle);
		}

		bool joinable()
		{
			return native::is_valid_handle(native_handle());
		}

		void join()
		{
			if( joinable() )
			{
				int ret = native::join(native_handle());

				if( ret != 0 )
					throw_system_error(ret);
			}
		}

		id get_id()
		{
			return m_id;
		}

		native_handle_type native_handle()
		{
			return m_native_handle;
		}

	private:
		native_handle_type m_native_handle;
		id m_id;

		void throw_system_error(int ret)
		{
			std::stringstream ss;
			ss << ret;
			throw std::system_error(ss.str());
		}
	};


	namespace this_thread
	{
		template<typename TimeDuration>
		void sleep(const TimeDuration& td)
		{
			native::sleep(td);
		}

		void yield()
		{
			native::yield();
		}
	}


	struct once_flag
	{
	};

}