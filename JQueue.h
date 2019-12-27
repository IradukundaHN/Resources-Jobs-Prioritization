// JQueue: a job queue using a max-heap and function pointers

#ifndef _JQUEUE_H
#define _JQUEUE_H

#include <stdexcept>
#include <iostream>
#include <string>

// Job scheduling parameters, min and max values

const int MINPRI = 0;       // Min and max priority
const int MAXPRI = 100;

const int MINU = 0;         // Min and max user id
const int MAXU = 100;

const int MING = 0;         // Min and max group id
const int MAXG = 100;

const int MINPROC = 1;      // Min and max number of procs
const int MAXPROC = 255;

const int MINMEM = 1;       // Min and max mem
const int MAXMEM = 65535;   // Multiple of 1 Mb

const int MINTIME = 1;      // Min and max wall clock time in seconds
const int MAXTIME = 172800; // 48 hours

//
// job_struct: Simple job struct with default and non-default
// constructors
//
struct job_struct {

	// Default job constructor
	job_struct() {
		m_name = ""; m_priority = 0; m_user = 0; m_group = 0;
		m_proc = 0; m_mem = 0; m_time = 0;
	}

	// Non-default job constructor
	job_struct(std::string name, int pri, int u, int g, int proc, int mem, int time) {
		if ((pri < MINPRI || pri > MAXPRI) || (u < MINU || u > MAXU) || (g < MING || g > MAXG) ||
			(proc < MINPROC || proc > MAXPROC) || (mem < MINMEM || mem > MAXMEM) ||
			(time < MINTIME || time > MAXTIME))
			throw std::out_of_range("Bad value to job constructor.");

		m_name = name; m_priority = pri; m_user = u; m_group = g;
		m_proc = proc; m_mem = mem; m_time = time;
	}

	std::string m_name;  // job name
	int m_priority;      // job priority, user selected
	int m_user;          // user id
	int m_group;         // group id
	int m_proc;          // number of processors
	int m_mem;           // memory per proc, multiples of 1Mb
	int m_time;          // wall time, in sec
};

typedef struct job_struct job_t;         // job type
typedef int (*prifn_t)(const job_t&);    // priority function pointer type

// Overloaded insertion operator for job_t
std::ostream & operator<<(std::ostream & sout, const job_t & job);

class Grader;  // forward declaration of Grader (for grading purposes)

//
// JQueue: a max-priority queue implemenation of a job scheduler queue
//
class JQueue {
public:

	friend Grader; // for grading purposes

	// Constructor.  Create a job queue of specified size with given
	// priority function
	JQueue(int capacity, prifn_t priFn);

	// Destructor
	~JQueue();

	// Copy constructor
	JQueue(const JQueue& rhs);

	// Assignment Operator
	JQueue& operator=(const JQueue& rhs);

	// Add a job to the queue
	void insertJob(const job_t input);

	
	// Extract the highest priority job
	job_t getNextJob();

	// Return number of jobs in queue
	int numJobs() const;

	// Print the queue.  This will be in array order *not* priority
	// order, although the first job should have highest priority
	// overall.
	void printJobQueue() const;

	// Get the current priority function
	prifn_t getPriorityFn();

	// Set a new priority function
	// Must rebuild the heap!!!
	void setPriorityFn(prifn_t priFn);

	// Dump the heap.  For debugging purposes.
	//void dump() const;

	

private:

	int m_capacity;     // capacity of the heap
	int m_size;         // current size of the heap
	job_t* m_heap;      // heap of jobs

	// Function pointer; function to compute job priority
	prifn_t priority;

	//
	// Declarations of private helper functions go here!!!!
	//

	//HELPER METHODS
	void deallocate();
	bool isFull() const;
	bool isEmpty() const;
	void bubbleUp(int i);
	void bubbleDown(int i);
	void extractMax(job_t& i);
	int getSize() const;
	int getParent(int i);
	int getLeft(int i);
	int getRight(int i);
};

#endif
