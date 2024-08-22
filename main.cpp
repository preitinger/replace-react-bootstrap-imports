#include "files.hpp"

#include <regex>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>

std::string importLine(const std::string &identifier)
{
    return "import " + identifier + " from \"react-bootstrap/" + identifier + "\";";
}

std::string replaceImports(const std::string &s)
{
    const std::string pattern = "import[ \\t]+\\{[ \\t]*([A-Za-z][A-Za-z0-9_]*)(([ \\t]*,[ \\t]*([A-Za-z][A-Za-z0-9_]*))*)[ \\t]* \\}[ \\t]*from[ \\t]*\"react-bootstrap\"[;]";
    std::regex r(pattern);
    std::string result;

    std::smatch sm;
    std::regex_search(s, sm, r);
    std::string rest = s;
    const std::string pattern2 = "([A-Za-z][A-Za-z0-9_]*)";
    std::regex r2(pattern2);

    while (std::regex_search(rest, sm, r))
    {
        result += sm.prefix();
        std::vector<std::string> identifiers;
        identifiers.push_back(sm.str(1));
        std::string nested = sm.str(2);
        std::cout << "nested [" << nested << "]\n";
        std::smatch sm2;
        std::string rest2 = nested;
        while (std::regex_search(rest2, sm2, r2))
        {
            identifiers.push_back(sm2.str(0));
            rest2 = sm2.suffix();
        }

        for (auto i = identifiers.begin(); i != identifiers.end();)
        {
            result += importLine(*i);
            if (++i == identifiers.end())
            {
                break;
            }

            result += "\n";
        }
        // for (auto identifier : identifiers)
        // {
        //     result += importLine(identifier);
        //     std::cout << "identifier: [" << identifier << "]\n";
        // }

        rest = sm.suffix();
    }

    result += rest;
    return result;
}

void firstRegexTests()
{
    const std::string pattern = "import[ \\t]+\\{[ \\t]*([A-Za-z][A-Za-z0-9_]*)(([ \\t]*,[ \\t]*([A-Za-z][A-Za-z0-9_]*))*)[ \\t]* \\}[ \\t]*from[ \\t]*\"react-bootstrap\"";
    std::cout << "pattern:\n"
              << pattern << std::endl;
    std::regex r(pattern);
    const std::string testText = R"(import Accordion from "@/app/_lib/pr-client-utils/Accordion";
import AccordionPage from "@/app/_lib/pr-client-utils/AccordionPage";
import BootstrapAccordion from "./BootstrapAccordion";
import { Container, Form, FormControl, InputGroup } from "react-bootstrap";
import Button from "react-bootstrap/Button";
import { useState } from "react";
)";

    const std::string result = replaceImports(testText);
    std::cout << "result:\n"
              << result << "\nEND OF RESULT\n";
    // import { Container, Form, FormControl, InputGroup } from "react-bootstrap

    for (std::sregex_iterator i = std::sregex_iterator(testText.begin(), testText.end(), r); i != std::sregex_iterator(); ++i)
    {
        std::smatch match = *i;
        std::cout << "ready: " << match.ready() << "\n";
        std::cout << "str(): " << match.str() << std::endl;
        std::cout << "str(0): " << match.str(0) << "\n";
        std::cout << "str(1): " << match.str(1) << "\n";
        std::cout << "str(2): " << match.str(2) << "\n";
        std::cout << "str(3): " << match.str(3) << "\n";
        std::cout << "submatches:\n";
        std::cout << "match.length()" << match.length() << std::endl;
        std::cout << "match.size(): " << match.size() << std::endl;
        int c = 0;
        for (auto j = match.begin(); j != match.end(); ++j)
        {
            std::cout << "  submatch: " << *j << "\n";
            ++c;
        }

        std::cout << "c=" << c << std::endl;
    }

    // std::cout << "\n Now token iterator\n\n";
    // for (std::sregex_token_iterator i = std::sregex_token_iterator(testText.begin(), testText.end(), r); i != std::sregex_token_iterator(); ++i) {
    //     auto v = *i;
    //     v.
    //     std::cout << "submatch: " << v.str() << "\n";
    // }

    std::cout << "\n\npositioned sub-matches:\n\n";
    const std::sregex_token_iterator iend;

    for (std::sregex_token_iterator i = std::sregex_token_iterator(testText.begin(), testText.end(), r, 2); i != iend; ++i)
    {
        std::cout << "sub-match[2]: [" << *i << "]" << std::endl;
    }

    std::cout << "\n\nENDE\n\n"
              << std::endl;

    {
        std::cout << "\nregex_search\n\n";
        std::smatch res;
        std::regex_search(testText, res, r);
        std::cout << "size()" << res.size() << std::endl;
        auto sub = res[0];
    }
}

void myOp(const std::filesystem::path &p, uintmax_t fileSize)
{
    std::cout << "Should operate now " << p.generic_string() << "\n";
    std::cout << "fileSize: " << fileSize << std::endl;
    if (fileSize > 1000000)
    {
        throw "File size too large!";
    }

    char *buf = new char[fileSize + 1];

    {
        std::ifstream s(p);
        s.read(buf, fileSize);
    }
    buf[fileSize] = 0;

    std::string text(buf);
    delete[] buf;
    buf = nullptr;
    std::string replacedText = replaceImports(text);
    std::cout << "replaced:\n"
              << replacedText << "\n\n";

    {
        std::ofstream s(p);
        s.write(replacedText.c_str(), replacedText.length());
    }
}

int main(int argc, const char *argv[])
{
    // TODO scan tsx Files in current and its sub-directories and replace unefficient imports from "react-bootstrap" by specific imports like "react-bootstrap/Container"

    // For simplicity, we assume that source files have reasonable sizes so that having the complete file in a string in memory does not cause any problem.

    std::cout << "\nATTENTION!\nIt is strongly recommended to embed a call to " << argv[0] << " into a script which creates safety copies of all source files before the call itself.\n";
    std::cout << "Hit enter to continue.\n\n";
    {
        std::string dummyLine;
        std::getline(std::cin, dummyLine);
    }
    iterateOverSourceFiles(myOp);

    return 0;
}