#include "priorityQueue.h"
#include<iostream> 

using namespace std;

// Global constant
const bool DEBUG_MODE = false;

/*
Author: Melissa Paul Creation Date: 4/8/19
Last Modification Date: 4/14/19
Purpose: To initialize private data members when instance of class created.
*/
priorityQueue::priorityQueue() { // Default constructor 
	head = nullptr;
	jobsInLine = 0;
}

/*
Author: Melissa Paul Creation Date: 4/8/19
Last Modification Date: 4/9/19
Purpose: To delete the priority queue once the program is finished.
*/
priorityQueue::~priorityQueue() { // Destructor 
	job *deleteP = head;

	while (head) {
		head = head->nextPtr; 
		deleteP->nextPtr = nullptr; 
		delete deleteP;
		deleteP = head;
	}
}

// Operations

/*
Author: Melissa Paul
Creation Date: 4/8/19
Last Modification Date: 4/8/19
Purpose: To tell whether the priority queue is empty.
*/
bool priorityQueue::isEmpty() const {
	// Return true if empty, false otherwise 
	if (!head) {
		return true;

	}
	else {
		return false;
	}
}

/*
Author: Melissa Paul Creation Date: 4/8/19
Last Modification Date: 4/14/19
Purpose: To add new tasks to the queue in order of priority.
*/
bool priorityQueue::enqueue(const job & newEntry) {
	// If equal priority, add after
	job* curP = nullptr, *prevP = nullptr, *tempP = nullptr; 
	bool equal = false;

	jobsInLine++; 
	
	if (!head) {
		head = new job(newEntry); 
		curP = head;

		if (DEBUG_MODE) {
			cout << endl << "Enqueue {" << head->arriveTime << ", " << head->jobNum << ", " 
				<< head->jobType << ", " << head->jobTypeNum << ", " << head->processTime << ", " << head->waitTime << "}: " << endl;
		}
	}
	else {
		prevP = nullptr; 
		curP = head;
		tempP = new job(newEntry); 
		if (DEBUG_MODE) {
			cout << endl << "Enqueue {" << tempP->arriveTime << ", " << tempP->jobNum << ", "
				<< tempP->jobType << ", " << tempP->jobTypeNum << ", " << tempP->processTime << ", " <<
				tempP->waitTime << "}: " << endl;
		}

		while (curP) {
			if (curP->arriveTime == tempP->arriveTime) {
				tempP->nextPtr = curP->nextPtr; 
				curP->nextPtr = tempP;
				equal = true; 
				break;
			}

			curP = curP->nextPtr;
		}

		if (!equal) {
			curP = head;

			while (tempP->arriveTime > curP->arriveTime && curP->nextPtr) {
				prevP = curP;
				curP = curP->nextPtr;
			}

			if (curP->arriveTime > tempP->arriveTime) {
				if (curP == head) {
					head = tempP;
				}
				else {
					prevP->nextPtr = tempP;
				}

				tempP->nextPtr = curP;
			}
			else {
				curP->nextPtr = tempP;
			}
		}
	}

	if (DEBUG_MODE) {
		job* moveP = head;

		while (moveP) {
			cout << moveP->arriveTime << " " << moveP->jobNum << " " << moveP->jobType << " " << moveP->jobTypeNum << " "
				<< moveP->processTime << " " << moveP->waitTime << endl;

			moveP = moveP->nextPtr;
		}
	}

	return true;
}

/*
Author: Melissa Paul Creation Date: 4/8/19
Last Modification Date: 4/14/19
Purpose: To remove task from front of priority queue with highest priority.
*/
bool priorityQueue::dequeue() {
	// Error if priority queue is empty 
	job *deleteP = nullptr;;

	if (isEmpty()) {
		cout << "Error: priority queue is empty" << endl; return false;
	}
	else {
		jobsInLine--;

		if (DEBUG_MODE) {
			cout << endl << "Dequeue {" << head->arriveTime << ", " << head->jobNum << ", "

				<< head->jobType << ", " << head->jobTypeNum << ", " << head->processTime << ", " <<
					head->waitTime << "}: " << endl;
		}

		deleteP = head;
		head = head->nextPtr; 
		deleteP->nextPtr = nullptr; 
		delete deleteP;
		deleteP = nullptr;
	}

	if (DEBUG_MODE) {
		job* moveP = head;

		while (moveP) {
			cout << moveP->arriveTime << " " << moveP->jobNum << " " << moveP->jobType << " " << moveP->jobTypeNum << " "
				<< moveP->processTime << " " << moveP->waitTime << endl;

			moveP = moveP->nextPtr;
		}
	}

	return true;
}

/*
Author: Melissa Paul Creation Date: 4/8/19
Last Modification Date: 4/12/19
Purpose: To return the next highest priority task without removing from queue.
*/
job priorityQueue::peek() const {
	job first;

	first.arriveTime = head->arriveTime;

	first.jobNum = head->jobNum; first.jobType = head->jobType; first.jobTypeNum = head->jobTypeNum; first.processTime = head->processTime; first.waitTime = head->waitTime;

	return first;
}

/*
Author: Melissa Paul Creation Date: 4/11/19
Last Modification Date: 4/14/19
Purpose: To remove the old depart event for the wait job from the event list.
*/
void priorityQueue::deleteDepart(job waitJob) {
	job* deleteP = head, *preP = nullptr;

	jobsInLine--; // Only matters for eventListPQ 
	
	while (deleteP) {
		if (waitJob.jobNum == deleteP->jobNum) {
			if (DEBUG_MODE) {
				cout << endl << "Delete {" << deleteP->arriveTime << ", " <<
					deleteP->jobNum << ", " << deleteP->jobType << ", " << deleteP->jobTypeNum << ", " <<
					deleteP->processTime  << ", " << deleteP->waitTime << "}: " << endl;
			}

			if (deleteP == head) {
				dequeue();
			}
			else {
				preP->nextPtr = deleteP->nextPtr; 
				deleteP->nextPtr = nullptr;
				delete deleteP;

				if (DEBUG_MODE) {
					job* moveP = head;

					while (moveP) {
						cout << moveP->arriveTime << " " << moveP->jobNum << " " << moveP->jobType << " "
							<< moveP->jobTypeNum << " " << moveP->processTime << " " << moveP->waitTime << endl;

						moveP = moveP->nextPtr;
					}
				}
			}

			deleteP = nullptr;
			break;
		}
		else {
			preP = deleteP;
			deleteP = deleteP->nextPtr;
		}
	}
}

/*
Author: Melissa Paul Creation Date: 4/16/19
Last Modification Date: 4/16/19
Purpose: To partially display the priority queue to help with analysis of the simulation.
*/
void priorityQueue::partDisplay() {
	job* moveP = head;

	for (int i = 0; i < 200; i++) {
		cout << moveP->jobType << " " << moveP->arriveTime << " " << moveP->processTime << endl;

		moveP = moveP->nextPtr;
	}
}
