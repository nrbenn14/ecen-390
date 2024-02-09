#include <stdio.h>  // printf
#include <stdlib.h> // malloc, free, abort
#include <string.h> // strncpy
 
#include "queue.h"

 
// Allocates memory for the queue (the data* pointer) and initializes all
// parts of the data structure. Prints out an error message if malloc() fails
// and calls assert(false) to print-out line-number information and die.
// The queue is empty after initialization. To fill the queue with known
// values (e.g. zeros), call queue_overwritePush() up to queue_size() times.
void queue_init(queue_t *q, queue_size_t size, const char *name) {
	// Always points to the next open slot.
	q->indexIn = 0;
	// Always points to the next element to be removed
	// from the queue (or "oldest" element).
	q->indexOut = 0;
	// Keep track of the number of elements currently in queue.
	q->elementCount = 0;
	// Queue capacity.
	q->size = size;
	// Points to a dynamically-allocated array.
	q->data = malloc(size * sizeof(queue_data_t));
	if (q->data == NULL) abort();
	// True if queue_pop() is called on an empty queue. Reset
	// to false after queue_push() is called.
	q->underflowFlag = false;
	// True if queue_push() is called on a full queue. Reset to
	// false once queue_pop() is called.
	q->overflowFlag = false;
	// Name for debugging purposes.
	strncpy(q->name, name, QUEUE_MAX_NAME_SIZE);
	q->name[QUEUE_MAX_NAME_SIZE-1] = '\0';
}
 
// Get the user-assigned name for the queue.
const char *queue_name(queue_t *q) {
	return q->name;
}
 
// Returns the capacity of the queue.
queue_size_t queue_size(queue_t *q) {
	return q->size;
}
 
// Returns true if the queue is full.
bool queue_full(queue_t *q) {
    if (q->elementCount == q->size) {
        return true;
    }

    else {
        return false;
    }
}
 
// Returns true if the queue is empty.
bool queue_empty(queue_t *q) {
    if (q->elementCount == 0) {
        return true;
    }

    else {
        return false;
    }
}
 
// If the queue is not full, pushes a new element into the queue and clears the
// underflowFlag. IF the queue is full, set the overflowFlag, print an error
// message and DO NOT change the queue.
void queue_push(queue_t *q, queue_data_t value) {

    // Make sure the queue isn't full, then move our tail pointer, 
    // push the new value, and then increase the element count.
    if (queue_full(q) == false) {
        q->data[q->indexIn] = value;
        q->indexIn++;
        q->elementCount++;
        q->underflowFlag = false;
    }

    // Otherwise print an error and mark overflow
    else {
        q->overflowFlag = true;
        printf("Error - queue is full.");
    }
}
 
// If the queue is not empty, remove and return the oldest element in the queue.
// If the queue is empty, set the underflowFlag, print an error message, and DO
// NOT change the queue.
queue_data_t queue_pop(queue_t *q) {
    
    // Make sure that the queue isn't empty (there is something to pop),
    // then 
    if (queue_empty(q) ==false) {
        queue_data_t element = q->data[q->indexOut];
        q->indexOut++;
        q->elementCount--;
        q->overflowFlag = false;
        return element;
    }

    else {
        q->underflowFlag = true;
        printf("Error - queue is empty.");
    }
}
 
// If the queue is full, call queue_pop() and then call queue_push().
// If the queue is not full, just call queue_push().
void queue_overwritePush(queue_t *q, queue_data_t value) {
    if (queue_full(q) == true) {
        queue_pop(q);
        queue_push(q, value);
    }

    else {
        queue_push(q, value);
    }
}
 
// Provides random-access read capability to the queue.
// Low-valued indexes access older queue elements while higher-value indexes
// access newer elements (according to the order that they were added). Print a
// meaningful error message if an error condition is detected.
queue_data_t queue_readElementAt(queue_t *q, queue_index_t index) {
    if ((q->indexOut + index) <= (q->indexIn)) {
        return q->data[q->indexOut + index];
    }

    else {
        printf("Index doesn't exist.");
    }
}
 
// Returns a count of the elements currently contained in the queue.
queue_size_t queue_elementCount(queue_t *q) {
	return q->elementCount;
}
 
// Returns true if an underflow has occurred (queue_pop() called on an empty
// queue).
bool queue_underflow(queue_t *q) {
	return q->underflowFlag;
}
 
// Returns true if an overflow has occurred (queue_push() called on a full
// queue).
bool queue_overflow(queue_t *q) {
	return q->overflowFlag;
}
 
// Frees the storage that you malloc'd before.
void queue_garbageCollect(queue_t *q) {
	free(q->data);
}