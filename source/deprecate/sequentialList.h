#pragma once

#include<iostream>
#include<functional>
#include<cassert>
#include<cstring>
#include<cstdlib>
#include<unordered_map>
#include<vector>

using ElemType = int;

enum class Status : int
{
	ERROR = 0,
	OK = 1,
	TRUE = 1,
	FALSE = 0,
	OVER = -2
};

using Compare = std::function<Status(ElemType, ElemType)>;
//typedef Status (*Compare)(ElemType, ElemType);


const uint32_t LIST_INIT_SIZE = 100;
const uint32_t LISTINCREMENT = 10;

class SeqList
{
private:
	ElemType* firstElement;     //the first location of storage space
	int       length;      //current length of sequential list
	int       listSize;    //the storage space of allocation

public:
	//initialize
	Status InitList()
	{
		firstElement = (ElemType*)malloc(LIST_INIT_SIZE * sizeof(ElemType));
		if (!firstElement)
			exit(static_cast<int>(Status::OVER));
		length = 0;
		listSize = LIST_INIT_SIZE;
		return Status::OK;
	}

	//insert an element before the location
	Status ListInsert(int location, ElemType element)
	{
		if (location < 0 || location > length)
			return Status::ERROR;
		if (length >= listSize)
		{
			auto newBase = (ElemType*)realloc(firstElement, (listSize + LISTINCREMENT) * sizeof(ElemType));
			if (!newBase)
				exit(static_cast<int>(Status::ERROR));
			firstElement = newBase;
			listSize += LISTINCREMENT;
		}
		ElemType* locationElement = firstElement + location;
		//form back to front
		ElemType* changedElement = firstElement + length;
		while (changedElement > locationElement)
		{
			*changedElement = *(changedElement - 1);
			changedElement--;
		}
		*locationElement = element;
		length++;

		return Status::OK;
	}

	//delete the element in the location
	Status ListDelete(int location)
	{
		if (location < 0 || location > length - 1)
			return Status::ERROR;
		ElemType* locationElement = firstElement + location;
		ElemType* lastElement = firstElement + length - 1;
		while (locationElement < lastElement)
		{
			*locationElement = *(locationElement + 1);
			locationElement++;
		}
		length--;
		return Status::OK;
	}

	//pop_back
	Status PopBack()
	{
		if (Empty())
			return Status::ERROR;
		length--;
		return Status::OK;
	}

	//push_back
	Status PushBack(ElemType element)
	{
		if (length >= listSize)
		{
			auto newBase = (ElemType*)realloc(firstElement, (listSize + LISTINCREMENT) * sizeof(ElemType));
			if (!newBase)
				exit(static_cast<int>(Status::ERROR));
			firstElement = newBase;
			listSize += LISTINCREMENT;
		}

		*(firstElement + length) = element;
		length++;
		return Status::OK;
	}

	//find the first element which satisfy the compare function
	//compare will be like compare(element, value)
	//return the index if found while -1 if not
	int LookUp(ElemType element, const Compare& compare)
	{
		for (int i = 0; i < length; i++)
		{
			if (compare(element, *(firstElement + i)) == Status::TRUE)
				return i;
			else
				i++;
		}
		return -1;
	}

	ElemType& operator[](int i)
	{
		assert(i >= 0 && i <= length - 1);
		return *(firstElement + i);
	}

	const ElemType& operator[](int i) const
	{
		assert(i >= 0 && i <= length - 1);
		return *(firstElement + i);
	}

	int Capacity() const
	{
		return listSize;
	}

	int Length() const
	{
		return length;
	}

	bool Empty() const
	{
		return length == 0;
	}

	ElemType* begin() const
	{
		return firstElement;
	}

	ElemType* end() const
	{
		return firstElement + length;
	}
};



//1. ElemType = int
Status deleteMin(SeqList& list, ElemType deleteElement)
{
	if (list.Empty())
		return Status::ERROR;
	int index = 0;
	ElemType variable = INT_MAX;
	for (int i = 0; i < list.Length(); i++)
	{
		if (list[i] < variable)
		{
			variable = list[i];
			index = i;
		}
	}
	list[index] = list[list.Length() - 1];
	deleteElement = variable;
	return Status::OK;
}

//2. ElemType = int
void reverse(SeqList& list, int left, int right)
{
	if (list.Empty() || left > list.Length() - 1 || left >= right || right < 0)
		return;
	while (left < right)
	{
		ElemType tmp = list[right];
		list[right] = list[left];
		list[left] = tmp;
		left++;
		right--;
	}
}

//3. ElemType = int
void deleteSame(SeqList& list, ElemType element)
{
	int fast = 0;
	int slow = 0;
	int length = list.Length();
	while (fast <= length)
	{
		if (list[fast] != element)
		{
			if (slow != fast)
				list[slow] = list[fast];
			slow++;
		}
		fast++;
	}
	int nums = fast - slow;
	while (nums--)
		list.PopBack();
}

//4. ElemType = int
Status deleteOrderedZone(SeqList& list, ElemType lower, ElemType upper)
{
	int left = 0;
	int right = list.Length() - 1;
	if (lower > list[right] || upper < list[left] || list.Empty() || lower > upper)
		return Status::ERROR;
	//binary search the lower and upper
	while (left <= right)
	{
		int middle = (left + right) >> 1;
		if (lower <= list[middle])
			right = middle - 1;
		else
			left = middle + 1;
	}
	int lowIndex = right + 1;

	left = right + 2;
	right = list.Length() - 1;
	while (left <= right)
	{
		int middle = (left + right) >> 1;
		if (upper >= list[middle])
			left = middle + 1;
		else
			right = middle - 1;
	}
	int upIndex = left - 1;

	int length = list.Length();
	int fast = upIndex + 1;
	int slow = lowIndex;
	while (fast <= length)
	{
		list[slow] = list[fast];
		fast++;
		slow++;
	}

	int nums = upIndex - lowIndex + 1;
	while (nums--)
		list.PopBack();
	return Status::OK;
}

//5. ElemType = int
Status deleteZone(SeqList& list, ElemType lower, ElemType upper)
{
	int left = 0;
	int right = list.Length() - 1;
	if (lower > list[right] || upper < list[left] || list.Empty() || lower > upper)
		return Status::ERROR;

	int fast = 0;
	int slow = 0;
	while (fast <= right)
	{
		if (!(list[fast] >= lower && list[fast] <= upper))
		{
			if (fast != slow)
				list[slow] = list[fast];
			slow++;
		}
		fast++;
	}

	int nums = fast - slow;
	while (nums--)
		list.PopBack();
	return Status::OK;
}

//6. ElemType = int
void deleteRepetitive(SeqList& list)
{
	int right = list.Length() - 1;
	int fast = 0;
	int slow = 0;
	while (fast <= right)
	{
		if (list[fast] != list[slow])
		{
			slow++;
			if (slow != fast)
				list[slow] = list[fast];
		}
		fast++;
	}

	int nums = fast - slow - 1;
	while (nums--)
		list.PopBack();
}

//7. ElemType = int
SeqList mergeOrderedList(SeqList& list1, SeqList& list2)
{
	SeqList out{};
	out.InitList();

	int head1 = 0;
	int head2 = 0;
	while (head1 <= list1.Length() - 1 && head2 <= list2.Length() - 1)
	{
		if (list1[head1] < list2[head2])
		{
			out.PushBack(list1[head1]);
			head1++;
		}
		else
		{
			out.PushBack(list2[head2]);
			head2++;
		}
	}

	while (head1 <= list1.Length() - 1)
	{
		out.PushBack(list1[head1]);
		head1++;
	}

	while (head2 <= list2.Length() - 1)
	{
		out.PushBack(list2[head2]);
		head2++;
	}

	return out;
}

//8. ElemType = int
void exchangePosition(SeqList& list, int listM, int listN)
{
	reverse(list, 0, list.Length() - 1);
	reverse(list, 0, listM - 1);
	reverse(list, listM, list.Length() - 1);
}

//9. ElemType = int
void searchElement(SeqList& list, ElemType element)
{
	int left = 0;
	int right = list.Length() - 1;
	while (left <= right)
	{
		int middle = (left + right) >> 1;
		if (element < list[middle])
			right = middle - 1;
		else if (element > list[middle])
			left = middle + 1;
		else
		{
			ElemType tmp = list[middle];
			list[middle] = list[middle + 1];
			list[middle + 1] = tmp;
			return;
		}
	}

	list.ListInsert(left, element);
}

//10. ElemType = int
void rotate(SeqList& list, int k)
{
	//time O(n), space O(1)
	reverse(list, 0, k - 1);
	reverse(list, k, list.Length() - 1);
	reverse(list, 0, list.Length() - 1);
}

//11. ElemType = int
ElemType searchMidNumber(SeqList& list1, SeqList& list2)
{
	//precise position
	int midPosition = ((list1.Length() + list2.Length()) >> 1) + 1;
	//index
	int list1Left = 0;
	int list2Left = 0;
	while (midPosition)
	{
		//get the k min number
		int middle = std::max((midPosition >> 1) - 1, 0);
		int list1Mid = list1Left + middle;
		int list2Mid = list2Left + middle;
		if (list1[list1Mid] >= list2[list2Mid])
			list2Left = list2Mid + 1;
		else
			list1Left = list1Mid + 1;
		midPosition -= middle + 1;
	}
	return list1[list1Left];
}

//12. ElemType = int
int getMainElement(SeqList& list)
{
	std::unordered_map<ElemType, int> hash;
	for (auto& i : list)
	{
		if (hash.count(i))
			hash[i]++;
		else
		{
			if (hash.empty())
				hash.insert(std::make_pair(i, 1));
			else
				hash.begin()->second--;
			if (hash.begin()->second == 0)
				hash.erase(hash.begin()->first);
		}
	}

	if (!hash.empty())
	{
		int count = 0;
		ElemType m = hash.begin()->first;
		for (auto& i : list)
		{
			if (i == m)
				count++;
		}
		if (count > list.Length() >> 1)
			return m;
	}
	return -1;
}

//13. ElemType = int
ElemType getMinPositive(SeqList& list)
{
	std::vector<ElemType> storage(list.Length());
	for (auto& i : list)
	{
		if (i <= list.Length() && i > 0)
			storage[i - 1]++;
	}

	for (int i = 0; i < list.Length(); i++)
	{
		if (storage[i] == 0)
			return i + 1;
	}
	return list.Length() + 1;
}

//14. ElemType = int

