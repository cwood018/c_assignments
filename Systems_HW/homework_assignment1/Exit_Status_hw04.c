// Christopher Wood
//COSC 4348
//3.25.19
//HW4
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <math.h>
#include <errno.h>


#define N 6
 pid_t pid;

void My_Exit_Status_Check(int status)
{
 int x;
 int conversion[31]; //Used for binary conversion
 int tempStore; //Holds the original status code 
 int signalCode = 0; //Holds signal code
 int statusCode = 0; //Holds exit status
 tempStore = status; //Transfers the int status

 for(int a = 31; a >= 0; a--) //Transfer of code into Binary
 {
 	x = tempStore >> a;

 	if(x & 1) //Used to transfer to binary 
 		conversion[a] = 1;
 	else
 		conversion[a] = 0; //Used to transfer to binary
 }
 for(int a = 1; a <= 6; a++) //Used to find placement in signal status
 {
 	if(conversion[a] == 1) //Finds a 1 in binary within the binary string
 		signalCode += pow(2,a); //Used to find the signal code and using pow() to convert from binary to integer
 }
 for(int a = 8; a <= 31; a++) //Used to find status code in binary string
 {
 	if(conversion[a] == 1) //If a one is found the number is converted using pow() to transfer from binary to int
 		statusCode += pow(2,a-8); //Calculating the exit status
 }
 if(signalCode == 0) 
 {
 	printf("The exit status for child %d using my status check function is: Normal Termination, exit status is: %d\n",pid,statusCode);
 }
 else
 {
 	printf("The exit status for child %d using the my status check function is: Abnormal Termination, signal is: %d\n",pid,signalCode);
 }
}

void C_Macros_Exit_Status_Check(int status)
{
 if(WIFEXITED(status)) //Checks to see if the process terminated with normal termination and seeks for exit status
 {
 	printf("The exit status for child %d using C Macro Exit Status function is: Normal Termination, exit status is: %d\n", pid, WEXITSTATUS(status));
 }
 else if(WIFSIGNALED(status)) //Checks to see if the process terminated with abnormal termination and seeks the signal code
 {
 	printf("The exit status for child %d using C Macro Exit Status function is: Abnormal Termination, signal is: %d\n",pid, WTERMSIG(status));
 }
}


int main() 
{
    unsigned int status, i;
   //

    /* Parent creates N children */
    for(i = 0; i < N; i++)                       
		if((pid = fork()) == 0)  /* child */
		{
			if(i == 3)
	    		exit(i/0);
			exit(100 + i);                          
		}
    /* Parent reaps N children in no particular order */
    while ((pid = waitpid(-1, &status, 0)) > 0) 
	{
		My_Exit_Status_Check(status);			// call your code here.
		C_Macros_Exit_Status_Check(status);     // and here.
	}

    /* The only normal termination is if there are no more children */
    if (errno != ECHILD)                          
	   perror("waitpid error");

    exit(0);
}