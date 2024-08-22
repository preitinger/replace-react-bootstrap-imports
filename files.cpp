#include "files.hpp"

#include <iostream>

void iterateOverSourceFiles(const SourceOperator &op)
{
    std::filesystem::recursive_directory_iterator it(std::filesystem::current_path() / "app");
    const std::filesystem::recursive_directory_iterator end;

    for (; it != end; ++it)
    {
        auto &x = *it;
        auto &p = x.path(); 
        // std::cout << "path: " << p << "\n";
        // std::cout << "extension: " << p.extension() << "\n";
        // std::cout << "character_file: " << x.is_character_file() << "\n";
        // std::cout << "regular_file: " << x.is_regular_file() << "\n";
        if (x.is_regular_file() && p.extension() == ".tsx")
        {
            std::cout << "path: " << p << "\n";
            op(p, x.file_size());
        }
    }
}