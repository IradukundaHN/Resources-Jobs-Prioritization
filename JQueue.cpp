// JQueue: a job queue using a max-heap and function pointers

#include "JQueue.h"

//
// JQueue.cpp implementation
//

//This is the constructor for the JQueue class.
//It must be provided with the capacity of the queueand a pointer to the prioritization function.If the capacity is less than one, throw an out_of_range exception.
//JQueue::JQueue(int capacity, int (*priFn)(const job_t& job))
JQueue::JQueue(int capacity, prifn_t priFn)
{
	m_heap = new job_t[capacity+1]; //create a heap array to hold enough size appropriated to heap binary tree, and index start at 1
	m_capacity = capacity; //capacity of the queue
	m_size = 0; //initialize the size to zero

	priority = priFn; //initialize pointer to the prioritization function

	//If the capacity is less than one
	if (capacity < 1)
		throw std::out_of_range("Invalid capacity");

}

//Destructor for the JQueue class.
//All dynamically - allocated data must be deallocated.
JQueue::JQueue::~JQueue()
{
	deallocate();
}

//Deallocate helper method
void JQueue::deallocate()
{
	if (m_capacity > 0)
		delete[] m_heap;
}

//Copy constructor for the JQueue class.
//Must make a deep copy of the rhs object.
JQueue::JQueue(const JQueue& rhs)
{
	//copy instances variables of host
	m_capacity = rhs.m_capacity;
	m_size = rhs.m_size;
	priority = rhs.priority;

	//Initialize the array
	m_heap = new job_t[rhs.m_capacity + 1];

	//deep copy of the heap array contents
	for (int i = 1; i <= m_capacity; i++)
		m_heap[i] = job_t(rhs.m_heap[i]);
}

//Assignment operator for the JQueue class
//Remember to check for self - assignment and to free all dynamically allocated data members of the host.
//No use the copy constructor in the implementation of the assignment operator.
JQueue& JQueue::operator=(const JQueue& rhs)
{
	//self assignment check
	if (this == &rhs)
		return *this;

	//free all dynamically allocated data members
	deallocate();

	//copy variable instances of the host
	m_size = rhs.m_size;
	m_capacity = rhs.m_capacity;

	//Initialize the array
	m_heap = new job_t[rhs.m_capacity + 1];

	//deep copy of the heap array contents
	for (int i = 0; i < m_capacity; i++)
	{
		m_heap[i] = job_t(rhs.m_heap[i]);
	}
	return *this;
}

//Insert a job into the job queue, must run O(log n) time
//Must maintain the max-heap property.
void JQueue::insertJob(const job_t input)
{
	//case when the size reach the heap capacity
	if(isFull() == true)
		throw std::out_of_range("Full Jobs");
	else
	{
		//increment heap size first
		m_size = m_size+1;

		//insert the job
		m_heap[m_size] = input;
		//m_heap[m_size] = -1;

		//bubble up after each insertion
		bubbleUp(m_size);
	}
} 

//getter helper method to get the parent of a node in the tree
int JQueue::getParent(int index)
{
	if (index <= 0)
		return -1;
	else
		return index / 2;
}
//getter helper method to get the left child in the tree
int JQueue::getLeft(int index)
{
	return index*2;
}
//getter helper method to get the right child in the tree
int JQueue::getRight(int index)
{
	return (index*2)+1;
}
//recursive bubble up helper method
void JQueue::bubbleUp(int index)
{
	//the index of the parent
	int parent = getParent(index);

	//nothing in the queue
	if (index == 0)
		return;
	//If index is not the root
	// maintain a max-heap based on the computed priority of each job
	if (index > 1 && priority(m_heap[parent]) < priority(m_heap[index]))
	{
		//swap a larger element with its parent 
		job_t temp = m_heap[parent];
		m_heap[parent] = m_heap[index];
		m_heap[index] = temp;

		//continue this process until the max heap property is satisfied
		bubbleUp(parent);
	}
}


//isFull helper method 
bool JQueue::isFull() const
{
	return m_size == m_capacity;
}

//isEmpty helper method 
bool JQueue::isEmpty() const
{
	return m_size == 0;
}

//Extract the highest priority job from the job queue, must run O(log N) 
//Must maintain the max - heap property.
job_t JQueue::getNextJob()
{
	//remove and return the highest element
	job_t highest = m_heap[1];
	extractMax(highest);
	return highest;

}
//extract maximum helper method
void JQueue::extractMax(job_t& input)
{
	if (isEmpty() == true)
		throw std::out_of_range("No Job, Heap underflow");
	else
	{
		//root at last element 
		m_heap[1] = m_heap[m_size];

		//decrement queue size
		m_size = m_size-1;
		          
		//recursively bubbling down
		if (m_size > 0)   
			bubbleDown(1); 
	}
}
//bubble down helper method
void JQueue::bubbleDown(int index)
{
	int left = getLeft(index); //get left child at the index
	int right = getRight(index);//get right child at the index

	int largest; //largest used to know largest between left and right
	
	/*
	if (right > m_size)
	{
		if (left> m_size)
			return; //done
		else
			largest = left; //set left to largest
	}
	else
	{
		// maintain a max-heap based on the computed priority of each job
		if (priority(m_heap[right]) < priority(m_heap[left]))
		{
			largest = right; //set right to largest
		}
		else
		{
			largest = left; //set left to largest
		}
	}
	*/
	//comparing job priority of left and right 

	if (left <= m_size && priority(m_heap[index]) < priority(m_heap[left]))
		largest = left; //set left to largest
	else
		largest = index;

	if (right <= m_size && priority(m_heap[largest]) < priority(m_heap[right]))
	{
		largest = right; //set right to largest
	}
	// maintain a max-heap based on the computed priority of each job
	if (largest != index )
		//&& priority(m_heap[largest]) < priority(m_heap[index]))
	{
		//swap with index having greater priority
		job_t temp = m_heap[largest];
		m_heap[largest] = m_heap[index];
		m_heap[index] = temp;

		//bubble down recursively
		bubbleDown(largest);
	}
}
//Return the current number of jobs in the queue.
int JQueue::numJobs() const
{
	return m_size;
}
//Return the size of jobs in the queue.
int JQueue::getSize() const
{
	return m_size;
}
//Print the contents of the job queue.
//The jobs should be listed be in array order not priority order(although the first job listed should have highest priority).
void JQueue::printJobQueue() const
{
	
	std::cout << "Job Queue Size :";
	std::cout << getSize();
	std::cout << std::endl;
	std::cout << "Number of jobs in the queue: ";
	std::cout << numJobs();
	std::cout << std::endl;

	for (int i = 1; i <= getSize(); i++)
	{
		std::cout << "[" << i << "]";
		std::cout << m_heap[i];
		std::cout << " (" << priority(m_heap[i]) << ")";
	
		std::cout << std::endl;
	}
}

//Get the current priority function.
prifn_t JQueue::getPriorityFn()
{
	return priority;
}

//Set a new priority function
//Rebuild the heap, using max heapifying, rebuild runs in linear time 
void JQueue::setPriorityFn(prifn_t priFn)
{
	priority = priFn;
	//using max heapifying in a bottom up manner
	for (int i = m_size/2; i >=1 ; --i)
	{
		//bubbleDown(priority(m_heap[i])); 
		bubbleDown(i);
	}
	/*
	//Or by inserting again into the heap 
	//m_size = 0;
	priority = priFn;
	for (int i = m_size; i >= 1; --i)
	{
		insertJob(m_heap[i]);
	}
	*/
}

/*
//Dump the entire heap, not just the portion currently in use.For debugging purposes.
void JQueue::dump() const
{

}
*/
// Overloaded insertion operator for job_t
std::ostream& operator<<(std::ostream& sout, const job_t& job)
{
	sout << "Job: " << job.m_name << ", pri: " << job.m_priority
		<< ", u: " << job.m_user << ", g: " << job.m_group << ", proc: "
		<< job.m_proc << ", mem: " << job.m_mem << ", time: " << job.m_time;
	return sout;
}
