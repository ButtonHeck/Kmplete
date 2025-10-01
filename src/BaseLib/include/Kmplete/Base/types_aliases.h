#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <cstdint>


namespace Kmplete
{
    template<typename T>
    using Vector = std::vector<T>;

    using String = std::string;
    using WString = std::wstring;
    using StringVector = Vector<String>;

    using Filepath = std::filesystem::path;
    using FilepathVector = Vector<Filepath>;

    using UByte = uint8_t;
    using UInt8 = uint8_t;
    using UInt16 = uint16_t;
    using UInt32 = uint32_t;
    using UInt64 = uint64_t;

    using Int8 = int8_t;
    using Int16 = int16_t;
    using Int32 = int32_t;
    using Int64 = int64_t;
}
