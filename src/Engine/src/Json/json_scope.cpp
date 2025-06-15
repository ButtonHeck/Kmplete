#include "Kmplete/Json/json_scope.h"
#include "Kmplete/Core/log.h"
#include "Kmplete/Utils/string_utils.h"

namespace Kmplete
{
    void JsonScope::Push(const String& entry)
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

        KMP_LOG_ERROR("JsonScope: cannot pop from empty scope");
        return false;
    }
    //--------------------------------------------------------------------------
}