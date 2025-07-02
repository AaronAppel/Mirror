#pragma once

#include <filesystem>

#include "Buffer.h"

typedef std::filesystem::path Path;

namespace Files {

    struct Extensions
    {
        // #TODO QF_PathDefines.h
        // For safety and convenience
        const char* const Preference = ".qpref";
        const char* const Registry = ".qreg";
        const char* const Scene = ".qscene";
        const char* const Setting = ".qsetting";
    };

    bool Exists(const char* filePath);

    Buffer LoadFile(const char* filePath);

    Path FileName(const char* filePath);
    Path FileExtension(const char* filePath);

    void CreateEmptyFile(const char* const filePath);

    void WriteStringToFile(const char* const fileData, const char* const filePath);

}

