#pragma once

#include <gd.h>
#include <string>
#include <filesystem>
#include <fstream>
#include <Windows.h>

namespace gdmake {
    namespace extra {
        template<typename T>
        static T getChild(cocos2d::CCNode* x, int i) {
            if (i < 0)
                i = x->getChildrenCount() + i;
            if (static_cast<int>(x->getChildrenCount()) <= i)
                return nullptr;
            return reinterpret_cast<T>(x->getChildren()->objectAtIndex(i));
        }
        template <typename T, typename R>
        T as(R const v) { return reinterpret_cast<T>(v); }
        inline std::string operator"" _s (const char* _txt, size_t) {
            return std::string(_txt);
        }
        template<typename T, typename U> constexpr size_t offsetOf(U T::*member) {
            return (char*)&((T*)nullptr->*member) - (char*)nullptr;
        }
        template<typename T>
        constexpr const T& clamp( const T& v, const T& lo, const T& hi) {
            return v < lo ? lo : hi < v ? hi : v;
        }
    }
    [[nodiscard]]
    static std::string readFileString(const std::string & _path) {
        std::ifstream in(_path, std::ios::in | std::ios::binary);
        if (in) {
            std::string contents;
            in.seekg(0, std::ios::end);
            contents.resize((const size_t)in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&contents[0], contents.size());
            in.close();
            return(contents);
        }
        return "";
    }
    [[nodiscard]]
    static std::vector<uint8_t> readFileBinary(const std::string & _path) {
        std::ifstream in(_path, std::ios::in | std::ios::binary);
        if (in)
            return std::vector<uint8_t> ( std::istreambuf_iterator<char>(in), {});
        return {};
    }
    static bool writeFileString(const std::string & _path, const std::string & _cont) {
        std::ofstream file;
        file.open(_path);
        if (file.is_open()) {
            file << _cont;
            file.close();
            return true;
        }
        file.close();
        return false;
    }
    static bool writeFileBinary(const std::string & _path, std::vector<uint8_t> const& _bytes) {
        std::ofstream file;
        file.open(_path, std::ios::out | std::ios::binary);
        if (file.is_open()) {
            file.write(reinterpret_cast<const char*>(_bytes.data()), _bytes.size());
            file.close();
            return true;
        }
        file.close();
        return false;
    }
    static constexpr unsigned int hash(const char* str, int h = 0) {
        return !str[h] ? 5381 : (hash(str, h+1) * 33) ^ str[h];
    }
    inline bool patchBytes(
        uintptr_t const address,
        std::vector<uint8_t> const& bytes
    ) {
        return WriteProcessMemory(
            GetCurrentProcess(),
            reinterpret_cast<LPVOID>(gd::base + address),
            bytes.data(),
            bytes.size(),
            nullptr
        );
    }
    // returns overwritten bytes
    // nothing is returned on error
    inline std::vector<uint8_t> patchBytesEx(
        uintptr_t const address,
        std::vector<uint8_t> const& bytes,
        bool hardOverwrite = false
    ) {
        auto hProcess = GetCurrentProcess();
        auto nTarget = reinterpret_cast<LPVOID>(gd::base + address);
        std::vector<uint8_t> ret(bytes.size());
        if (!ReadProcessMemory(
            hProcess,
            nTarget,
            ret.data(),
            ret.size(),
            nullptr
        )) return {};
         
        DWORD oldprotect;
        if (hardOverwrite)
            VirtualProtectEx(
                hProcess,
                nTarget,
                bytes.size(),
                PAGE_EXECUTE_READWRITE,
                &oldprotect
            );
        if (!WriteProcessMemory(
            hProcess,
            nTarget,
            bytes.data(),
            bytes.size(),
            nullptr
        )) return {};
    
        if (hardOverwrite)
            VirtualProtectEx(
                hProcess,
                nTarget,
                bytes.size(),
                oldprotect,
                &oldprotect
            );
        return ret;
    }
    inline bool patchBytesAbs(
        uintptr_t const absoluteAddress,
        std::vector<uint8_t> const& bytes
    ) {
        return WriteProcessMemory(
            GetCurrentProcess(),
            reinterpret_cast<LPVOID>(absoluteAddress),
            bytes.data(),
            bytes.size(),
            nullptr
        );
    }
    using unknown_t = uintptr_t;
    using edx_t = uintptr_t;
}

#define CCARRAY_FOREACH_B_BASE(__array__, __obj__, __type__, __index__) \
    if (__array__ && __array__->count()) \
    for (auto [__index__, __obj__] = std::tuple<unsigned int, __type__> { 0u, nullptr }; \
        (__index__ < __array__->count() && (__obj__ = reinterpret_cast<__type__>(__array__->objectAtIndex(__index__)))); \
        __index__++)
#define CCARRAY_FOREACH_B_TYPE(__array__, __obj__, __type__) \
    CCARRAY_FOREACH_B_BASE(__array__, __obj__, __type__*, ix)
#define CCARRAY_FOREACH_B(__array__, __obj__) \
    CCARRAY_FOREACH_B_BASE(__array__, __obj__, cocos2d::CCObject*, ix)


#define GDMAKE_HOOK(...)
#define GDMAKE_ORIG(...) 10
#define GDMAKE_ORIG_V(...)
#define GDMAKE_ORIG_P(...) nullptr

#include <matdash.hpp>
#include <matdash/minhook.hpp>
#include <vector>

namespace matstuff {
    std::vector<void(*)()>& get_hooks();
}
#define MAT_GDMAKE_HOOK(addr, func) \
    static auto __STR_CAT__(_foo_wow_, __LINE__) = (matstuff::get_hooks().push_back([] { \
        matdash::add_hook<&func>(gd::base + addr); \
    }), 69);