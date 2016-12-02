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
	int		iStart;
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
			case 0:
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

				break;
			}
			case 1:
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

				break;
			}
			case 2:
			{
				break;
			}
			case 3:
			{
				break;
			}
			case 4:
			{
				break;
			}
			default:
			{
				break;
			}
		}
	}

	inputFile.close();

    return 0;
}

