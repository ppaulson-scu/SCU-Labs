/*
 * File:	deque.h
 *
 * Copyright:	2015, Darren C. Atkinson
 *
 * Description:	This file contains the public function declarations for a
 *		deque abstract data type for integers.  A deque is a
 *		doubly-ended queue, in which items can only be added to or
 *		removed from the front or rear of the list.
 */

# ifndef DEQUE_H
# define DEQUE_H

class Deque {
    int count;
    class Node *head;

public:
    Deque();
    ~Deque();
    int size();
    void addFirst(int x);
    void addLast(int x);
    int removeFirst();
    int removeLast();
    int getFirst();
    int getLast();
};


class Node{
	int data;
	class Node *next;
	class Node *prev;

public:
	Node();
	~Node();
	class Node *getNext();
	class Node *getPrev();
	void setNext(class Node *node);
	void setPrev(class Node *node);
	int getData();
	void setData(int x);
};

# endif /* DEQUE_H */
