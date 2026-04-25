#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/type_traits.h"


namespace Kmplete
{
    //! Helper functions with common vectors operations
    namespace Utils
    {
        template<typename Value> requires(IsMoveConstructible<Value>::value && IsMoveAssignable<Value>::value)
        void MergeVectors(Vector<Value>& source, Vector<Value>& destination)
        {
            if (source.empty())
            {
                return;
            }

            destination.reserve(destination.size() + source.size());
            std::move(source.begin(), source.end(), std::back_inserter(destination));
            source.clear();
        }
        //--------------------------------------------------------------------------

        template<typename Value> requires(IsCopyConstructible<Value>::value && IsCopyAssignable<Value>::value)
        void AppendVectors(const Vector<Value>& source, Vector<Value>& destination)
        {
            if (source.empty())
            {
                return;
            }

            destination.reserve(destination.size() + source.size());
            std::copy(source.begin(), source.end(), std::back_inserter(destination));
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