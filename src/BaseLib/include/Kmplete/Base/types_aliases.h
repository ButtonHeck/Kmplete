#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <array>


namespace Kmplete
{
    //! Codestyle-friendly aliases for commonly used types

    template<typename T>
    using Vector = std::vector<T>;

    template<typename Key, typename Value, typename Hasher = std::hash<Key>>
    using HashMap = std::unordered_map<Key, Value, Hasher>;

    template<typename Value, typename Hasher = std::hash<Value>>
    using HashSet = std::unordered_set<Value, Hasher>;

    template<typename Key, typename Value, typename Comparator = std::less<Key>>
    using Map = std::map<Key, Value, Comparator>;

    template<typename Value, typename Comparator = std::less<Value>>
    using Set = std::set<Value, Comparator>;

    template<typename Value, size_t Size>
    using Array = std::array<Value, Size>;

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

    using BinaryBuffer = Vector<UByte>;
}
