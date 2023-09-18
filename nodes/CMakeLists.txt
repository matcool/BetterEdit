add_library(nodes INTERFACE)

target_sources(nodes INTERFACE
    BGCheckbox.cpp
    BrownAlertDelegate.cpp
    HorizontalSelectMenu.cpp
    InputNode.cpp
    InputPrompt.cpp
    MonoSpaceLabel.cpp
    PickMenu.cpp
)

target_include_directories(nodes INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})