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
	TEST_ASSERT((*op_iter++) == PO_ADD, "The first operator isn't '+'");
	TEST_ASSERT((*op_iter++) == PO_MULTIPLY, "The 2nd operator isn't '*'");


}

TEST_FUNCTION(code_line)
{
	LexerTree lex("3 + 4 * 2 / (1 - 5)^2");
	lex.doLexing();

	map<string, double> vars;
	ParserTree par(lex, vars);

	list<CodePosition> cp = par.getCode();		// should be '3 4 2 * 1 5 - 2 ^ / +'

	TEST_ASSERT(cp.size() == 11, "Code string length isn't 11");

	list<CodePosition>::iterator iter = cp.begin();

	TEST_ASSERT((*iter).getType() == CPT_VALUE, "The first item should be a value");
	TEST_ASSERT(abs((*iter++).getValue().getValue() - 3) < 0.0001, "The first item value should be 3");

	TEST_ASSERT((*iter).getType() == CPT_VALUE, "The 2nd item should be a value");
	TEST_ASSERT(abs((*iter++).getValue().getValue() - 4) < 0.0001, "The 2nd item value should be 4");

	TEST_ASSERT((*iter).getType() == CPT_VALUE, "The 3rd item should be a value");
	TEST_ASSERT(abs((*iter++).getValue().getValue() - 2) < 0.0001, "The 3rd item value should be 2");

	TEST_ASSERT((*iter).getType() == CPT_OPERATION, "The 4th item should be an operator");
	TEST_ASSERT((*iter++).getOperation() == PO_MULTIPLY, "The 4th item value should be '*'");

	TEST_ASSERT((*iter).getType() == CPT_VALUE, "The 5th item should be a value");
	TEST_ASSERT(abs((*iter++).getValue().getValue() - 1) < 0.0001, "The 5th item value should be 1");

	TEST_ASSERT((*iter).getType() == CPT_VALUE, "The 6th item should be a value");
	TEST_ASSERT(abs((*iter++).getValue().getValue() - 5) < 0.0001, "The 6th item value should be 5");

	TEST_ASSERT((*iter).getType() == CPT_OPERATION, "The 7th item should be an operator");
	TEST_ASSERT((*iter++).getOperation() == PO_SUBTRACT, "The 7th item value should be '*'");

	TEST_ASSERT((*iter).getType() == CPT_VALUE, "The 8th item should be a value");
	TEST_ASSERT(abs((*iter++).getValue().getValue() - 2) < 0.0001, "The 8th item value should be 2");

	TEST_ASSERT((*iter).getType() == CPT_OPERATION, "The 9th item should be an operator");
	TEST_ASSERT((*iter++).getOperation() == PO_POWER, "The 9th item value should be '^'");

	TEST_ASSERT((*iter).getType() == CPT_OPERATION, "The 10th item should be an operator");
	TEST_ASSERT((*iter++).getOperation() == PO_DIVIDE, "The 10th item value should be '/'");

	TEST_ASSERT((*iter).getType() == CPT_OPERATION, "The 11th item should be an operator");
	TEST_ASSERT((*iter++).getOperation() == PO_ADD, "The 11th item value should be '+'");

	double value = par.execute();
}

TEST_FUNCTION(fractal)
{
	// Something similar the Mandelbrot calculation

	LexerTree lex("x ^ 2 + c");
	lex.doLexing();

	map<string, double> vars;
	vars.insert(pair<string, double>("x", 0));
	vars.insert(pair<string, double>("c", 0));

	ParserTree par(lex, vars);

	for (double c = 0; c < 1; c += 1.0 / 200 / 150)
	{
		vars["c"] = c;
		vars["x"] = 0;
		for (int i = 0; i < 10; i++)
		{
			vars["x"] = par.execute();
		}
	}
}

void TestParser()
{
	printf("\nStarting tests for Parser:\n");

	TEST_FUNCTION_RUN(parse_simple);
	TEST_FUNCTION_RUN(code_line);
	TEST_FUNCTION_RUN_TIMING(fractal);
}
