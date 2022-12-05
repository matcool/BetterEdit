cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(betteredit-suckless)

add_library(betteredit-suckless SHARED
    main.cpp
    BetterEdit.cpp

    hooks/CustomizeObjectLayer.cpp
    hooks/EditLevelLayer.cpp
    hooks/EditorPauseLayer.cpp
    hooks/EditorUI.cpp
    hooks/freezeFix.cpp
    hooks/GJRotationControl.cpp
    hooks/HSVWidgetPopup.cpp
    hooks/LevelEditorLayer.cpp
    hooks/LoadingLayer.cpp
    hooks/PauseLayer.cpp
    hooks/saveHook.cpp
    hooks/SetGroupIDLayer.cpp
    hooks/SetupPickupTriggerPopup.cpp
    hooks/UILayer.cpp

    utils/addTab.cpp
    utils/BGLabel.cpp
    utils/InfoButton.cpp
    utils/moveGameLayer.cpp
)

file(GLOB_RECURSE TOOLS_SOURCE_FILES
    tools/AutoColorTriggers/*.cpp
    tools/AutoSave/*.cpp
    # tools/ClickableDots/*.cpp
    tools/CustomKeybinds/*.cpp
    tools/CustomPasteState/*.cpp
    # tools/CustomStartPos/*.cpp
    tools/EditorLayerInput/*.cpp
    tools/EditTab/*.cpp
    tools/EnterToSearch/*.cpp
    tools/ExtraObjectInfo/*.cpp
    tools/EyeDropper/*.cpp
    tools/FLAlertLayerFix/*.cpp
    tools/GlobalClipboard/*.cpp
    # tools/gmd/*.cpp
    tools/GridSize/*.cpp
    tools/GroupIDFilter/*.cpp
    tools/GroupSummary/*.cpp
    tools/Guidelines/*.cpp
    tools/HighlightTriggers/*.cpp
    tools/IDRemap/*.cpp
    tools/LevelPercent/*.cpp
    tools/other/*.cpp
    tools/PasteString/*.cpp
    tools/Presets/*.cpp
    tools/RGBColorInput/*.cpp
    tools/RotateSaws/*.cpp
    # tools/Screenshot/*.cpp
    tools/ScrollWheel/*.cpp
    tools/settings/*.cpp
    tools/SliderImprovement/*.cpp
    tools/VisibilityTab/*.cpp
)

target_sources(betteredit-suckless PRIVATE ${TOOLS_SOURCE_FILES})

include(cmake/CPM.cmake)

CPMAddPackage("gh:matcool/mat-dash#cdc9f70")
CPMAddPackage("gh:HJfod/cocos-headers#cbf79fc")
CPMAddPackage("gh:HJfod/gd.h#90f2110")
CPMAddPackage("gh:TsudaKageyu/minhook#4a45552")
CPMAddPackage("gh:hjfod/gd-nodes#38ac99d")

target_compile_definitions(betteredit-suckless PRIVATE CC_DLL=)

target_include_directories(betteredit-suckless PRIVATE
    nuts
    ${gd.h_SOURCE_DIR}/include ${gd.h_SOURCE_DIR}
    ${minhook_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
    include
)

add_library(nodes INTERFACE)

target_sources(nodes INTERFACE
    ${gd-nodes_SOURCE_DIR}/BGCheckbox.cpp
    ${gd-nodes_SOURCE_DIR}/BrownAlertDelegate.cpp
    ${gd-nodes_SOURCE_DIR}/HorizontalSelectMenu.cpp
    ${gd-nodes_SOURCE_DIR}/InputNode.cpp
    ${gd-nodes_SOURCE_DIR}/InputPrompt.cpp
    ${gd-nodes_SOURCE_DIR}/MonoSpaceLabel.cpp
    ${gd-nodes_SOURCE_DIR}/PickMenu.cpp
)

target_include_directories(nodes INTERFACE ${gd-nodes_SOURCE_DIR})


target_link_libraries(betteredit-suckless mat-dash cocos2d minhook nodes)