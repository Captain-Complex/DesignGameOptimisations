#include "CppUnitTest.h"
#include "../CDDS_Optimise/DoubleLinkedList.hpp"



using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CDDSTest
{

	TEST_CLASS(CDDSTest)
	{
	public:
		
		TEST_METHOD(PushBackEmptyList)
		{
			DLinkList<int> number;
			number.PushBack(5);
			Assert::AreEqual(1, number.listSize);
			Assert::AreEqual(5, number.head->data);
		}
		TEST_METHOD(PushBackList)
		{
			DLinkList<int> numbers{};
			numbers.PushBack(6);
			numbers.PushBack(3);
			Assert::AreEqual(2, numbers.listSize );
			Assert::AreEqual(6, numbers.head->data);
			Assert::AreEqual(3, numbers.tail->data);
		}
		TEST_METHOD(EraseListEmpty)
		{
			DLinkList<int> num;
			num.PushBack(4);
			num.Erase(num.head);
			Assert::AreEqual(0, num.listSize);
			
		}
		TEST_METHOD(EraseOnlyHeadList)
		{
			DLinkList<int> num;
			num.PushBack(4);
			num.PushBack(6);
			num.Erase(num.head);
			Assert::AreEqual(1, num.listSize);
			Assert::AreEqual(6, num.head->data);
		}
		TEST_METHOD(EraseOnlyTailList)
		{
			DLinkList<int> num;
			num.PushBack(4);
			num.PushBack(6);
			num.Erase(num.tail);
			Assert::AreEqual(1, num.listSize);
			Assert::AreEqual(4, num.tail->data );
		}
		TEST_METHOD(EraseList)
		{
			DLinkList<int> num;
			num.PushBack(4);
			num.PushBack(6);
			num.PushBack(3);
			num.Erase(num.head->next);
			Assert::AreEqual(2, num.listSize);
			Assert::AreEqual(4, num.head->data);
			Assert::AreEqual(3, num.head->next->data);
			Assert::AreEqual(3, num.tail->data );
		}
	};
}
