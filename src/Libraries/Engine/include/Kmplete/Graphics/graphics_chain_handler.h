#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Graphics/graphics_chain_unit.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class KMP_API GraphicsChainHandler
        {
            KMP_DISABLE_COPY_MOVE(GraphicsChainHandler)
            KMP_LOG_CLASSNAME(GraphicsChainHandler)

        public:
            static constexpr auto PhysicalDeviceUnitSID = "PhysicalDevice"_sid;
            static constexpr auto LogicalDeviceUnitSID = "LogicalDevice"_sid;
            static constexpr auto SwapchainUnitSID = "Swapchain"_sid;
            static constexpr auto RendererUnitSID = "Renderer"_sid;

        public:
            GraphicsChainHandler() = default;
            ~GraphicsChainHandler() = default;

            bool AddHandler(Nullable<GraphicsChainUnit*> unit);
            KMP_NODISCARD bool HandleStartFrame(StringID unitSid, float frameTimestep);
            bool HandleEndFrame(StringID unitSid);

        private:
            StringIDHashMap<Nullable<GraphicsChainUnit*>> _units;
        };
        //--------------------------------------------------------------------------
    }
}