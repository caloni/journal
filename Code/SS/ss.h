#include <sstream>

struct ss
{
    template<typename T>
    ss& operator << (const T& t)
    {
        _ss << t; 
        return *this;
    }
    
    operator std::string ()
    { 
        return _ss.str();
    }

    std::ostringstream _ss;
};


