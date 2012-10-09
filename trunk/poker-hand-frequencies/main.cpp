/*
 * 
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

Hands CheckHand(vector<Card> vecHand);

//void processSlave(int, int);
//void processMaster(int);
int main(int argc, char *argv[])
{
	//Initialize MPI
	if( MPI_Init(&argc, &argv) == MPI_SUCCESS )
	{
		//Obtain the rank and size of the processes
		int size, rank;
		MPI_Comm_size(MPI_COMM_WORLD, &size);
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);

		/*if(rank == 0) 
			processMaster(size);
		else
			processSlave(rank, size);
		*/
	
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