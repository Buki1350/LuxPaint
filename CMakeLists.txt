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
    src/main.cpp
    src/App.cpp
        src/Static/FilesManager/FilesManager.cpp
        src/PaintTools/Shared/Tool.cpp
        src/PaintTools/Shared/Tool.h
        src/PaintTools/ToolSet/ToolSet.cpp
        src/Render/UIObjectManager.cpp
        src/Render/UIObjectManager.h
        src/Render/UIObject.cpp
        src/Render/UIObject.h
        src/PaintTools/PenTool.cpp
        src/PaintTools/PenTool.h
        src/UI/ToolBox/ToolBox.h
        src/UI/ToolBox/ToolBox.cpp
        src/Static/Utils/Utils.cpp
        src/Static/Utils/Utils.h
        src/UI/ManagerButton/ManagerButton.cpp
        src/UI/ManagerButton/ManagerButton.h
        src/UI/ColorHolder/ColorHolder.cpp
        src/UI/ColorHolder/ColorHolder.h
        src/Updatables/Updatable.h
        src/Updatables/Updatables.h
        src/Updatables/Updatables.cpp
        src/Static/Animator/Animator.cpp
        src/Static/Animator/Animator.h
        src/Defines.h
        src/Render/Text.cpp
        src/Render/Text.h
)

# Linkowanie i include w zależności od systemu
if (WIN32)
    target_include_directories(LuxPaint PRIVATE ${RAYLIB_INCLUDE_DIR})
    target_link_directories(LuxPaint PRIVATE ${RAYLIB_LIB_DIR})
    target_link_libraries(LuxPaint PRIVATE raylib opengl32 gdi32 winmm)

    # Ukrycie konsoli, ale zachowanie main()
    set_target_properties(LuxPaint PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
    )

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
