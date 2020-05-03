#include "largeinteger.h"


int main()
{
	int what;

	largeinteger A(100), B(100);

	A.add(1000);
	A.mult(123456789);

	B.add(1000);
	B.mult(123456789);

	A.printnumbers();
	A.subtract(B);

	A.printnumbers();




	printf_s("\nRESULT : ");
	A.display();
}
