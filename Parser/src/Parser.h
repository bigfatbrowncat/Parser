#ifndef PARSER_H_
#define PARSER_H_

#include <list>
#include <map>
#include <string>
#include <complex>
#include <stdlib.h>
#include <math.h>

#include "Lexer.h"

using namespace std;

template <typename T> class ParserTree;
template <typename T> class ParserValue;

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

template<typename T> class Converter
{
public:
	virtual T fromString(string str) const = 0;
	virtual ~Converter() {}
};

class DoubleConverter : public Converter<double>
{
public:
	virtual double fromString(string str) const { return atof(str.c_str()); }
	virtual ~DoubleConverter() {}
};

typedef complex<double> d_complex;

class ComplexConverter : public Converter<d_complex>
{
public:
	virtual d_complex fromString(string str) const { return atof(str.c_str()); }
	virtual ~ComplexConverter() {}
};

template <typename T> class CodePosition
{
private:
	CodePositionType type;
	union
	{
		ParserOperation operation;
		ParserValue<T>* value;
	} content;
public:
	CodePosition() {}
	CodePositionType getType() const
	{
		return type;
	}

	const ParserOperation getOperation() const
	{
		return content.operation;
	}
	const ParserValue<T>& getValue() const
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
	static CodePosition<T> withValue(ParserValue<T>& value)
	{
		CodePosition cp;
		cp.type = CPT_VALUE;
		cp.content.value = &value;
		return cp;
	}
};

template <typename T> class ParserValue
{
protected:
public:
	virtual void pushToCodeString(list<CodePosition<T> >& code)
	{
		code.push_back(CodePosition<T>::withValue(*this));
	}
	virtual T getValue() const = 0;
	virtual ~ParserValue() {}
};

template <typename T> class ParserConstant : ParserValue<T>
{
	friend class ParserTree<T>;
protected:
	T value;
public:
	ParserConstant() {}
	virtual T getValue() const
	{
		return value;
	}
	virtual ~ParserConstant() {}
};

template <typename T> class ParserVariable : ParserValue<T>
{
	friend class ParserTree<T>;
protected:
	string name;
	T value;
public:
	ParserVariable(string name) : name(name) { }
	virtual T getValue() const
	{
		return value;
	}
	void setValue(T value)
	{
		this->value = value;
	}
	string getName()
	{
		return name;
	}
	virtual ~ParserVariable() {}
};

template <typename T> class ParserItem : ParserValue<T>
{
	friend class ParserTree<T>;
protected:
	list<ParserValue<T>*> innerItems;
	list<ParserOperation> innerOperations;
	ParserItem() {}
public:
	virtual void pushToCodeString(list<CodePosition<T> >& code);
	const list<ParserValue<T>*>& getInnerItems() const { return innerItems; }
	const list<ParserOperation>& getInnerOperations() const { return innerOperations; }
	virtual T getValue() const
	{
		// TODO: implement this
		return 0;
	}
	virtual ~ParserItem();
};

template <typename T> class ParserTree
{
private:
	const Converter<T>& converter;
	list<CodePosition<T> > code;
	ParserValue<T>* root;
protected:
	map<string, ParserVariable<T>*> variables;
	ParserValue<T>* createParserValue(const LexerTreeItem& ltr);
	void compile();
public:
	T execute();
	const list<CodePosition<T> >& getCode() const { return code; }
	ParserTree(const LexerTree& lexerTree, const Converter<T>& converter);
	~ParserTree();
	const ParserValue<T>& getRoot() const { return *root; }
	ParserVariable<T>& getVariable(string name) { return *(variables[name]); }
};

template <typename T> ParserItem<T>::~ParserItem()
{
    for (typename list<ParserValue<T>*>::iterator iter = innerItems.begin(); iter != innerItems.end(); iter++)
            delete (*iter);
}

template <typename T> ParserTree<T>::ParserTree(const LexerTree& lexerTree, const Converter<T>& converter) : converter(converter)
{
	root = createParserValue(lexerTree.getRoot());
	compile();
}
template <typename T> ParserTree<T>::~ParserTree()
{
	delete root;
}

template <typename T> ParserValue<T>* ParserTree<T>::createParserValue(const LexerTreeItem& ltr)
{
	const list<LexerTreeItem>& lexerItems = ltr.getInnerItems();
	if (lexerItems.size() == 0)
	{
		if (ltr.getInnerText()[0] >= '0' && ltr.getInnerText()[0] <= '9')
		{
			ParserConstant<T>* res = new ParserConstant<T>;
			res->value = converter.fromString(ltr.getInnerText());
			return res;
		}
		else
		{
			ParserVariable<T>* res = new ParserVariable<T>(ltr.getInnerText());
			variables.insert(pair<string, ParserVariable<T>*>(res->name, res));
			return res;
		}
	}
	else if (lexerItems.size() == 1)
	{
		return createParserValue(lexerItems.back());
	}
	else
	{
		ParserItem<T>* res = new ParserItem<T>;
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

template <typename T> void ParserTree<T>::compile()
{
	code.clear();
	root->pushToCodeString(code);
}

template <typename T> T ParserTree<T>::execute()
{
	T valueStack[256];
	int stackTop = 0;
	for (typename list<CodePosition<T> >::const_iterator iter = code.begin(); iter != code.end(); iter++)
	{
		if ((*iter).getType() == CPT_VALUE)
		{
			valueStack[stackTop] = (*iter).getValue().getValue();
			stackTop ++;
		}
		else if ((*iter).getType() == CPT_OPERATION)
		{
			T b = valueStack[stackTop - 1];
			T a = valueStack[stackTop - 2];
			stackTop -= 2;

			ParserOperation pop = (*iter).getOperation();

			if (pop == PO_ADD)
			{
				valueStack[stackTop] = a + b;
				stackTop++;
			}
			else if (pop == PO_SUBTRACT)
			{
				valueStack[stackTop] = a - b;
				stackTop++;
			}
			else if (pop == PO_MULTIPLY)
			{
				valueStack[stackTop] = a * b;
				stackTop++;
			}
			else if (pop == PO_DIVIDE)
			{
				valueStack[stackTop] = a / b;
				stackTop++;
			}
			else if (pop == PO_POWER)
			{
				valueStack[stackTop] = pow(a, b);
				stackTop++;
			}
		}
	}
	return valueStack[stackTop];
}

template <typename T> void ParserItem<T>::pushToCodeString(list<CodePosition<T> >& code)
{
	list<ParserOperation> opstack;

	list<ParserOperation>::const_iterator op_iter = innerOperations.begin();
	typename list<ParserValue<T>*>::const_iterator itm_iter = innerItems.begin();

	for (int i = 0; i < this->innerOperations.size(); i++)
	{
		// Pushing the next operand to the code line
		(*itm_iter)->pushToCodeString(code);

		// Poping the operators from stack if they have higher priority
		while (opstack.size() > 0 && operationPriority[*op_iter] <= operationPriority[opstack.back()])
		{
			code.push_back(CodePosition<T>::withOperation(opstack.back()));
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
		code.push_back(CodePosition<T>::withOperation(opstack.back()));
		opstack.pop_back();
	}
}
#endif /* PARSER_H_ */
