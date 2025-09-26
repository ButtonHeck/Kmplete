#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Event/event.h"


namespace Kmplete
{
    class FrameListener
    {
    public:
        KMP_API explicit FrameListener(const String& name) noexcept;
        virtual ~FrameListener() = default;

        KMP_API virtual void Update(KMP_MB_UNUSED float frameTimestep, KMP_MB_UNUSED bool applicationIsIconified) {}
        KMP_API virtual void Render() {}
        KMP_API virtual void OnEvent(Event&) {}

        KMP_API void SetActive(bool active) noexcept;
        KMP_NODISCARD KMP_API bool IsActive() const noexcept;

    private:
        const String _name;
        bool _active;
    };
    //--------------------------------------------------------------------------
}