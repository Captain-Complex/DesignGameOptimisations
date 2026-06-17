#pragma once
#include <algorithm>
template<typename T>
struct Node
{
public:

	//node variables
	T data;
	Node* prev = nullptr;
	Node* next = nullptr;

	//node constructors
	Node()
	{
		prev = nullptr;
		next = nullptr;
	}
	Node(T value):data(value)
	{
		prev = nullptr;
		next = nullptr;
	}
};
template<typename T>
struct DLinkList
{
public:
	//double linked list variables
	Node<T>* head = nullptr;
	Node<T>* tail = nullptr;
	int listSize = 0;

	//double linked list constructors
	DLinkList()
	{
		head = nullptr;
		tail = nullptr;
	}
	DLinkList(const DLinkList& other)
	{
		Node* temp = other.head;

		while (temp != nullptr)
		{
			this->PushBack(temp->data);
			temp = temp->next;
		}
	}
	//double linked list functions
	void PushBack(T value) // add node to the end
	{
		Node<T>* newNode = new Node<T>(value);

		if (tail == nullptr)
		{
			head = newNode;
			tail = newNode;
		}
		else
		{
			tail->next = newNode;
			newNode->prev = tail;
			tail = newNode;
		}
		++listSize;
	}
	void PopFront() // remove first node 
	{
		if (head != nullptr)
		{
			Node<T>* N = head;

			if (head->next != nullptr)
			{
				head->next->prev = nullptr;
			}

            head = head->next;
		    delete N;
		}
		--listSize;
	}

	
};