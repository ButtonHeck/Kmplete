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
            Log::CoreCritical("Assertion: [file {}, function {}, line {}]: {}", _file, _function, _line, _stream.str());
            std::terminate();
        }
        else
        {
            Log::CoreError("Assertion: [file {}, function {}, line {}]: {}", _file, _function, _line, _stream.str());
        }
    }
    //--------------------------------------------------------------------------

    Assertion& Assertion::Message(const std::string& message)
    {
        Utils::ToSStream(_stream, "\n\tMessage: ", message);
        return *this;
    }
    //--------------------------------------------------------------------------

    void Assertion::Exception()
    {
        _consequence = Consequence::Exception;
        throw std::exception();
    }
    //--------------------------------------------------------------------------
}