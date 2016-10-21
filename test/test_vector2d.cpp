
#include "geometry/vector2d.hpp"

#include <cstdlib>
#include <ctime>
#include <cmath>

#include <iostream>
#include <vector>

#include "futil/general/language.hpp"
#include "futil/math/constants.h"
#include "futil/math/more_random.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

struct named_function
{
	void (*pointer)();
	string name;
	named_function& operator()() { pointer(); return *this; }
	named_function(void (*func_ptr)(), const string& str) : pointer(func_ptr), name(str) {}
};

vector<named_function> tests;
#define unit_test(custom_test_name) void custom_test_name(); char custom_test_name##_aux = (tests.push_back(named_function(custom_test_name, #custom_test_name)), 0); void custom_test_name()

unsigned fail_count = 0;

inline void test(bool expressionResult, string message)
{
	cout << (expressionResult==true? "passed" : "failed") << ": " << message << endl;
	if(expressionResult == false) fail_count++;
}

unit_test(no_argument_test)
{
	Vector2D v;
	test(v.x == 0 and v.y == 0, "No-args constructor should create a (0, 0) vector");
}

unit_test(two_argument_test)
{
	int a1 = rand(), a2 = rand();
	Vector2D v1(a1, a2);
	test(v1.x == a1 and v1.y == a2, "2-arg constructor should give x and y values as the 2 arguments respectively");
}

unit_test(equality_operator_case_equal_test)
{
	int a1 = rand(), a2 = rand();
	Vector2D v1(a1, a2), v2(a1, a2);
	test((v1 == v2) == (v1.x == v2.x and v1.y == v2.y), "Operator== should be true if both x and y components of both comparands are equal");
}

unit_test(equality_operator_case_x_diff_test)
{
	int a1 = rand(), a2 = rand();
	int aux = rand(); do { aux = rand(); } while (aux == 0); //a non-zero value
	Vector2D v1(a1, a2), v2(a1 + aux, a2);
	test((v1 == v2) == (v1.x == v2.x and v1.y == v2.y), "Operator== should be false if x component of both comparands are not equal");
}

unit_test(equality_operator_case_y_diff_test)
{
	int a1 = rand(), a2 = rand();
	int aux = rand(); do { aux = rand(); } while (aux == 0); //a non-zero value
	Vector2D v1(a1, a2), v2(a1, a2 + aux);
	test((v1 == v2) == (v1.x == v2.x and v1.y == v2.y), "Operator== should be false if y component of both comparands are not equal");
}

unit_test(equality_operator_case_xy_diff_test)
{
	int a1 = rand(), a2 = rand();
	int aux1 = rand(); do { aux1 = rand(); } while (aux1 == 0); //a non-zero value
	int aux2 = rand(); do { aux2 = rand(); } while (aux2 == 0); //a non-zero value
	Vector2D v1(a1, a2), v2(a1 + aux1, a2 + aux2);
	test((v1 == v2) == (v1.x == v2.x and v1.y == v2.y), "Operator== should be false if both x an y components of both comparands are not equal");
}

unit_test(equality_operator_random_test)
{
	Vector2D v1(rand(), rand()), v2(rand(), rand());
	test((v1 == v2) == (v1.x == v2.x and v1.y == v2.y), "Operator== should be true if both x and y components of both comparands are equal, otherwise false");
}

unit_test(reflexive_equality_test)
{
	Vector2D v1(rand(), rand());
	test(v1 == v1, "A vector should be equal to itself (reflexivity)");
}

unit_test(equality_test)
{
	int a1 = rand(), a2 = rand();

	Vector2D v1(a1, a2), v2(a1, a2);
	test(v1 == v2, "Equally constructed vectors should be equal");
}

unit_test(inequality_test)
{
	int a1 = rand(), a2 = rand();
	int aux1 = rand(); do { aux1 = rand(); } while (aux1 == 0); //a non-zero value
	int aux2 = rand(); do { aux2 = rand(); } while (aux2 == 0); //a non-zero value

	Vector2D v1(a1, a2), v2(a1 + aux1, a2 + aux2);
	test((v1 == v2) == false, "Distinctly constructed vectors should NOT be considered equal");
}

unit_test(pointer_equality_test)
{
	int a1 = rand(), a2 = rand();
	Vector2D *vptr1, *vptr2;

	vptr1 = new Vector2D(a1, a2);
	vptr2 = new Vector2D(a1, a2);

	test(vptr1 != vptr2, "Vector pointers should not be equal, even if they are equally constructed");

	test(*vptr1 == *vptr2, "The content of vectors pointers should be equal if they are equally constructed");

	delete vptr2; vptr2 = vptr1;
	test(vptr1 == vptr2, "Vector pointers should be equal, if they points to the same instance");
}

unit_test(no_arg_equality_test)
{
	test(Vector2D() == Vector2D(), "No-args constructor should produce equal instances.");
}

unit_test(copy_constructor_test)
{
	Vector2D v1(rand(), rand());
	Vector2D v2(v1);
	test(v2 == v1, "Copy constructor should create a operator== equal vector");
}

unit_test(copy_constructor_pointer_test)
{
	Vector2D *vptr1, *vptr2;
	vptr1 = new Vector2D(rand(), rand());
	vptr2 = new Vector2D(*vptr1);
	test((vptr1 == vptr2) == false, "Copy constructor should create instances with different pointers");
}

unit_test(clone_test)
{
	Vector2D v1(rand(), rand());
	test(v1.clone() == v1, "A clone of a vector should be equal to itself.");
}

//============== operators ==============

unit_test(inequality_operator_test)
{
	Vector2D v1(rand(), rand()), v2(rand(), rand());
	test((v1 != v2) == not (v1 == v2), "The operator!= should work as opposite of operator==");
}

unit_test(unary_minus_operator_test)
{
	Vector2D v1(rand(), rand()), v2(rand(), rand());
	test((-v1).x == -v1.x and (-v1).y == -v1.y, "A unary-minus vector should have oposite signal coordinates");
}

unit_test(rotation_operator_test)
{
	Vector2D v1(1, 0), v2(0, 1);
	v1 = (v1 < M_PI_2);
	double diff = fabs(v1.x - v2.x) + fabs(v1.y - v2.y);
	test(diff < 0.00001, "Rotating X_VERSOR by Pi/2 radians should give Y_VERSOR");
}

unit_test(in_place_rotation_operator_test)
{
	Vector2D v1(1, 0), v2(0, 1);
	v1 << M_PI_2;
	double diff = fabs(v1.x - v2.x) + fabs(v1.y - v2.y);
	test(diff < 0.00001, "Rotating X_VERSOR by Pi/2 radians should give Y_VERSOR (in-place operator)");
}

unit_test(projection_operator_with_versors_test)
{
	Vector2D v1(5, 7), v2(5, 0), v3(0, 7);
	Vector2D v1_proj_on_x_versor = (v1 || Vector2D::X_VERSOR);
	Vector2D v1_proj_on_y_versor = (v1 || Vector2D::Y_VERSOR);

	double diff = 0;

	diff += fabs(v1_proj_on_x_versor.x - v2.x) + fabs(v1_proj_on_x_versor.y - v2.y);
	diff += fabs(v1_proj_on_y_versor.x - v3.x) + fabs(v1_proj_on_y_versor.y - v3.y);

	test(diff < 0.00001, "Projection operator should filter out components when second operand is a versor");
}

//============== other methods ==============

unit_test(perpendicular_method_test)
{
	Vector2D v1(0, 1), v2(-1, 0);
	v1 = v1.perpendicular();
	double diff = fabs(v1.x - v2.x) + fabs(v1.y - v2.y);
	test(diff < 0.00001, "Perpendicular of Y_VERSOR should give -X_VERSOR (in-place operator)");
}

//============== semantic tests ==============

unit_test(no_arg_constructed_vector_length_test)
{
	Vector2D v1;
	test(v1.length() == 0, "Default constructor vector length should be zero.");
}

unit_test(arithmetic_commutativity_test)
{
	Vector2D v1(rand(), rand()), v2(rand(), rand());
	test(v1 + v2 == v2 + v1, "Vectors should have commutativity of addition.");
}

// ============== operators aliases ==============

unit_test(alias_operator_length_test)
{
	Vector2D v1(rand(), rand());
	test(v1.length() == ~v1, ".length() should have the same result as the ~ operator");
}

unit_test(alias_operator_magnitude_test)
{
	Vector2D v1(rand(), rand());
	test(v1.magnitude() == ~v1, ".magnitude() should have the same result as the ~ operator");
}

unit_test(alias_operator_opposite_test)
{
	Vector2D v1(rand(), rand());
	test(v1.opposite() == -v1, ".opposite() should have the same result as unary minus operator");
}

unit_test(alias_operator_rotation_test)
{
	Vector2D v1(rand(), rand());
	double angle = random_decimal_between(-2*M_PI, 2*M_PI);
	test(v1.rotation(angle) == (v1 < angle), ".rotation() should have the same result as the < operator");
}

unit_test(alias_operator_rotate_test)
{
	Vector2D v1(rand(), rand());
	double angle = random_decimal_between(-2*M_PI, 2*M_PI);
	test(v1.rotate(angle) == (v1 << angle), ".rotate() should have the same result as the << operator");
}

unit_test(alias_operator_projection_test)
{
	Vector2D v1(rand(), rand()), v2(rand(), rand());
	test(v1.projection(v2) == (v1 || v2), ".projection() should have the same result as the || operator");
}

// =============================================================================================================================
//ToDo write more tests regarding math consistency
//https://en.wikipedia.org/wiki/Euclidean_vector

int main(int arc, char* argv[])
{
	srand(time(NULL));
	cout << "Testing Vector2D's methods...\n" << endl;

	foreach(named_function&, test, vector<named_function>, tests)
		test();

	cout << "\n**exaustive test: " << "equality_operator_random_test" << endl;
	for(int i = 100; i >= 0; i--)
	{
		equality_operator_random_test();
	}

	cout << "\nTest finished. " << fail_count << " failure(s)." << endl;
}

