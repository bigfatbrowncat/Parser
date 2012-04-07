#include "Parser.h"

#include <list>
#include <math.h>
#include <stdio.h>

ParserItem::~ParserItem()
{
    for (list<ParserValue*>::iterator iter = innerItems.begin(); iter != innerItems.end(); iter++)
            delete (*iter);
}

ParserTree::ParserTree(const LexerTree& lexerTree, const map<string, double>& variableValues) : variableValues(variableValues)
{
	root = createParserValue(lexerTree.getRoot());
	compile();
}
ParserTree::~ParserTree()
{
	delete root;
}

ParserValue* ParserTree::createParserValue(const LexerTreeItem& ltr)
{
	const list<LexerTreeItem>& lexerItems = ltr.getInnerItems();
	if (lexerItems.size() == 0)
	{
		if (ltr.getInnerText()[0] >= '0' && ltr.getInnerText()[0] <= '9')
		{
			ParserConstant* res = new ParserConstant;
			res->value = atof(ltr.getInnerText().c_str());
			return res;
		}
		else
		{
			ParserVariable* res = new ParserVariable(variableValues);
			res->name = ltr.getInnerText();
			return res;
		}
	}
	else if (lexerItems.size() == 1)
	{
		return createParserValue(lexerItems.back());
	}
	else
	{
		ParserItem* res = new ParserItem;
		for (list<LexerTreeItem>::const_iterator iter = lexerItems.begin(); iter != lexerItems.end(); iter++)
		{
			// TODO Check the right order! For example, now it's correct to write "3 4 5 + -" and it would mean "3 + 4 - 5"
			// TODO Also check if braces are correct

			if ((*iter).getInnerText() == "+")
				res->innerOperations.push_back(PO_ADD);
			else if ((*iter).getInnerText() == "-")
				res->innerOperations.push_back(PO_SUBTRACT);
			else if ((*iter).getInnerText() == "*")
				res->innerOperations.push_back(PO_MULTIPLY);
			else if ((*iter).getInnerText() == "/")
				res->innerOperations.push_back(PO_DIVIDE);
			else if ((*iter).getInnerText() == "^")
				res->innerOperations.push_back(PO_POWER);
			else
				res->innerItems.push_back(createParserValue(*iter));
		}
		return res;
	}
}

void ParserTree::compile()
{
	code.clear();
	root->pushToCodeString(code);
}

double ParserTree::execute()
{
	list<double> valueStack;
	for (list<CodePosition>::const_iterator iter = code.begin(); iter != code.end(); iter++)
	{
		if ((*iter).getType() == CPT_VALUE)
		{
			valueStack.push_back((*iter).getValue().getValue());
		}
		else if ((*iter).getType() == CPT_OPERATION)
		{
			double b = valueStack.back();
			valueStack.pop_back();
			double a = valueStack.back();
			valueStack.pop_back();

			if ((*iter).getOperation() == PO_ADD) valueStack.push_back(a + b);
			else if ((*iter).getOperation() == PO_SUBTRACT) valueStack.push_back(a - b);
			else if ((*iter).getOperation() == PO_MULTIPLY) valueStack.push_back(a * b);
			else if ((*iter).getOperation() == PO_DIVIDE) valueStack.push_back(a / b);
			else if ((*iter).getOperation() == PO_POWER) valueStack.push_back(pow(a, b));
		}
	}
	return valueStack.back();
}

void ParserItem::pushToCodeString(list<CodePosition>& code)
{
	list<ParserOperation> opstack;

	list<ParserOperation>::const_iterator op_iter = innerOperations.begin();
	list<ParserValue*>::const_iterator itm_iter = innerItems.begin();

	for (int i = 0; i < this->innerOperations.size(); i++)
	{
		// Pushing the next operand to the code line
		(*itm_iter)->pushToCodeString(code);

		// Poping the operators from stack if they have higher priority
		while (opstack.size() > 0 && operationPriority[*op_iter] <= operationPriority[opstack.back()])
		{
			code.push_back(CodePosition::withOperation(opstack.back()));
			opstack.pop_back();
		}

		opstack.push_back(*op_iter);

		itm_iter++;
		op_iter++;
	}

	// Pushing the last operand to the code line
	(*itm_iter)->pushToCodeString(code);

	// Poping the remaining operators from stack
	while (opstack.size() > 0)
	{
		code.push_back(CodePosition::withOperation(opstack.back()));
		opstack.pop_back();
	}
}
