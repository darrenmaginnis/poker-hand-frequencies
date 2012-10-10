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

using namespace std;

enum Hands { Royal_Flush = 0, Straight_Flush, Four_of_a_Kind, Full_House, Flush, Straight, Three_of_a_Kind, Two_Pair, Pair, High_Card };
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
	Card::Rank rank = vecHand[0].CardRank;
	for(vector<Card>::iterator handIt = vecHand.begin(); handIt != vecHand.end(); handIt++)
	{
		if (handIt->CardSuit != suit)
		{
			isSameSuit = false;
			break;
		}
	}

	return Flush;
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