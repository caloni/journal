#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <tchar.h>
#include <string.h>
 
 
template<int S>
class SizedString
{
public:
       char buff[S+1];
 
       SizedString(char * str)
       {
             strncpy(buff, str, S);
             buff[S] = '\0';
       }
};
 
 
template<typename T, SizedString<T::S>(*F)()>
class UseSizedString
{
public:
       typedef SizedString<T::S>  id_type;
 
       id_type GetId()
       {
             return F();
       }
};
 
typedef SizedString<5> MyIdType;
 
MyIdType MyGetId()
{
       return MyIdType("Teste");
}

struct IGetTheSize
{
    static const int S = 5;
};
 
 
typedef UseSizedString<IGetTheSize, MyGetId> IdGetter;

int _tmain(int argc, _TCHAR* argv[])
{
       IdGetter x;
       printf("%s", x.GetId().buff);
       return 0;
}
