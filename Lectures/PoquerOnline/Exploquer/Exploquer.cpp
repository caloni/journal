// Exploquer.cpp : Defines the entry point for the console application.
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


void ShuffleCards(size_t seed, CardsDeque& cards)
{
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


bool DistributeCards(CardsDeque& cards, size_t numberOfPlayers, size_t player,
	const CardsDeque& playerCards, const CardsDeque& tableCards)
{
	CardsDeque tryTableCards;
	vector<CardsDeque> tryPlayersCards;

	DistributeCards(cards, numberOfPlayers, tryPlayersCards, tryTableCards);

	if (tryPlayersCards[player] == playerCards && tryTableCards == tableCards)
	{
		PrintTable(0, tryPlayersCards, tryTableCards);
		return true;
	}
	else
	{
		//cout << "Not!\n";
		//PrintRawCards(cards);
	}

	return false;
}


int GetCardMultiplier(char suit)
{
	int ret;
	switch (suit)
	{
	case 'e': ret = 0; break;
	case 'c': ret = 1; break;
	case 'o': ret = 2; break;
	case 'p': ret = 3; break;
	default:  ret = 0;  break;
	}
	return ret;
}


void GeneratePossibleShuffles(int seconds, vector<CardsDeque>& shuffles)
{
	size_t seed = (size_t)time(nullptr);
	for (size_t sd = seed - seconds; sd < seed; ++sd)
	{
		CardsDeque cards = GetOriginalCards();
		ShuffleCards(sd, cards);
		//cout << sd << ' ';
		//PrintRawCards(cards);
		shuffles.push_back(cards);
	}
}


int GetCardFromString(const char* card)
{
	char suitClass = card[0];
	int suitMultiplier = GetCardMultiplier(suitClass);
	int cardNumber = atoi(&card[1]) - 1;
	int cardBase = 13 * suitMultiplier;
	int ret = cardNumber + cardBase;
	return ret;
}


int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 8)
		return -1;

	CardsDeque tableCards;
	CardsDeque playerCard;

	size_t numberOfPlayers = atoi(argv[1]);
	size_t player = atoi(argv[2]) - 1;
	playerCard.push_back(GetCardFromString(argv[3]));
	playerCard.push_back(GetCardFromString(argv[4]));
	tableCards.push_back(GetCardFromString(argv[5]));
	tableCards.push_back(GetCardFromString(argv[6]));
	tableCards.push_back(GetCardFromString(argv[7]));

	if (argc >= 9)
	{
		int seed = atoi(argv[8]);
		CardsDeque cards = GetOriginalCards();
		ShuffleCards(seed, cards);
		PrintRawCards(cards);
		
		if (DistributeCards(cards, numberOfPlayers, player, playerCard, tableCards))
		{
			cout << "Found!\n";
		}
	}
	else
	{
		vector<CardsDeque> shuffles;
		GeneratePossibleShuffles(1200, shuffles);

		for (size_t i = 0; i < shuffles.size(); ++i)
		{
			if (DistributeCards(shuffles[i], numberOfPlayers, player, playerCard, tableCards))
			{
				break;
			}
		}
	}
}
