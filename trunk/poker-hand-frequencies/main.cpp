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
#include <time.h>
#include<iterator>

#include "Card.h"


using namespace std;

enum Hands { Royal_Flush = 0, Straight_Flush, Four_of_a_Kind, Full_House, Flush, Straight, Three_of_a_Kind, Two_Pair, Pair, High_Card };
int poolSize, instance;

Hands CheckHand(vector<Card> vecHand);
void processSlave();
void processMaster();
const int TAG_HAND = 0;
const int TAG_KILL = 1;

int main(int argc, char *argv[])
{


	//Initialize MPI
	if( MPI_Init(&argc, &argv) == MPI_SUCCESS )
	{
		//Obtain the instance and the poolsize
		MPI_Comm_size(MPI_COMM_WORLD, &poolSize);
		MPI_Comm_rank(MPI_COMM_WORLD, &instance);
		
		//seed the random
		srand ( static_cast<unsigned>(time(NULL))+ instance );
		
		// start 
		if(instance == 0) 
			processMaster();
		else
			processSlave();
		
		//shutdown
		return MPI_Finalize();
	}
	return EXIT_FAILURE;
}

Hands CheckHand(vector<Card> vecHand){
	assert(vecHand.size() == 5);

	bool isRoyal, isFlush, isStraight, isFour, isThree, isHouse, isTwoPair, isPair;
	isRoyal = isFlush = isStraight = isFour = isThree = isHouse = isTwoPair = isPair = true;

	//Flush
	Card::Suit suit = vecHand[0].CardSuit;
	sort(vecHand.begin(), vecHand.end());
	//same suit
	for(vector<Card>::iterator handIt = vecHand.begin(); handIt != vecHand.end(); handIt++){
		if (handIt->CardSuit != suit){
			isFlush = false;
			break;
		}
	}

	//Straight
	int rank = static_cast<int>(vecHand.begin()->CardRank);	
	for(vector<Card>::iterator handIt = vecHand.begin() + 1; handIt != vecHand.end(); handIt++){
		int current = static_cast<int>(handIt->CardRank);
		if (current != ++rank){
			isStraight = false;
			break;
		}		
	}
	
	//Royal Flush or Straight Flush
	if(isFlush && isStraight){
		if(vecHand[0].CardRank == Card::Ten){
			return Royal_Flush;
		}
		else{
			return Straight_Flush;
		}
	}

	//Four of a Kind
	Card::Rank cardRank = vecHand.begin()->CardRank;
	for(int i = 1 ; i < 4; i++){
		if ( vecHand[i].CardRank != cardRank){
			isFour = false;
			break;
		}		
	}
	if (!isFour){
		cardRank = vecHand.rbegin()->CardRank;
		isFour = true;
		for(int i = 3 ; i > 0; i--){
			if ( vecHand[i].CardRank != cardRank){
				isFour = false;
				break;
			}		
		}	
	}

	if(isFour){
		return Four_of_a_Kind;
	}
	if(isFlush){
		return Flush;
	}
	if(isStraight){
		return Straight;
	}

	//Three of a Kind
	cardRank = vecHand.begin()->CardRank;
	for(int i = 1 ; i < 3; i++){
		if ( vecHand[i].CardRank != cardRank){
			isThree = false;
			break;
		}
	}

	if (!isThree){
		cardRank = vecHand.rbegin()->CardRank;
		isThree = true;
		for(int i = 3 ; i > 1; i--) {
			if ( vecHand[i].CardRank != cardRank) {
				isThree = false;
				break;
			}		
		}	
	}

	//Full House
	if(isThree){
		if(vecHand[0].CardRank != vecHand[2].CardRank){
			if(vecHand[0].CardRank == vecHand[1].CardRank){
				return Full_House;
			}
		}else{
			if(vecHand[3].CardRank == vecHand[4].CardRank){
				return Full_House;
			}
		}
		return Three_of_a_Kind;
	}

	//Two Pair
	if ( (vecHand[0].CardRank == vecHand[1].CardRank && vecHand[3].CardRank == vecHand[4].CardRank) ||
		 (vecHand[0].CardRank == vecHand[1].CardRank && vecHand[2].CardRank == vecHand[3].CardRank) ||
		 (vecHand[1].CardRank == vecHand[2].CardRank && vecHand[3].CardRank == vecHand[4].CardRank) ){
		 return Two_Pair;
	}


	//Pair
	for(vector<Card>::iterator handIt = vecHand.begin(); handIt != vecHand.end(); handIt++)
	{	
		if (handIt + 1 != vecHand.end() && handIt->CardRank == (handIt + 1)->CardRank)
		{
			return Pair;
		}	
	}

	//Default High Card
	return High_Card;
}

void processSlave(){
	bool found[10] = {false};
	int frequency[10] = {0};
	
	// set Pair, High_Card as found 
	found[Pair] = true;
	found[High_Card] = true;
	bool stop = false;

	//Fill the deck;
	vector<Card> deck = Card::CreatDeck();
	assert(deck.size() == 52);
	//generate hand.
	vector<Card> hand;
	hand.reserve(5);

	MPI_Request send_Request, recv_Request;	
	static int rcv_buff, recvFlag;
	MPI_Status status;
	recv_Request = 0;
	//main loop
	do{ 
		random_shuffle ( deck.begin(), deck.end() );
		//draw the hand
		for(int i = 0; i < 5; ++i){
			hand.push_back(deck[i]);
		}
		
		//Check hand
		Hands result = CheckHand(hand);
		frequency[result]++;
		if(!found[result]){
			found[result] = true;
			//TODO: tell others
			for(int i = 0; i < poolSize; ++i)
			{
				if(i == instance){ // dont send to self
					continue;
				}
				int r = static_cast<int>(result);
				MPI_Isend(&r, 1, MPI_INT, i, TAG_HAND, MPI_COMM_WORLD, &send_Request);
			}
		}
		hand.clear();
		
		//listen asynchronously
		if( recv_Request )
		{
			// Already listening for a message Test to see if message has been received
			MPI_Test( &recv_Request, &recvFlag, &status );
			if( recvFlag )
			{
				// Message received hand found
				if( status.MPI_TAG == TAG_HAND ){
					found[rcv_buff] = true;
				} else if(status.MPI_TAG == TAG_KILL){
					stop = true;
				}
				// Reset the request handle
				recv_Request = 0;
			}
		}
		// Start listening again
		if( !recv_Request && !stop ){
			MPI_Irecv(&rcv_buff, 1, MPI_INT,MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &recv_Request);
		}
	}while(!stop);
	//Send the Results
	MPI_Send(&frequency, 10, MPI_INT, 0, TAG_KILL, MPI_COMM_WORLD);
	
}

void processMaster(){	
	bool found[10] = {false};
	int frequency[10] = {0};
	
	// set Pair, High_Card as found 
	found[Pair] = true;
	found[High_Card] = true;
	bool stop = false;

	//Fill the deck;
	vector<Card> deck = Card::CreatDeck();
	assert(deck.size() == 52);
	//generate hand.
	vector<Card> hand;
	hand.reserve(5);

	MPI_Request send_Request, recv_Request;	
	static int rcv_buff, recvFlag;
	MPI_Status status;
	recv_Request = 0;
	//main loop
	do{ 
		//listen asynchronously
		if( recv_Request )
		{
			// Already listening for a message Test to see if message has been received
			MPI_Test( &recv_Request, &recvFlag, &status );
			if( recvFlag )
			{
				// Message received hand found
				if( status.MPI_TAG == TAG_HAND ){
					found[rcv_buff] = true;
				}
				// Reset the request handle
				recv_Request = 0;
			}
		}
		// Start listening again
		if( !recv_Request ){
			MPI_Irecv(&rcv_buff, 1, MPI_INT,MPI_ANY_SOURCE, TAG_HAND, MPI_COMM_WORLD, &recv_Request);
		}
		random_shuffle ( deck.begin(), deck.end() );
		//draw the hand
		for(int i = 0; i < 5; ++i){
			hand.push_back(deck[i]);
		}
		
		//Check hand
		Hands result = CheckHand(hand);
		frequency[result]++;
		if(!found[result]){
			found[result] = true;
			//TODO: tell others			
			for(int i = 0; i < poolSize; ++i)
			{
				if(i == instance){ // dont send to self
					continue;
				}
				int r = static_cast<int>(result);
				MPI_Isend(&r, 1, MPI_INT, i, TAG_HAND, MPI_COMM_WORLD, &send_Request);
			}
		}
		hand.clear();

		//check if all found
		for(int i = 0; i < 10; ++i){
			stop = found[i];
			if(!stop){
				break;
			}
		}
	}while(!stop);
	
	//shut down slaves
	for(int i = 1; i < poolSize; ++i){
		MPI_Isend(&i, 1, MPI_INT, i, TAG_KILL, MPI_COMM_WORLD, &send_Request);
	}
	
	//collect totals
	MPI_Status results;
	for(int i = 1; i < poolSize; ++i){
		int slaveResults[10] = {0};		
		MPI_Recv(&slaveResults, 10, MPI_INT, i, TAG_KILL, MPI_COMM_WORLD,  &results);
		for(int idx = 0; idx < 10; ++idx){
			frequency[idx] += slaveResults[idx];
		}
	}

	//TODO: Display
	for(int i = 0; i < 10; ++i){
		cout << i << " " << frequency[i] << endl;
	}
}