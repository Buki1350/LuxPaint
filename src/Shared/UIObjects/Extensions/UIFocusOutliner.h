#pragma once
#include "../../Animator/Animator.h"
#include "../../UIObjects/Prefabs/Button.h"
#include "../../UIObjects/Prefabs/InputField.h"
#include "../../UIObjects/UIObject.h"
#include <cmath>

class UIFocusOutliner final : public Updatable, public Singleton<UIFocusOutliner> {
    inline static UIObject* _focused = nullptr;
    inline static float _pulseTime = 0.0f;
    inline static Color _baseColor {};

public:
    static void focus(UIObject* obj) {
        if (_focused == obj) return;

        // przywróć poprzedni
        if (_focused) {
            Animator::animateOutline(_focused, UIOBJECT_OUTLINE_SCALE, 0.15f);
            Animator::animateColor(_focused, _baseColor, 0.15f);
        }

        _focused = obj;
        if (!_focused) return;

        _baseColor = _focused->color;

        Animator::animateOutline(_focused, UIOBJECT_OUTLINE_SCALE * 2.0f, 0.15f);
        Animator::animateColor(_focused, uti::colors::lightenColor(_focused->color, 0.15f), 0.15f);
        _pulseTime = 0.0f;
    }

    static void unfocus() {
        if (!_focused) return;
        Animator::animateOutline(_focused, UIOBJECT_OUTLINE_SCALE, 0.15f);
        Animator::animateColor(_focused, _baseColor, 0.15f);
        _focused = nullptr;
    }

    void update() override {
        if (!_focused) return;

        _pulseTime += uti::time::getDeltaTime();
        float pulse = 0.5f + 0.5f * std::sin(_pulseTime * 3.0f);

        _focused->outlineScale = UIOBJECT_OUTLINE_SCALE * (1.3f + 0.2f * pulse);
    }

    static UIObject* getFocused() { return _focused; }

    static bool focusable(UIObject* obj) {
        return dynamic_cast<Button*>(obj) != nullptr || dynamic_cast<InputField*>(obj) != nullptr;
    }
};
