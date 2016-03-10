/* ============================================================================
 *
 * This file is part of the parsy parser generator.
 *
 * Copyright (C) 2016 Nicolas Winkler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * ==========================================================================*/
#include "Structure.h"

using namespace parsy;

ParserUnit* ParserUnit::createUnit(DescriptionFile* file)
{
    ParserUnit* pu = new ParserUnit();
    for (size_t i = 0; i < file->rules.size(); i++) {
        Rule* r = file->rules[i];
        Nonterminal* nt = new Nonterminal(r->nonterminal);
        pu->nonterminals.push_back(nt);
    }

    for (size_t i = 0; i < file->rules.size(); i++) {
        Rule* r = file->rules[i];
        Nonterminal* nt = pu->nonterminals[i];
        for (size_t j = 0; j < r->patterns.size(); j++) {
            Pattern* pattern = r->patterns[j];
            SymbolPattern* sp = new SymbolPattern();
            nt->patterns.push_back(sp);
            for (auto k = pattern->pieces.begin();
                    k != pattern->pieces.end(); k++) {
                Symbol* s = pu->getSymbol(*k);
                if (s != nullptr) {
                    sp->addSymbol(s);
                }
                else {
                    Terminal* t = new Terminal(*k);
                    pu->addTerminal(t);
                    sp->addSymbol(t);
                }
            }
        }
    }
    return pu;
}


Nonterminal* ParserUnit::getNonterminal(const std::string& name)
{
    for (auto i = nonterminals.begin(); i != nonterminals.end(); i++) {
        if ((*i)->getName() == name) {
            return *i;
        }
    }
    return nullptr;
}


Terminal* ParserUnit::getTerminal(const std::string& name)
{
    for (auto i = terminals.begin(); i != terminals.end(); i++) {
        if ((*i)->getName() == name) {
            return *i;
        }
    }
    return nullptr;
}
 

void ParserUnit::generateParserCode(std::ostream& header, std::ostream& source)
{
    using namespace std;

#define TAB "\s\s\s\s"

    header <<
        "/// parsy-generated header file\n"
        "#include <vector>\n\n"
        "typedef unsigned int TokenId;\n\n";

    header <<
        "class Parser\n{\n"
        "    void parse(Tokenizer& tokenizer);\n"
        "};\n\n\n";

    header << "// Terminals\n";
    for (size_t i = 0; i < terminals.size(); i++) {
        source << "const TokenId " << terminals[i]->getName() <<
            "_id = " << i << ";\n";
    }

    header << "\n// Nonterminals\n";
    for (size_t i = 0; i < nonterminals.size(); i++) {
        source << "const TokenId " << nonterminals[i]->getName() <<
            "_id = " << i + terminals.size() << ";\n";
    }

    header << "\n\n"
        "struct StackElement\n{\n"
        "    TokenId tokenId;\n"
        "    AstNode* node;\n"
        "};\n\n\n";
    
    source <<
        "/// parsy-generated source file\n"
        "#include \"Parser.h\"\n";

    source <<
        "void Parser::parse(Tokenizer& tokenizer)\n{\n"
        "    std::vector<StackElement> stack;\n\n"
        "    while (!tokenizer.eof()) {\n"
        "        stack.push_back(tokenizer.getToken());\n"
        "        const size_t size = stack.size();\n";

    for (size_t i = 0; i < nonterminals.size(); i++) {
        Nonterminal* n = nonterminals[i];
        for (size_t j = 0; j < n->patterns.size(); j++) {
            SymbolPattern* p = n->patterns[j];
            source <<
                "\n        // check for " << n->getName() << "\n"
                "        if (size >= " << p->symbols.size() << " &&\n";
            // assert: p->symbols.size() > 0
            for (size_t k = p->symbols.size() - 1; k != ~0; k--) {
                Symbol* sym = p->symbols[k];
                source <<
                    "            stack[size - " << (p->symbols.size() - k) <<
                    "].tokenId == " << sym->getName() << "_id";
                if (k != 0)
                    source << " &&\n";
            } 

            source << ") {\n";
            source << "        }\n";
        }
    }

    source <<
        "    }\n"
        "}\n\n";

}


   
