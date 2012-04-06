#ifndef PARSER_H_
#define PARSER_H_

#include <list>
#include <string>
#include <stdlib.h>

#include "Lexer.h"

using namespace std;

class ParserTree;

enum ParserOperation
{
	LO_ADD, LO_SUBTRACT, LO_MULTIPLY, LO_DIVIDE
};

class ParserValue
{
public:
	virtual double getValue() = 0;
	virtual ~ParserValue() {}
};

class ParserConstant : ParserValue
{
	friend class ParserTree;
protected:
	double value;
public:
	ParserConstant() {}
	virtual double getValue()
	{
		return value;
	}
};

class ParserVariable : ParserValue
{
	friend class ParserTree;
protected:
	string name;
public:
	ParserVariable() {}
	virtual double getValue()
	{
		// TODO: implement this
		return 0;
	}
	string getName()
	{
		return name;
	}
};

class ParserItem : ParserValue
{
	friend class ParserTree;
protected:
	list<ParserValue*> innerItems;
	list<ParserOperation> innerOperations;
	ParserItem() {}
public:
	const list<ParserValue*>& getInnerItems() const { return innerItems; }
	const list<ParserOperation>& getInnerOperations() const { return innerOperations; }
	virtual double getValue()
	{
		// TODO: implement this
		return 0;
	}
	virtual ~ParserItem();
};

class ParserTree
{
protected:
	static ParserValue* createParserValue(const LexerTreeItem& ltr)
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
				ParserVariable* res = new ParserVariable;
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
	ParserValue* root;

public:
	ParserTree(const LexerTree& lexerTree);
	~ParserTree();
	const ParserValue& getRoot() const { return *root; }
};

#endif /* PARSER_H_ */
