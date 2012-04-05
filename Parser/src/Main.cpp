#include <stdio.h>

#include <list>
#include <string>

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

class ParserItem : ParserValue
{
	friend class ParserTree;
protected:
	list<ParserItem*> innerItems;
	list<ParserOperation> innerOperations;
	ParserItem() {}
public:
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
	ParserValue* createParserValue(const LexerTreeItem& ltr)
	{
		const list<LexerTreeItem>& innerItems = ltr.getInnerItems();
		if (innerItems.size() == 0)
		{
			ParserConstant* res = new ParserConstant;
			res->value = 3;		// TODO: change to number parsing!
			return res;
		}
		else
		{
			ParserItem* res = new ParserItem;
			for (list<LexerTreeItem>::const_iterator iter = innerItems.begin();
			     iter != innerItems.end(); iter++)
			{
				// TODO: add all operands and operators
			}
		}
	}
};

ParserItem::~ParserItem()
{
	for (list<ParserItem*>::iterator iter = innerItems.begin(); iter != innerItems.end(); iter++)
		delete (*iter);

}

int main()
{
	printf("Main product here.\n");
	return 0;
}
