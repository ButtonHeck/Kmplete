#pragma once

#if !defined (KMP_PRODUCTION_BUILD)

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/macro.h"
#include "Kmplete/Core/types.h"
#include "Kmplete/Utils/string_utils.h"

#include <sstream>

#if !defined (KMP_ASSERTS_ACTIVE)
    #if defined NDEBUG
        #define KMP_ASSERTS_ACTIVE false
    #else
        #define KMP_ASSERTS_ACTIVE true
    #endif
#endif

namespace Kmplete
{
    class Assertion
    {
    public:
        enum class Consequence
        {
            Log,
            Terminate
        };

    public:
        KMP_API Assertion(const String& expression, const String& file, const String& function, int line, Consequence consequence = Consequence::Terminate);
        KMP_API ~Assertion();

        KMP_API Assertion& Message(const String& message);

        template<typename T>
        Assertion& Watch(T&& val, const char* name)
        {
            Utils::ToSStream(_stream, "\n\t", name, " = ", std::forward<T>(val));
            return *this;
        }

    private:
        const String _file;
        const String _function;
        const int _line;
        Consequence _consequence;
        std::ostringstream _stream;
    };
    //--------------------------------------------------------------------------
}

#define KMP_ASSERT(expr) (!KMP_ASSERTS_ACTIVE || bool(expr)) \
    ? (void)0 \
    : (void)Kmplete::Assertion{ KMP_M_STRINGIFY(expr), __FILE__, __FUNCTION__, __LINE__ }

#define KMP_CHECK(expr) bool(expr) \
    ? (void)0 \
    : (void)Kmplete::Assertion{ KMP_M_STRINGIFY(expr), __FILE__, __FUNCTION__, __LINE__, \
        KMP_ASSERTS_ACTIVE \
        ? Kmplete::Assertion::Consequence::Terminate \
        : Kmplete::Assertion::Consequence::Log }

#define KMP_ASSERT_WATCH(...) KMP_M_DISPATCH_VA(KMP_AW_, __VA_ARGS__)
#define KMP_AW_(expr) Watch((expr), KMP_M_STRINGIFY(expr))
#define KMP_AW_1_(a) KMP_AW_(a)
#define KMP_AW_2_(a, b) KMP_AW_(a).KMP_AW_(b)
#define KMP_AW_3_(a, b, c) KMP_AW_(a).KMP_AW_(b).KMP_AW_(c)
#define KMP_AW_4_(a, b, c, d) KMP_AW_(a).KMP_AW_(b).KMP_AW_(c).KMP_AW_(d)
#define KMP_AW_5_(a, b, c, d, e) KMP_AW_(a).KMP_AW_(b).KMP_AW_(c).KMP_AW_(d).KMP_AW_(e)

#else
#define KMP_ASSERT(expr)
#define KMP_CHECK(expr)
#define KMP_ASSERT_WATCH(...)
#endif