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

#ifndef PARSY_STRUCTURE_H_
#define PARSY_STRUCTURE_H_

#include <vector>
#include <iostream>

#include "Tokens.h"
#include "ParserAST.h"

namespace parsy
{
    class ParserUnit;

    struct Symbol;
    struct Terminal;
    struct Nonterminal;

    class SymbolPattern;
}


class parsy::ParserUnit
{
    Nonterminal* start;
    std::vector<Nonterminal*> nonterminals;
    std::vector<Terminal*> terminals;
public:

    static ParserUnit* createUnit(DescriptionFile* file);

    Nonterminal* getNonterminal(const std::string& name);
    Terminal* getTerminal(const std::string& name);

    inline void addTerminal(Terminal* t) { terminals.push_back(t); }

    inline Symbol* getSymbol(const std::string& name)
    {
        return ((Symbol*) getTerminal(name)) ? :
               ((Symbol*) getNonterminal(name));
    }

    /*!
     * generates the parser code.
     *
     * \param header the stream to write the header to
     * \param output the stream to write the source file to
     * \param headerName the name to include from the source file
     */
    void generateParserCode(std::ostream& header, std::ostream& output,
            const std::string& headerName);
};


struct parsy::Symbol
{
    std::string name;
    TokenId tokenId;
    inline Symbol(const std::string& name) : name(name) {}

    inline const std::string& getName(void) const { return name; }
};


struct parsy::Terminal : public Symbol
{
    inline Terminal(const std::string& name) : Symbol(name) {}
};


struct parsy::Nonterminal : public Symbol
{
    inline Nonterminal(const std::string& name) : Symbol(name) {}
    std::vector<SymbolPattern*> patterns;
};


class parsy::SymbolPattern
{
public:
    std::vector<Symbol*> symbols;
    inline void addSymbol(Symbol* symbol) { symbols.push_back(symbol); }
};

#endif // PARSY_STRUCTURE_H_


