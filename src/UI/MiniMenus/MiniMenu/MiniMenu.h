#pragma once
#include <functional>
#include <vector>

#include "../../../Automatition/Updatables/Updatable.h"
#include "../../../Shared/UIObjects/UIObject.h"

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

    void _handleClosing();
    void _calculateTransforms();
    void _handleDeleting();
    void _handleKeybindings();
    void _focusNext();
    void _focusPrevious();

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
    static MiniMenu* createInstance();
    static void destroyInstance(MiniMenu* miniMenu);
    void destroy();

    const UIObject* getOBackground();

    MiniMenu* pack(UIObject *object);
    MiniMenu *packRow(std::initializer_list<ObjectWithSavedSize> objects);
    void update() override;
    void onDestroy(std::function<void()> labdaFunction);
    void disableClosing();
    void enableClosing();

    void createSmallSeparator();

    static ObjectWithSavedSize flexSeparator();

    static void setBackgroundColorForAll(Color color);
};