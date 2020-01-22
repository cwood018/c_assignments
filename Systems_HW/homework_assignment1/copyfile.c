//Christopher Wood
//3.25.19
//COSC4348
//HW04

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>

int bytes; //Amount of bytes the program uses to transfer from source to destination file
int outputF, sourceF, fileRead; //File descriptors
double timeSpent; //Time spent during transfer 


void copyFile(int, int, int); 

void copyFile(int outputF, int sourceF, int bytes) //Copying of the file from source to destination
{
	char buffer[bytes]; //Amount of bytes is placed in the buffer array
	
	if(sourceF > 0) //If the file descriptor is more than 1, the file is than read
	{
	while(read(sourceF,buffer,sizeof(buffer)) > 0) //while the file descriptor is more than 1, the file is read based on the size of the buffer
	{
	if(write(outputF,buffer,sizeof(buffer)) == -1) //The buffer then writes into the destination file
	{
		perror("ERROR"); //Error checking 
		exit(1);
	}
	}
	} 
	close(sourceF); //Closing of File
	close(outputF); //Closing of file
}

int main(int argc, char *argv[])
{

	if(argc != 4) //Error handling for arguments 
	{
		printf("Error, format must be ./copyfile [source file] [destination file] [# of bytes]");
		exit(-1);
	}
	struct timeval start, end; //Struct to hold time during the function

	if((outputF = open(argv[2], O_CREAT | O_RDWR, 0666)) == -1) //Opening of the destination file 0666 allows permission for all users to open and view the file
	{
		perror("Open"); //Error checking 
	}
	sourceF = open(argv[1], O_RDONLY); //Opening of the source file in read only mode
	bytes = atoi(argv[3]); //Setting of the bytes 
	
	gettimeofday(&start, NULL); //Starting of the timer
	copyFile(outputF,sourceF,bytes); //Calls the function
	gettimeofday(&end, NULL); //Stops the timer after function is complete 

	long seconds = (end.tv_sec - start.tv_sec); //Math to find the seconds that occurred during the function
	long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec); //Calculation to find the microseconds 

	printf("Time took for file transfer: %d seconds and %d microseconds", seconds, micros); //Printing of the time of transfer 

	return 0;


}