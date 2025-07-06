#if !defined (KMP_CONFIG_TYPE_PRODUCTION)

#include "Kmplete/Core/assertion.h"
#include "Kmplete/Log/log.h"

namespace Kmplete
{
    Assertion::Assertion(const String& expression, const String& file, const String& function, int line, Consequence consequence /*= Consequence::Terminate*/)
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
            KMP_LOG_CRITICAL("Assertion: [file {}, function {}, line {}]: {}", _file, _function, _line, _stream.str());
            std::terminate();
        }
        else
        {
            KMP_LOG_ERROR("Assertion: [file {}, function {}, line {}]: {}", _file, _function, _line, _stream.str());
        }
    }
    //--------------------------------------------------------------------------

    Assertion& Assertion::Message(const String& message)
    {
        Utils::ToSStream(_stream, "\n\tMessage: ", message);
        return *this;
    }
    //--------------------------------------------------------------------------
}
#endif