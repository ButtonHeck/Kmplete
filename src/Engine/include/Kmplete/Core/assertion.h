#pragma once

#include "kmplete_export.h"
#include "Kmplete/Core/macro.h"
#include "Kmplete/Utils/string_utils.h"

#include <string>
#include <sstream>
#include <source_location>

#ifndef KMPLETE_ASSERTS_ACTIVE
    #ifdef NDEBUG
        #define KMPLETE_ASSERTS_ACTIVE false
    #else
        #define KMPLETE_ASSERTS_ACTIVE true
    #endif
#endif

namespace Kmplete
{
    class KMPLETE_API Assertion
    {
    public:
        enum class Consequence
        {
            Log,
            Terminate,
            Exception
        };

    public:
        Assertion(const std::string& expression, const std::source_location& location, Consequence consequence = Consequence::Terminate);
        ~Assertion();

        Assertion& Message(const std::string& message);

        template<typename T>
        Assertion& Watch(T&& val, const char* name)
        {
            Utils::ToSStream(_stream, "\n\t", name, " = ", std::forward<T>(val));
            return *this;
        }

        [[noreturn]]
        void Exception();

    private:
        const std::source_location _sourceLocation;
        Consequence _consequence;
        std::ostringstream _stream;
    };
    //--------------------------------------------------------------------------
}

#define KMPLETE_ASSERT(expr) (!KMPLETE_ASSERTS_ACTIVE || bool(expr)) ? void(0) : (void)Kmplete::Assertion{ KMPLETE_M_STRINGIFY(expr), std::source_location::current() }
#define KMPLETE_CHECK(expr) bool(expr) ? void(0) : (void)Kmplete::Assertion{ KMPLETE_M_STRINGIFY(expr), std::source_location::current(), KMPLETE_ASSERTS_ACTIVE ? Kmplete::Assertion::Consequence::Terminate : Kmplete::Assertion::Consequence::Log }

#define KMPLETE_ASSERT_WATCH(...) KMPLETE_M_DISPATCH_VA(KMPLETE_AW_, __VA_ARGS__)
#define KMPLETE_AW_(expr) Watch((expr), KMPLETE_M_STRINGIFY(expr))
#define KMPLETE_AW_1_(a) KMPLETE_AW_(a)
#define KMPLETE_AW_2_(a, b) KMPLETE_AW_(a).KMPLETE_AW_(b)
#define KMPLETE_AW_3_(a, b, c) KMPLETE_AW_(a).KMPLETE_AW_(b).KMPLETE_AW_(c)
#define KMPLETE_AW_4_(a, b, c, d) KMPLETE_AW_(a).KMPLETE_AW_(b).KMPLETE_AW_(c).KMPLETE_AW_(d)
#define KMPLETE_AW_5_(a, b, c, d, e) KMPLETE_AW_(a).KMPLETE_AW_(b).KMPLETE_AW_(c).KMPLETE_AW_(d).KMPLETE_AW_(e)