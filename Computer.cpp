/********************************************************************************
Author:        Olga Kent
Date Created:  May 18, 2017
Course:        CSCI 340
Professor:     Pavel Shostak
Assignment:    OS simulation program
File name:     Computer.cpp

Description:   This file contains functions used for simulation of some aspects
of Operating System such as to manage disks and printers with the queues as well as
a ready queue for the CPU, display help Menu, and the state of the memory with
the range of addresses occupied by each process in the system.
*******************************************************************************/
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "Computer.h"

using namespace std;

void Computer::helpMenu() {
cout << "***************************************************************************\n\n"
     << " Help Menu \n\n"
     << " Please enter one of the following commands:\n"
     << "A # #  - to create a new process with priority number and size(in bytes) \n"
     << "t      - to terminate the process that currently uses the CPU\n"
     << "d #    - to request the hard disk# for the currently running process\n"
     << "p #    - to request the printer# for the currently running process\n"
     << "D #    - to finish the work of the hard disk# for one process\n"
     << "P #    - to finish the work of the printer# for one process\n"
     << "S r    - to show a snapshot of the process that currently uses the CPU\n"
     << "              the processes that waits on each level of the ready-queue\n"
     << "S i    - to show what processes are currently using the I/O devices\n"
     << "              and what processes are waiting to use them\n"
     << "S m    - to show the state of Memory ( displays the range of addresses\n"
     << "              occupied by each process in the system)\n"
     << "x      - to exit the application immediately\n"
     << "***************************************************************************\n";
}


Queue::Queue(int index)
:id(index)
{
	head = NULL;
	tail = NULL;
}

Queue::Queue()
{
	head = NULL;
	tail = NULL;
}


void Queue::push(ProcessBlock *item)
{
	if (head == NULL)
	{
		head = item;
		item->next = NULL;
		tail = head;
	}
	else
	{
		tail->next = item;
		item->next = NULL;
		tail = tail->next;
	}
}

// Goes through memory holes and connects them together
void Computer::mergeSlots(MemorySlot *&slot)
{
	if (slot != NULL)
	{
		if (slot->pid != -1)
		{
			mergeSlots(slot->next);
		}
		else if (slot->next != NULL)
		{
			if (slot->next->pid != -1)
			{
				mergeSlots(slot->next);
			}
			else
			{
				MemorySlot *temp = slot->next;
				slot->size += slot->next->size;
				slot->next = slot->next->next;
				delete temp;
				mergeSlots(slot);
			}
		}

	}
}
ProcessBlock * Queue::pull()
{
	ProcessBlock *returnValue = head;
	head = head->next;
	if (head == NULL)
	{
		tail = NULL;
	}
	return returnValue;
}

void Queue::print()
{
		ProcessBlock *node = head;
		while (node!=NULL)
		{
			node->print();
			node = node->next;
		}
}

ProcessBlock::ProcessBlock(int index, int priority, int size)
:id(index),priority(priority),size(size)
{

}

void ProcessBlock::print()
{
	cout << "pid" << id << endl;
}


Computer::Computer()
{
	cout << "How many printers the simulated computer has? ";
	cin >> printerCount;
	int i;
	printers = new Queue*[printerCount];
	for(i = 0; i < printerCount; i++)
	{
		printers[i] = new Queue();
	}
	cout << "How many disks the simulated computer has? ";
	cin >> diskCount;
	disks = new Queue*[diskCount];
	for(i = 0; i < diskCount; i++)
	{
		disks[i] = new Queue();
	}
	nextID = 0;
	cout << "How much RAM memory is there on the simulated computer? (in bytes) ";
	unsigned int size;
	cin >> size;
	memory = new MemorySlot(0,size,-1,NULL);
}


bool Computer::execute(char command, int index)
{
	Queue *queue = NULL;
	if (index < 0)
		return false;
	switch (command)
	{
	case 'P':
	case 'p':
		if (index >= diskCount)
			return false;
		queue = printers[index];
		break;
	case 'd':
	case 'D':
		if (index >= diskCount)
			return false;
		queue = disks[index];
		break;
	default:
		return false;
	}
	if(queue == NULL)
		return false;
	if (command == 'D' || command == 'P')
	{
		ProcessBlock *proccess = queue->pull();
		addProcess(proccess);
	}
	else
	{
		if (currentProcess == NULL)
		{
			cout << "CPU is idle" << endl;
			return false;
		}
		else
		{
			ProcessBlock *proccess = currentProcess;
			currentProcess = getNextProcces();
			queue->push(proccess);
		}
	}
	return true;
}

bool Computer::allocateSlot(ProcessBlock *process)
{
	MemorySlot *node = memory;
	MemorySlot *firstSlot = NULL;
	while (node != NULL)
	{
		if (process->size <= node->size&&node->pid==-1)
		{
			firstSlot = node;
			break;
		}
		node = node->next;
	}
	if (firstSlot != NULL)
	{
		if (firstSlot->size - process->size > 0)
		{
			MemorySlot *newSlot = new MemorySlot(firstSlot->start + process->size, firstSlot->size - process->size, -1, firstSlot->next);
			firstSlot->pid = process->id;
			firstSlot->size = process->size;
			firstSlot->next = newSlot;
		}
		else
		{
			firstSlot->pid = process->id;
		}
		return true;
	}
	return false;

}

ProcessBlock * Computer::getNextProcces()
{
	for (int i = 0; i < 5; i++)
	{
		if (!readyQueue[4-i].isEmpty())
		{
			return readyQueue[4-i].pull();
		}
	}
	return NULL;
}

void Computer::freeMemory(ProcessBlock *pcb)
{
	MemorySlot *current = memory;
	while (current != NULL)
	{
		if (current->pid == pcb->id)
		{
			current->pid = -1;
		}
		current = current->next;
	}
	mergeSlots(memory);
}
// Displays the range of the addresses occupied by each proccess in the memory
void Computer::displayMemory()
{
	MemorySlot *node = memory;
    cout << "*************************************\n";
    cout << "          THE STATE OF MEMORY     " << endl;
	cout << "Size          pid     Start     End\n" ;
	while (node != NULL)
	{
        if (node->pid != -1)
        {
            cout << setw(5) << node->size  << setw(10) << node->pid << "  ";
            cout << setw(10) << node->start << setw(10) << (node->start + node->size -1)<< endl;
        }
        node = node->next;
		//cout << setw(5) << node->start << setw(10) << node->size << "  ";
		/*
        if (node->pid == -1)
		{
			cout << " FREE" << endl;
		}
		else
		{
			cout << " " << node->pid << endl;
		}
		node = node->next;
        */
	}
    cout << "*************************************\n";

}
// Displays what processes are currently using the printers
void Computer::displayIO()
{
	int i;
	for (i = 0; i<printerCount; i++)
	{
		cout << "Printer " << i << endl;
		printers[i]->print();
	}
	//for (i = 0; i<diskCount; i++)
	//{
	//	cout << "Printer " << i << endl;
	//	disks[i]->print();
	//}
}
// Displays a snapshot of the process that currently uses the CPU
// the processes that waits on each level of the ready-queue
void Computer::display()
{
	if (currentProcess != NULL)
	{
		cout << "CPU proccess " << currentProcess->id << endl;
	}
	else
		cout << "CPU is idle \n";
	for (int i = 0; i<5; i++)
	{
		cout << "Level " << i << endl;
		readyQueue[i].print();
	}
}

// Terminates the process and releases the memory used by it
void Computer::terminate()
{
	if (currentProcess != NULL)
	{
		freeMemory(currentProcess);
		delete currentProcess;
		currentProcess = getNextProcces();
		cout << "Success!\n";
	}
	else
	{
		cout << "Invalid command\n";
	}
}

bool Computer::processArrival(int priority, int size)
{
	if (!(priority < 0 || priority>4))
	{
		ProcessBlock * process = new ProcessBlock(++nextID, priority,size);
		if (!allocateSlot(process))
			return false;
		addProcess(process);
		return true;
	}
	return false;
}


void Computer::addProcess(ProcessBlock * pcb)
{
	readyQueue[pcb->priority].push(pcb);
	if (currentProcess == NULL)
	{
		currentProcess = getNextProcces();
	}
}
