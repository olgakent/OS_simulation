/********************************************************************************
Author:        Olga Kent
Date Created:  May 18, 2017
Course:        CSCI 340
Professor:     Pavel Shostak
Assignment:    OS simulation program
File name:     Computer.h

Description:   This header/declaration file contains data structures used as
resources for simulation of some aspects of Operating System. It contains data
structures that help to:
- keep information about the process in Process Control block,
- manage the queues and
- represent memory in the Computer.
*******************************************************************************/
#ifndef COMPUTER_H
#define COMPUTER_H
struct ProcessBlock
{
	ProcessBlock *next;
	int id,priority,size;
	ProcessBlock(int index, int  priority,int size);
	void print();
};

class Queue
{
public:
	void push(ProcessBlock *);
	ProcessBlock * pull();
	bool isEmpty() { return head == NULL;};
	Queue(int index);
	Queue();
	void print();
private:
	ProcessBlock *head;
	ProcessBlock *tail;
	int id;
};

class Computer
{
private:
	struct MemorySlot
	{
		unsigned int start, size, pid;
		MemorySlot() {};
		MemorySlot(unsigned int start, unsigned int  size, int  PID, MemorySlot *next)
			:start(start), size(size), pid(PID), next(next) {};
		MemorySlot *next;
	};
	Queue** disks;
	Queue** printers;
	Queue readyQueue[5];
	ProcessBlock *currentProcess;
	int nextID,diskCount,printerCount;
	MemorySlot *memory;
	void mergeSlots(MemorySlot *&);
	ProcessBlock * getNextProcces();
public:
	Computer();
	void helpMenu();
	bool execute(char command, int index);
	bool allocateSlot(ProcessBlock *);
	void freeMemory(ProcessBlock *);
	void addProcess(ProcessBlock *);
	void displayMemory();
	void displayIO();
	void display();
	void terminate();
	bool processArrival(int priority, int size);
};
#endif //Computer.h
