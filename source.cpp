/*
Author: Melissa Paul Creation Date: 4/2/19
Last Modification Date: 4/16/19
Purpose: To simluate a processor using a priority queue and generate metrics.
*/

#include<iostream> 
#include<fstream> 
#include<iomanip> 
#include<string> 
#include<vector> 
#include<time.h> 
#include"priorityQueue.h"

using namespace std;

// Global constant
const int MAX_TIME = 10000;

// Global variables 
int curTime = 0,
Acount = 0, Bcount = 0, Ccount = 0, Dcount = 0,
jobCount = 0, interruptCount = 0, Darrive = 0, idleTime = 0, runTime = 0,
departTime = 0, departTime2 = 0, departTime3 = 0; 
double totalWait = 0.0;
bool processorAvailable = true, processor2Available = true, processor3Available = true, first = false, first2 = false, first3 = false;
job runJob, runJob2, runJob3;

// Function prototypes
void processArrive(job, priorityQueue&, priorityQueue&, priorityQueue&, priorityQueue&, ofstream&); 
void processDepart(job, priorityQueue&, priorityQueue&, priorityQueue&, priorityQueue&, ofstream&); 
void findMin(vector<int>, int&);

/*
Author: Melissa Paul Creation Date: 4/2/19
Last Modification Date: 4/16/19
Purpose: To generate the random event file, create the event list priority queue, process all of the events, and then display the metrics for the simulation.
*/
int main() {
	ofstream outs, log; 
	ifstream ins;
	int occur, Atime = 0, Btime = 0, Ctime = 0, Dtime = 0, processorNum = 3, minDepart; 
	const bool DEBUG_MODE = false;
	priorityQueue jobPQ, eventListPQ, jobPQ2, jobPQ3; 
	job newJob;
	string columns;

	if (DEBUG_MODE) { // Test priorityQueue class 
		priorityQueue testQ, testCopyQ;
		job add = { 'A', 0, 0, 2, 12 }, add2 = { 'B', 0, 0, 4, 12 },
			add3 = { 'C', 0, 0, 1, 12 }, add4 = { 'D', 0, 0, 3, 12 },
			temp;

		cout << "Is the queue empty?: " << testQ.isEmpty() << endl; // Test isEmpty() 
		
		testQ.enqueue(add); // Test enqueue()
		testQ.enqueue(add2); 
		testQ.enqueue(add3); 
		testQ.enqueue(add4);
		
		cout << endl << "Is the queue empty?: " << testQ.isEmpty() << endl; 
		
		temp = testQ.peek(); // Test peek()
		cout << endl << "First job: " << temp.arriveTime << " " << temp.jobNum << " " <<
			temp.jobType << " " << temp.jobTypeNum << " " << temp.processTime << endl;

		testQ.dequeue(); // Test dequeue()
	}

	outs.open("eventList.txt"); log.open("logOfActions.txt");

	srand(time(NULL));

	while (Atime < MAX_TIME && Btime < MAX_TIME && Ctime < MAX_TIME && Dtime <
		MAX_TIME) { // Generate random event file
		occur = rand() % 5 + 1;

		for (int i = 0; i < occur; i++) {
			Atime += (4 + rand() % 3);
			outs << endl << "A" << setw(6) << Atime << setw(6) << 1 + rand() % 5; 
			Btime += (9 + rand() % 3);
			outs << endl << "B" << setw(6) << Btime << setw(6) << 6 + rand() % 5; 
			Ctime += (24 + rand() % 3);
			outs << endl << "C" << setw(6) << Ctime << setw(6) << 11 + rand() % 5; 
			Dtime += (25 + rand() % 11);
			outs << endl << "D" << setw(6) << Dtime << setw(6) << 8 + rand() % 5;
		}
	}

	outs.close();
	ins.open("eventList.txt"); 
	getline(ins, columns);
	while (!ins.eof()) { // Create eventListPQ
		ins >> newJob.jobType >> newJob.arriveTime >> newJob.processTime; 
		eventListPQ.enqueue(newJob);
	}

	if (DEBUG_MODE) {
		eventListPQ.partDisplay();
	}

	ins.close();

	while (!eventListPQ.isEmpty()) { // Event loop 
		newJob = eventListPQ.peek();
		curTime = newJob.arriveTime; // Get current time

		if (!newJob.jobNum) {
			switch (newJob.jobType) {
			case 'A':
				Acount++; 
				newJob.jobTypeNum = Acount; 
				jobCount++;
				newJob.jobNum = jobCount; 
				break;
			case 'B':
				Bcount++;
				newJob.jobTypeNum = Bcount;
				jobCount++;
				newJob.jobNum = jobCount;
				break;
			case 'C':
				Ccount++;
				newJob.jobTypeNum = Ccount; 
				jobCount++;
				newJob.jobNum = jobCount; 
				break;
			case 'D':
				Dcount++; 
				newJob.jobTypeNum = Dcount; 
				jobCount++;
				newJob.jobNum = jobCount; 
				break;
			}
		}

		if (newJob.jobType != 'X') {
			processArrive(newJob, eventListPQ, jobPQ, jobPQ2, jobPQ3, log);

		}
		else {
			processDepart(newJob, eventListPQ, jobPQ, jobPQ2, jobPQ3, log);
		}
	}

	log.close();

	if (curTime != departTime) {
		idleTime += (curTime - departTime);
	}
	if (curTime != departTime2) {
		idleTime += (curTime - departTime2);
	}
	if (curTime != departTime3) {
		idleTime += (curTime - departTime3);
	}

	// Metrics
	cout << endl << "Average time in queue: " << totalWait / jobCount << " time units." << endl;
	cout << "Idle time: " << idleTime << " time units." << endl;
	cout << "Total number of jobs A arrived: " << Acount << endl; 
	cout << "Total number of jobs B arrived: " << Bcount << endl; 
	cout << "Total number of jobs C arrived: " << Ccount << endl; 
	cout << "Total number of jobs D arrived: " << Dcount << endl;
	cout << "Total wait time in queue: " << totalWait << " time units." << endl; 
	cout << "Jobs interrupted: " << interruptCount << endl;
	cout << "Total jobs completed: " << jobCount - interruptCount << endl; 
	cout << "Number of processor(s) used: " << processorNum << endl;
	cout << "Total number of time units the processor(s) run: " << runTime << endl;

	system("pause"); 
	return 0;
}

// Function definitions

/*
Author: Melissa Paul 
Creation Date: 4/10/19
Last Modification Date: 4/15/19 
Purpose: To process an arrival event.
*/
void processArrive(job newJob, priorityQueue& eventListPQ, priorityQueue& jobQueue, priorityQueue& jobQueue2,
	priorityQueue& jobQueue3, ofstream& log) {
	const bool DEBUG_MODE = false;
	job departJob, waitJob, departJob2, departJob3; 
	vector<int> jobLineNums;
	int minJobs;

	if (DEBUG_MODE) {
		cout << endl << "Event list ";
	}
	eventListPQ.dequeue(); // Remove event from eventList

	if (jobQueue.isEmpty() && processorAvailable) {
		if (first) {
			idleTime += curTime; 
			first = false;
		}
		else {
			idleTime += curTime - departTime;
		}

		runJob = newJob;
		runTime += runJob.processTime; 
		runJob.waitTime = curTime - runJob.arriveTime; 
		totalWait += runJob.waitTime;
		log << curTime << setw(5) << ") Arrival Job " << runJob.jobType << ": Overall Job " << runJob.jobNum << ", Job "
			<< runJob.jobType << " " << runJob.jobTypeNum << ", Processing time " << runJob.processTime <<
			";" << endl << "Begin Processing Job " << runJob.jobType << " " << runJob.jobTypeNum << " CPU 1 - end time " <<
			curTime + newJob.processTime << "; PQ Empty 1;" << endl;

		departJob.arriveTime = curTime + newJob.processTime;
		departTime = departJob.arriveTime;
		departJob.jobType = 'X'; 
		departJob.jobNum = newJob.jobNum;

		if (DEBUG_MODE) {
			cout << endl << "Event list ";
		}
		eventListPQ.enqueue(departJob); 
		processorAvailable = false;
	}
	else if (jobQueue2.isEmpty() && processor2Available) {
		if (first2) {
			idleTime += curTime; 
			first2 = false;
		}
		else {
			idleTime += curTime - departTime2;
		}

		runJob2 = newJob;
		runTime += runJob2.processTime; 
		runJob2.waitTime = curTime - runJob2.arriveTime; 
		totalWait += runJob2.waitTime;
		log << curTime << setw(5) << ") Arrival Job " << runJob2.jobType << ": Overall Job " << runJob2.jobNum << ", Job "
			<< runJob2.jobType << " " << runJob2.jobTypeNum << ", Processing time " << runJob2.processTime <<
			";" << endl << "Begin Processing Job " << runJob2.jobType << " " << runJob2.jobTypeNum << " CPU 2 - end time " <<
			curTime + newJob.processTime << "; PQ Empty 2;" << endl;

		departJob2.arriveTime = curTime + newJob.processTime; 
		departTime2 = departJob2.arriveTime; 
		departJob2.jobType = 'X';
		departJob2.jobNum = newJob.jobNum;

		if (DEBUG_MODE) {
			cout << endl << "Event list ";
		}

		eventListPQ.enqueue(departJob2); 
		processor2Available = false;
	}
	else if (jobQueue3.isEmpty() && processor3Available) {
		if (first3) {
			idleTime += curTime; 
			first3 = false;
		}
		else {
			idleTime += curTime - departTime3;
		}

		runJob3 = newJob;
		runTime += runJob3.processTime; 
		runJob3.waitTime = curTime - runJob3.arriveTime; 
		totalWait += runJob3.waitTime;
		log << curTime << setw(5) << ") Arrival Job " << runJob3.jobType << ": Overall Job " << runJob3.jobNum << ", Job "
			<< runJob3.jobType << " " << runJob3.jobTypeNum << ", Processing time " << runJob3.processTime <<
			";" << endl << "Begin Processing Job " << runJob3.jobType << " " << runJob3.jobTypeNum << " CPU 3 - end time " <<
			curTime + newJob.processTime << "; PQ Empty 3;" << endl;

		departJob3.arriveTime = curTime + newJob.processTime; 
		departTime3 = departJob3.arriveTime; 
		departJob3.jobType = 'X';
		departJob3.jobNum = newJob.jobNum;

		if (DEBUG_MODE) {
			cout << endl << "Event list ";
		}
		eventListPQ.enqueue(departJob3); 
		processor3Available = false;
	}
	else if (newJob.jobType == 'D') {
		waitJob = runJob;
		interruptCount++;
		// New process time = Old process time - time passed
		waitJob.processTime = waitJob.processTime - (newJob.arriveTime - (runJob.arriveTime + runJob.waitTime));
		runTime -= waitJob.processTime;
		if (DEBUG_MODE) {
			cout << endl << "Job queue 1";
		}
		jobQueue.enqueue(waitJob); if (DEBUG_MODE) {
			cout << endl << "Event list ";
		}
		eventListPQ.deleteDepart(waitJob);
		runJob = newJob;
		runTime += runJob.processTime;
		log << curTime << setw(5) << ") Arrival Job " << runJob.jobType << ": Overall Job " << runJob.jobNum <<
			", Job " << runJob.jobType << " " << runJob.jobTypeNum << ", Processing time "
			<< runJob.processTime <<
			"; Interrupt job " << waitJob.jobType << " " << waitJob.jobTypeNum << "," << endl << "Interrupted jobs: "
			<< interruptCount << ", New high priority job goes into PQ 1; Processor Job " <<
			waitJob.jobType << " "
			<< waitJob.jobTypeNum << endl << "Added to queue with processing time " <<
			waitJob.processTime << ";" << endl;
		Darrive = runJob.arriveTime;
		runJob.waitTime = curTime - runJob.arriveTime;
		totalWait += runJob.waitTime;

		departJob.arriveTime = curTime + newJob.processTime;
		departTime = departJob.arriveTime;
		departJob.jobType = 'X';
		departJob.jobNum = newJob.jobNum;

		if (DEBUG_MODE) {
			cout << endl << "Event list ";
		}
		eventListPQ.enqueue(departJob);
		processorAvailable = false;
	}
	else {
		jobLineNums.push_back(jobQueue.jobsInLine); 
		jobLineNums.push_back(jobQueue2.jobsInLine); 
		jobLineNums.push_back(jobQueue3.jobsInLine);

		findMin(jobLineNums, minJobs);

		if (minJobs == jobQueue.jobsInLine) {
			if (DEBUG_MODE) {
				cout << endl << "Job queue 1";
			}

			jobQueue.enqueue(newJob);
			log << curTime << setw(5) << ") Arrival Job " << newJob.jobType << ": Overall Job " << newJob.jobNum << ", Job "
			<< newJob.jobType << " " << newJob.jobTypeNum << ", Processing time " << newJob.processTime << "; Join PQ 1;" << endl;
		}
		else if (minJobs == jobQueue2.jobsInLine) {
			if (DEBUG_MODE) {
				cout << endl << "Job queue 2";
			}
			jobQueue2.enqueue(newJob);
			log << curTime << setw(5) << ") Arrival Job " << newJob.jobType << ": Overall Job " << newJob.jobNum << ", Job "
				<< newJob.jobType << " " << newJob.jobTypeNum << ", Processing time " << newJob.processTime << "; Join PQ 2;" << endl; 
		}
		else if (minJobs == jobQueue3.jobsInLine) {
			if (DEBUG_MODE) {
				cout << endl << "Job queue 3";
			}
			jobQueue3.enqueue(newJob);
			log << curTime << setw(5) << ") Arrival Job " << newJob.jobType << ": Overall Job " << newJob.jobNum << ", Job "
				<< newJob.jobType << " " << newJob.jobTypeNum << ", Processing time " << newJob.processTime << "; Join PQ 3;" << endl;
		}
	}
}

/*
Author: Melissa Paul 
Creation Date: 4/14/19
Last Modification Date: 4/14/19 
Purpose: To find the minimum number.
*/
void findMin(vector<int> vec, int& min) {
	min = vec[0];

	for (int i = 0; i < vec.size(); i++) {
		if (min > vec[i]) {
			min = vec[i];
		}
	}
}

/*
Author: Melissa Paul 
Creation Date: 4/10/19
Last Modification Date: 4/15/19 
Purpose: To process a departure event.
*/
void processDepart(job departJob, priorityQueue& eventListPQ, priorityQueue& jobQueue, priorityQueue& jobQueue2,
	priorityQueue& jobQueue3, ofstream& log) {
	job nextDepartJob, nextDepartJob2, nextDepartJob3; 
	const bool DEBUG_MODE = false;

	if (DEBUG_MODE) {
		cout << endl << "Event list ";
	}
	eventListPQ.dequeue(); // Remove event from eventList

	if (departJob.jobNum == runJob.jobNum) {
		if (!jobQueue.isEmpty()) {
			log << curTime << setw(5) << ") Job " << runJob.jobType << " " << runJob.jobTypeNum << " Completed; ";
			runJob = jobQueue.peek(); // Customer at front begins processing
			log << "Begin Processing Job " << runJob.jobType << " " << runJob.jobTypeNum
				<< " CPU 1 - end time "
				<< curTime + runJob.processTime << ";" << endl;

			if (DEBUG_MODE) {
				cout << endl << "Job queue 1";
			}
			jobQueue.dequeue();
			idleTime += curTime - departTime;
			runTime += runJob.processTime;
			if (!runJob.waitTime) {
				runJob.waitTime = curTime - runJob.arriveTime;
				totalWait += runJob.waitTime;
			}
			else {
				runJob.waitTime += (curTime - Darrive);
				totalWait += (curTime - Darrive);
			}

			nextDepartJob.arriveTime = curTime + runJob.processTime;
			departTime = nextDepartJob.arriveTime;
			nextDepartJob.jobType = 'X';
			nextDepartJob.jobNum = runJob.jobNum;

			if (DEBUG_MODE) {
				cout << endl << "Event list ";
			}
			eventListPQ.enqueue(nextDepartJob);
		}
		else {
			log << curTime << setw(5) << ") Job " << runJob.jobType << " " << runJob.jobTypeNum << " Completed; PQ 1 Empty; CPU 1 Idle time;" << endl;
			processorAvailable = true;
		}

	}
	else if (departJob.jobNum == runJob2.jobNum) {
		if (!jobQueue2.isEmpty()) {
			log << curTime << setw(5) << ") Job " << runJob2.jobType << " " << runJob2.jobTypeNum << " Completed; ";
			runJob2 = jobQueue2.peek(); // Customer at front begins processing 
			log << "Begin Processing Job " << runJob2.jobType << " " <<
			runJob2.jobTypeNum << " CPU 2 - end time "
				<< curTime + runJob2.processTime << ";" << endl; 
			if (DEBUG_MODE) {
				cout << endl << "Job queue 2";
			}
			jobQueue2.dequeue();
			idleTime += curTime - departTime2; 
			runTime += runJob2.processTime;

			runJob2.waitTime = curTime - runJob2.arriveTime; 
			totalWait += runJob2.waitTime;

			nextDepartJob2.arriveTime = curTime + runJob2.processTime; 
			departTime2 = nextDepartJob2.arriveTime; 
			nextDepartJob2.jobType = 'X';
			nextDepartJob2.jobNum = runJob2.jobNum;

			if (DEBUG_MODE) {
				cout << endl << "Event list ";
			}
			eventListPQ.enqueue(nextDepartJob2);
		}
		else {
			log << curTime << setw(5) << ") Job " << runJob2.jobType << " " <<
				runJob2.jobTypeNum << " Completed; PQ 2 Empty; CPU 2 Idle time;" << endl;
			processor2Available = true;
		}
	}
	else if (departJob.jobNum == runJob3.jobNum) {
		if (!jobQueue3.isEmpty()) {
			log << curTime << setw(5) << ") Job " << runJob3.jobType << " " << runJob3.jobTypeNum << " Completed; ";
			runJob3 = jobQueue3.peek(); // Customer at front begins processing 
			log << "Begin Processing Job " << runJob3.jobType << " " <<
				runJob3.jobTypeNum << " CPU 3 - end time "
				<< curTime + runJob3.processTime << ";" << endl;
			if (DEBUG_MODE) {
				cout << endl << "Job queue 3";
			}
			jobQueue3.dequeue();
			idleTime += curTime - departTime3;
			runTime += runJob3.processTime;

			runJob3.waitTime = curTime - runJob3.arriveTime;
			totalWait += runJob3.waitTime;

			nextDepartJob3.arriveTime = curTime + runJob3.processTime;
			departTime3 = nextDepartJob3.arriveTime;
			nextDepartJob3.jobType = 'X';
			nextDepartJob3.jobNum = runJob3.jobNum;

			if (DEBUG_MODE) {
				cout << endl << "Event list ";
			}
			eventListPQ.enqueue(nextDepartJob3);
		}
		else {
			log << curTime << setw(5) << ") Job " << runJob3.jobType << " " <<
				runJob3.jobTypeNum << " Completed; PQ 3 Empty; CPU 3 Idle time;" << endl;
			processor3Available = true;
		}
	}
}
