#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Event/event.h"


namespace Kmplete
{
    class ApplicationFrameListener
    {
    public:
        KMP_API explicit ApplicationFrameListener(const String& name) noexcept;
        virtual ~ApplicationFrameListener() = default;

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