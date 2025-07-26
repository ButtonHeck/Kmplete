#pragma once

#include <string>
#include <vector>
#include <filesystem>

namespace Kmplete
{
    template<typename T>
    using Vector = std::vector<T>;

    using String = std::string;
    using WString = std::wstring;
    using StringVector = Vector<String>;

    using Filepath = std::filesystem::path;
    using FilepathVector = Vector<Filepath>;
}
