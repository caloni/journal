#include "lotomania.h"
#include "../../common/hresult.h"
#include <atlbase.h>
#include <iostream>

using namespace std;


int __cdecl _tmain(int argc, _TCHAR* argv[])
{
   HResult hr;
   Lotomania lm;

   try
   {
      hr = lm.LoadResults("lotomania.txt");

      for( unsigned int i = 0; i < 4194304; ++i )
      {
         Lotomania::Randomize(i);
         
         for( unsigned int j = 0; j < 500; ++j )
         {
            Lotomania::Raffle r = Lotomania::GenerateRaffle();

            if( lm.Find(r) )
            {
               cout << "Found! i: " << i
                  << " j: " << j << '\n'
                  << "Array: " << r << '\n';
            }
         }
      }
   }
   catch(CAtlException& e)
   {
      cout << "Error 0x" << hex << e.m_hr;
   }
   catch(...)
   {
      cout << "Catastrofic error!\n";
   }

   return (int) hr;
}
