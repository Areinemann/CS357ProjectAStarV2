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
	int*** iFinalTransitions;
	char*	cAlfa;
	int		iStart,
			iNumStates,
			iNumAlfa,
			iNumTrans,
			iNumFinalStates;
	int*	iFinal;
};

//printNFA
//
//prints a NFA to a txt file
//
//nfaToPrint	the NFA being printed
int printNFA(nfa nfaToPrint)
{
	//Variables
	string		sFileName,
				sStream;	//the output to write.
	ofstream	outputFile;
	int			i;			//counter

	//get the file's location
	cout << "\nPlease enter the file location you would like A* to print to:\n";
	cin >> sFileName;

	sFileName = sFileName + "\\AStarNFA.txt";

	//start stream
	sStream = "({";

	//add states to stream
	for (i = 0; i < nfaToPrint.iNumStates; i++)
	{
		sStream = sStream + nfaToPrint.sStates[i];
		if(i < (nfaToPrint.iNumStates-1))//add the division between states
			sStream = sStream + ",";
	}

	//close the states and open the alphabet
	sStream = sStream + "},{";

	//add alphabet to stream
	for (i = 0; i < nfaToPrint.iNumAlfa-1; i++)
	{
		sStream = sStream + nfaToPrint.cAlfa[i];
		if (i < (nfaToPrint.iNumAlfa - 2))//add the division between alphabet chars
			sStream = sStream + ",";
	}
	
	//close the alphabet and open transitions
	sStream = sStream + "},{";

	//write transitions
	int count = 0;
	for (i = 0; i < nfaToPrint.iNumStates; i++)
	{//loop through cols of trans table
		for (int j = 0; j < nfaToPrint.iNumStates; j++)
		{//loop through rows of trans table
			for (int m = 0; m < nfaToPrint.iNumAlfa; m++)
			{//loop through alphabet
				if (nfaToPrint.iFinalTransitions[i][j][m] != -1)
				{//check trans exists
					if (m == nfaToPrint.iNumAlfa-1)
					{//check if epsilon
						sStream = sStream + "(" + nfaToPrint.sStates[i] + "," + nfaToPrint.sStates[j] + ",E)";
						count++;

						if (count < nfaToPrint.iNumTrans)//if not last trans, add divider
							sStream += ",";
					}
					else
					{
						sStream = sStream + "(" + nfaToPrint.sStates[i] + "," + nfaToPrint.sStates[j] + "," + nfaToPrint.cAlfa[m] + ")";
						count++;

						if (count < nfaToPrint.iNumTrans)//if not last trans, add divider
							sStream += ",";
					}
				}
			}//for m
		}//for j
	}//for i
	//close the transitions and add division for start state.
	sStream = sStream + "},";

	//write the start state
	if (nfaToPrint.iStart == -1)
	{
		sStream = sStream + " (Invalid Start State Specified) ";
	}
	else
	{
		sStream = sStream + nfaToPrint.sStates[nfaToPrint.iStart];
	}

	//divide start state and start final states
	sStream = sStream + ",{";

	//write final states
	for (i = 0; i < nfaToPrint.iNumFinalStates; i++)
	{
		if (nfaToPrint.iFinal[i] == -1)
		{
			sStream = sStream + " (Invalid Accept State Specified) ";
		}
		else
		{
			sStream = sStream + nfaToPrint.sStates[nfaToPrint.iFinal[i]];
		}
		if (i < (nfaToPrint.iNumFinalStates - 1))//add the division between final states
			sStream = sStream + ",";
	}//for i

	//close final states and formal definition
	sStream = sStream + "})\n";

	//open the file
	outputFile.open (sFileName);

	//write the stream to file
	outputFile << sStream;

	//close the file
	outputFile.close();

	return 0;
}//printNFA

//gets the index of a state in the NFA
int indexOfState(string tempString, nfa myNfa) {

	for (int i = 0; i < myNfa.iNumStates; i++) 
		{
			if (tempString.compare(myNfa.sStates[i]) == 0)
			{	
				return i;//find something case
			}
		}
	return -1;//find nothing case

}//indexOfState

//gets the index of a char in the NFA's alfphabet
int indexOfAlfa(char charToFind, nfa myNfa) {

	if (charToFind == 'E')
	{//cehck if epsilon
		return myNfa.iNumAlfa - 1;
	}

	for (int i = 0; i < myNfa.iNumAlfa; i++)
	{
		if (charToFind == myNfa.cAlfa[i])
		{
			return i;//find something case
		}
	}
	return -1;//find nothing case

}//indexOfAlfa

//main function for retrieving an NFA and converting it into A* 
int main()
{
	//Variables
	string		sFileName;
	ifstream	inputFile;
	nfa			myNFA;		//NFA used in program
	int			i;			//counter
	string		sTempTransitionHolder; //this will store the transitions so we can use the data later.
	

	//get the file's location
	cout	<< "\nPlease enter the file location for the NFA or DFA you wish to convert to the formate A*:\n";
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
			}//case 0



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
				myNFA.iNumAlfa = y+1;

				for (j = 0; j < y; j++)
				{//add chars to NFA
					myNFA.cAlfa[j] = cTemp[j];
				}


				
				break;
			}//case 1
			
			
			
			case 2://finds the transitions of the NFA
			{
				//Just store this line as a entire string. I will parse it below and put it into a triple array
				//for later: string[number of states][number of states][3 for the three alphabet charecters]
				//for (j = 0; j < len; j++) 
				//{
					sTempTransitionHolder = line;//This parses the transitions into a form we will be able to handle lower down
				//}


				break;
			}//case 2
			
			
			
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
			}//case 3
			
			
			
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

				//TODO: add check for final states

				break;
			}//case 4

		}//switch

	}//for i: loops through input file


	 //close the file
	inputFile.close();
	
	//I think this is where I want to parse the 

	//This sets the size of our triple array.
	myNFA.iFinalTransitions = new int**[myNFA.iNumStates];
	for (i = 0; i<myNFA.iNumStates; i++)
	{
		myNFA.iFinalTransitions[i] = new int*[myNFA.iNumStates];

		for (int dog = 0; dog< myNFA.iNumStates; dog++)
		{
			myNFA.iFinalTransitions[i][dog] = new int[myNFA.iNumAlfa];

			for (int k = 0; k < myNFA.iNumAlfa; k++)
			{
				myNFA.iFinalTransitions[i][dog][k] = -1;
			}

		}
	}
	
	
	//This is where we will parse and store the transitions
	string sTempString1;//this will hold the first state of the transtion
	string sTempString2;//this will hold the state you are in after the transition
	char cTempString3				= ' ';//this will be what it does the transtion on
	int iParseingInt				= 0;//This will be used to keep track of where I am in each tuple I am parsing. ex (0,1,2)
	int iNumberOfTransitions		= 0;
	int iTempTranstionHolderSize	= (int) sTempTransitionHolder.size();

	myNFA.iNumTrans = 0;

	for (int i = 0; i, i < iTempTranstionHolderSize; i++) {

		
		while (sTempTransitionHolder[i] != '|')
		{

			if (iParseingInt == 0 && sTempTransitionHolder[i] != '|' && sTempTransitionHolder[i] != ',')
			{
				int i1Temp = 0;
				sTempString1 = sTempString1 + sTempTransitionHolder[i];
				i1Temp++;
			}

			if (iParseingInt == 1 && sTempTransitionHolder[i] != '|' && sTempTransitionHolder[i] != ',')
			{
				int i2Temp = 0;
				sTempString2 += sTempTransitionHolder[i];
				i2Temp++;
			}

			if (iParseingInt == 2 && sTempTransitionHolder[i] != '|' && sTempTransitionHolder[i] != ',')
			{
				int i3Temp = 0;
				cTempString3 = sTempTransitionHolder[i];
				i3Temp++;
			}

			if (sTempTransitionHolder[i] == ',') {
				iParseingInt++;
			}
			i++;
		}//while (sTempTransitionHolder[i] != '|')
		int levelDeep = 0;



		//************This needs to change******************
		//for (int jk = 0; jk < myNFA.iNumAlfa; jk ++ ) {

			int indexStateOne	= indexOfState(sTempString1, myNFA),
				indexStateTwo	= indexOfState(sTempString2, myNFA),
				indexAlfa		= indexOfAlfa(cTempString3, myNFA);

			myNFA.iFinalTransitions[indexStateOne][indexStateTwo][indexAlfa] = 1;
			myNFA.iNumTrans++;

			//if ((myNFA.iFinalTransitions[indexOfState(sTempString1, myNFA)][indexOfState(sTempString2, myNFA)][levelDeep]).compare("")) {
			//	levelDeep++;
			//}
		//}//for jk

		//This line stores the transitions in our data structure.
			//myNFA.iFinalTransitions[indexOfState(sTempString1, myNFA)][indexOfState(sTempString2, myNFA)][levelDeep] = sTempString3;
		


	//*************End Big changes*******************


		//this resets the tracker to zero so we can parse the next transition.
		if (sTempTransitionHolder[i] == '|') {
			iParseingInt = 0;
			iNumberOfTransitions++;
			sTempString1 = "";
			sTempString2 = "";
			cTempString3 = ' ';
		}




	}//end for loop

	for (int i = 0; i < myNFA.iNumFinalStates; i++) 
	{
		myNFA.iFinalTransitions[myNFA.iFinal[i]][myNFA.iStart][myNFA.iNumAlfa - 1] = 1;
	}


	//add transition from Qnewstart to old start


	//prints the nfa
	printNFA(myNFA);

    return 0;
}//main

