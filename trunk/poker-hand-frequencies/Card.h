#ifndef __CARD_H__
#define __CARD_H__
#include <cstdlib>

class Card
{
public:
	enum Suit{ Spades = 0, Hearts, Diamonds, Clubs };
	enum Rank { Two = 0, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };


	// Function name   : Card
	// Description     : Generates A Random Card
	// Return type     : Card
	// Argument        : void
	Card(void);
	
	// Function name   : Card
	// Description     : Generates a Card with the supplyed Suit and Rank
	// Return type     : Card
	// Argument        : Suit s
	// Argument        : Rank r
	Card(Suit s, Rank r);

	// Function name   : ~Card
	// Description     : Card Destructor
	// Argument        : void
	~Card(void);

	Suit GetSuit();
	Rank GetRank();
private:
	Suit m_CardSuit;
	Rank m_CardRank;
};
	
#endif //__CARD_H__