#pragma once
#include "thread.h"
#include <windows.h>


namespace std
{
	namespace win
	{
		typedef HANDLE native_handle_type;

		template<typename F>
		static DWORD WINAPI thread_func(PVOID param)
		{
			F* f = static_cast<F*>(param);

			(*f)();

			delete f;
			return 0;
		}

		template<typename F>
		int create_thread(HANDLE& h, F f)
		{
			DWORD ret = 0;

			F* pf = new F(f);
			PTHREAD_START_ROUTINE tf = (PTHREAD_START_ROUTINE) &thread_func<F>;
			DWORD tid = 0;
			h = CreateThread(NULL, 0, tf, pf, 0, &tid);

			if( h == NULL )
				ret = GetLastError();

			return int(ret);
		}

		void start_native_handle(native_handle_type& native_handle)
		{
			native_handle = NULL;
		}

		bool is_valid_handle(native_handle_type native_handle)
		{
			return native_handle != INVALID_HANDLE_VALUE &&
				native_handle != NULL;
		}

		void close_native_handle(native_handle_type& native_handle)
		{
			if( is_valid_handle(native_handle) )
			{
				CloseHandle(native_handle);
				start_native_handle(native_handle);
			}
		}

		int join(native_handle_type native_handle)
		{
			DWORD ret = WaitForSingleObject(native_handle, INFINITE);

			if( ret != WAIT_OBJECT_0 )
				ret = GetLastError();
			else
				ret = ERROR_SUCCESS;

			return ret;
		}

		template<typename TimeDuration>
		void sleep(const TimeDuration& td)
		{
			std::milliseconds ms(td);
			DWORD tm = static_cast<DWORD>(ms.get_count());
			Sleep(tm);
		}

		void yield()
		{
			sleep(std::milliseconds(0));
		}
	}
}
