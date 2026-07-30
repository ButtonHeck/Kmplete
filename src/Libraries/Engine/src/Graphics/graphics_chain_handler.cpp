#include "Kmplete/Graphics/graphics_chain_handler.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Core/assertion.h"


namespace Kmplete
{
    namespace Graphics
    {
        bool GraphicsChainHandler::AddHandler(Nullable<GraphicsChainUnit*> unit)
        {
            KMP_ASSERT(unit);

            if (_units.contains(unit->_sid))
            {
                KMP_LOG_WARN("already contains graphics unit with sid '{}'", unit->_sid);
                return false;
            }

            const auto [iterator, hasEmplaced] = _units.emplace(unit->_sid, unit);
            return hasEmplaced;
        }
        //--------------------------------------------------------------------------

        bool GraphicsChainHandler::HandleStartFrame(StringID unitSid, float frameTimestep)
        {
            if (not _units.contains(unitSid))
            {
                KMP_LOG_WARN("cannot handle starting frame for unit with sid '{}' - unit not found", unitSid);
                return false;
            }

            return _units[unitSid]->_StartFrame(frameTimestep);
        }
        //--------------------------------------------------------------------------

        bool GraphicsChainHandler::HandleEndFrame(StringID unitSid)
        {
            if (not _units.contains(unitSid))
            {
                KMP_LOG_WARN("cannot handle ending frame for unit with sid '{}' - unit not found", unitSid);
                return false;
            }

            _units[unitSid]->_EndFrame();
            return true;
        }
        //--------------------------------------------------------------------------
    }
}