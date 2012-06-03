#pragma once
#include "Array.h"
#include <windows.h>
#include <iostream>
#include <vector>


class Lotomania
{
public:
   static const int TotalValues = 100; // n�meros v�lidos no sorteio
   static const int DrewValues = 20; // n�meros escolhidos por sorteio

   typedef unsigned short Unit; // uma unidade
   typedef TArray<Unit, DrewValues> Raffle; // um sorteio
   typedef std::vector<Raffle> RaffleList; // uma lista de sorteios

public:
   Lotomania();
   ~Lotomania();

   void Add(const Raffle&);
   void Clear();
   bool Find(const Raffle&);

   HRESULT LoadResults(const char* fileName);
   HRESULT LoadSimulation(size_t count);

   HRESULT PrintResults() const;
   HRESULT PrintFrequency() const;

   static Raffle GenerateRaffle();
   static void Randomize(unsigned int val = 0);

protected:
   std::ostream& OStr() const;
   void OStr(std::ostream*);

private:
   RaffleList m_raffles;
   std::ostream* m_os;
};
