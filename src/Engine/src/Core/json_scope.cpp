#include "Kmplete/Core/json_scope.h"
#include "Kmplete/Core/log.h"
#include "Kmplete/Utils/string_utils.h"

namespace Kmplete
{
    void JsonScope::Push(const std::string& entry)
    {
        scope.push_back(entry);
        scopeString = Utils::StringVectorToString(scope, '/');
    }
    //--------------------------------------------------------------------------

    bool JsonScope::Pop()
    {
        if (!scope.empty())
        {
            scope.pop_back();
            scopeString = Utils::StringVectorToString(scope, '/');
            return true;
        }

        Log::CoreError("JsonScope: cannot pop from empty scope");
        return false;
    }
    //--------------------------------------------------------------------------
}