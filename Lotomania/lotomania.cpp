#include "lotomania.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <stdlib.h>

using namespace std;


Lotomania::Lotomania()
{
   m_os = 0;
}


Lotomania::~Lotomania()
{
   m_os = 0;
}


ostream& Lotomania::OStr() const
{
   return m_os ? *m_os : cout;
}


void Lotomania::OStr(ostream* os)
{
   m_os = os;
}


HRESULT Lotomania::LoadResults(const char* fileName)
{
   Raffle r;
   ifstream ifs(fileName);
   size_t i = 0;
   Unit val;

   Clear();

   while( ifs >> val )
   {
      r.at(i++) = val;
      if( i % DrewValues == 0 )
      {
         m_raffles.push_back(r);
         i = 0;
      }
   }

   return S_OK;
}


HRESULT Lotomania::LoadSimulation(size_t count)
{
   Clear();
   Randomize();

   for( size_t i = 0; i < count; ++i )
      GenerateRaffle();
   
   return S_OK;
}


void Lotomania::Clear()
{
   m_raffles.clear();
}


void Lotomania::Randomize(unsigned int val)
{
   if( !val )
   {
      SYSTEMTIME st;
      GetSystemTime(&st);
      val = st.wYear + st.wMonth + st.wDay + st.wHour + 
         st.wMinute + st.wSecond + st.wMilliseconds;
   }

   srand(val);
}


void Lotomania::Add(const Raffle& r)
{
   m_raffles.push_back(r);
}


bool Lotomania::Find(const Lotomania::Raffle& r)
{
   return find(m_raffles.begin(), m_raffles.end(), r) != m_raffles.end();
}


Lotomania::Raffle Lotomania::GenerateRaffle()
{
   Raffle r;

   for( size_t i = 0; i < DrewValues; ++i )
      r.at(i) = rand() % TotalValues;

   return r;
}


HRESULT Lotomania::PrintResults() const
{
   for( size_t i = 0; i < m_raffles.size(); ++i )
      OStr() << m_raffles[i] << '\n';

   return S_OK;
}


HRESULT Lotomania::PrintFrequency() const
{
   vector<Unit> values(TotalValues);

   for( size_t i = 0; i < m_raffles.size(); ++i )
   {
      Raffle r = m_raffles[i];
   
      for( size_t k = 0; k < DrewValues; ++k )
      {
         values[r[k]]++;
      }
   }

   for( size_t i = 0; i < values.size(); ++i )
   {
      OStr() << int(i) << ";" << values[i] << '\n';
   }

   return S_OK;
}
