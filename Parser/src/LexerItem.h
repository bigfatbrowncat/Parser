/*
 * LexerItem.h
 *
 *  Created on: Apr 2, 2012
 *      Author: imizus
 */

#ifndef LEXERITEM_H_
#define LEXERITEM_H_

#include <list>
#include <string>

using namespace std;

#define ERROR_LEXER_UNEXPECTED_CHAR		1

class LexerItem
{
private:
	string innerText;
	list<LexerItem> innerItems;

	bool isDigit(char ch)
	{
		return (ch >= '0' && ch <= '9');
	}

	bool isDot(char ch)
	{
		return ch == '.';
	}

	bool isLetter(char ch)
	{
		return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
	}

	bool isOperator(char ch)
	{
		return (ch == '+') || (ch == '-') || (ch == '*') || (ch == '/');
	}

	bool isWhitespace(char ch)
	{
		return (ch == ' ') || (ch == '\t') || (ch == '\r') || (ch == '\n');
	}

public:
	LexerItem(string innerText) : innerText(innerText) {}

	const string& getInnerText() const { return innerText; }
	const list<LexerItem>& getInnerItems() const { return innerItems; }

	void doLexing(list<LexerItem*>& nextIteration);
};


#endif /* LEXERITEM_H_ */
