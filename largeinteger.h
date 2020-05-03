#pragma once
#include <iostream>
#include <stdio.h>
#include <math.h>

/*
	In The Event Of Changing The Base, Please Note That :
		The Base should Optimised to be of 10^n.
		The printf function in largeinteger::display should be edited accordingly.

*/

const int digitsperindex = (int)log10((UINT_MAX >> 1));
const unsigned int base = pow(10, digitsperindex);
const unsigned int base9 = base - 1;

/*
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	|				RETURN INDEX					|
	|												|
	|	 0	:	Proper Completion Of Operation		|
	|	-1	:	OverFlow Condition					|
	|	-2	:	No Pointer Condition				|
	|												|
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/


class largeinteger	// Base 10^n = ? (Depended on the system)
{
protected:
	unsigned int* num;	// The Pointer To The array Where All The Numbers Are Stored.
	unsigned int size;	// Maximum No. of Digits it can hold is digitsperindex * size.

	int add(unsigned int*, unsigned int, unsigned int); // Used For Internal Addition Of Special Kind.

public:
	//SYSTEM FUNCTIONS
	largeinteger(unsigned int);
	largeinteger(largeinteger&);
	~largeinteger();
	int redefine(unsigned int, int);// Re-creates the entire array, either as a fresh start (type == 0 (default)), or maintaining old value (type == 1)
	void display();					// Displays the Number On the Screen (With Sign, if Negative) (PS: No additional Modifications are brought about)
	void clear();					// Value Becomes ZERO
	void chgsign();					// Toggles the Sign
	void chksign();					// Checks If the Number is perfectly ZERO, In which case, makes *num '0'
	unsigned int* getnumber();		// Returns a pointer to the Place where the numbers are stored as an Array
	int printnumbers();				// Error Checking

	//COMPARISON FUNCTIONS
	int equate(long long int);
	int equate(largeinteger&);

	//OPERATION FUNCTIONS
	int add(long long int);
	int add(largeinteger&);

	int mult(long long int);
	int mult(largeinteger&);

	int subtract(long long int);
	int subtract(largeinteger&);

	int divide(long long int);
	int divide(largeinteger&);
};


//SYSTEM FUNCTIONS

int largeinteger::printnumbers()
{
	printf("\n");
	for (unsigned int i = 0; i < size + 2; ++i)
		printf("NUMBER %09u : %u\n", i, num[i]);
	return 1;
}

largeinteger::largeinteger(unsigned int A) // Where A is the number of digits to be supportd
{
	size = (A + digitsperindex - 1) / digitsperindex; // Size From No.Of Digits (Formulae)
	num = new unsigned int[size + 2];

	for (unsigned int i = 0; i < size + 2; ++i)
		num[i] = 0;
}

largeinteger::largeinteger(largeinteger& B)
{
	size = B.size;
	num = new unsigned int[size + 2];

	for (unsigned int i = 0; i < size + 2; ++i)
		num[i] = B.num[i];
}

largeinteger::~largeinteger()
{
	if (num)
		delete num;
}

int largeinteger::redefine(unsigned int A, int type = 0)
{
	unsigned int* old_num = num;
	unsigned int old_size = size;

	size = (A + digitsperindex - 1) / digitsperindex;
	num = new unsigned int[size + 2];

	if (num)
	{
		if (type)
		{
			if (add(old_num, old_size, 0))
			{
				clear();
				return -1;
			}
		}
		else
			for (unsigned int i = 0; i < size + 2; ++i)
				num[i] = 0;

		delete old_num;
		return 0;
	}
	else
	{
		num = old_num;
		size = old_size;
		return -1;
	}
}

void largeinteger::display()
{
	if (!num)
		return;

	unsigned int* start = num;
	unsigned int* end = num + size;

	while (*(++start) == 0 && start < end); // Brings Start to the first digit(s) to work with

	if (*num == '-')
		printf_s("-");

	printf_s("%d", *(start));
	while (++start <= end)
		printf_s("%09u", (*start));
}

void largeinteger::clear()
{
	if (!num)
		return;

	for (unsigned int i = 0; i <= size + 1; ++i)
		num[i] = 0;
}

void largeinteger::chgsign()
{
	if (!num)
		return;

	if (*num == '+')
		*num = '-';
	else if (*num == '-')
		*num = '+';
}

void largeinteger::chksign()
{
	if (!num)
		return;

	if (*num)
	{
		unsigned int* start = num;
		unsigned int* end = num + size;
		while (*(++start) == 0 && start <= end);
		if (end < start)
			*num = 0;
	}

}

unsigned int* largeinteger::getnumber()
{
	return num;
}

int largeinteger::add(unsigned int* Bnum, unsigned int Bsize, unsigned int shift = 0)
{
	if (!num)
		return -2;

	if (*Bnum == 0)
		return 0;
	else if ((*num == '+' && *Bnum == '-') || (*num == '-' && *Bnum == '+'))
		return 0; // Subtraction Condition

	if (*num == 0)
		*num = *Bnum;

	unsigned int* startB = Bnum;
	unsigned int* endA = num + size - shift;
	unsigned int* endB = Bnum + Bsize;

	int carry = 0;

	while (*(++startB) == 0 && startB <= endB);

	if (endB < startB)
	{
		*Bnum = 0;
		return 0;
	}

	do
	{
		*endA += *endB; // Addition Process

		if (carry) // Carry-Over added later so that when added to itself causes no errors
		{
			++(*endA);
			--carry;
		}

		if (*endA >= base)
		{
			*endA -= base;
			++carry;

			if (endA == num + 1) // Overflow Condition
				return -1;
		}
	} while (--endA > num&& --endB >= startB);

	if (endA == num && startB < endB) // Overflow Condition in which more left to add, but no place left in A.
		return -1;

	if (carry)
	{
		++(*endA);
		--carry;
	}

	while (*(endA) >= base && endA > num) //Special Condition : If The Last Carry lead to the completion of the next place
	{
		*endA -= base;

		if (endA == num + 1) // Overflow Condition
			return -1;

		++(*((endA)-1)); // Carry-Over Added to Next Place-Value
		--endA;
	}

	return 0;
}

//OPERATION FUNCTIONS

int largeinteger::add(long long int A)
{
	if (!num)
		return -2;

	if (A == 0)
		return 0;
	else if ((A > 0 && *num == '-') || (A < 0 && *num == '+'))
	{
		A *= -1;
		return subtract(A);
	}

	if (*num == 0 && A > 0)
		*num = '+';
	else if (*num == 0 && A < 0)
		*num = '-';

	long long int temp = abs(A);
	unsigned int pos = size;
	unsigned int sub;

	while (temp)
	{
		sub = temp - ((temp / base) * base); // Next Digit(s) that fit in a place
		temp /= base;

		if (!pos) // Overflow Condition : More Left to Add but no more place left
			return -1;

		num[pos] += sub;

		while (num[pos] >= base)
		{
			num[pos] -= base;

			if (pos == 1) // Overflow Condition
				return -1;

			++num[pos - 1];
		}
		--pos;
	}

	while (pos > 0) // Special Conition : Last Carry-over exceeds the limit of the next place
	{
		while (num[pos] >= base)
		{
			num[pos] -= base;
			++num[pos - 1];
		}
		--pos;
	}
	return 0;
}

int largeinteger::add(largeinteger& B)
{
	if (!num)
		return -2;

	if (*B.num == 0)
		return 0;
	else if ((*num == '+' && *B.num == '-') || (*num == '-' && *B.num == '+'))
	{
		B.chgsign();
		int result = subtract(B);
		B.chgsign();
		return result;
	}


	if (*num == 0)
	{
		if (*B.num == '+')
			*num = '+';
		else if (*B.num == '-')
			*num = '-';
	}

	unsigned int* startA = num;
	unsigned int* startB = B.num;
	unsigned int* endA = num + size;
	unsigned int* endB = B.num + B.size;

	int carry = 0;

	while (*(++startA) == 0 && startA < endA);
	while (*(++startB) == 0 && startB <= endB);

	if (endB < startB)
	{
		*B.num = 0;
		return 0;
	}

	do
	{
		*endA += *endB + carry;
		if (carry)
			--carry;

		if (*endA >= base)
		{
			*endA -= base;
			++carry;

			if (endA == num + 1) // Overflow Condition
				return -1;
		}
	} while (--endA > num&& --endB >= startB);

	if (endA == num && startB < endB)
		return -1;

	if (carry)
	{
		++(*endA);
		--carry;
	}

	while (*(endA) >= base) //Special Condition : If The Last Carry lead to the completion of the next place
	{
		*endA -= base;

		if (endA == num + 1) // Overflow Condition
			return -1;

		++(*((endA--) - 1)); // Carry-Over Added to Next Place-Value
	}

	return 0;
}

int largeinteger::mult(long long int A)
{
	if (!num)
		return -2;

	if (A == 0)
	{
		if (*num)
			clear();
		return 0;
	}
	else if (A == -1)
	{
		chgsign();
		return 0;
	}
	else if (A == 1)
		return 0;
	else if (!*num)
		return 0;

	if (A < 0)
	{
		chgsign();
		A *= -1;
	}

	largeinteger B(size * digitsperindex);
	B.add(num, size);

	int process = 0;

	/*
		Multiplication is done by spliting the number into binary form.
		largeinteger B (initialised with Original) is added to itself each time to mimic multiplication by 2.
		If the power of 2 is used in the split multiplier, it is added to the original largeinteger.
	*/

	if (!(A & 1)) // Initial Value Not added if not multiplied by an Odd Number
		clear();

	while (!process && !B.add(B) && (A = A >> 1))
		if (A & 1)
			process = add(B);

	return process;
}

int largeinteger::mult(largeinteger& B)
{
	if (!num)
		return -2;

	if (*B.num == 0 || *num == 0)
	{
		clear();
		return 0;
	}

	if (*B.num == '-')
		chgsign();

	unsigned int* startA = num;
	unsigned int* startB = B.num;
	unsigned int* endA = num + size;
	unsigned int* endB = B.num + B.size;

	if (*num)
		while (*(++startA) == 0 && startA < endA); // StartA points to the first digit(s) of A
	else
		startA = endA;

	while (*(++startB) == 0 && startB <= endB); // StartA points to the first digit(s) of B, if not found B is zero.
	if (endB < startB)
	{
		*num = 0;
		*B.num = 0;
		return 0;
	}


	largeinteger temp(size * digitsperindex);
	largeinteger result(size * digitsperindex);
	result.add(num, size);
	unsigned int counter = 0;
	unsigned int process;

	/*
		The Long-Multiplication process is used here.
		Original is Stored in largeinteger result and a temp is created
		First element of B is multiplied with result.
		While elements are still left in B
			temp is initialised with original and multiplied with the next element of B
			The instantaneous product is added to the largeinteger result with a shift
	*/

	process = result.mult(*endB);
	for (; --endB >= startB && !process;)
	{
		process = temp.add(num, size);
		process |= temp.mult(*endB);
		process |= result.add(temp.num, size, ++counter);

	}
	if (!process)
	{
		clear();
		add(result.getnumber(), size, 0);
	}
	return process;
}

int largeinteger::subtract(long long int A)
{
	if (!num)
		return -2;

	if (A == 0)
		return 0;
	else if ((A > 0 && *num == '-') || (A < 0 && *num == '+') || *num == 0)
		return add(A * -1);

	unsigned int* endA = num + size;
	unsigned int* startA = num;
	while (*(++startA) == 0 && startA < endA);

	int carry = 0;

	long long int temp = abs(A);
	unsigned int sub;

	do
	{
		sub = temp - ((temp / base) * base);
		temp /= base;

		if (*endA >= sub && !carry)
			*endA = *endA - sub;

		else if (*endA > sub&& carry)
		{
			*endA = *endA - sub - 1;
			carry = 0;
		}

		else if (*endA < sub && !carry)
		{
			carry = 1;
			*endA = *endA + base - sub;
		}

		else if (*endA <= sub && carry)
		{
			if (*endA != 0)
				*endA = *endA + base9 - sub;
		}
		--endA;
	} while (temp && (startA <= endA));

	if (carry && !temp && startA <= endA)
	{
		while (carry && startA <= endA)
		{
			if (*endA > 0)
			{
				--(*endA);
				carry = 0;
			}
			else
				*endA = *endA + base9;
			--endA;
		}
	}

	else if (!carry && !temp && startA <= endA);	//intentionally left to reach the return statement

	else if (carry && !temp && endA < startA)
	{
		endA = num + size;
		while (startA <= endA)
		{
			*startA = base9 - *startA;
			++startA;
		}
		if (*num == '+' || *num == 0)
		{
			*num = '-';
			add(-1);
		}
		else
		{
			*num = '+';
			add(1);
		}
	}

	else if (temp <= base && endA == num) // Perfect Overflow Condition. Last usable unit (num + 1) is fully filled after 
	{

		//unsigned int enable = 0;
		//if ((!carry && temp == 1) || carry && temp == 0)
			//enable = 1;
	}

	else if (temp)
	{
		do
		{
			sub = temp - ((temp / base) * base);
			temp /= base;

			if (sub && !carry)
			{
				carry = 1;
				*endA = base - sub;
			}

			else if (!(sub) && carry)
			{
				*endA = base9 - sub;
			}

			if (endA == num && *endA != base9)
				return -1;
			else if (endA == num && *endA == base9)
			{
				startA = num + 1;
				endA = num + size;
			}

			--endA;
		} while (temp);
	}
	chksign();
	return 0;
}

int largeinteger::subtract(largeinteger& B)
{
	if (!num)
		return -2;

	if (*B.num == 0)
		return 0;
	else if (*num != *B.num || *num == 0)
	{
		B.chgsign();
		int result = add(B);
		B.chgsign();
		return result;
	}

	unsigned int* endA = num + size;
	unsigned int* endB = B.num + B.size;
	unsigned int* startA = num;
	unsigned int* startB = B.num;

	while (*(++startA) == 0 && startA < endA);
	while (*(++startB) == 0 && startB <= endB);
	if (endB < startB)
	{
		*B.num = 0;
		return 0;
	}

	/*
		The 10^n's Compliment Subtraction is followed here
		First before Entering the loop
			last element of B is 10^n's complimented and added to last element of A.
			This if more than base carry is made 1...

		Inside The Loop Starting with the second last element
			the next element is (10^n - 1)'s complimented and added to last element of A along with carry(which mad zero if used).
			This if more than base carry is made 1...

		In the end, if carry still exists it's a positive number, else it's a negative result
	*/

	int carry = 0;
	unsigned int zero_only = 0; // Used To check If the Final Result is a Perfect Zero, in which case *num must be made zero

	*endA += base - *endB; // First is 10^n's compliment.. The rest is (10^n - 1)'s compliment.

	if (*endA >= base) // Checking If Carry Over Is Required.
	{
		*endA -= base;
		++carry;

		if (endA == num + 1 && endB > startB) // Overflow Condition where there is nowhere left to add the carry-over to unless it is the last carry.
			return -1;
	}
	zero_only |= *endA;

	while (--endA > num&& --endB >= startB)
	{
		*(endA) += base9 - *endB + carry;

		if (*endA >= base)
		{
			*endA -= base;
			carry = 1;

			if (endA == num + 1 && endB > startB) // Overflow Condition where there is nowhere left to add the carry-over to unless it is the last carry.
				return -1;
		}
		else
			carry = 0;
		zero_only |= *endA;
	}

	if (endA == num && startB < endB) // Overflow Condition in which more left to add, but no place left in A.
		return -1;

	if (carry) // If the number subtracted all turned out to be zeros then the number could altogether be zero.
	{
		while (!zero_only)
		{
			if (endA == num)
			{
				*num = 0;
				break;
			}
			zero_only |= *(endA--);
		}
	}
	else
	{
		unsigned int* old_startA = startA;
		startA = endA + 1;
		endA = num + size;
		carry = 0;
		if (*endA == 0) // Taking The 10^n's Compliment for the first digit.
			carry = 1;
		else
			*endA = base - *endA;

		while (--endA >= startA) // Taking The (10^n - 1)'s Compliment for the first digit.
		{
			*endA = base9 - *endA + carry;

			if (*endA >= base)
			{
				*endA -= base;
				carry = 1;
			}
			else
				carry = 0;
		}

		if (old_startA >= startA) // If no of digits of B >= no. of digits of A, it's possible to go negative
			chgsign();
	}
	return 0;
}

int largeinteger::divide(long long int A)
{
	return 0;
}

int largeinteger::divide(largeinteger& B)
{
	if (!num)
		return -2;

	if (*B.num == 0)
		return -2;
	else if (*B.num == '-')
		chgsign();

	int value = -1;
	if (*num == '+')
		value = 1;

	largeinteger divident(size);
	divident.add(num, size);
	char sign = *divident.num;

	largeinteger quotient(size);

	while (!divident.add(value) && sign == *divident.num)
	{
		if (quotient.add(1))
			return -1;
	}

	clear();
	add(quotient);
	return 0;
}