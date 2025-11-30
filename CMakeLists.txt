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
        src/Shared/FilesManager/FilesManager.cpp
        src/Tools/Shared/Tool.cpp
        src/Tools/Shared/Tool.h
        src/Tools/ToolSet/ToolSet.cpp
        src/Shared/UIObjects/UIObjectsManager.cpp
        src/Shared/UIObjects/UIObjectsManager.h
        src/Shared/UIObjects/UIObject.cpp
        src/Shared/UIObjects/UIObject.h
        src/Tools/PaintTools/PenTool.h
        src/UI/ToolBox/ToolBox.h
        src/UI/ToolBox/ToolBox.cpp
        src/Shared/Utils/Utils.cpp
        src/Shared/Utils/Utils.h
        src/UI/ManagerButton/ManagerButton.cpp
        src/UI/ManagerButton/ManagerButton.h
        src/UI/ColorHolder/ColorHolder.cpp
        src/UI/ColorHolder/ColorHolder.h
        src/Automatition/Updatables/Updatable.h
        src/Automatition/Updatables/UpdatablesManager.h
        src/Automatition/Updatables/UpdatablesManager.cpp
        src/Shared/Animator/Animator.cpp
        src/Shared/Animator/Animator.h
        src/Defines.h
        src/Shared/UIObjects/Text.cpp
        src/Shared/UIObjects/Text.h
        src/Canvas/Canvas.cpp
        src/Canvas/Canvas.h
        src/Math/vec2.h
        src/Automatition/Initializables/Initializable.h
        src/Automatition/Initializables/Initializables.h
        src/Automatition/Initializables/Initializable.cpp
        src/Automatition/Initializables/Initializables.cpp
        src/Canvas/Selectors/MarchingAntsSelector.cpp
        src/Canvas/Selectors/MarchingAntsSelector.h
        src/Shared/Singleton/Singleton.h
        src/Math/Matrx.h
        src/UI/MiniMenu/MiniMenu.cpp
        src/UI/MiniMenu/MiniMenu.h
        src/UI/ManagerButton/BlankCanvasBuilder.h
        src/Shared/Animator/AnimationValue.h
        src/Shared/Animator/AnimationValue.tpp
        src/Macros.h
        src/Shared/UIObjects/Prefabs/InputField.cpp
        src/Shared/UIObjects/Prefabs/InputField.h
        src/Shared/UIObjects/Prefabs/Button.h
        src/Shared/DelayedAction/DelayedAction_inSeconds.h
        src/Tools/PaintTools/BrushTool.h
        src/UI/ColorPicker/ColorPicker.cpp
        src/UI/ColorPicker/ColorPicker.h
        src/Shared/UIObjects/Prefabs/Slider.cpp
        src/Shared/UIObjects/Prefabs/Slider.h
        src/Tools/SelectionTools/RectangleSelection.h
        src/Tools/SelectionTools/RectangleSelection.cpp
        src/Shared/KeyBindings/KeyBindings.cpp
        src/Shared/KeyBindings/KeyBindings.h
        src/Shared/UIObjects/Extensions/UIFocusOutliner.h
        src/UI/ManagerButton/SettingsBuilder.h
        src/UI/MiniMenu/MiniMenuBuilder.h
        src/Shared/DelayedAction/DelayedActions.h
        src/Shared/DelayedAction/DelayedAction_until.h
        src/Canvas/Selectors/SelectionMask.cpp
        src/Canvas/Selectors/SelectionMask.h
        src/Shared/InputManager/InputManager.cpp
        src/Shared/InputManager/InputManager.h
        src/Shared/KeyBindings/KeyNames.h
        src/Debug/Debug.cpp
        src/Debug/Debug.h
        src/Tools/PaintTools/BucketTool.h
        src/Tools/ShapesTools/RectangleTool.h
        src/Tools/ShapesTools/CircleTool.h
        src/UI/ToolSizeSlider/ToolSizeSlider.cpp
        src/UI/ToolSizeSlider/ToolSizeSlider.h
        src/Shared/Calculations/Calc.h
        src/Shared/Calculations/CalcUIO.cpp
        src/Shared/Utils/UtilsColors.cpp
        src/Shared/Utils/UtilsFiles.cpp
        src/Shared/Utils/UtilsView.cpp
        src/Shared/Utils/UtilsInput.cpp
        src/Shared/Utils/UtilsAppdata.cpp
        src/Shared/Utils/UtilsConvert.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)

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
