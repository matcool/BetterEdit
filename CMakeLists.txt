cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "x86_64")

project(betteredit-v4)

add_library(${PROJECT_NAME} SHARED
    main.cpp
    BetterEdit.cpp

    hooks/CustomizeObjectLayer.cpp
    hooks/EditLevelLayer.cpp
    hooks/EditorPauseLayer.cpp
    # hooks/EditorUI.cpp
    hooks/freezeFix.cpp
    hooks/HSVWidgetPopup.cpp
    hooks/LevelEditorLayer.cpp
    hooks/PauseLayer.cpp
    hooks/SetGroupIDLayer.cpp
    hooks/SetupPickupTriggerPopup.cpp

    utils/addTab.cpp
    utils/BGLabel.cpp
    utils/InfoButton.cpp
    utils/moveGameLayer.cpp
)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)


file(GLOB_RECURSE TOOLS_SOURCE_FILES
    tools/AutoColorTriggers/*.cpp
    # tools/AutoSave/*.cpp
    # # tools/ClickableDots/*.cpp
    # tools/CustomKeybinds/*.cpp
    # tools/CustomPasteState/*.cpp
    # # tools/CustomStartPos/*.cpp
    # tools/EditorLayerInput/*.cpp
    # tools/EditTab/*.cpp
    # tools/EnterToSearch/*.cpp
    # tools/ExtraObjectInfo/*.cpp
    # tools/EyeDropper/*.cpp
    # tools/FLAlertLayerFix/*.cpp
    # tools/GlobalClipboard/*.cpp
    # # tools/gmd/*.cpp
    # tools/GridSize/*.cpp
    # tools/GroupIDFilter/*.cpp
    # tools/GroupSummary/*.cpp
    # tools/Guidelines/*.cpp
    # tools/HighlightTriggers/*.cpp
    tools/IDRemap/*.cpp
    tools/LevelPercent/*.cpp
    tools/other/*.cpp
    tools/PasteString/*.cpp
    tools/Presets/*.cpp
    # tools/RGBColorInput/*.cpp
    # tools/RotateSaws/*.cpp
    # # tools/Screenshot/*.cpp
    # tools/ScrollWheel/*.cpp
    tools/settings/*.cpp
    # tools/SliderImprovement/*.cpp
    tools/VisibilityTab/*.cpp
)

target_sources(${PROJECT_NAME} PRIVATE ${TOOLS_SOURCE_FILES})

# CPMAddPackage("gh:matcool/mat-dash#cdc9f70")
# CPMAddPackage("gh:HJfod/cocos-headers#cbf79fc")
# CPMAddPackage("gh:HJfod/gd.h#90f2110")
# CPMAddPackage("gh:TsudaKageyu/minhook#4a45552")
# CPMAddPackage("gh:hjfod/gd-nodes#38ac99d")

# target_compile_definitions(${PROJECT_NAME} PRIVATE CC_DLL=)

target_include_directories(${PROJECT_NAME} PRIVATE
    nuts
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(nodes)

target_link_libraries(${PROJECT_NAME} nodes)

setup_geode_mod(${PROJECT_NAME})
