#pragma once

#include <string>
#include <vector>
#include <filesystem>

namespace Kmplete
{
    using String = std::string;
    using WString = std::wstring;
    using StringVector = std::vector<String>;
    using Path = std::filesystem::path;
    using PathVector = std::vector<Path>;
}
