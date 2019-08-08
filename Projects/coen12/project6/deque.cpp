#include <iostream.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "deque.h"

class Node;

//O(1)
Deque::Deque(){

	head = new Node();	
	count = 0;
	head->setNext(head);
	head->setPrev(head);
}
//O(n)
Deque::~Deque(){
	
	class Node *cur = head;

	for(int i = 0; i < count; i++){
		cur = cur->getNext();
		delete cur;
	}
}

//O(1)
int Deque::size(){

	return count;
}

//O(1)
void Deque::addFirst(int x){	
		
	class Node *add = new Node();
	add->setData(x);
	add->setNext(head->getNext());
	add->setPrev(head);
	(head->getNext())->setPrev(add);
	head->setNext(add);
	count++;
}

//O(1)
void Deque::addLast(int x){
	
	class Node *add = new Node();
	add->setData(x);
	add->setNext(head);
	add->setPrev(head->getPrev());
	head->getPrev()->setNext(add);
	head->setPrev(add);
	count++;
}

//O(1)
int Deque::removeFirst(){
		
	assert(count !=0);
	int x = head->getNext()->getData();
	head->getNext()->getNext()->setPrev(head);
	class Node *del = head->getNext();
	head->setNext(del->getNext());
	delete del;
	count--;
	return x;
}

//O(1)
int Deque::removeLast(){

	assert(count !=0);
	int x = head->getPrev()->getData();
	head->getPrev()->getPrev()->setNext(head);
	class Node *del = head->getPrev();
	head->setPrev(del->getPrev());
	delete del;
	count--;
	return x;
}

//O(1)
int Deque::getFirst(){

	assert(count !=0);
	return head->getNext()->getData();
}

//O(1)
int Deque::getLast(){
	
	assert(count != 0);
	return head->getPrev()->getData();
}


/* Node */


Node::Node(){
}

Node::~Node(){

}

//O(1)
class Node *Node::getNext(){
	
	return next;
}

//O(1)	
class Node *Node::getPrev(){
	
	return prev;
}

//O(1)
void Node::setNext(class Node *node){
	next = new Node();
	next = node;
}

//O(1)
void Node::setPrev(class Node *node){
	prev = new Node();
	prev = node;
}

//O(1)
int Node::getData(){
	return data;
}

//O(1)
void Node::setData(int x){
	data = x;
}
