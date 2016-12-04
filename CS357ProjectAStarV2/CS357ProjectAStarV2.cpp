// CS357ProjectAStarV2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

struct nfa
{
	string* sStates;
	string*** sFinalTransitions;
	char*	cAlfa;
	int		iStart,
			iNumStates,
			iNumAlfa,
			iNumTrans,
			iNumFinalStates;
	int*	iFinal;
};


int main()
{
	//Variables
	string		sFileName;
	ifstream	inputFile;
	nfa			myNFA;		//NFA used in program
	int			i;			//counter
	string		sTempTransitionHolder; //this will store the transitions so we can use the data later.
	

	//get the file's location
	cout	<< "\n Please enter the file location for the NFA or DFA you wish to convert to the formate A*:\n";
	cin		>> sFileName;

	//open the file
	inputFile.open(sFileName);

	//loop through each line in the file: there should be 5 lines
	for (i = 0; i < 5; i++)
	{
		int j,			//secondary counter
			len;
		string	line;	//line from the file
		string*	temp;	//temporary string array

		
		//get the line and stor both it and its length
		getline(inputFile, line);
		len = line.size();
		
		//set up string array: larger than necessary
		temp = new string[len];

		switch(i)//Checks which line it is parsing. 
		{
			case 0://finds the set of states
			{
				int y = 0;
				//loop through characters in the line
				for (j = 0; j < len; j++)
				{
					if (line[j] == ',')
					{//state divison
						y++;
					}
					else if (line[j] != '\n')
					{//add char to state's placement in temp string
						temp[y] = temp[y] + line[j];
					}
				}

				//add one for new state
				y++;

				//set up the NFA's states to the proper number
				myNFA.sStates = new string[y];
				myNFA.iNumStates = y;

				//loop through temp array moving states to NFA
				for (j = 0; j < y; j++)
				{
					myNFA.sStates[j] = temp[j];
				}

				//add new state to NFA
				myNFA.sStates[y - 1] = "Qnewstart";



				break;
			}



			case 1://finds the alphabet of the NFA
			{
				int y = 0;

				//new array to hold alfa characters
				char* cTemp = new char[len];

				//loop through line and extract charactors
				for (j = 0; j < len; j++)
				{
					if (line[j] == ',')
					{//charactor divison found
						y++;
					}
					else if (line[j] != '\n')
					{//put character in temp
						cTemp[y] = line[j];
					}
				}

				//initialize the NFA's array of alfa chars
				myNFA.cAlfa = new char[y];
				myNFA.iNumAlfa = y;

				for (j = 0; j < y; j++)
				{//add chars to NFA
					myNFA.cAlfa[j] = cTemp[j];
				}
				
				break;
			}
			
			
			
			case 2://finds the transitions of the NFA
			{
				//Just store this line as a entire string. I will parse it below and put it into a triple array
				//for later: string[number of states][number of states][3 for the three alphabet charecters]
				for (j = 0; j < len; j++) 
				{
					sTempTransitionHolder[j] = line[j];//This parses the transitions into a form we will be able to handle lower down
				}


				break;
			}
			
			
			
			case 3://finds the start state of the NFA
			{
				string tempState;
				for (j = 0; j < len; j++)
				{//aquire the starting states name
					if ((line[j] != ',') && (line[j] != '\n'))
					{
						tempState = tempState + line[j];
					}
				}

				//find the address of the start state
				myNFA.iStart = -1;
				for (j = 0; j < myNFA.iNumStates; j++)
				{
					if (tempState.compare(myNFA.sStates[j]) == 0)
					{//address found
						myNFA.iStart = j;
						j = myNFA.iNumStates;
					}
				}

				if (myNFA.iStart == -1)
				{//determin if the address was not found
					cout << "\n Start State is not in the set of states!\n";
				}

				break;
			}
			
			
			
			case 4://finds the accept states of the NFA.
			{
				int y = 0;
				//loop through the line and get the names of the finish states
				for (j = 0; j < len; j++)
				{
					if (line[j] == ',')
					{//state division found
						y++;
					}
					else if (line[j] != '\n')
					{
						temp[y] = temp[y] + line[j];
					}
				}

				//initialize the NFA's array of final state addresses
				myNFA.iNumFinalStates = y;
				myNFA.iFinal = new int[y];
				for (j = 0; j < y; j++)
				{
					myNFA.iFinal[j] = -1;
				}

				
				int m;		//tertiary counter

				for (j = 0; j < y; j++)
				{//loop through final states
					for (m = 0; m < myNFA.iNumStates; m++)
					{//loop through all states
						if (temp[j].compare(myNFA.sStates[m]) == 0)
						{//save the address of the states
							myNFA.iFinal[j] = m;
							m = myNFA.iNumStates;
							
						}
					}


				}

				break;
			}

		}

	}
	
	//I think this is where I want to parse the 

	//This sets the size of our triple array.
	myNFA.sFinalTransitions[myNFA.iNumStates][myNFA.iNumStates][3];

	//This sets all points in our array to 'z' this will allow us to check
	for (int i = 0; i < myNFA.iNumStates; i++) {
		for (int j = 0; j < myNFA.iNumStates; j++) {
			for (int k = 0; k < 3; k++) {
				myNFA.sFinalTransitions[i][j][k] = 'z';
			}
		}
	}

	//This is where we will parse and store the transitions
	string* sTempString1;
	string* sTempString2;
	string* sTempString3;
	int iParseingInt = 0;//This will be used to keep track of where I am in each tuple I am parsing. ex (0,1,2)
	int iNumberOfTransitions = 0;
	int iTempTranstionHolderSize = (int) sTempTransitionHolder.size();

	for (int i = 0; i, i < iTempTranstionHolderSize; i++) {

		if (sTempTransitionHolder[i] == '(')
		{
			iParseingInt = 0;
		}
		if (sTempTransitionHolder[i] == ',') {
			iParseingInt++;
		}
		if (iParseingInt == 0 && sTempTransitionHolder[i] != '(' && sTempTransitionHolder[i] != ',' && sTempTransitionHolder[i] != ')')
		{
			sTempString1[iParseingInt][i]=;
		}




	}//end for loop

	


	//close the file
	inputFile.close();

    return 0;
}

