#include <GDMake.h>

using addEditorTabFunc = std::function<EditButtonBar*(EditorUI*)>;
void addEditorTab(const char* spr, addEditorTabFunc bbar);
