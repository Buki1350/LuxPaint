#include "Tool.h"
#include "../../App/App.h"
#include "Shared/SnapshotsManager/SnapshotManager.h"
#include "Shared/SnapshotsManager/SnapshotsTypes/ImageSnapshot.h"

#include <iostream>

void Tool::handleMousePressed(UIObject *ui_object) {
  handleMousePressedImpl(ui_object);
}

void Tool::handleMouseDown(UIObject *ui_object) {
  handleMouseDownImpl(ui_object);
}

void Tool::handleMouseRelease(UIObject *ui_object) {
  handleMouseReleaseImpl(ui_object);
  App::instance().snapshotManager.saveSnapshot(
    std::make_unique<ImageSnapshot>(App::instance().canvas.getLayersInfo()));
}

void Tool::setSize(int val) { size = val; }

Tool::Tool(const std::string &name) {
  this->_name = name;
  icon = {};

  Image img = Serializer::LoadImage(name + ".png");

  icon = LoadTextureFromImage(img);

  // Zwolnienie pamięci CPU
  UnloadImage(img);

  // Dodatkowa kontrola
  if (icon.id == 0) {
    std::cerr << "Texture upload failed for: " << name << "\n";
  }
}

