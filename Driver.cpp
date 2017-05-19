/********************************************************************************
Author:        Olga Kent
Date Created:  May 18, 2017
Course:        CSCI 340
Professor:     Pavel Shostak
Assignment:    OS simulation program
File name:     Driver.cpp

Description: Driver  is the main entry point into the application that executes
the main program loop. It serves as the main interaction point with the user,
controlling input and output in the program.
*******************************************************************************/
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

#include "Computer.h"

int main()
{
cout << "***************************************************************************\n"
     << "                            System simulation                              \n"
     << "***************************************************************************\n";
	char command,type;
	int number;
	int priority;
	unsigned int size;
	Computer computer;
	computer.helpMenu();
	do
	{
		cout << ">" ;
		cin >> command;
		switch (command)
		{
		case 'P':
		case 'p':
		case 'd':
		case 'D':
			cin >> number;
			computer.execute(command, number);
			break;
		case 't':
			computer.terminate();
			break;
		case 'A':
			cin >> priority;
			cin >> size;
			if(!computer.processArrival(priority, size))
				cout << "Error!\n";
			break;
		case 'S':
			cin >> type;
			switch (type)
			{
			case 'r':
				computer.display();
				break;
			case 'm':
				computer.displayMemory();
				break;
			case 'i':
				computer.displayIO();
				break;
			}
			break;
		}
	} while (command!='x');
	return 0;
}
