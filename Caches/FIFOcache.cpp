#include <iostream>

using namespace std;

// Creating the structure for linkedlist
struct node {
	int key;
	int val;
	node* next;
	node* prev;
};

// Creating three lists for having head, a temporarily list and a list for tail
node* head;
node* tail;
node* temp;

int status;

// Function to add new node in the list
int newNode(int key)
{
	// if head is NULL create the new node and assigning to head
	if (head == NULL) {
		head = (node*) malloc(sizeof(node));
						// allocate memory of node size to a node pointer head
		if (head == NULL) {
			cout <<"Unable to allocate space\n";
			return -2;
		}

		head->key = key;
		head->val = key;
		tail = head;
		head->prev = NULL;
	}
	else {

		temp = tail;
		tail->next = (node*) malloc(sizeof(node));
						// allocate memory of node size to a node pointer tail->next
		if (tail->next == NULL) {
			cout <<"Unable to allocate space\n";
			return -2;
		}

		tail->next->key = key;
		tail->next->val = key;
		tail = tail->next;
		tail->prev = temp;
	}
	tail->next = NULL;

	return 0;
}

// Function to print the linked list
int Display()
{
	if (head == NULL) {
		cout <<"Add a node first\n";
		return -2;
	}
	else {
		temp = head;
		while (temp != NULL) {
			cout <<"{"<< temp->key << " : " << temp->val <<"} ";
			temp = temp->next;
		}
		cout <<"\n";
	}
	return 0;
}

// Function to search the elements is already present in the list or not
int SearchCache(int key)
{
	if (head == NULL) {
		cout <<"No nodes in cache\n";
		return -1;
	}

	// Store head temporarily.
	temp = head;

	// Traverse Double Linked List.
	while (temp != NULL) {
		// If value in list matches with given value.
		if (temp->key == key) {
			return 0;
		}

		// Keep iterating the loop.
		temp = temp->next;
	}

	// For new elements.
	temp = tail->prev;

	// Shift all value to the right and over-write the last value.
	while (temp != NULL) {
		temp->next->key = temp->key;
		temp->next->val = temp->val;
		temp = temp->prev;
	}

	// Place new value at head.
	head->key = key;
	head->val = key;
	return 0;
}

// Initializing function that will create the list with values 0 in it.
int ListOfNodes(int number)
{
	static int i = 0;
	for (i = 0; i < number; i += 1) {
		status = newNode(0);

		// if status is 0 then it will return
		if (status < 0) {
			cout <<"Could not assign node\n";
			return status;
		}
	}
	return 0;
}

// Function to perform FIFO operations
void FIFOrefer(int arr[], int n)
{

	// Iterating through the elements so that FIFO operation can take place
	for (int i = 0; i < n; ++i) {

		SearchCache(arr[i]);

		// If the status is negative then return
		if (status < 0)
			exit(1);

		// Printing it every time
		status = Display();
	}
}

// Driver Code
int main(void)
{
	// Pre defining the size of the cache
	int cachSize = 3;
	status = ListOfNodes(cachSize);

	// The Numbers to be added in FIFO List.
	//int arr[] = { 1, 2, 3, 4, 5, 2, 10, 7, 11, 1 };
	int arr[] = { 2, 3, 2, 1, 5, 2, 4, 5, 3, 2, 5, 2 };
	
	// Number of elements to be added in FIFO List.
	int n = sizeof(arr) / sizeof(int);

	FIFOrefer(arr, n);

	return 0;
}
