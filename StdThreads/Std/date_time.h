#pragma once


namespace std
{

	class nanoseconds 
	{
	public:
		typedef long long tick_type;

		nanoseconds(long long tm = 0) : m_tm(tm) { }
		nanoseconds(const nanoseconds& rhs) : m_tm(rhs.m_tm) { }
		~nanoseconds() { m_tm = 0; }

		//traits information
		static tick_type ticks_per_second() { return 1000000000; }
		static tick_type seconds_per_tick() { return 0; }
		static bool is_subsecond() { return true; }

		//+ common functions
		tick_type get_count() { return m_tm; }

	private:
		long long m_tm;
	};


	class microseconds 
	{
	public:
		typedef long long tick_type;

		microseconds(long long tm = 0) : m_tm(tm) { }
		microseconds(const microseconds& rhs) : m_tm(rhs.m_tm) { }
		~microseconds() { m_tm = 0; }

		//traits information
		static tick_type ticks_per_second() { return 1000000; }
		static tick_type seconds_per_tick() { return 0; }
		static bool is_subsecond() { return true; }

		//conversions
		operator nanoseconds() const { return m_tm * 1000; }

		//+ common functions
		tick_type get_count() { return m_tm; }

	private:
		long long m_tm;
	};


	class milliseconds
	{
	public:
		typedef long long tick_type;

		milliseconds(long long tm = 0) : m_tm(tm) { }
		milliseconds(const milliseconds& rhs) : m_tm(rhs.m_tm) { }
		~milliseconds() { m_tm = 0; }

		//traits information
		static tick_type ticks_per_second() { return 1000; }
		static tick_type seconds_per_tick() { return 0; }
		static bool is_subsecond() { return true; }

		//conversions
		operator nanoseconds() const { return m_tm * 1000000; }
		operator microseconds() const { return m_tm * 1000; }

		//+ common functions
		tick_type get_count() { return m_tm; }

	private:
		long long m_tm;
	};


	class  seconds 
	{ 
	public:
		typedef long long tick_type;

		seconds(long long tm = 0) : m_tm(tm) { }
		seconds(const seconds& rhs) : m_tm(rhs.m_tm) { }
		~seconds() { m_tm = 0; }

		//traits information
		static tick_type ticks_per_second() { return 1; }
		static tick_type seconds_per_tick() { return 0; }
		static bool is_subsecond() { return false; }

		//conversions
		operator nanoseconds() const { return m_tm * 1000000000; }
		operator microseconds() const { return m_tm * 1000000; }
		operator milliseconds() const { return m_tm * 1000; }

		//+ common functions
		tick_type get_count() { return m_tm; }

	private:
		long long m_tm;
	};


	class  minutes 
	{
	public:
		typedef long long tick_type;

		minutes(long long tm = 0) : m_tm(tm) { }
		minutes(const minutes& rhs) : m_tm(rhs.m_tm) { }
		~minutes() { m_tm = 0; }

		//traits information
		static tick_type ticks_per_second() { return 0; }
		static tick_type seconds_per_tick() { return 60; }
		static bool is_subsecond() { return false; }

		//conversions
		operator nanoseconds() const { return m_tm * 1000000000 * 60; }
		operator microseconds() const { return m_tm * 1000000 * 60; }
		operator milliseconds() const { return m_tm * 1000 * 60; }
		operator seconds() const { return m_tm * 60; }

		//+ common functions
		tick_type get_count() { return m_tm; }

	private:
		long long m_tm;
	};


	class  hours 
	{
	public:
		typedef long long tick_type;

		hours(long long tm = 0) : m_tm(tm) { }
		hours(const hours& rhs) : m_tm(rhs.m_tm) { }
		~hours() { m_tm = 0; }

		//traits information
		static tick_type ticks_per_second() { return 0; }
		static tick_type seconds_per_tick() { return 3600; }
		static bool is_subsecond() { return false; }

		//conversions
		operator nanoseconds() const { return m_tm * 1000000000 * 60 * 60; }
		operator microseconds() const { return m_tm * 1000000 * 60 * 60; }
		operator milliseconds() const { return m_tm * 1000 * 60 * 60; }
		operator seconds() const { return m_tm * 60 * 60; }
		operator minutes() const { return m_tm * 60; }

		//+ common functions
		tick_type get_count() { return m_tm; }

	private:
		long long m_tm;
	};

}
