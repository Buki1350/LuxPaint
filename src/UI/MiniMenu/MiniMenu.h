#pragma once
#include <functional>
#include <vector>

#include "../../Automatition/Updatables/Updatable.h"
#include "../../Shared/UIObjects/UIObject.h"

class MiniMenu final : public Updatable {
    template <typename T> friend class MiniMenuBuilderBase;
    struct ObjectWithSavedSize; // predef

    UIObject* _oBackground = nullptr;
    Vec2f _targetSize;

    // ... for multiple objects in one row
    using Row = std::vector<ObjectWithSavedSize>;
    std::vector<Row> _rows;
    std::vector<ObjectWithSavedSize> _oPackedObjects;

    // ... same time as popup, but time is saved to delete pointer after animation
    const float _deletingDuration = ANIMATION_POPUP_DURATION;
    float _deletingElapsed = 0.0f;
    bool _markedForDeletion = false;
    std::function<void()> _onDestructionFunc;

    // ... keybindings
    UIObject* _currentSelected = nullptr;
    std::vector<UIObject*> _buttonsAndInputs;

    // ... helper for not closing when clicked not on this uiObject
    bool _disableClosing = false;

    void _HandleClosing();
    void _CalculateTransforms();
    void _HandleDeleting();
    void _HandleKeybindings();
    void _FocusNext();
    void _FocusPrevious();

    struct ObjectWithSavedSize {
        UIObject* object;
        Vec2f initialSize;
        bool flexible = false;

        ObjectWithSavedSize(UIObject* obj)
        : object(obj), initialSize(obj->size), flexible(false) {}

        ObjectWithSavedSize(UIObject* obj, bool flex)
        : object(obj), initialSize(obj->size), flexible(flex) {}
    };

public:
    static std::vector<MiniMenu*> instances;
    bool centerElements = false;
    static MiniMenu* CreateInstance();
    static void DestroyInstance(MiniMenu* miniMenu);
    void Destroy();

    MiniMenu* Pack(UIObject *object);
    MiniMenu *PackRow(std::initializer_list<ObjectWithSavedSize> objects);
    static ObjectWithSavedSize FlexSeparator();
    void Update() override;
    void OnDestroy(std::function<void()> labdaFunction);
    void DisableClosing();
    void EnableClosing();

    static void SetBackgroundColorForAll(Color color);
};