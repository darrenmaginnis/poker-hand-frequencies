#include<iostream>
#include<cstdlib>
#include<string>
#include<vector>
#include <mpi.h>

using namespace std;

int main(int argc, char *argv[])
{
	//Initialize MPI
	if( MPI_Init(&argc, &argv) == MPI_SUCCESS )
	{
		//Obtain the rank and size of the processes
		int numProcs, rank;
		MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);

		string hearts [13] = {"2H","3H","4H","5H","6H","7H","8H","9H","10H","JH","QH","KH","AH"};
		string clubs [13] = {"2C","3C","4C","5C","6C","7C","8C","9C","10C","JC","QC","KC","AC"};
		string spades [13] = {"2S","3S","4S","5S","6S","7S","8S","9S","10S","JS","QS","KS","AS"};
		string diamonds [13] = {"2D","3D","4D","5D","6D","7D","8D","9D","10D","JD","QD","KD","AD"};

		string hands [10] = {"Royal Flush","Straight Flush","Four of a Kind","Full House","Flush","Straight","Three of a Kind","Two Pair","Pair","High Card"};

	
		return MPI_Finalize();

	}
	return EXIT_FAILURE;
}