#ifndef PARSER_H_
#define PARSER_H_

#include <list>
#include <map>
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
	const map<string, double>& variableValues;
	string name;
public:
	ParserVariable(const map<string, double>& variableValues) : variableValues(variableValues) {}
	virtual double getValue()
	{
		return (variableValues.find(name))->second;
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
	ParserValue* root;
protected:
	const map<string, double>& variableValues;
	ParserValue* createParserValue(const LexerTreeItem& ltr);

public:
	ParserTree(const LexerTree& lexerTree, const map<string, double>& variableValues);
	~ParserTree();
	const ParserValue& getRoot() const { return *root; }
};

#endif /* PARSER_H_ */
