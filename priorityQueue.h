#pragma once

// Structure 
struct job {
	char jobType; 
	int arriveTime;
	int processTime = 0; 
	int waitTime = 0;
	int jobTypeNum = 0; // Sequentially assigned as job type arrives 
	int jobNum = 0; // Sequentially assigned as job type arrives
	job* nextPtr = nullptr;
};

class priorityQueue {
private:
	job* head; // Pointer to sorted list of items in priority queue
public:
	int jobsInLine;
	priorityQueue(); // Default constructor
	~priorityQueue(); // Destructor

	// Operations after
	bool isEmpty() const; // Return true if empty, false otherwise
	bool enqueue(const job& newEntry); // Add new task to queue in order of priority; if equal priority, add

	bool dequeue(); // Error if priority queue is empty; remove task from front with highest priority 
	job peek() const; // Return next highest priority task without removing from queue
	void deleteDepart(job waitJob);
	void partDisplay();
};






