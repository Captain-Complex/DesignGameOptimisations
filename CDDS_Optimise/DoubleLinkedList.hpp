#pragma once

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
		T data;
		prev = nullptr;
		next = nullptr;
	}
	Node(T value)
	{
		data(value);
		prev = nullptr;
		next = nullptr;
	}
	//node functions

};
template<typename T>
struct DLinkList: public Node<T>
{
public:
	//double linked list variables
	DLinkList* head = nullptr;
	DLinkList* tail = nullptr;
	int listSize[0];

	//double linked list constructors
	DLinkList()
	{
		head = nullptr;
		tail = nullptr;
		listSize[0];
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
		Node* newNode = new Node(value);

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
	}
	void PopFront() // remove first node 
	{
		if (DLinkList::head != nullptr)
		{
			DLinkList* N = DLinkList::head;

			if (DLinkList::head::next != nullptr)
			{
				DLinkList::head::next::prev = nullptr;
			}

            DLinkList::head = DLinkList::head::next;
		    delete N;
		}
	}
};