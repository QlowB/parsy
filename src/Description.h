#ifndef PARSY_DESCRIPTION_H_
#define PARSY_DESCRIPTION_H_

#include <string>
#include <vector>

namespace parsy
{
    struct DescriptionFile;

    /*!
     * base class for terminals and nonterminals
     */
    struct MinalBase;
    struct Terminal;
    struct Nonterminal;

    struct Pattern;
}


struct parsy::DescriptionFile
{
    std::vector<Nonterminal*> nonterminals;
};


struct parsy::MinalBase
{
    std::string name;
};


struct parsy::Terminal : public MinalBase
{
    int terminalId;
};


struct parsy::Nonterminal : public MinalBase
{
    std::vector<Pattern*> patterns;
};


struct parsy::Pattern
{
    std::vector<std::string> pieces;
};


#endif // PARSY_DESCRIPTION_H_

