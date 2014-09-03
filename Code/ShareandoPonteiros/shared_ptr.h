#pragma once

// Não façam isso em casa! Usem modelo de smart pointers já construídos (como o do boost).
template<typename T>
class shared_ptr
{
public:
	shared_ptr() : m_p(), m_counter()
	{
	}

	shared_ptr(T* p) 
		: m_p(p), m_counter(new int(1)) 
	{
	}

	shared_ptr(const shared_ptr& left)
		: m_p(left.m_p), m_counter(left.m_counter)
	{
		if( m_counter )
			++*m_counter;
	}

	shared_ptr& operator = (const shared_ptr& left)
	{
		if( m_p )
		{
			if( --*m_counter == 0 )
			{
				delete m_counter;
				delete m_p;
				m_counter = 0;
				m_p = 0;
			}
		}
		if( m_counter = left.m_counter )
			++*m_counter;
		m_p = left.m_p;
	}

	~shared_ptr()
	{
		if( --*m_counter == 0 )
		{
			delete m_counter;
			delete m_p;
			m_counter = 0;
			m_p = 0;
		}
	}

	T* operator -> ()
	{
		return m_p;
	}

private:
	int* m_counter;
	T* m_p;
};
