#include "Card.h"


Card::Card(void)
{
	m_CardSuit =  static_cast<Suit>(rand() % 4);
	m_CardRank =  static_cast<Rank>(rand() % 13);
}

Card::Card(Suit s, Rank r)
{
	m_CardRank = r;
	m_CardSuit = s;
}

Card::Rank Card::GetRank(){
	return m_CardRank;
}

Card::Suit Card::GetSuit(){
	return m_CardSuit;
}



Card::~Card(void)
{
}
