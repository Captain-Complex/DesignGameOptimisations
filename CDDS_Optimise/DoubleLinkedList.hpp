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
	~DLinkList()
	{
		int removeList = listSize;
		for (int i = 0; i < removeList; i++)
		{
			DLinkList::Erase(head);
		}
	}
	DLinkList(const DLinkList& other)
	{
		Node<T>* temp = other.head;

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

		if (tail == nullptr /*&& head == nullptr*/)
		{
			head = newNode;
			tail = newNode;
			/*head->next = tail;
			tail->prev = head;*/
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
		//taking advantage of Erase function for popFront
		Node<T>* front = head;
		Node<T>* tempFront = front;
		front = front->next;
		Erase(tempFront);
	}
	void Erase(Node<T>* value)
	{
		if ( value->prev != nullptr && value->next != nullptr )
		{
			Node<T>* temp = value;
			value->prev->next = value->next;
			value->next->prev = value->prev;
			delete temp;
			--listSize;
			return;
		}
		if (value->prev != nullptr && value->next == nullptr)
		{
			Node<T>* temp = value;
			value->prev->next = value->next;
			tail = value->prev;
			delete temp;
			--listSize;
			return;
		}
		if (value->prev == nullptr && value->next != nullptr)
		{
			Node<T>* temp = value;
			value->next->prev = value->prev;
			head = value->next;
			delete temp;
			--listSize;
			return;
		}
		if (value->prev == nullptr && value->next == nullptr)
		{
			head = value->prev;
			tail = value->next;
			delete value;
			--listSize;
			return;
		}
	}
	
};