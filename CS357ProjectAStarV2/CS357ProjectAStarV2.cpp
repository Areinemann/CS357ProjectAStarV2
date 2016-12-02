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
	string		sFileName;
	ifstream	inputFile;
	nfa			myNFA;
	int			i;


	cout	<< "\n Please enter the file location for the NFA or DFA you wish to convert to the formate A*:\n";
	cin		>> sFileName;

	inputFile.open(sFileName);

	for (i = 0; i < 5; i++)
	{
		int j,
			len;
		string	line;
		string*	temp;

		

		getline(inputFile, line);
		len = line.size();
		
		temp = new string[len];

		switch(i)
		{
			case 0://finds the set of states
			{
				int y = 0;
				for (j = 0; j < len; j++)
				{
					if (line[j] == ',')
					{
						y++;
					}
					else if (line[j] != '\n')
					{
						temp[y] = temp[y] + line[j];
					}
				}

				y++;

				myNFA.sStates = new string[y];

				for (j = 0; j < y; j++)
				{
					myNFA.sStates[j] = temp[j];
				}

				myNFA.sStates[y - 1] = "Qnewstart";

				myNFA.iNumStates = y;

				break;
			}
			case 1://finds the alphabet of the NFA
			{
				int y = 0;
				char* cTemp = new char[len];
				for (j = 0; j < len; j++)
				{
					if (line[j] == ',')
					{
						y++;
					}
					else if (line[j] != '\n')
					{
						cTemp[y] = line[j];
					}
				}

				myNFA.cAlfa = new char[y];

				for (j = 0; j < y; j++)
				{
					myNFA.cAlfa[j] = cTemp[j];
				}

				myNFA.iNumAlfa = y;

				break;
			}
			case 2://finds the transitions of the NFA
			{
				break;
			}
			case 3://finds the start state of the NFA
			{
				string tempState;
				for (j = 0; j < len; j++)
				{
					if ((line[j] != ',') && (line[j] != '\n'))
					{
						tempState = tempState + line[j];
					}
				}

				myNFA.iStart = -1;
				for (j = 0; j < myNFA.iNumStates; j++)
				{
					if (tempState.compare(myNFA.sStates[j]) == 0)
					{
						myNFA.iStart = j;
						j = myNFA.iNumStates;
					}
				}

				if (myNFA.iStart == -1)
				{
					cout << "\n Start State is not in the set of states!\n";
				}

				break;
			}
			case 4://finds the accept states of the NFA.
			{
				int y = 0;
				for (j = 0; j < len; j++)
				{
					if (line[j] == ',')
					{
						y++;
					}
					else if (line[j] != '\n')
					{
						temp[y] = temp[y] + line[j];
					}
				}

				myNFA.iNumFinalStates = y;
				myNFA.iFinal = new int[y];
				
				int m;

				for (j = 0; j < y; j++)
				{
					for (m = 0; j < myNFA.iNumStates; m++)
					{
						if (temp[j].compare(myNFA.sStates[m]) == 0)
						{
							myNFA.iFinal[j] = m;
							m = myNFA.iNumStates;
						}
					}
				}

				break;
			}
		}
	}

	inputFile.close();

    return 0;
}

