#pragma once
#include <windows.h>


namespace std
{

	class mutex
	{
	public:
		typedef HANDLE native_handle_type;

		mutex()
		{
			m_native_handle = CreateMutex(NULL, FALSE, NULL);
		}

		void lock()
		{
			WaitForSingleObject(m_native_handle, INFINITE);
		}

		bool try_lock()
		{
			bool ret = true;

			if( ERROR_TIMEOUT == WaitForSingleObject(m_native_handle, 0) )
				ret = false;

			return ret;
		}

		template<typename TimeDuration>
		bool timed_lock(const TimeDuration& td)
		{
			bool ret = false;

			if( ret == WaitForSingleObject(m_native_handle, td.m_secs * 1000) )
				ret = true;

			return ret;
		}

		void unlock()
		{
			ReleaseMutex(m_native_handle);
		}


		native_handle_type native_handle()
		{
			return m_native_handle;
		}

	private:
		native_handle_type m_native_handle;
	};


	template<typename Mutex>
	class lock_guard
	{
	public:
		typedef Mutex mutex_type;

		explicit lock_guard(mutex_type& m)
			: m_m(m)
		{
		}

		~lock_guard()
		{
			m_m.unlock();
		}

	private:
		mutex_type m_m;
	};

}
