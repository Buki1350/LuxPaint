cmake_minimum_required(VERSION 3.10.0)
project(LuxPaint VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Ścieżki do raylib (Windows - lokalne, Linux - systemowe)
if (WIN32)
    set(RAYLIB_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/raylib/include")
    set(RAYLIB_LIB_DIR "${CMAKE_SOURCE_DIR}/raylib/lib")
elseif(UNIX)
    find_package(raylib 4.0 REQUIRED) # Wymaga: sudo apt install libraylib-dev
endif()

# Tworzymy plik wykonywalny
add_executable(LuxPaint
        ../otherLibs/nfd/nfd_win.cpp
        #../otherLibs/nfd/nfd_gtk.c
        #../otherLibs/nfd/nfd_cocoa.m
        src/main.cpp
        src/App.cpp
        src/StaticShared/FilesManager/FilesManager.cpp
        src/Tools/Shared/Tool.cpp
        src/Tools/Shared/Tool.h
        src/Tools/ToolSet/ToolSet.cpp
        src/Render/UIObjectsManager.cpp
        src/Render/UIObjectsManager.h
        src/Render/UIObject.cpp
        src/Render/UIObject.h
        src/Tools/PaintTools/PenTool.h
        src/UI/ToolBox/ToolBox.h
        src/UI/ToolBox/ToolBox.cpp
        src/StaticShared/Utils/Utils.cpp
        src/StaticShared/Utils/Utils.h
        src/UI/ManagerButton/ManagerButton.cpp
        src/UI/ManagerButton/ManagerButton.h
        src/UI/ColorHolder/ColorHolder.cpp
        src/UI/ColorHolder/ColorHolder.h
        src/Automatition/Updatables/Updatable.h
        src/Automatition/Updatables/UpdatablesManager.h
        src/Automatition/Updatables/UpdatablesManager.cpp
        src/StaticShared/Animator/Animator.cpp
        src/StaticShared/Animator/Animator.h
        src/Defines.h
        src/Render/Text.cpp
        src/Render/Text.h
        src/Canvas/Canvas.cpp
        src/Canvas/Canvas.h
        src/Math/Vec2.h
        src/Automatition/Initializables/Initializable.h
        src/Automatition/Initializables/Initializables.h
        src/Automatition/Initializables/Initializable.cpp
        src/Automatition/Initializables/Initializables.cpp
        src/Canvas/MarchingAntsSelector/MarchingAntsSelector.cpp
        src/Canvas/MarchingAntsSelector/MarchingAntsSelector.h
        src/StaticShared/Singleton/Singleton.h
        src/Math/Matx.h
        src/UI/MiniMenu/MiniMenu.cpp
        src/UI/MiniMenu/MiniMenu.h
        src/UI/ManagerButton/BlankCanvasBuilder.h
        src/StaticShared/Animator/AnimationValue.h
        src/StaticShared/Animator/AnimationValue.tpp
        src/Macros.h
        src/Render/Prefabs/InputField.cpp
        src/Render/Prefabs/InputField.h
        src/Render/Prefabs/Button.h
        src/StaticShared/DelayedAction/DelayedAction.h
        src/Tools/PaintTools/BrushTool.h
        src/UI/ColorPicker/ColorPicker.cpp
        src/UI/ColorPicker/ColorPicker.h
        src/Render/Prefabs/Slider.cpp
        src/Render/Prefabs/Slider.h
        src/Tools/SelectionTools/RectangleSelection.h
        src/Tools/SelectionTools/RectangleSelection.cpp
        src/StaticShared/KeyBindings/KeyBindings.cpp
        src/StaticShared/KeyBindings/KeyBindings.h
        src/Render/Extensions/UIFocusOutliner.h
)

# Linkowanie i include w zależności od systemu
if (WIN32)
    target_include_directories(LuxPaint PRIVATE ${RAYLIB_INCLUDE_DIR})
    target_link_directories(LuxPaint PRIVATE ${RAYLIB_LIB_DIR})
    target_link_libraries(LuxPaint PRIVATE raylib opengl32 gdi32 winmm)

    # Ukrycie konsoli, ale zachowanie main()
#    set_target_properties(LuxPaint PROPERTIES
#        LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
#    )

    # Kopiowanie DLL po buildzie
    add_custom_command(
        TARGET LuxPaint
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${RAYLIB_LIB_DIR}/raylib.dll"
                $<TARGET_FILE_DIR:LuxPaint>
    )

elseif(UNIX)
    target_link_libraries(LuxPaint PRIVATE raylib m pthread dl GL)
endif()
