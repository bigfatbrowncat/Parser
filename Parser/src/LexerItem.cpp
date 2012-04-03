/*
 * LexerItem.cpp
 *
 *  Created on: Apr 2, 2012
 *      Author: imizus
 */

#include "LexerItem.h"

#include <list>
#include <string>

using namespace std;

void LexerItem::doLexing(list<LexerItem*>& nextIteration)
{
	enum state { ident, oper, number, whitespace, inbraces } curState = whitespace;
	class braces_stack
	{
	private:
		list<brace> braces;
	public:
		void process(char ch)
		{
			if (ch == '(') braces.push_back(round);
			else if (ch == '[') braces.push_back(square);
			else if (ch == '{') braces.push_back(curly);
			else if (ch == ')')
			{
				if (braces.back() == round)
					braces.pop_back();
				else
					throw ERROR_LEXER_UNEXPECTED_CLOSING_BRACE;
			}
			else if (ch == ']')
			{
				if (braces.back() == square)
					braces.pop_back();
				else
					throw ERROR_LEXER_UNEXPECTED_CLOSING_BRACE;
			}
			else if (ch == '}')
			{
				if (braces.back() == curly)
					braces.pop_back();
				else
					throw ERROR_LEXER_UNEXPECTED_CLOSING_BRACE;
			}
		}

		bool thereAreOpened() { return braces.size() > 0; }
	} braces;

	string curItemText;

	unsigned int i = 0;
	while (i < innerText.length())
	{
		braces.process(innerText[i]);

		// Reading the next character
		if (curState == whitespace)
		{
			if (braces.thereAreOpened())
			{
				curState = inbraces;
				curItemText = innerText[i];
			}
			else if (isWhitespace(innerText[i]))
			{
				// do nothing, just skip the whitespace
			}
			else if (isDigit(innerText[i]))
			{
				curState = number;
				curItemText = innerText[i];
			}
			else if (isLetter(innerText[i]) || innerText[i] == '_')
			{
				curState = ident;
				curItemText = innerText[i];
			}
			else if (isOperator(innerText[i]))
			{
				curState = oper;
				curItemText = innerText[i];
			}
			else
				throw ERROR_LEXER_UNEXPECTED_CHAR;
		}
		else if (curState == ident)
		{
			if (braces.thereAreOpened())
			{
				// Braces opened. Saving current item
				LexerItem li(curItemText);
				innerItems.push_back(li);
				nextIteration.push_back(&(innerItems.back()));

				curState = inbraces;
				// Adding first brace char to next item
				curItemText = innerText[i];
			}
			else if (isLetter(innerText[i]) || isDigit(innerText[i]) || innerText[i] == '_')
			{
				// Still identifier
				curItemText += innerText[i];
			}
			else if (isOperator(innerText[i]))
			{
				// Changed to operator. Saving current item
				LexerItem li(curItemText);
				innerItems.push_back(li);
				nextIteration.push_back(&(innerItems.back()));

				curState = oper;
				// Adding first operator char to next item
				curItemText = innerText[i];
			}
			else if (isWhitespace(innerText[i]))
			{
				// Identifier ended. Saving it
				LexerItem li(curItemText);
				innerItems.push_back(li);
				nextIteration.push_back(&(innerItems.back()));

				curState = whitespace;
				// Clearing current item text
				curItemText = "";
			}
			else
				throw ERROR_LEXER_UNEXPECTED_CHAR;
		}
		else if (curState == oper)
		{
			if (braces.thereAreOpened())
			{
				// Braces opened. Saving current item
				LexerItem li(curItemText);
				innerItems.push_back(li);
				nextIteration.push_back(&(innerItems.back()));

				curState = inbraces;
				// Adding first brace char to next item
				curItemText = innerText[i];
			}
			else if (isOperator(innerText[i]))
			{
				// Still operator
				curItemText += innerText[i];
			}
			else if (isLetter(innerText[i]) || innerText[i] == '_')
			{
				// Changed to identifier. Saving current item
				LexerItem li(curItemText);
				innerItems.push_back(li);
				nextIteration.push_back(&(innerItems.back()));

				curState = ident;
				// Adding first operator char to next item
				curItemText = innerText[i];
			}
			else if (isDigit(innerText[i]))
			{
				// Changed to number. Saving current item
				LexerItem li(curItemText);
				innerItems.push_back(li);
				nextIteration.push_back(&(innerItems.back()));

				curState = number;
				// Adding first operator char to next item
				curItemText = innerText[i];
			}
			else if (isWhitespace(innerText[i]))
			{
				// Identifier ended. Saving it
				LexerItem li(curItemText);
				innerItems.push_back(li);
				nextIteration.push_back(&(innerItems.back()));

				curState = whitespace;
				// Clearing current item text
				curItemText = "";
			}
			else
				throw ERROR_LEXER_UNEXPECTED_CHAR;
		}
		else if (curState == number)
		{
			if (braces.thereAreOpened())
			{
				// Braces opened. Saving current item
				LexerItem li(curItemText);
				innerItems.push_back(li);
				nextIteration.push_back(&(innerItems.back()));

				curState = inbraces;
				// Adding first brace char to next item
				curItemText = innerText[i];
			}
			else if (isDigit(innerText[i]) || innerText[i] == '.')
			{
				// Still number
				curItemText += innerText[i];
			}
			else if (isOperator(innerText[i]))
			{
				// Changed to operator. Saving current item
				LexerItem li(curItemText);
				innerItems.push_back(li);
				nextIteration.push_back(&(innerItems.back()));

				curState = oper;
				// Adding first operator char to next item
				curItemText = innerText[i];
			}
			else if (isWhitespace(innerText[i]))
			{
				// Number ended. Saving it
				LexerItem li(curItemText);
				innerItems.push_back(li);
				nextIteration.push_back(&(innerItems.back()));

				curState = whitespace;
				// Clearing current item text
				curItemText = "";
			}
			else
				throw ERROR_LEXER_UNEXPECTED_CHAR;
		}
		else if (curState == inbraces)
		{
			curItemText += innerText[i];

			if (!braces.thereAreOpened())
			{
				// Braces closed. Saving current item
				LexerItem li(curItemText);
				innerItems.push_back(li);
				nextIteration.push_back(&(innerItems.back()));

				curState = whitespace;
				curItemText = "";
			}
		}

		i++;
	}

	// Saving the last lexer item
	if (curItemText != "")
	{
		LexerItem li(curItemText);
		innerItems.push_back(li);
		nextIteration.push_back(&(innerItems.back()));
	}
}
