// POCer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

typedef vector<int> CardsDeque;


CardsDeque GetOriginalCards()
{
	CardsDeque ret;
	for (int i = 0; i < 52; ++i)
		ret.push_back(i);
	return ret;
}


string GetCardName(int card)
{
	static const char* suitNames[] = { "Espadas", "Copas", "Ouros", "Paus" };
	static const char* cardNames[] = { "As", 
		"2", "3", "4", "5", "6", "7", "8", "9", "10", 
		"Valete (11)", "Rainha (12)", "Rei (13)" };
	ostringstream os;
	os << cardNames[card % 13] << " de " << suitNames[(card / 13) % 4];
	return os.str();
}


void PrintCards(const CardsDeque& cards)
{
	for (size_t c = 0; c < cards.size(); ++c)
		cout << GetCardName(cards[c]) << endl;
}


void PrintRawCards(const CardsDeque& cards)
{
	for (size_t c = 0; c < cards.size(); ++c)
		cout << cards[c] << ' ';
	cout << endl;
}


void ShuffleCards(CardsDeque& cards)
{
	size_t seed = (size_t)time(nullptr); // <-- ESSA PARTE É A MAIS IMPORTANTE (FALHA #4)

	srand(seed);

	for (size_t cardPos = 0; cardPos < cards.size(); ++cardPos)
	{
		int& card = cards[cardPos];
		int randCardPos = rand() % cards.size();
		int& swapCard = cards[randCardPos];
		swap(card, swapCard);
	}
}


int DistributeCards(CardsDeque& cards, size_t players, vector<CardsDeque>& playersCards, CardsDeque& tableCards)
{
	int currCard = 0;
	playersCards.resize(players);

	for (size_t c = 0; c < 2; ++c)
	{
		for (size_t p = 0; p < players; ++p)
		{
			playersCards[p].push_back(cards[currCard++]);
		}
	}

	for (size_t t = 0; t < 3; ++t)
		tableCards.push_back(cards[currCard++]);

	return currCard;
}


void PrintTable(int player, vector<CardsDeque>& playersCards, CardsDeque& tableCards)
{
	for (size_t p = 0; p < playersCards.size(); ++p)
	{
		if (!player || player == p + 1)
		{
			cout << "Jogador #" << p+1 << ":\n";
			PrintCards(playersCards[p]);
			cout << endl;
		}
	}

	cout << "Mesa: \n";
	PrintCards(tableCards);
}


int _tmain(int argc, _TCHAR* argv[])
{
	size_t players = argc >= 2 ? (size_t) atoi(argv[1]) : 4;
	size_t player = argc >= 3 ? (size_t) atoi(argv[2]) : 1;

	CardsDeque cards = GetOriginalCards();
	ShuffleCards(cards);
	//PrintRawCards(cards);

	CardsDeque tableCards;
	vector<CardsDeque> playersCards;

	DistributeCards(cards, players, playersCards, tableCards);
	PrintTable(player, playersCards, tableCards);

	getchar();
}
