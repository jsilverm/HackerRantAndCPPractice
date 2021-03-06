// SingleLinkedListWithUniquePtrs.cpp : Trivial single-linked list implementation with unique ptr's
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <memory>
#include <unordered_set>
#include <stack>
using std::unique_ptr;	using std::string;
using std::cout;	using std::move;	using std::make_unique;
using std::unordered_set;	using std::stack;
class LinkedList;
class Node {
	string data;
	unique_ptr<Node>  next = nullptr;
public:
	Node(const string &rData) : data(rData) { cout << "Node Copy constructor!\n"; }
	Node(string &&rData) : data(rData) { cout << "Node Move constructor!\n"; }
	~Node() { cout << "Node destructor!\n"; }
	string getData() { return data; }
	friend LinkedList;
};

class LinkedList {
	unique_ptr<Node> head = nullptr;
public:
	LinkedList() { cout << "Default LinkedList constructor!\n"; }
	LinkedList(LinkedList &&rhs) : head( move(rhs.head) )
			{ cout << "LinkedList Move constructor!\n"; }
	~LinkedList() { cout << "LinkedList destructor!\n"; }

	Node *getTail() {
		Node *pNode = head.get();
		if (nullptr == pNode) return nullptr;
		else while (pNode->next != nullptr) pNode = pNode->next.get();
		return pNode;
	}

	Node *getHead() {
		Node *pNode = head.get();
		return pNode;
	}

	bool deleteNode(const string &ddata) {
		Node *pNode = head.get();
		if (nullptr == pNode) return false;
		else if (pNode->data == ddata) {
			head = move(pNode->next);
			return true;
		}
		else while (pNode->next != nullptr && pNode->next->data != ddata)
			pNode = pNode->next.get();
		 if (pNode->next == nullptr) return false;
		 else pNode->next = move(pNode->next->next);
		 return true;
	}

	Node *findNode(const string &data) {
		Node *pNode = head.get();
		while (pNode != nullptr && pNode->data != data)
			pNode = pNode->next.get();
		return pNode;
	}

	void addNode(const string &data) {
		Node *pTail = getTail();
		if (pTail == nullptr) head = make_unique<Node>(data);
		else pTail->next = make_unique<Node>(data);
	}

	void printNodes() {
		Node *pNode = head.get();
		while (pNode != nullptr) {
			cout << pNode->data << "\n";
			pNode = pNode->next.get();
		}
	}

	void removeDups() {
		// this will go thru an unsorted linked list removing all duplicates
		Node *pNode = head.get();
		if (nullptr == pNode || nullptr == pNode->next) return; // it takes 2 to dup!
		unordered_set<string> values;
		auto prev = pNode;
		values.insert(pNode->data);
		pNode = pNode->next.get();
		while (pNode != nullptr) {
			if (values.count(pNode->data) != 0) {
				// delete this node
				auto temp = pNode->next.get();
				prev->next = move(pNode->next);
				pNode = temp;
			}
			else {
				values.insert(pNode->data);
				prev = pNode;
				pNode = pNode->next.get();
			}

		}
	}

	Node *findKelementsFromTail(int k) {
		Node *pNode = head.get();
		if (nullptr == pNode) return nullptr;
		auto lead = pNode;
		auto countDown = k;
		while (lead != nullptr && countDown-- != 0)	lead = lead->next.get();
		if (countDown > 0) return nullptr;
		while (nullptr != lead->next.get() ) {
			lead = lead->next.get();
			pNode = pNode->next.get();
		}
		return pNode;
	}

	void deleteThisNode(Node *pNode) {
		// it seems impossible as this is a singly-linked list and we have neither
		// a pointer to the head nor to previous node
		// what we do instead is delete the next node and impersonate them!
		if (nullptr == pNode || nullptr == pNode->next) return;
		auto pNextNode = pNode->next.get();
		pNode->data = pNextNode->data;	// we are impersonating them!  Now kill 'em!
		pNode->next = move(pNode->next->next);
	}

	void moveNodeToNewLocation(Node *pFromAfterThis, Node *pToAfterThis) {
		// move non-Head node to a new location, given the prev pointer before
		// and the new prev pointer we want to insert it after
		if (nullptr == pFromAfterThis || nullptr == pToAfterThis) return;
		auto pNodeToMove = move(pFromAfterThis->next);
		//if (nullptr == pNodeToMove) return; don't call this for tail nodes!!!
		pFromAfterThis->next = move(pNodeToMove->next);	
		pNodeToMove->next = move(pToAfterThis->next);
		pToAfterThis->next = move(pNodeToMove);
	}

	void moveHeadNodeToNewLocation(Node *pToAfterThis) {
		// move Head node to a new location, given the new prev pointer we want
		// to move it after.  update Head (duh).
		if (nullptr == pToAfterThis || nullptr == head) return;
		auto pNodeToMove = move(head);
		//if (nullptr == pNodeToMove) return; don't call this for tail nodes!!!
		head = move(pNodeToMove->next);
		pNodeToMove->next = move(pToAfterThis->next);
		pToAfterThis->next = move(pNodeToMove);
	}

	void partitionAboutX(string x) {
		// note: this does not "sort".  It is not required to be a stable partition.
		// we go thru the list, if an element is >= x we make it the new tail.
		// when we get to the original tail, we are done partitioning.
		// we could check if we are already partioned about X first but don't yet.
		// we should also instead of original tail use
		// the point after last low number as optimization maybe?
		auto originalTail = getTail();	
		auto tail = originalTail;

		// if called on null list will never get into the while loop
		while (head.get() != originalTail && head->data >= x) {
			// make this node the new tail and its next the new head
			moveHeadNodeToNewLocation(originalTail);
			}
		auto prev = head.get();
		auto pNode = head->next.get();
		if (nullptr == pNode) return;
		// the head is now good, let's continue
		//
		while (pNode != originalTail) {
			if (pNode->data >= x) {
				// make this node the new tail and its next the new this
				auto temp = pNode->next.get();
				moveNodeToNewLocation(prev, tail);
				tail = getTail();
				pNode = temp;
			}
			else {
				prev = pNode;
				pNode = pNode->next.get();
			}
		}
		// once we hit the originalTail, we are good!
		return;
	}

	int linkedListToIntegerReverse() const {
		// 1->2->3->4 returns 4321
		int sum = 0;
		int factor = 1;
		if (nullptr == head) return 0;
		auto pNode = head.get();
		while (nullptr != pNode) {
			string digit = pNode->data;
			int value = stoi(digit);
			sum += value * factor;
			factor *= 10;
			pNode = pNode->next.get();
		}
		return sum;
	}

	int linkedListToIntegerForward() const {
		// 1->2->3->4 returns 1234
		if (nullptr == head) return 0;
		int sum = 0;
		int factor = 1;
		stack<int> intStack;
		auto pNode = head.get();
		while (nullptr != pNode) {
			string digit = pNode->data;
			int value = stoi(digit);
			intStack.push(value);
			pNode = pNode->next.get();
		}
		while (!intStack.empty()) {
			sum += factor * intStack.top();
			intStack.pop();
			factor *= 10;
		}
		return sum;
	}

	bool isPalindrome() {
		stack<string> reverseIt;
		auto pNode = head.get();
		while (nullptr != pNode) {
			reverseIt.push(pNode->data);
			pNode = pNode->next.get();
		}
		pNode = head.get();
		while (!reverseIt.empty()) {
			if (pNode->data != reverseIt.top() ) return false;
			reverseIt.pop();
			pNode = pNode->next.get();
		}
		return true;	// they were all the same!
	}

	bool doesIntersect() {
		auto pNode1 = head.get();
		if (nullptr == pNode1) return false;
		while (nullptr != pNode1->next) {
			pNode1 = pNode1->next.get();
		}
		auto pNode2 = head.get();
		if (nullptr == pNode2) return false;
		while (nullptr != pNode2->next) {
			pNode2 = pNode2->next.get();
		}
		return pNode1 == pNode2;
	}
};



LinkedList integerToLinkedListReverse(int i) {
	// 4321 returns 1->2->3->4
	LinkedList l;
	if (i < 0) return l;
	int remaining = i;
	while (remaining > 0) {
		int digit = remaining % 10;
		l.addNode(std::to_string(digit));
		remaining /= 10;
	}
	return l;
}

LinkedList sumOfTwoLinkedListsReverse(const LinkedList &list1,
										const LinkedList &list2) {
	// 1->2->3->4 + 2->0->5 returns 3->2->8->4

	auto val1 = list1.linkedListToIntegerReverse();
	auto val2 = list2.linkedListToIntegerReverse();
	auto sum = val1 + val2;
	return integerToLinkedListReverse(sum);

}



LinkedList integerToLinkedListForward(int i) {
	// 1234 returns 1->2->3->4
	LinkedList l;
	if (i < 0) return l;
	stack<int> intStack;
	int remaining = i;
	while (remaining > 0) {
		int digit = remaining % 10;
		intStack.push(digit);
		remaining /= 10;
	}
	while (!intStack.empty()) {
		int digit = intStack.top();
		intStack.pop();
		l.addNode(std::to_string(digit));
	}
	return l;
}

LinkedList sumOfTwoLinkedListsForward(const LinkedList &list1, 
				const LinkedList &list2) {
	// 1->2->3->4 + 2->0->5 returns 1->4->3->9
	auto val1 = list1.linkedListToIntegerForward();
	auto val2 = list2.linkedListToIntegerForward();
	auto sum = val1 + val2;
	return integerToLinkedListForward(sum);
}

int main()
{
	LinkedList myList;
	myList.addNode("Alfie");
	myList.addNode("Amy");
	myList.addNode("Jesse");

	cout << "Current values of list:\n";
	myList.printNodes();

	Node *pAmy = myList.findNode("Amy");
	if (pAmy != nullptr)
		cout << "Found Amy node with value: " << pAmy->getData() << "\n";
	myList.deleteNode("Alfie");
	myList.addNode("Alfie");
	myList.addNode("Amy");
	myList.addNode("Amy");
	myList.addNode("Jesse");
	myList.addNode("Amy");

	auto p4FromTail = myList.findKelementsFromTail(4);
	cout << "Four elements from tail is:\n";
	cout << p4FromTail->getData() << "\n";
	myList.deleteThisNode(p4FromTail);
	auto p8FromTail = myList.findKelementsFromTail(8);
	if (nullptr == p8FromTail) cout << "Fewer than 8 elements in list!\n";

	cout << "Current values of list:\n";
	myList.printNodes();

	myList.removeDups();

	cout << "Current values of list:\n";
	myList.printNodes();
	myList.addNode("Kathy");
	myList.addNode("Violet");

	myList.addNode("Alfie");
	myList.addNode("Aardvark");

	cout << "Before Partition: values of list:\n";
	myList.printNodes();
	myList.partitionAboutX("Jesse");
	cout << "After Partition: values of list:\n";
	myList.printNodes();

	LinkedList oneTwoThreeFour = integerToLinkedListReverse(1234);
	LinkedList fiveZeroSeven = integerToLinkedListReverse(507);
	LinkedList oneSevenFourOne = sumOfTwoLinkedListsReverse(oneTwoThreeFour,
		fiveZeroSeven);
	cout << "The sum of " << oneTwoThreeFour.linkedListToIntegerReverse() << " and "
		<< fiveZeroSeven.linkedListToIntegerReverse() << "  is: "
		<< oneSevenFourOne.linkedListToIntegerReverse() << "\n";
	// 1->2->3->4 + 2->0->5 returns 3->2->8->4
	LinkedList oneTwoThreeFourF = integerToLinkedListForward(1234);
	LinkedList fiveZeroSevenF = integerToLinkedListForward(507);
	LinkedList oneSevenFourOneF = sumOfTwoLinkedListsForward(oneTwoThreeFourF,
		fiveZeroSevenF);
	cout << "The sum of " << oneTwoThreeFourF.linkedListToIntegerForward() << " and "
		<< fiveZeroSevenF.linkedListToIntegerForward() << "  is: "
		<< oneSevenFourOneF.linkedListToIntegerForward() << "\n";

	cout << "Was the list of names a palindrome? " << (myList.isPalindrome() ? "Yup"
		: "Nope") << "\n";

	LinkedList palindrome;
	palindrome.addNode("One");
	palindrome.addNode("Two");
	palindrome.addNode("Three");
	palindrome.addNode("Two");
	palindrome.addNode("One");

	cout << "Was purposely-created palindrome really a palindrome? "
		<< (palindrome.isPalindrome() ? "Yup" : "Nope") << "\n";
    return 0;
}

