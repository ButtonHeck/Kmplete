#pragma once

#define KMP_PROFILE_MANUAL_SET true
#if ((!defined KMP_BUILD_STATIC || KMP_PROFILE_MANUAL_SET) && !defined (KMP_CONFIG_TYPE_PRODUCTION)) || defined (KMP_CONFIG_TYPE_RELWITHDEBINFO)
#define KMP_PROFILE

#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/type_traits.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/macro.h"
#include "Kmplete/Log/log_class_macro.h"

#include <fstream>
#include <chrono>
#include <thread>
#include <mutex>


namespace Kmplete
{
    struct ProfileResult
    {
        String name;
        std::chrono::high_resolution_clock::time_point start;
        std::chrono::high_resolution_clock::time_point end;
        std::thread::id threadId;
    };
    static_assert(IsMoveConstructible<ProfileResult>::value);
    //--------------------------------------------------------------------------


    struct ProfilingSession
    {
        String name;
        int profilesCount;
    };
    //--------------------------------------------------------------------------


    class Profiler
    {
        KMP_LOG_CLASSNAME(Profiler)

    public:
        KMP_DISABLE_COPY_MOVE(Profiler)

        KMP_NODISCARD KMP_API static Profiler& Get();

        KMP_API void SetLevel(unsigned int level);
        KMP_NODISCARD KMP_API unsigned int GetLevel() const;

        KMP_API void SetActive(bool active);
        KMP_NODISCARD KMP_API bool IsActive() const;

        KMP_API void BeginSession(const String& name, const Filepath& filepath, int storageSize);
        KMP_API void EndSession();

    private:
        Profiler() noexcept;
        ~Profiler();

        void _BeginSessionInternal(const String& name, const Filepath& filepath, int storageSize);
        void _EndSessionInternal();

        void _WriteProfileHeader(std::ofstream& outputFileStream) const;
        void _WriteProfileResults(std::ofstream& outputFileStream) const;
        void _WriteProfileResultsToIntermediate() const;
        void _WriteProfileResultsFromIntermediate(std::ofstream& outputFileStream) const;
        void _WriteProfileFooter(std::ofstream& outputFileStream) const;
        Filepath _CreateIntermediateFilepath(int intermediateCount) const;
        void _BeginNewCycle();

    private:
        friend class ProfilerTimer;

    private:
        unsigned int _level;
        bool _active;
        std::mutex _mutex;
        UPtr<ProfilingSession> _currentSession;
        Filepath _outputFilepath;
        Vector<ProfileResult> _profileResults;
        int _storageSize;
        int _storeCycles;
    };
    //--------------------------------------------------------------------------


    class ProfilerTimer
    {
    public:
        KMP_DISABLE_COPY_MOVE(ProfilerTimer)

        KMP_API explicit ProfilerTimer(const char* name, unsigned int level = 0);
        KMP_API ~ProfilerTimer();

        KMP_API void SetName(const char* name);

    private:
        const char* _name;
        const bool _skip;
        std::chrono::high_resolution_clock::time_point _start;
    };
    //--------------------------------------------------------------------------


    namespace ProfilerUtils
    {
        template<size_t length>
        struct ReplaceResult
        {
            char data[length];
        };

        template<size_t lengthSrc, size_t lengthRemove, size_t lengthReplace>
        consteval auto ReplaceString(const char(&src)[lengthSrc], const char(&remove)[lengthRemove], const char(&replace)[lengthReplace])
        {
            ReplaceResult<lengthSrc> result = {};

            size_t srcIndex = 0;
            size_t dstIndex = 0;
            while (srcIndex < lengthSrc)
            {
                size_t matchIndex = 0;
                while ((matchIndex < lengthRemove - 1) && (srcIndex + matchIndex < lengthSrc - 1) && (src[srcIndex + matchIndex] == remove[matchIndex]))
                {
                    matchIndex++;
                }

                if (matchIndex == lengthRemove - 1)
                {
                    size_t replaceIndex = 0;
                    while (replaceIndex < lengthReplace - 1)
                    {
                        result.data[dstIndex++] = replace[replaceIndex++];
                    }

                    srcIndex += matchIndex;
                }
                else
                {
                    result.data[dstIndex++] = src[srcIndex];
                    srcIndex++;
                }
            }

            return result;
        }
    }
    //--------------------------------------------------------------------------
}

#define KMP_PROFILE_BEGIN_SESSION(name, filepath, storageSize) ::Kmplete::Profiler::Get().BeginSession(name, filepath, storageSize)
#define KMP_PROFILE_END_SESSION() ::Kmplete::Profiler::Get().EndSession()

#define _KMP_PROFILE_SCOPE_LINE2(name, line, level) \
    constexpr auto fixedNameCdecl##line      = ::Kmplete::ProfilerUtils::ReplaceString(name, "__cdecl ", "");\
    constexpr auto fixedNameKmplete##line    = ::Kmplete::ProfilerUtils::ReplaceString(fixedNameCdecl##line.data, "Kmplete::", "");\
    constexpr auto fixedNameUPtr##line       = ::Kmplete::ProfilerUtils::ReplaceString(fixedNameKmplete##line.data, "std::unique_ptr", "UPtr");\
    constexpr auto fixedNamePtr##line        = ::Kmplete::ProfilerUtils::ReplaceString(fixedNameUPtr##line.data, "std::shared_ptr", "Ptr");\
    constexpr auto fixedNameString##line     = ::Kmplete::ProfilerUtils::ReplaceString(fixedNamePtr##line.data, "std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >", "String");\
    constexpr auto fixedNameWString##line    = ::Kmplete::ProfilerUtils::ReplaceString(fixedNameString##line.data, "std::basic_string<wchar_t,struct std::char_traits<wchar_t>,class std::allocator<wchar_t> >", "WString");\
    constexpr auto fixedNameInt64##line      = ::Kmplete::ProfilerUtils::ReplaceString(fixedNameWString##line.data, "__int64", "int64");\
    constexpr auto fixedNamePath##line       = ::Kmplete::ProfilerUtils::ReplaceString(fixedNameInt64##line.data, "std::filesystem::path", "Filepath");\
    constexpr auto fixedNameClass##line      = ::Kmplete::ProfilerUtils::ReplaceString(fixedNamePath##line.data, "class ", "");\
    constexpr auto fixedNameStruct##line     = ::Kmplete::ProfilerUtils::ReplaceString(fixedNameClass##line.data, "struct ", "");\
    constexpr auto fixedNameVector##line     = ::Kmplete::ProfilerUtils::ReplaceString(fixedNameStruct##line.data, "std::vector", "Vector");\
    constexpr auto fixedNameVoidParams##line = ::Kmplete::ProfilerUtils::ReplaceString(fixedNameVector##line.data, "(void)", "()");\
    constexpr auto fixedNameRapidjson##line  = ::Kmplete::ProfilerUtils::ReplaceString(fixedNameVoidParams##line.data, "rapidjson::GenericDocument<rapidjson::UTF8<char>,rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>,rapidjson::CrtAllocator>", "rapidjson::Document");\
    ::Kmplete::ProfilerTimer timer##line(fixedNameRapidjson##line.data, level)
#define _KMP_PROFILE_SCOPE_LINE(name, line, level) _KMP_PROFILE_SCOPE_LINE2(name, line, level)
#define KMP_PROFILE_SCOPE(name, level) _KMP_PROFILE_SCOPE_LINE(name, __LINE__, level)
#define KMP_PROFILE_FUNCTION(level) KMP_PROFILE_SCOPE(KMP_FUNC_SIG, level)

#define KMP_PROFILE_CONSTRUCTOR_DECLARE() \
    private:\
    UPtr<ProfilerTimer> _constructorProfilerTimer;

#define KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS() \
    _constructorProfilerTimer(CreateUPtr<ProfilerTimer>("")) ,

#define KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS_NO_INIT_LIST() \
    _constructorProfilerTimer(CreateUPtr<ProfilerTimer>(""))

#define KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS() \
    , _constructorProfilerTimer(CreateUPtr<ProfilerTimer>(""))

#define KMP_PROFILE_CONSTRUCTOR_END() \
    constexpr auto fixedNameCdecl##line      = ::Kmplete::ProfilerUtils::ReplaceString(KMP_FUNC_SIG, "__cdecl ", "");\
    constexpr auto fixedNameKmplete##line    = ::Kmplete::ProfilerUtils::ReplaceString(fixedNameCdecl##line.data, "Kmplete::", "");\
    constexpr auto fixedNameUPtr##line       = ::Kmplete::ProfilerUtils::ReplaceString(fixedNameKmplete##line.data, "std::unique_ptr", "UPtr");\
    constexpr auto fixedNamePtr##line        = ::Kmplete::ProfilerUtils::ReplaceString(fixedNameUPtr##line.data, "std::shared_ptr", "Ptr");\
    constexpr auto fixedNameString##line     = ::Kmplete::ProfilerUtils::ReplaceString(fixedNamePtr##line.data, "std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >", "String");\
    constexpr auto fixedNameWString##line    = ::Kmplete::ProfilerUtils::ReplaceString(fixedNameString##line.data, "std::basic_string<wchar_t,struct std::char_traits<wchar_t>,class std::allocator<wchar_t> >", "WString");\
    constexpr auto fixedNameInt64##line      = ::Kmplete::ProfilerUtils::ReplaceString(fixedNameWString##line.data, "__int64", "int64");\
    constexpr auto fixedNamePath##line       = ::Kmplete::ProfilerUtils::ReplaceString(fixedNameInt64##line.data, "std::filesystem::path", "Filepath");\
    constexpr auto fixedNameClass##line      = ::Kmplete::ProfilerUtils::ReplaceString(fixedNamePath##line.data, "class ", "");\
    constexpr auto fixedNameStruct##line     = ::Kmplete::ProfilerUtils::ReplaceString(fixedNameClass##line.data, "struct ", "");\
    constexpr auto fixedNameVector##line     = ::Kmplete::ProfilerUtils::ReplaceString(fixedNameStruct##line.data, "std::vector", "Vector");\
    constexpr auto fixedNameVoidParams##line = ::Kmplete::ProfilerUtils::ReplaceString(fixedNameVector##line.data, "(void)", "()");\
    constexpr auto fixedNameRapidjson##line  = ::Kmplete::ProfilerUtils::ReplaceString(fixedNameVoidParams##line.data, "rapidjson::GenericDocument<rapidjson::UTF8<char>,rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>,rapidjson::CrtAllocator>", "rapidjson::Document");\
    _constructorProfilerTimer->SetName(fixedNameRapidjson##line.data);\
    _constructorProfilerTimer.reset(nullptr);

#else
#define KMP_PROFILE_BEGIN_SESSION(name, filepath, storageSize)
#define KMP_PROFILE_END_SESSION()

#define KMP_PROFILE_SCOPE(name, level)
#define KMP_PROFILE_FUNCTION(level)

#define KMP_PROFILE_CONSTRUCTOR_DECLARE()
#define KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
#define KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS_NO_INIT_LIST()
#define KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS()
#define KMP_PROFILE_CONSTRUCTOR_END()
#endif


namespace Kmplete
{
    enum ProfileLevel : unsigned int
    {
        ProfileLevelAlways = 0,
        ProfileLevelImportantFunctions = 1,
        ProfileLevelImportantFunctionsVerbose = 2,
        ProfileLevelMinorFunctions = 3,
        ProfileLevelMinorFunctionsVerbose = 4
    };
    //--------------------------------------------------------------------------
}