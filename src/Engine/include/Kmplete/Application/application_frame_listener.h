#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Event/event.h"

namespace Kmplete
{
    class ApplicationFrameListener
    {
    public:
        KMP_API explicit ApplicationFrameListener(const String& name);
        virtual ~ApplicationFrameListener() = default;

        KMP_API virtual void Update(KMP_MB_UNUSED float frameTimestep, KMP_MB_UNUSED bool applicationIsIconified) {}
        KMP_API virtual void Render() {}

        KMP_API virtual void OnEvent(Event&) {}

    protected:
        const String _name;
    };
    //--------------------------------------------------------------------------
}