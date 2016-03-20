
#include "geometry/vector2d.hpp"

using std::cout; using std::endl;

int main(int arc, char* argv[])
{
	cout << "Testing vector2d methods" << endl;

	Vector2D v1, v2;

	reportCaseFail(v1 == v2, "No-args constructor produces non-equal instances.");
	reportCaseFail(v1.length() != 0, "Default constructor vector lenght is not zero");
	//ToDo implement more tests regarding math consistency
}

void reportCaseFail(bool expression, string failMessage)
{
	if(expression == true) return;
	cout << failMessage << endl;
}


