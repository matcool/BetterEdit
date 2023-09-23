#include "../../BetterEdit.hpp"

// bool verifyObjectString(std::string const& str) {
//     std::vector<std::string> objs;
//     if (str.find(";") != std::string::npos) {
//         objs = stringSplit(str, ";");
//     } else {
//         objs = { str };
//     }

//     if (!objs.size())
//         return false;

//     for (auto const& obj : objs) {
//         if (
//             obj.size() > 2 &&
//             obj.find_first_not_of(' ') != std::string::npos &&
//             obj.find(",") == std::string::npos
//         )
//             return false;

//         auto kv = stringSplit(obj, ",");
//         for (auto ix = 0u; ix < kv.size(); ix += 2u) {
//             auto key = kv[ix];
//             if (
//                 !key.size() ||
//                 key.find_first_not_of(' ') != std::string::npos
//             )
//                 return false;

//             try {
//                 std::stoi(key);
//             } catch (...) {
//                 return false;
//             }
//         }
//     }

//     return true;
// }

// void  EditorUI_onCopy(EditorUI* self,  CCObject* pSender) {
//     matdash::orig<&EditorUI_onCopy>(self,  pSender);

//     if (BetterEdit::getCopyObjectsToClipboard()) {
//         copyToWin32Clipboard(self->m_sClipboard);
//     }
// } MAT_GDMAKE_HOOK(0x87fb0, EditorUI_onCopy);

// void  EditorUI_onPaste(EditorUI* self,  CCObject* pSender) {
//     if (BetterEdit::getCopyObjectsToClipboard()) {
//         auto str = readWin32Clipboard();

//         if (str.size()) {
//             auto orig = self->m_sClipboard;
//             self->m_sClipboard = str;
//             matdash::orig<&EditorUI_onPaste>(self,  pSender);
//             self->m_sClipboard = orig;
//             return;
//         }
//     }
//     matdash::orig<&EditorUI_onPaste>(self,  pSender);
// } MAT_GDMAKE_HOOK(0x880c0, EditorUI_onPaste);
