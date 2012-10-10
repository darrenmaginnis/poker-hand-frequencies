/*
 * Program:	CPA2-6
 * Course:	INFO-5083
 * Professor:	Tony Haworth
 * Project:	Computer Simulation for the Analysis of Poker Hand Frequencies
 * Due Date:	Friday, October 12th, 2012 @ 7:00 p.m.
 * Author Darren Maginnis 0026561
 * Author Ryan Johnston 0068199
 */

#include<iostream>
#include<cstdlib>
#include<string>
#include<vector>
#include <mpi.h>
#include<cassert>
#include "Card.h"
#include<iterator>
#include<time.h>

using namespace std;

enum Hands { Royal_Flush = 0/*done*/, Straight_Flush /* done*/, Four_of_a_Kind/*done*/, Full_House/*done*/, Flush/*done*/, Straight/*done*/, Three_of_a_Kind/*done*/, Two_Pair/*done*/, Pair/*done*/, High_Card/*done*/ };
int poolSize, instance;

Hands CheckHand(vector<Card> vecHand);
void processSlave();
void processMaster();

int main(int argc, char *argv[])
{
	//Initialize MPI
	if( MPI_Init(&argc, &argv) == MPI_SUCCESS )
	{
		//Obtain the rank and size of the processes
		MPI_Comm_size(MPI_COMM_WORLD, &poolSize);
		MPI_Comm_rank(MPI_COMM_WORLD, &instance);
		//seed the random
		srand ( time(NULL) + instance );
		vector<Card> deck = Card::CreatDeck();
		vector<Card> hand;
		for(int i = 0; i < 5; ++i){
			hand.push_back(deck.back());
			deck.pop_back();
		}
		  vector<Card> h;
		  h.push_back(Card(Card::Clubs, Card::Three));
		  h.push_back(Card(Card::Clubs, Card::Three));
		  h.push_back(Card(Card::Hearts, Card::Four));
		  h.push_back(Card(Card::Clubs, Card::Queen));
		  h.push_back(Card(Card::Clubs, Card::Queen));

		CheckHand(h);
		if(instance == 0) 
			processMaster();
		else
			processSlave();
		
	
		return MPI_Finalize();
	}
	return EXIT_FAILURE;
}

Hands CheckHand(vector<Card> vecHand)
{
	assert(vecHand.size() == 5);
	
	//Royal Flush
	bool isSameSuit = true;
	Card::Suit suit = vecHand[0].CardSuit;
	sort(vecHand.begin(), vecHand.end());
	//same suit
	for(vector<Card>::iterator handIt = vecHand.begin(); handIt != vecHand.end(); handIt++)
	{
		if (handIt->CardSuit != suit)
		{
			isSameSuit = false;
			break;
		}
	}

	//check Rank
	int rank = static_cast<int>(vecHand.begin()->CardRank);
	bool isStraight = true;
	
	for(vector<Card>::iterator handIt = vecHand.begin() + 1; handIt != vecHand.end(); handIt++)
	{
		int current = static_cast<int>(handIt->CardRank);
		if (current != ++rank)
		{
			isStraight = false;
			break;
		}
		
	}

	//Four of a Kind
	Card::Rank cardRank = vecHand.begin()->CardRank;
	bool isFour = true;
	for(int i = 1 ; i < 4; i++)
	{

		if ( vecHand[i].CardRank != cardRank)
		{
			isFour = false;
			break;
		}
		
	}

	if (!isFour)
	{
		cardRank = vecHand.rbegin()->CardRank;
		isFour = true;
		for(int i = 3 ; i > 0; i--)
		{

			if ( vecHand[i].CardRank != cardRank)
			{
				isFour = false;
				break;
			}
		
		}
	
	}

	//Three of a Kind
	cardRank = vecHand.begin()->CardRank;
	bool isThree = true;
	for(int i = 1 ; i < 3; i++)
	{

		if ( vecHand[i].CardRank != cardRank)
		{
			isThree = false;
			break;
		}
	}

	if (!isThree)
	{
		cardRank = vecHand.rbegin()->CardRank;
		isThree = true;
		for(int i = 3 ; i > 1; i--)
		{

			if ( vecHand[i].CardRank != cardRank)
			{
				isThree = false;
				break;
			}
		
		}
	
	}

	//Two Pair
	bool isTwoPair = false;
	for(int i = 0 ; i < 4; i++)
	{

		if ( vecHand[0].CardRank == vecHand[1].CardRank && vecHand[3].CardRank == vecHand[4].CardRank)
		{
			isTwoPair = true;
			break;
		}

		if ( vecHand[0].CardRank == vecHand[1].CardRank && vecHand[2].CardRank == vecHand[3].CardRank)
		{
			isTwoPair = true;
			break;
		}

		if ( vecHand[1].CardRank == vecHand[2].CardRank && vecHand[3].CardRank == vecHand[4].CardRank)
		{
			isTwoPair = true;
			break;
		}

	}

	//Pair
	bool isTwo = false;
	for(vector<Card>::iterator handIt = vecHand.begin(); handIt != vecHand.end(); handIt++)
	{	
		if (handIt + 1 != vecHand.end() && *handIt == *(handIt + 1))
		{
			isTwo = true;
			break;
		}	
	}

	if(isStraight && isSameSuit)
	{
		if(vecHand[0].CardRank == Card::Ten)
			return Royal_Flush;
		else
			return Straight_Flush;
	}

	else if (isFour)
		return Four_of_a_Kind;

	else if (isThree && isTwo)
		return Full_House;

	else if(isSameSuit)
		return Flush;

	else if(isStraight)
		return Straight;

	else if(isThree)
		return Three_of_a_Kind;

	else if(isTwoPair)
		return Two_Pair;

	else if(isTwo)
		return Pair;

	return High_Card;
}

void processSlave(){
	bool found[10] = {false};
	int frequency[10] = {0};

	// set Pair, High_Card as found 
	found[Pair] = true;
	found[High_Card] = true;

	//main loop
	
}

void processMaster(){	
	bool found[10] = {false};
	int frequency[10] = {0};
	
	// set Pair, High_Card as found 
	found[Pair] = true;
	found[High_Card] = true;
	bool stop = false;

	//main loop
	do{
		vector<Card> deck = Card::CreatDeck();
		

		//check if all found
		for(int i = 0; i < 10; ++i){
			stop = found[i];
			if(!found[i]){
				break;
			}
		}
	}while(!stop);



}