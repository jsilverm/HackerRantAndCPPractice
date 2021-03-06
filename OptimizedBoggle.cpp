// OptimizedBoggle.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include <list>
#include <iostream>
#include <cmath>

using namespace std;

// Didn't quite work yet, changed the loop value by 1 or something and
// accidentally passed all test cases and ended the challenge.  Ooops!

// passes quick linear tests.  This was said to be intended as a quick exercise.
// Therefore, I have only come up with four very obvious checks to implement.
// In general in life, partial solutions are undervalued.  In programming, quick tests
// for special cases are undervalued, especially when the O() of the full monty
// solution are very bad, because they are essentially free.  When the O() of the full
// solution is bad, and the essentially free checks are not exceedingly rare, but
// quite common, it is madness not to make them.  Many solutions I have crafted
// outperformed the competition, and when that happens it is not so often the case
// that I found a better solution that works all the time, but that simple special
// cases shoot the big scary guy with a huge whip often enough that the time saved by
// never entering the general case those times saves the day.  I started to try to
// think of more, which I would do "in real life", and remembered this was a quick
// exercise, and let this comment essay and my four checks make the point.
// Also, I never owned a board game version of this growing up, so my strategies are
// relatively weak.  On to the checks.

bool oddNumberOfOdds(const list<int> &numbers) {
	int oddCount = 0;
	for (auto it = cbegin(numbers); it != cend(numbers); ++it)
		if ((*it & 1) == 1) ++oddCount;
	return ((oddCount & 1) == 1) ? true : false;
}

bool isMaxSoHighItsImpossible(int magic_number, const list<int> &numbers) {
	int maxSoFar = 0;
	int sumSoFar = 0;
	for (auto it = cbegin(numbers); it != cend(numbers); ++it) {
		if (*it > maxSoFar) maxSoFar = *it;
		sumSoFar += *it;
	}
	int sumOfRest = sumSoFar - maxSoFar;
	if (maxSoFar > sumOfRest && (maxSoFar - sumOfRest) > magic_number)
		return true;
	else return false;
}

bool isMagicNumberSoHighItsImpossible(int magic_number, const list<int> &numbers) {
	int sumTotal = 0;
	for (auto it = cbegin(numbers); it != cend(numbers); ++it) {
		sumTotal += *it;
	}

	if (sumTotal < magic_number)
		return true;
	else return false;
}

bool passesQuickLinearTests(int magic_number, list<int> numbers) {
	// attempt to say "No" in linear time for a variety of relatively common special
	// cases that fail a "pre-flight check" as per the above unasked-for rant.
	bool soOdd = oddNumberOfOdds(numbers);
	// an odd # of odds can only combine to an odd regardless of +/-.
	if (soOdd && (magic_number & 1) == 0) return false;
	// we just answered 1/4 of all tests given random data!
	if (!soOdd && (magic_number & 1) == 1) return false;
	// we just answered another 1/4 of all tests without hitting our horrible 2^n
	// solution.  It feels worth it.
	// These last two are more special-case-y, but we only even get this far half the
	// time, anyway, and are about to descend into the horrors of 2^n otherwise.
	if (isMaxSoHighItsImpossible(magic_number, numbers)) return false;
	if (isMagicNumberSoHighItsImpossible(magic_number, numbers)) return false;

	// Hey, we tried!
	return true;
}

int addEmUp(list<int> numbers, int comboToTry) {
	int total;
	int n = numbers.size();
	auto it = cbegin(numbers);
	int flag;

	cerr << "comboToTry: " << comboToTry << "\n";
	cerr << "n= " << n << "\t";
	// (2 ^ n) & 1 will be the sign for each term.  after each, we << comboToTry by 1
	while (n >= 0) {
		flag = static_cast<int> (pow(2, n)) & comboToTry;
		if (flag == 0) total += *it;
		else total -= *it;
		--n;
		++it;
	}
	cerr << "total: " << total << "\n";
	return total;
}

bool arithmeticBoggle(int magic_number, list<int> numbers) {

	// It is quite odd that the default data structure passed in here is a list.
	// In C++, vector is quite asymmetrically the default, mostly because of cache
	// coherency and locality of reference, with the only exception being when we are
	// frequently splicing in or cutting out elements far from the end.  Even if we
	// were frequently messing with the beginning, that by itself would make us lean
	// towards a deque, not a list.

	// That said, anything not requiring random access, including most parts of the
	// STL, work just as well (in source code!) with lists as with vectors, so it is
	// not materially too different.  We show we can work with a list, as requested.

	// We try six special cases before we give up and go brute force.
	// Last Resort: brute force.  Represent #'s to be subtracted by 1 bits, and
	// #'s to be added by 0 bits.  We can easily generate all possible add/subtract
	// combos by generating all possible n-bit #'s with no particular insight.
	// for small #'s of n, say 24 or less, this should work fine.  for larger #'s of
	// numbers this may not scale too well.  It isn't clear before trying what the
	// largest inputs we need to be able to handle will be.

	// Also, it is unclear from the problem specification whether the very first
	// number in the list can be subtracted or not -- it appears that it probably
	// can't, but a single example would have made that clear a priori.

	// Okay, attempts to probe the limit of the largest input we are meant to deal
	// with by early submission end in frustration, behind each locked door of a
	// failed test is another locked door upon success.  We will continue with our
	// original inspiration of n-bit unsigned #'s representing the operations.

	int n = numbers.size();
	if (0 == n) {
		if (0 == magic_number) return true;
		else return false;
	}
	else if (1 == n) {
		const auto it = cbegin(numbers);
		if (*it == magic_number) return true;
		else return false;
	}

	if (passesQuickLinearTests(magic_number, numbers) == false) return false;
	// quick and easy checks are done (for now, others are possible) let's proceed.

	// we will represent all the possible combinations by a series of sequential
	// (n - 1)-bit unsigned #'s, i.e. 0 thru 2 ^ (n - 1) - 1.
	// I am leaning towards the intent of the game being that the first number
	// is required to be interpreted as a positive starting point and is not eligible
	// to be subtracted from the rest.  I should have played more games growing up.

	// A long here could go up to 64-bits,
	// but we would all retire before the loop ever finished in the worst case.
	if (n > 32) cerr << "Uh-Oh!  Big trouble in Little Number Town!\n";
	//else cerr << "Whew!  Only " << n << " digits.\n";
	int upperLimit = pow(2, n - 1);
	for (int comboToTry = 0; comboToTry != upperLimit; ++comboToTry)
	{
		if (addEmUp(numbers, comboToTry) == magic_number) return true;
	}
	return false;
}

int main() {
	cout << "No main here, this code was a slave to hidden locked test code"
		<< "\n and so far I haven't gotten to write a driver for it...\n";

	cout << "I think this version is 1 or 2 chars in addEmUp() away from]n"
		<< "working properly for all cases...the system unexpectedly\n"
		<< "ate my code the instant it did so...";
}
