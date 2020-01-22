//Christopher Wood
//4.12.19
//COSC 4348
//Homework 5

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>


void analyzeFile(char* fileName); //Determines if file is directory or file based on stat() function
void analyzeDirectory(char* directoryName); //Opens and examines the directory and checks for subdirectories
void addChangeCheck(char* fileName, struct stat* buf); //Determines if file has been added or changed 
void deleteCheck(); //Checks if file has been deleted from last cycle


#define FILE_MAX 100 //Max amount of files allowed 
#define MAX_FILENAME 100 //Max number of characters for a name of a file

struct statinfo //Structure given from HW prompt
{
	char fileName [MAX_FILENAME]; //Holds the name of the file 
	int lastCycle, thisCycle; //Used to determine changes from the last cycle and current cycle
	struct stat status; //Uses the stat() function to find the status of a file
};

int loop = -1; //Number used to hold the amount of loops, -1 is first to be infinite loop if no command is entered
int delay = 10; //Seconds between the loop, default of program is 10
int scanCount = 1; //Scan counter to print what number scan the program is on, will increment till completion 
char* directoryName; //Holds the name of the directory given 
char newArgv[100][100]; //Array for commands after fork occurs
struct statinfo fileStat [FILE_MAX]; //One per matching file
struct stat buf; //Buffer used from the stat() function
int statNum; //Holds the number given after stat() function is executed
pid_t pid; //PID for extra credit

void analyzeFile(char* fileName) { //Processes what is a file and what is a directory
	
	statNum = stat(fileName, &buf);  //stat() is used to get file status

	if(statNum == -1) //Error Handling
	{ 
	fprintf(stderr, "File could not be retrieved.");
	exit(1);
	}

	if(S_ISDIR (buf.st_mode)) //Checks if given is directory based on file type and mode
	{
		analyzeDirectory(fileName); //Sends to directory function
	}
	else if(S_ISREG (buf.st_mode)) //Checks if given is file based on file type and mode
	{
		addChangeCheck(fileName, &buf); //Sends to file functions
	}
	else
		fprintf(stderr, "File could not be identified."); //Error statement if file cannot be identified
}

void analyzeDirectory(char* directoryName) { //Function to check directory
	struct dirent *direntp; //Returns a pointer if containing information about the next directory entry 
	DIR *dirp; //pointer for directory
	char fileName[MAX_FILENAME]; //Filename given 

	dirp = opendir(directoryName); //Opens the directory
	if(dirp == NULL) //Error handling when opening the directory
	{
		fprintf(stderr, "Error opening directory");
	}

	while(direntp = readdir(dirp)) //Begins reading the directory
	 { 
		if(direntp == NULL) //Error handling
		{
			fprintf(stderr, "Error reading directory");
		}

		else if(strcmp(direntp->d_name, ".") && strcmp(direntp->d_name,"..") != 0)  //Checks for sub directory
		{
			sprintf(fileName, "%s/%s", directoryName, direntp->d_name); //Stores the path of subdirectory

			//pid = fork(); //Attempt at forking for the extra credit
			//if(pid == 0) 
			//{
			//	execvp(newArgv[0],newArgv);
			//}

			analyzeFile(fileName); //Recursive function is called incase of sub directory
		}
	}
	if(closedir(dirp) == -1) //Closes directory with error handling
	{
		fprintf(stderr, "Error closing directory");
	} 
}

void addChangeCheck(char* fileName, struct stat* buf) { //Checks if file has been added or changed
	int x; //X used in for loop functions
	int firstIndex; //Initial index
	int fileIndex; //File index recieved 
	

for(x = 0; x < FILE_MAX; x++)
	{
		if(fileStat[x].lastCycle && strcmp(fileStat[x].fileName, fileName) == 0) //Checks to see if file already is added
		{
			firstIndex = x; //Sets index to x
			break; //breaks out of the loop
		}
	}
	if(firstIndex != x) //If the first index is not equal to x
	{
		for(int x = 0; x < FILE_MAX; x++)
		{
			if(!fileStat[x].lastCycle && !fileStat[x].thisCycle) //Checks if the last cycle and this cycle
			{
				fileIndex = x; //Sets x as the file index if received 
			}
			else if(x == -1)  //if x is -1 after the if loop then file index is set to -1
			{
				fileIndex = -1;
			}
		}

		if(fileIndex == -1)
		{
			firstIndex = -1;
		}

	strcpy(fileStat[fileIndex].fileName, fileName);
	fileStat[fileIndex].status = *buf; //Add status info to buffer 
	printf("ADDED - /%s\n", fileStat[fileIndex].fileName); //Prints the newly added file 
	 firstIndex = fileIndex; //Sets the intiial index back to the OG index 
	}
	else 
		if(fileStat[firstIndex].status.st_mtime != buf->st_mtime)  //Checks to see if the last modification of file is the same using st_mtime
	{
		printf("CHANGED - /%s\n", fileStat[firstIndex].fileName); //Prints the changed file
		fileStat[firstIndex].status = *buf; //Sets buffer to status
	}
		fileStat[firstIndex].thisCycle = 1; //sets to 1 for cycle comparison
}

void deleteCheck() //Deletes files if its been deleted from joker
{
	for(int x = 0; x < FILE_MAX; x++) 
		{
		if(fileStat[x].lastCycle && !fileStat[x].thisCycle) //If the last cycle doesnt match the current cycle the file is printed with deleted
		{
			printf("DELETED - %s\n", fileStat[x].fileName); //Deleted is printed with file name
		}
		fileStat[x].lastCycle = fileStat[x].thisCycle; //Last cycle and this cycle set to = for intialization of another cycle 
		fileStat[x].thisCycle = 0; //Set to 0 for next cycle comparison
		}
}



int main (int argc, char* argv[]) {


	if(argc == 4) //If all arguments are given
	{
		delay = atoi(argv[1]); //Argument one sets the delay
		loop = atoi(argv[2]); //Sets the amount of loops
		directoryName = argv[3]; //Name the directory
		strcpy(newArgv[0],argv[0]); //Storing argument in array for forking 
		strcpy(newArgv[1],argv[1]); //Storing argument in array for forking
		strcpy(newArgv[2],argv[2]); //Storing argument in array for forking
		strcpy(newArgv[3],argv[3]); //Storing argument in array for forking
	}
	else if(argc > 4) //If there are more than 4 arguments given, error
	{
		printf("Error! Proper format should be ./monitor [time of delay] [loop count] [directory name]");
	}
	else if(argc == 2) //If no loop or delay are given, default takes place
	{
		directoryName = argv[1]; //Directory name given
		strcpy(newArgv[0],argv[0]); //Storing argument in array for forking
		strcpy(newArgv[1],argv[1]); //Storing argument in array for forking
	}
	else if(argc == 3) //If only three arguments are given, the loop is changed 
	{
		loop = atoi(argv[1]); //Takes in loop count
		directoryName = argv[2]; //Directory name taken in
		strcpy(newArgv[0],argv[0]); //Storing argument in array for forking
		strcpy(newArgv[1],argv[1]); //Storing argument in array for forking
		strcpy(newArgv[2],argv[2]); //Storing argument in array for forking

	}	

	
	while(loop > 0 || -1) //While loop for when loop count is given or -1 for infinite scanning
		{
			printf("Process %d does Scan %d:",getpid(),scanCount); //Prints the scan amount each cycle
			printf("\n"); //Spacing 
			analyzeFile(directoryName); //Sent to analyze function
			deleteCheck(); //Checks the last cycle and this cycle for deletion
			printf("\n"); //Spacing
			sleep(delay); //Delay occurs
			loop--; //Subtract from loop count
			scanCount++; //Scan count increases
		}
	return(0);
}














