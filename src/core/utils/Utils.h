#pragma once

#include <fstream>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

namespace hamu::utils
{

    // 加上 inlince 防止出现 ODR(One Definition Rule) 违规
    inline std::string ReadText(const fs::path& path) {
        // ate 光标移动到末尾, binary 读取不会把 \r\n 读成 \n
        std::ifstream fin(path, std::ios::ate | std::ios::binary);
        if (!fin.is_open()) {
            std::cerr << "Failed to open file: " << path << std::endl;
            return "";
        }

        // 这里一定会输出物理上的字节光标位置, 和 openmode 无关
        std::streamsize size = fin.tellg();
        if (size <= 0) {
            return "";
        }

        fin.seekg(0, std::ios::beg);

        std::string content;
        content.resize(static_cast<size_t>(size));
        fin.read(&content[0], size);
        return content;
    }

} // namespace hamu::utils