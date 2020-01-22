//Christopher Wood
//12/5/2018
//COSC 3346
//Project 4

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <iomanip>

using namespace std;

//Global Variables
int processCount; //Process count for the program
int resourceCount; //Resource count for the program
int processNum;	//Holds the requesting process num	
string fileContent; //Holds the name of file
int counter = 0; //Counter used in safe state 
bool safe = false; //Bool for if safe state or not
int FinishVector[20]; //Holds the finish vector
int WorkVector[20];  //Holds the work vector
int RequestVector[20];	//Holds the request vector
int AllocationMatrix[20][20]; //Holds the allocation matrix
int MaxMatrix[20][20]; //Holds the max matrix
int NeedMatrix[20][20]; //Holds the need matrix
int AvailableMatrix[20]; //Holds the available matrix

//Function Prototypes
void labelMatrix(int);
void readFile(char *);
void allocationMatrix();
void maxMatrix();
void needMatrix();
void availableVector();
void isSafeState();
void requestVector();
void checkRequest();
void newRequest();




void labelMatrix(int resourceCount)	//Function that prints the letters for the matrices and vectors
{
	char x = 'A'; //Variable for the letter 
	cout << "     ";

	for(int i = 0; i < resourceCount; i++) //For loop to print letters
	{
		cout << (char)(x+i) << setw(3);
	}
}
void readFile(char* fileName) //Function to read the contents of the file
{
	string fileContent(fileName);		//Converts the file anme to a sting 	
	
	

 	ifstream infile(fileName); //Opening of the file

	if(!infile.is_open()) //Error handling for opening the file
	{
		cout << " Error opening file " << endl;
		exit(0);
	}	
	
	infile >> processCount; 	// takes in number of processes 
	infile >> resourceCount;	// takes in number of resources

	for(int i = 0; i < processCount; i++)
	{
		for(int j = 0; j < resourceCount; j++) //For loop for Allocation Matrix
		{
			infile >> AllocationMatrix[i][j]; //Takes in the allocation matrix	
		}
	}

	for(int i = 0; i < processCount; i++)
	{
		for(int j = 0; j < resourceCount; j++) //For loop for max matrix
		{
			infile >> MaxMatrix[i][j];	//Takes in the Max Matrix	
		}
	}

	for(int i = 0; i < resourceCount; i++) //For loop for available matrix
	{
		infile >> AvailableMatrix[i]; //Takes in available matrix		
	}
	
	infile >> processNum; //Takes in the requesting process number				
	infile.ignore();		

	for(int i = 0; i < resourceCount; i++) //For loop for Request vector
	{
		infile >> RequestVector[i];	//Takes in request vector	
	}	
	cout << endl;
	cout << " There are " << processCount << " processes in the system. " << endl; //Prints the number of processes in the system
	cout << " There are " << resourceCount << " resource types. " << endl; //Prints the number of resources in the system
}
void allocationMatrix() //Function to form the allocation matrix
{
	cout << " The Allocation Matrix is... " << endl;
	labelMatrix(resourceCount); //Prints the labeling from the label function
	for(int i = 0; i < processCount; i++)
	{	
		cout << endl; 
		cout << i << ": ";
		for(int j = 0; j < resourceCount; j++) //For loop to print the allocation matrix
		{
			cout << AllocationMatrix[i][j] << setw(3);	
		}
	}
	cout << endl;
	cout << endl;
}
void maxMatrix() //Function to form the max matrix
{
	cout << "The Max Matrix is... " << endl;
	labelMatrix(resourceCount); //PRints the labeling from the label function
	for(int i=0;i<processCount;i++)
	{
		cout << endl;
		cout << i << ": ";
		for(int j = 0; j < resourceCount; j++) //For loop to print the max matrix
		{
			cout << MaxMatrix[i][j] << setw(3);			
		}
	}
	cout << endl;
	cout << endl;
}
void needMatrix() //Function to form the need matric
{
	for(int i = 0; i < processCount; i++)
	{	
		for(int j = 0; j < resourceCount; j++) //For loop to form matrices
		{	
			NeedMatrix[i][j] = MaxMatrix[i][j] - AllocationMatrix[i][j]; //Calculation to form the need matrix 
		}
	}

	cout << "The Need Matrix is... " << endl;
	labelMatrix(resourceCount); //Prints labeling from the label function
	for(int i = 0; i < processCount; i++)
	{
		cout << endl;
		cout << i << ": ";
		for(int j = 0; j < resourceCount; j++) //For loop tor print the need matrix
		{
			cout << NeedMatrix[i][j] << setw(3);			
		}	
	}
	cout << endl;
	cout << endl;
}
void availableVector() //Function to print the available vector
{
	cout<< "The Available Vector is... " << endl;

	labelMatrix(resourceCount); //Prints labeling from label function
	cout << "\n     ";
	for(int j = 0; j < resourceCount; j++) //For loop to print the avaiable vector
	{
		cout << AvailableMatrix[j] << setw(3);
	}
	cout << endl;
	cout << endl;
}
void isSafeState() //Function to check if safe state 
{
	int j;
	for(int i = 0; i < resourceCount; i++) //For loop to set work to avaailable 
	{
		WorkVector[i] = AvailableMatrix[i];			
	}
	for(int i = 0; i < processCount; i++) //For loop to set finish vector to 0
	{
		FinishVector[i] = 0;					
	}
	while (counter < processCount)					
	{	
		for(int i = 0; i < processCount; i++)
		{
			if(FinishVector[i] == 0)			
			{				
				for(j = 0; j < resourceCount; j++)
					if(NeedMatrix[i][j] > WorkVector[j]) //If need is greater than work, break occurs	
						break;
	
				if(j == resourceCount)					
				{
					for(int k = 0; k < resourceCount; k++) //For loop to set work + allocation matrix
						WorkVector[k] += AllocationMatrix[i][k];

					counter++;
					FinishVector[i] = 1;
					safe = true; //Sets safe state to true
				}
					
			}
		}
		if(safe == false) //If safe state is false, message is printed and exits 						
		{
			cout << "\n THE SYSTEM IS NOT IN A SAFE STATE! " << endl; 
			exit(0);
		}
	}

	cout << "\n THE SYSTEM IS IN A SAFE STATE! " << endl;	// print safe state message if safe is true

}

void requestVector() //Function to print request vector
{
	cout << "\n The Request Vector is... " << endl;
	labelMatrix(resourceCount); //label function to print the labeling

	cout << "\n  " << processNum << ": ";

	for(int i = 0; i < resourceCount; i++) //For loop to print the request vector
	{
		cout << RequestVector[i] << setw(3);		
	}
	cout << endl;
	cout << endl;
}
void checkRequest() //Function to see if request can be granted 
{
	for(int i=0; i < resourceCount; i++)
	{
		if(RequestVector[i]>NeedMatrix[processNum][i]) //Checks request, if greater than error is handled		
		{
			cout << "\n THE REQUEST CAN NOT BE GRANTED! " << endl; //Printing of Denial
			exit(0);
		}
	}
	for(int i=0; i < resourceCount; i++)
	{
		if(RequestVector[i]>AvailableMatrix[i])	//Checks request, if greater message is printed			
		{
			cout << "\n THE REQUEST CAN NOT BE GRANTED! " << endl; //Printing of denial
			exit(0);
		}
	}
	for(int i=0; i < resourceCount; i++) //For loop for allocation of requested resources 
	{
			AvailableMatrix[i] -= RequestVector[i];					
			NeedMatrix[processNum][i] -= RequestVector[i];
			AllocationMatrix[processNum][i] += RequestVector[i];
	}
	
	cout << "\n THE REQUEST CAN BE GRANTED! " << endl; //Printing if request CAN be granted
	cout << endl;
	cout << endl;
}

void newRequest() //Function to print new avaialble vector
{
	cout << "The Available Vector is... " << endl;

	labelMatrix(resourceCount); //Calls the label function
	cout << endl;
	cout << "     ";
	for(int i = 0; i < resourceCount; i++) //For loop to print the nev available vector
	{
		cout << AvailableMatrix[i] << setw(3);				
	}
	cout << endl;
}

int main(int argc, char *argv[])
{

	if (argc != 2) //Error handling for taking in arguments 
	{
		cout << "Input error, please use ./banker [txt file]" << endl;
		exit(0);
	}

	readFile(argv[1]); //Calls the readfile Function

	allocationMatrix(); //Calls the allocation matrix function
	
	maxMatrix(); //Calls the max matrix function
	
	needMatrix(); //Calls the need matrix function
	
	availableVector(); //Calls the available vector function

	isSafeState(); //Calls the safe state function

	requestVector(); //Calls the request vector function

	checkRequest(); //Calls the check request function

	newRequest(); //Calls the new available vector request function
	
	return 0;
}
