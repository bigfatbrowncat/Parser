#include <stdio.h>
#include <stdlib.h>

#include "LexerItem.h"

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

#define TEST_FAILED					-1

static const char* test_name;

#define TEST_ASSERT(b, t)										\
	if (!(b))													\
	{															\
		printf("\n\nTest \"%s\" failed: %s\n", test_name, t);	\
		throw TEST_FAILED;										\
	}

#define TEST_FUNCTION(name)	void __TestFunction_##name ()
#define TEST_FUNCTION_RUN(name)									\
	test_name = #name;											\
	printf("Starting test \"%s\"... ", test_name);				\
	fflush(stdout); 											\
	__TestFunction_##name();									\
	printf("passed\n", test_name); 								\
	fflush(stdout);

TEST_FUNCTION(single_digit)
{
	LexerItem lit("1");
	list<LexerItem*> nextLevel;
	lit.doLexing(nextLevel);

	TEST_ASSERT(nextLevel.size() == 1, "size of '1' lexer items is not 1");
	list<LexerItem>::const_iterator inners = lit.getInnerItems().begin();
	TEST_ASSERT((*inners++).getInnerText() == "1", "first item isn't '1'");
}

TEST_FUNCTION(single_letter)
{
	LexerItem lit("a");
	list<LexerItem*> nextLevel;
	lit.doLexing(nextLevel);

	TEST_ASSERT(nextLevel.size() == 1, "size of 'a' lexer items is not 1");
	list<LexerItem>::const_iterator inners = lit.getInnerItems().begin();
	TEST_ASSERT((*inners++).getInnerText() == "a", "first item isn't 'a'");
}

TEST_FUNCTION(single_oper)
{
	LexerItem lit("+");
	list<LexerItem*> nextLevel;
	lit.doLexing(nextLevel);

	TEST_ASSERT(nextLevel.size() == 1, "size of '+' lexer items is not 1");
	list<LexerItem>::const_iterator inners = lit.getInnerItems().begin();
	TEST_ASSERT((*inners++).getInnerText() == "+", "first item isn't '+'");
}

TEST_FUNCTION(three_plus_five)
{
	LexerItem lit("3 + 5");
	list<LexerItem*> nextLevel;
	lit.doLexing(nextLevel);

	TEST_ASSERT(nextLevel.size() == 3, "size of '3 + 5' lexer items is not 3");
	list<LexerItem>::const_iterator inners = lit.getInnerItems().begin();
	TEST_ASSERT((*inners++).getInnerText() == "3", "first item isn't '3'");
	TEST_ASSERT((*inners++).getInnerText() == "+", "second item isn't '+'");
	TEST_ASSERT((*inners++).getInnerText() == "5", "third item isn't '5'");
}

TEST_FUNCTION(something_strange)
{
	LexerItem lit("112. 34 + ** /-* -- -5 0.213");
	list<LexerItem*> nextLevel;
	lit.doLexing(nextLevel);

	TEST_ASSERT(nextLevel.size() == 9, "size of '112. 34 + ** /-* -- -5 0.213' lexer items is not 9");
	list<LexerItem>::const_iterator inners = lit.getInnerItems().begin();
	TEST_ASSERT((*inners++).getInnerText() == "112.", "first item isn't '112.'");
	TEST_ASSERT((*inners++).getInnerText() == "34", "second item isn't '34'");
	TEST_ASSERT((*inners++).getInnerText() == "+", "third item isn't '+'");
	TEST_ASSERT((*inners++).getInnerText() == "**", "4th item isn't '**'");
	TEST_ASSERT((*inners++).getInnerText() == "/-*", "5th item isn't '/-*'");
	TEST_ASSERT((*inners++).getInnerText() == "--", "6th item isn't '--'");
	TEST_ASSERT((*inners++).getInnerText() == "-", "7th item isn't '-'");
	TEST_ASSERT((*inners++).getInnerText() == "5", "8th item isn't '5'");
	TEST_ASSERT((*inners++).getInnerText() == "0.213", "9th item isn't '0.213'");
}
TEST_FUNCTION(empty)
{
	LexerItem lit("");
	list<LexerItem*> nextLevel;
	lit.doLexing(nextLevel);

	TEST_ASSERT(nextLevel.size() == 0, "size of empty value lexer items is not 0");
}

TEST_FUNCTION(c_plus_plus)
{
	LexerItem lit("cc ++ + +");
	list<LexerItem*> nextLevel;
	lit.doLexing(nextLevel);

	TEST_ASSERT(nextLevel.size() == 4, "size of 'cc ++ + +' lexer items is not 4");
	list<LexerItem>::const_iterator inners = lit.getInnerItems().begin();
	TEST_ASSERT((*inners++).getInnerText() == "cc", "first item isn't 'cc'");
	TEST_ASSERT((*inners++).getInnerText() == "++", "second item isn't '++'");
	TEST_ASSERT((*inners++).getInnerText() == "+", "third item isn't '+'");
	TEST_ASSERT((*inners++).getInnerText() == "+", "4th item isn't '+'");
}

TEST_FUNCTION(braces)
{
	LexerItem lit("a+(qwe - zxc *(qqqq))+[5]");
	list<LexerItem*> nextLevel;
	lit.doLexing(nextLevel);

/*	for (list<LexerItem*>::iterator iter = nextLevel.begin(); iter != nextLevel.end(); iter++)
	{
		printf("%s\n", (*iter)->getInnerText().c_str());
		fflush(stdout);
	}*/

	TEST_ASSERT(nextLevel.size() == 5, "size of 'a+(qwe - zxc *(qqqq))+[5]' lexer items is not 3");
	list<LexerItem>::const_iterator inners = lit.getInnerItems().begin();

	TEST_ASSERT((*inners).getOuterBraces() == none, "there are braces around 1rd item");
	TEST_ASSERT((*inners++).getInnerText() == "a", "first item isn't 'a'");
	TEST_ASSERT((*inners).getOuterBraces() == none, "there are braces around 2nd item");
	TEST_ASSERT((*inners++).getInnerText() == "+", "second item isn't '+'");

	TEST_ASSERT((*inners).getOuterBraces() == round, "braces around 3rd item aren't round");
	TEST_ASSERT((*inners++).getInnerText() == "qwe - zxc *(qqqq)", "third item isn't 'qwe - zxc *(qqqq)'");

	TEST_ASSERT((*inners).getOuterBraces() == none, "there are braces around 4th item");
	TEST_ASSERT((*inners++).getInnerText() == "+", "4th item isn't '+'");
	TEST_ASSERT((*inners).getOuterBraces() == square, "braces around 5th item aren't square");
	TEST_ASSERT((*inners++).getInnerText() == "5", "5th item isn't '5'");

}


int main()
{
	try
	{
		TEST_FUNCTION_RUN(single_digit);
		TEST_FUNCTION_RUN(single_letter);
		TEST_FUNCTION_RUN(single_oper);
		TEST_FUNCTION_RUN(three_plus_five);
		TEST_FUNCTION_RUN(empty);
		TEST_FUNCTION_RUN(c_plus_plus);
		TEST_FUNCTION_RUN(something_strange);
		TEST_FUNCTION_RUN(braces);

		printf("\nAll tests have passed successfully.\n");
		return EXIT_SUCCESS;
	}
	catch (int i)
	{
		if (i == -1)
		{
			printf("\nTest falure has occured.\n");
		}
		return EXIT_FAILURE;
	}
}
