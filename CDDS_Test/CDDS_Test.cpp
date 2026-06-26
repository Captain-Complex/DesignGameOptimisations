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
			Assert::AreEqual(number.listSize, 1);
		}
		TEST_METHOD(PushBackList)
		{
			DLinkList<int> numbers{};
			numbers.PushBack(6);
			numbers.PushBack(3);
			numbers.PushBack(7);
			Assert::AreEqual(numbers.listSize, 3);
		}
		TEST_METHOD(EraseEmptyList)
		{
			DLinkList<int> num;
			num.PushBack(4);
			num.Erase(num.head);
			Assert::AreEqual(num.listSize, 0);
		}
		TEST_METHOD(EraseOnlyHeadList)
		{
			DLinkList<int> num;
			num.PushBack(4);
			num.PushBack(6);
			num.Erase(num.head);
			Assert::AreEqual(num.head->data, 6);
		}
		TEST_METHOD(EraseOnlyTailList)
		{
			DLinkList<int> num;
			num.PushBack(4);
			num.PushBack(6);
			num.Erase(num.tail);
			Assert::AreEqual(num.tail->data, 4);
		}
		TEST_METHOD(EraseList)
		{
			DLinkList<int> num;
			num.PushBack(4);
			num.PushBack(6);
			num.PushBack(3);
			num.PushBack(5);
			num.PushBack(1);
			num.Erase(num.head->next);
			Assert::AreEqual(num.head->next->data, 3);
		}
	};
}
