#include <stdio.h>

#include <list>
#include <string>

using namespace std;

enum ParserOperation
{
	LO_ADD, LO_SUBTRACT, LO_MULTIPLY, LO_DIVIDE
};

class ParserValue
{
public:
	virtual double getValue() = 0;
};

class ParserConstant
{
private:
	double value;
public:
	ParserConstant(double value): value(value) {}
	virtual double getValue()
	{
		return value;
	}
};

class ParserItem
{
private:
	list<ParserItem> innerItems;
	list<ParserOperation> innerOperations;

public:
};

int main()
{
	printf("Main product here.\n");
	return 0;
}
