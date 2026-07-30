#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/string_id.h"


namespace Kmplete
{
    namespace Graphics
    {
        class GraphicsChainHandler;


        //TODO: comments
        class KMP_API GraphicsChainUnit
        {
            KMP_DISABLE_COPY_MOVE(GraphicsChainUnit)

        public:
            GraphicsChainUnit(GraphicsChainHandler& handler, StringID sid);
            virtual ~GraphicsChainUnit() = default;

        protected:
            friend class GraphicsChainHandler;

            KMP_NODISCARD virtual bool _StartFrame(float frameTimestep) = 0;
            virtual void _EndFrame() = 0;

        protected:
            GraphicsChainHandler& _handler;
            const StringID _sid;
        };
        //--------------------------------------------------------------------------
    }
}