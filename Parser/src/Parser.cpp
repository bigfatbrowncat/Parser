#include "Parser.h"

ParserItem::~ParserItem()
{
    for (list<ParserValue*>::iterator iter = innerItems.begin(); iter != innerItems.end(); iter++)
            delete (*iter);
}

ParserTree::ParserTree(const LexerTree& lexerTree)
{
	root = createParserValue(lexerTree.getRoot());
}
ParserTree::~ParserTree()
{
	delete root;
}
