#include "Parser.h"

ParserItem::~ParserItem()
{
    for (list<ParserValue*>::iterator iter = innerItems.begin(); iter != innerItems.end(); iter++)
            delete (*iter);
}

ParserTree::ParserTree(const LexerTree& lexerTree, const map<string, double>& variableValues) : variableValues(variableValues)
{
	root = createParserValue(lexerTree.getRoot());
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
				res->innerOperations.push_back(LO_ADD);
			else if ((*iter).getInnerText() == "-")
				res->innerOperations.push_back(LO_SUBTRACT);
			else if ((*iter).getInnerText() == "*")
				res->innerOperations.push_back(LO_MULTIPLY);
			else if ((*iter).getInnerText() == "/")
				res->innerOperations.push_back(LO_DIVIDE);
			else
				res->innerItems.push_back(createParserValue(*iter));
		}
		return res;
	}
}
