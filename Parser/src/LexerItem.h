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

#define ERROR_LEXER_UNEXPECTED_CHAR					1
#define ERROR_LEXER_UNEXPECTED_CLOSING_BRACE		2

enum brace { none, round, square, curly };

class LexerItem
{
private:
	string innerText;
	list<LexerItem> innerItems;
	brace outerBraces;

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

	void extractBraces()
	{
		outerBraces = none;
		if (innerText.length() > 1)
		{
			if (innerText[0] == '(' && innerText[innerText.length() - 1] == ')')
			{
				outerBraces = round;
			}
			else if (innerText[0] == '[' && innerText[innerText.length() - 1] == ']')
			{
				outerBraces = square;
			}
			else if (innerText[0] == '{' && innerText[innerText.length() - 1] == '}')
			{
				outerBraces = curly;
			}
		}

		if (outerBraces != none)
		{
			innerText = innerText.substr(1, innerText.length() - 2);
		}
		this->innerText = innerText;

	}
public:
	LexerItem(string innerText) : innerText(innerText)
	{
		extractBraces();
	}

	const string& getInnerText() const { return innerText; }
	const list<LexerItem>& getInnerItems() const { return innerItems; }
	brace getOuterBraces() const { return outerBraces; }

	void doLexing(list<LexerItem*>& nextIteration);
};


#endif /* LEXERITEM_H_ */
