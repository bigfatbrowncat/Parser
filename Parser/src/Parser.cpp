#include <stdio.h>
#include <stdlib.h>

#include "Lexer.h"

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
	LexerTreeItem lit("1");
	list<LexerTreeItem*> nextLevel;
	lit.doLexing(nextLevel);

	TEST_ASSERT(nextLevel.size() == 0, "size of '1' lexer items is not 0");

	TEST_ASSERT(lit.getInnerText() == "1", "inner text isn't '1'");
}

TEST_FUNCTION(single_letter)
{
	LexerTreeItem lit("a");
	list<LexerTreeItem*> nextLevel;
	lit.doLexing(nextLevel);

	TEST_ASSERT(nextLevel.size() == 0, "size of 'a' lexer items is not 0");

	TEST_ASSERT(lit.getInnerText() == "a", "inner text isn't 'a'");
}

TEST_FUNCTION(single_oper)
{
	LexerTreeItem lit("+");
	list<LexerTreeItem*> nextLevel;
	lit.doLexing(nextLevel);

	TEST_ASSERT(nextLevel.size() == 0, "size of '+' lexer items is not 0");

	TEST_ASSERT(lit.getInnerText() == "+", "inner text isn't '+'");
}

TEST_FUNCTION(three_plus_five)
{
	LexerTreeItem lit("3 + 5");
	list<LexerTreeItem*> nextLevel;
	lit.doLexing(nextLevel);

	TEST_ASSERT(nextLevel.size() == 3, "size of '3 + 5' lexer items is not 3");
	list<LexerTreeItem>::const_iterator inners = lit.getInnerItems().begin();
	TEST_ASSERT((*inners++).getInnerText() == "3", "first item isn't '3'");
	TEST_ASSERT((*inners++).getInnerText() == "+", "second item isn't '+'");
	TEST_ASSERT((*inners++).getInnerText() == "5", "third item isn't '5'");
}

TEST_FUNCTION(something_strange)
{
	LexerTreeItem lit("112. 34 + ** /-* -- -5 0.213");
	list<LexerTreeItem*> nextLevel;
	lit.doLexing(nextLevel);

	TEST_ASSERT(nextLevel.size() == 9, "size of '112. 34 + ** /-* -- -5 0.213' lexer items is not 9");
	list<LexerTreeItem>::const_iterator inners = lit.getInnerItems().begin();
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
	LexerTreeItem lit("");
	list<LexerTreeItem*> nextLevel;
	lit.doLexing(nextLevel);

	TEST_ASSERT(nextLevel.size() == 0, "size of empty value lexer items is not 0");
}

TEST_FUNCTION(c_plus_plus)
{
	LexerTreeItem lit("cc ++ + +");
	list<LexerTreeItem*> nextLevel;
	lit.doLexing(nextLevel);

	TEST_ASSERT(nextLevel.size() == 4, "size of 'cc ++ + +' lexer items is not 4");
	list<LexerTreeItem>::const_iterator inners = lit.getInnerItems().begin();
	TEST_ASSERT((*inners++).getInnerText() == "cc", "first item isn't 'cc'");
	TEST_ASSERT((*inners++).getInnerText() == "++", "second item isn't '++'");
	TEST_ASSERT((*inners++).getInnerText() == "+", "third item isn't '+'");
	TEST_ASSERT((*inners++).getInnerText() == "+", "4th item isn't '+'");
}

TEST_FUNCTION(braces)
{
	LexerTreeItem lit("a+(qwe - zxc *(qqqq))+[5]");
	list<LexerTreeItem*> nextLevel;
	lit.doLexing(nextLevel);

/*	for (list<LexerItem*>::iterator iter = nextLevel.begin(); iter != nextLevel.end(); iter++)
	{
		printf("%s\n", (*iter)->getInnerText().c_str());
		fflush(stdout);
	}*/

	TEST_ASSERT(nextLevel.size() == 5, "size of 'a+(qwe - zxc *(qqqq))+[5]' lexer items is not 3");
	list<LexerTreeItem>::const_iterator inners = lit.getInnerItems().begin();

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

TEST_FUNCTION(deep_lexing)
{
	Lexer lex("1 *(23 + a[56] )");
	lex.doLexing();

	const LexerTreeItem& root = lex.getRoot();
	TEST_ASSERT(root.getInnerItems().size() == 3, "there should be 3 root inner items");

	list<LexerTreeItem>::const_iterator inners = root.getInnerItems().begin();
	TEST_ASSERT((*inners).getOuterBraces() == none, "there are braces around 1rd item");
	TEST_ASSERT((*inners++).getInnerText() == "1", "first item isn't '1'");
	TEST_ASSERT((*inners).getOuterBraces() == none, "there are braces around 2nd item");
	TEST_ASSERT((*inners++).getInnerText() == "*", "second item isn't '*'");

	const LexerTreeItem& rnd = *inners++;
	TEST_ASSERT(rnd.getOuterBraces() == round, "there should be round braces around 3rd item");
	TEST_ASSERT(rnd.getInnerItems().size() == 4, "there should be 4 items inside round braces");

	list<LexerTreeItem>::const_iterator inners2 = rnd.getInnerItems().begin();
	TEST_ASSERT((*inners2).getOuterBraces() == none, "there are braces around 1rd item inside round braces");
	TEST_ASSERT((*inners2++).getInnerText() == "23", "first item inside round braces isn't '23'");
	TEST_ASSERT((*inners2).getOuterBraces() == none, "there are braces around 2nd item inside round braces");
	TEST_ASSERT((*inners2++).getInnerText() == "+", "second item inside round braces isn't '+'");
	TEST_ASSERT((*inners2).getOuterBraces() == none, "there are braces around 3rd item inside round braces");
	TEST_ASSERT((*inners2++).getInnerText() == "a", "third item inside round braces isn't 'a'");
	TEST_ASSERT((*inners2).getOuterBraces() == square, "there should be square braces around 4rd item inside round braces");
	TEST_ASSERT((*inners2++).getInnerText() == "56", "4th item inside round braces isn't '56'");
}

TEST_FUNCTION(extra_opening_brace)
{
	try
	{
		Lexer lex("2 +  ( (123 - 654 )");
		lex.doLexing();
	}
	catch (int i)
	{
		if (i == ERROR_LEXER_MISSING_CLOSING_BRACE)
		{
			return;
		}
		else
			throw i;
	}
	TEST_ASSERT(false, "there should be an ERROR_LEXER_MISSING_CLOSING_BRACE exception here");
}


TEST_FUNCTION(extra_closing_brace)
{
	try
	{
		Lexer lex("2 +   (123 - 654 )) ");
		lex.doLexing();
	}
	catch (int i)
	{
		if (i == ERROR_LEXER_UNEXPECTED_CLOSING_BRACE)
		{
			return;
		}
		else
			throw i;
	}
	TEST_ASSERT(false, "there should be an ERROR_LEXER_UNEXPECTED_CLOSING_BRACE exception here");
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
		TEST_FUNCTION_RUN(deep_lexing);
		TEST_FUNCTION_RUN(extra_opening_brace);
		TEST_FUNCTION_RUN(extra_closing_brace);

		printf("\nAll tests have passed successfully.\n");
		return EXIT_SUCCESS;
	}
	catch (int i)
	{
		if (i == -1)
		{
			printf("\nTest falure has occured.\n");
		}
		else
		{
			printf("\nException number %d occured during the tests.\n", i);
		}
		return EXIT_FAILURE;
	}
}
