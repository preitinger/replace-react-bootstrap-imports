#include <vector>
#include <filesystem>

using SourceOperator = void(const std::filesystem::path &p, uintmax_t fileSize);
void iterateOverSourceFiles(const SourceOperator &op);
