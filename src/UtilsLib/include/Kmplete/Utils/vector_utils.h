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

            std::move(source.begin(), source.end(), std::inserter(destination, destination.end()));
        }
        //--------------------------------------------------------------------------
    }
}