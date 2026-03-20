#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"


namespace Kmplete
{
    //! Helper functions with common vectors operations
    namespace Utils
    {
        template<typename Value>
        void MergeVectors(const Vector<Value>& source, Vector<Value>& destination)
        {
            if (source.empty())
            {
                return;
            }

            std::move(source.begin(), source.end(), std::back_inserter(destination));
        }
        //--------------------------------------------------------------------------

        template<typename Value>
        bool VectorContains(const Vector<Value>& vector, const Value& value)
        {
            return std::find(vector.cbegin(), vector.cend(), value) != vector.cend();
        }
        //--------------------------------------------------------------------------

        template<typename Value, typename Predicate>
        bool VectorContainsIf(const Vector<Value>& vector, Predicate predicate)
        {
            return std::find_if(vector.cbegin(), vector.cend(), predicate) != vector.cend();
        }
        //--------------------------------------------------------------------------
    }
}