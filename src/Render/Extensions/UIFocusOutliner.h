#pragma once
#include "../../StaticShared/Animator/Animator.h"
#include "../../StaticShared/Utils/Utils.h"
#include "../../Render/UIObject.h"
#include "../../Render/Prefabs/InputField.h"
#include "../../Render/Prefabs/Button.h"
#include <cmath>

class UIFocusOutliner final : public Updatable, public Singleton<UIFocusOutliner> {
    inline static UIObject* _focused = nullptr;
    inline static float _pulseTime = 0.0f;
    inline static Color _baseColor {};

public:
    static void Focus(UIObject* obj) {
        if (_focused == obj) return;

        // przywróć poprzedni
        if (_focused) {
            Animator::AnimateOutline(_focused, UIOBJECT_OUTLINE_SCALE, 0.15f);
            Animator::AnimateColor(_focused, _baseColor, 0.15f);
        }

        _focused = obj;
        if (!_focused) return;

        _baseColor = _focused->color;

        Animator::AnimateOutline(_focused, UIOBJECT_OUTLINE_SCALE * 2.0f, 0.15f);
        Animator::AnimateColor(_focused, Utils::Colors::LightenColor(_focused->color, 0.15f), 0.15f);
        _pulseTime = 0.0f;
    }

    static void Unfocus() {
        if (!_focused) return;
        Animator::AnimateOutline(_focused, UIOBJECT_OUTLINE_SCALE, 0.15f);
        Animator::AnimateColor(_focused, _baseColor, 0.15f);
        _focused = nullptr;
    }

    void Update() override {
        if (!_focused) return;

        _pulseTime += Utils::Time::GetDeltaTime();
        float pulse = 0.5f + 0.5f * std::sin(_pulseTime * 3.0f);

        _focused->outlineScale = UIOBJECT_OUTLINE_SCALE * (1.3f + 0.2f * pulse);
    }

    static UIObject* GetFocused() { return _focused; }

    static bool Focusable(UIObject* obj) {
        return dynamic_cast<Button*>(obj) != nullptr || dynamic_cast<InputField*>(obj) != nullptr;
    }
};
