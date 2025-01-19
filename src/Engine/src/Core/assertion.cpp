#if !defined (KMP_PRODUCTION_BUILD)

#include "Kmplete/Core/assertion.h"
#include "Kmplete/Core/log.h"

namespace Kmplete
{
    Assertion::Assertion(const std::string& expression, const std::string& file, const std::string& function, int line, Consequence consequence)
        : _file(file)
        , _function(function)
        , _line(line)
        , _consequence(consequence)
    {
        Utils::ToSStream(_stream, "Assertion failed! \"", expression, "\"");
    }
    //--------------------------------------------------------------------------

    Assertion::~Assertion()
    {
        if (_consequence == Consequence::Terminate)
        {
            KMP_LOG_CORE_CRITICAL("Assertion: [file {}, function {}, line {}]: {}", _file, _function, _line, _stream.str());
            std::terminate();
        }
        else
        {
            KMP_LOG_CORE_ERROR("Assertion: [file {}, function {}, line {}]: {}", _file, _function, _line, _stream.str());
        }
    }
    //--------------------------------------------------------------------------

    Assertion& Assertion::Message(const std::string& message)
    {
        Utils::ToSStream(_stream, "\n\tMessage: ", message);
        return *this;
    }
    //--------------------------------------------------------------------------
}
#endif