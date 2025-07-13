#include "Kmplete/Json/json_scope.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Utils/string_utils.h"
#include "Kmplete/Profile/profiler.h"

namespace Kmplete
{
    void JsonScope::Push(const String& entry)
    {
        KMP_PROFILE_FUNCTION();

        scope.push_back(entry);
        scopeString = Utils::StringVectorToString(scope, '/');
    }
    //--------------------------------------------------------------------------

    bool JsonScope::Pop()
    {
        KMP_PROFILE_FUNCTION();

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