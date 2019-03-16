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
#ifndef PARSY_PARSERAST_H_
#define PARSY_PARSERAST_H_

#include <string>
#include <vector>
#include <memory>
#include <utility>

namespace parsy
{
    struct DescriptionFile;

    struct TerminalRule;

    struct Rule;

    struct Pattern;
}


struct parsy::DescriptionFile
{
    std::vector<std::unique_ptr<TerminalRule>> terminalRules;
    std::vector<std::unique_ptr<Rule>> rules;
};


struct parsy::TerminalRule
{
    std::string terminal;
    std::string regex;
};


struct parsy::Rule
{
    std::string nonterminal;
    std::vector<std::unique_ptr<Pattern>> patterns;
};


struct parsy::Pattern
{
    std::vector<std::string> pieces;
};


#endif // PARSY_PARSERAST_H_ 

