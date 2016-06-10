
#include "geometry/vector2d.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>

#include "futil/futil.hpp"

using std::cout;
using std::endl;
using std::string;

unsigned fail_count = 0;

inline void test(bool expressionResult, string message)
{
	cout << (expressionResult==true? "passed" : "failed") << ": " << message << endl;
	if(expressionResult == false) fail_count++;
}

int main(int arc, char* argv[])
{
	srand(time(NULL));
	cout << "Testing Vector2D's methods...\n" << endl;

	Vector2D v1, v2, *vptr1, *vptr2;
	int aux1, aux2, a1, a2;

	test(v1.x == 0 and v1.y == 0, "No-args constructor should create a (0, 0) vector");

	a1 = rand();
	a2 = rand();
	v1 = Vector2D(a1, a2);
	test(v1.x == a1 and v1.y == a2, "2-arg constructor should give x and y values as the 2 arguments respectively");

	cout << "\n**equality operator exaustive test** " << endl;
	for(int i = 0; i < 100; i++)
	{
		test((v1.x == v2.x and v1.y == v2.y) == (v1 == v2), "Operator== should be true if both x and y components of both comparands are equal and false otherwise");
	}
	cout << endl;

	a1 = rand();
	a2 = rand();
	v1 = Vector2D(a1, a2);
	v2 = Vector2D(a1, a2);
	test(v1 == v2, "Equally constructed vectors should be considered equals");

	a1 = rand();
	a2 = rand();
	v1 = Vector2D(a1, a2);
	do { aux1 = rand(); } while (aux1 == 0); //a non-zero value
	do { aux2 = rand(); } while (aux2 == 0); //a non-zero value
	v2 = Vector2D(a1 + aux1, a2 + aux2);
	test((v1 == v2) == false, "Distinctly constructed vectors should NOT be considered equals");

	v1 = Vector2D(rand(), rand());
	test(v1 == v1, "A vector should be equal to itself (reflexivity)");

	a1 = rand();
	a2 = rand();
	vptr1 = new Vector2D(a1, a2);
	vptr2 = new Vector2D(a1, a2);
	test(vptr1 != vptr2, "Vector pointers should not be equal, even if they are equally constructed");
	test(*vptr1 == *vptr2, "The content of vectors pointers should be equal if they are equally constructed");
	delete vptr2; vptr2 = vptr1;
	test(vptr1 == vptr2, "Vector pointers should be equal, if they points to the same instance");

	v1 = Vector2D(), v2 = Vector2D();
	test(v1 == v2, "No-args constructor should produce equal instances.");

	v2 = Vector2D(v1);
	test(v2 == v1, "Copy constructor should create a operator= valid vector");

	//operators
	test((-v1).x == -v1.x and (-v1).y == -v1.y, "A unary-minus vector should have oposite signal coordinates");

	v1.x = rand(); v1.y = rand();
	v2.x = rand(); v2.y = rand();

	test(v1 + v2 == v2 + v1, "test");

	v1 = Vector2D();
	test(v1.length() == 0, "Default constructor vector length should be zero.");

	//operators aliases
	test(-v1 == v1.opposite(), ".opposite() should have the same result as unary minus operator");

	//ToDo implement more tests regarding math consistency

	cout << "\nTest finished. " << fail_count << " failures." << endl;
}

