#pragma once



template<typename T>
class Node
{
public:

	//double linked list variables
	T data;
	Node* prev = nullptr;
	Node* next = nullptr;

	//double linked list constructors
	Node()
	{
		T data;
		Node* prev = nullptr;
		Node* next = nullptr;
	}
	Node(T other)
	{
		T data = other
		Node* prev = nullptr;
		Node* next = nullptr;
	}
	Node& Node(const Node&)
	{

	}
	~Node()
	{

	}
	
};
template<typename Y>
class DLinkList: public Node
{
public:
	//double linked list functions
	void PushBack(Y value)// adds node to the end
	{
		if (Node.next = nullptr)
		{
			Node.prev->next;
			Node.next->value;
		}
	}
	void PopFront()//removes first node
	{
		if (Node.data != NULL)
		{
			Y n = Node.data;
			if (Node.next != nullptr)
			{
				Node.next->prev = nullptr;

			}
			Node.data = Node.next;
			delete n;

		}
	}
};