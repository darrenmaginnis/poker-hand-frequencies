#ifndef __CARD_H__
#define __CARD_H__

#include <cstdlib>
#include <utility>

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
	~Card(void);

};

bool operator < (Card const& lhs, Card const& rhs ){
	return static_cast<int>(lhs.CardRank) < static_cast<int>(rhs.CardRank);
}

bool operator == (Card const& lhs, Card const& rhs ){
	return (static_cast<int>(lhs.CardRank) == static_cast<int>(rhs.CardRank)) 
		&& (static_cast<int>(lhs.CardSuit) == static_cast<int>(rhs.CardSuit));
}
	
#endif //__CARD_H__