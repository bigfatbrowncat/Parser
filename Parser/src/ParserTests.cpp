#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <typeinfo>

#include "Parser.h"

#include "tester_tools.h"

TEST_FUNCTION(parse_simple)
{
	LexerTree lex("2.5 + q * (3 - 5 + moo) ");
	lex.doLexing();

	map<string, double> vars;
	vars.insert(pair<string, double>("q", 10.0));
	vars.insert(pair<string, double>("moo", 3.0));

	ParserTree par(lex, vars);

	const ParserItem& lti = (ParserItem&)par.getRoot();

	TEST_ASSERT(lti.getInnerItems().size() == 3, "The items number should be 3");
	list<ParserValue*>::const_iterator it_iter = lti.getInnerItems().begin();

	TEST_ASSERT(typeid(*(*it_iter)) == typeid(ParserConstant), "The first item isn't a ParserConstant");
	TEST_ASSERT(abs(((ParserConstant*)(*it_iter++))->getValue() - 2.5 < 0.0001), "The first item's value isn't 2.5");

	TEST_ASSERT(typeid(*(*it_iter)) == typeid(ParserVariable), "The 2nd item isn't a ParserVariable");
	TEST_ASSERT(((ParserVariable*)(*it_iter++))->getName() == "q" , "The 2nd item's value isn't 'q'");

	TEST_ASSERT(typeid(*(*it_iter)) == typeid(ParserItem), "The 3rd item isn't a ParserItem");
	TEST_ASSERT(((ParserItem*)(*it_iter++))->getInnerItems().size() == 3 , "The 2nd item's value isn't 'q'");

	TEST_ASSERT(lti.getInnerOperations().size() == 2, "The operations number should be 2");
	list<ParserOperation>::const_iterator op_iter = lti.getInnerOperations().begin();
	TEST_ASSERT((*op_iter++) == LO_ADD, "The first operator isn't '+'");
	TEST_ASSERT((*op_iter++) == LO_MULTIPLY, "The 2nd operator isn't '*'");


}

void TestParser()
{
	printf("\nStarting tests for Parser:\n");

	TEST_FUNCTION_RUN(parse_simple);

}
