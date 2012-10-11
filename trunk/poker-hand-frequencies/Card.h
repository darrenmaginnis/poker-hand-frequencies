#ifndef __CARD_H__
#define __CARD_H__

#include <cstdlib>
#include <utility>
#include <algorithm>

class Card
{
public:
	enum Suit{ Spades = 0, Hearts, Diamonds, Clubs };
	enum Rank { Two = 0, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };

	Suit CardSuit;
	Rank CardRank;

	// Function name   : Card
	// Description     : Generates A Random Card
	// Return type     : Card
	// Argument        : void
	Card(void)
	{
		CardSuit =  static_cast<Suit>(rand() % 4);
		CardRank =  static_cast<Rank>(rand() % 13);
	}
	
	// Function name   : Card
	// Description     : Generates a Card with the supplyed Suit and Rank
	// Return type     : Card
	// Argument        : Suit s
	// Argument        : Rank r
	Card(Suit s, Rank r)
	{
		CardRank = r;
		CardSuit = s;
	}

	// Function name   : ~Card
	// Description     : Card Destructor
	// Argument        : void
	~Card(void){}


	// Function name   : CreatDeck
	// Description     : Creats a shuffled deck
	// Return type     : static vector<Card> 
	static std::vector<Card> CreatDeck(){
		std::vector<Card> deck;
		deck.reserve(52);
		for(int s = 0; s < 4; ++s){ //Suit
			for(int r = 0; r < 13; ++r){ //Rank
				deck.push_back(Card(static_cast<Suit>(s),static_cast<Rank>(r)));
			}
		}
		return deck;
	}
};

bool operator < (Card const& lhs, Card const& rhs ){
	return static_cast<int>(lhs.CardRank) < static_cast<int>(rhs.CardRank);
}

bool operator == (Card const& lhs, Card const& rhs ){
	return (static_cast<int>(lhs.CardRank) == static_cast<int>(rhs.CardRank)) 
		&& (static_cast<int>(lhs.CardSuit) == static_cast<int>(rhs.CardSuit));
}
	
#endif //__CARD_H__