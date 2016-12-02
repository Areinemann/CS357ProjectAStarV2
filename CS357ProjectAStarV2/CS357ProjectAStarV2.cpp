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
	nfa*		myNFA		= new nfa;
	int			i;


	cout	<< "\n Please enter the file location for the NFA or DFA you wish to convert to the formate A*:\n";
	cin		>> sFileName;

	inputFile.open(sFileName);

	for (i = 0; i < 5; i++)
	{
		int j;
		string line;

		getline(inputFile, line);

		switch(i)
		{
			case 0:
			{
				for()
				break;
			}
			case 1:
			{
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

	delete	myNFA;
    return 0;
}

