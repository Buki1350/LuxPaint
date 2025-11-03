#pragma once

#include "../../Automatition/Updatables/Updatable.h"
#include "../../Render/UIObject.h"

#include <functional>
#include <vector>

class MiniMenu final : public Updatable {

    struct ObjectWithSavedSize {
        UIObject* object;
        Vec2f initialSize;
        bool flexible = false;

        ObjectWithSavedSize(UIObject* obj)
        : object(obj), initialSize(obj->size), flexible(false) {}

        ObjectWithSavedSize(UIObject* obj, bool flex)
        : object(obj), initialSize(obj->size), flexible(flex) {}
    };

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

    public:
    UIObject* oBackground = nullptr;
    bool centerElements = false;
    static std::vector<MiniMenu*> Instances;
    static MiniMenu* CreateInstance();
    static void DestroyInstance(MiniMenu* miniMenu);

    MiniMenu* Pack(UIObject *object);
    MiniMenu* PackRow(std::initializer_list<ObjectWithSavedSize> objects);
    static ObjectWithSavedSize FlexSeparator();
    void _HandleClosing();
    void _CalculateTransforms();
    void _HandleDeleting();
    void _HandleKeybindings();
    void _FocusNext();
    void _FocusPrevious();
    void Update() override;
    void OnDestroy(std::function<void()> labdaFunction);
};