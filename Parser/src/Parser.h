#ifndef PARSER_H_
#define PARSER_H_

#include <list>
#include <map>
#include <string>
#include <stdlib.h>

#include "Lexer.h"

using namespace std;

class ParserTree;
class ParserValue;

enum ParserOperation
{
	PO_ADD = 0,
	PO_SUBTRACT = 1,
	PO_MULTIPLY = 2,
	PO_DIVIDE = 3,
	PO_POWER = 4
};

static const int operationPriority[] =
{
	0,	// PO_ADD
	0,	// PO_SUBTRACT
	1,	// PO_MULTIPLY
	1,	// PO_DIVIDE
	2	// PO_POWER
};

enum CodePositionType
{
	CPT_VALUE, CPT_OPERATION
};

class CodePosition
{
private:
	CodePositionType type;
	union
	{
		ParserOperation operation;
		ParserValue* value;
	} content;
public:
	CodePosition() {}
	CodePositionType getType()
	{
		return type;
	}

	const ParserOperation getOperation()
	{
		return content.operation;
	}
	const ParserValue& getValue()
	{
		return *content.value;
	}
	static CodePosition withOperation(ParserOperation operation)
	{
		CodePosition cp;
		cp.type = CPT_OPERATION;
		cp.content.operation = operation;
		return cp;
	}
	static CodePosition withValue(ParserValue& value)
	{
		CodePosition cp;
		cp.type = CPT_VALUE;
		cp.content.value = &value;
		return cp;
	}
};

class ParserValue
{
protected:
public:
	virtual void pushToCodeString(list<CodePosition>& code)
	{
		code.push_back(CodePosition::withValue(*this));
	}
	virtual double getValue() const = 0;
	virtual ~ParserValue() {}
};

class ParserConstant : ParserValue
{
	friend class ParserTree;
protected:
	double value;
public:
	ParserConstant() {}
	virtual double getValue() const
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
	virtual double getValue() const
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
	virtual void pushToCodeString(list<CodePosition>& code);
	const list<ParserValue*>& getInnerItems() const { return innerItems; }
	const list<ParserOperation>& getInnerOperations() const { return innerOperations; }
	virtual double getValue() const
	{
		// TODO: implement this
		return 0;
	}
	virtual ~ParserItem();
};

class ParserTree
{
private:
	list<CodePosition> code;
	ParserValue* root;
protected:
	const map<string, double>& variableValues;
	ParserValue* createParserValue(const LexerTreeItem& ltr);
	void compile();
public:
	const list<CodePosition>& getCode() const { return code; }
	ParserTree(const LexerTree& lexerTree, const map<string, double>& variableValues);
	~ParserTree();
	const ParserValue& getRoot() const { return *root; }
};

#endif /* PARSER_H_ */
