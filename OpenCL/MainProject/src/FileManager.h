#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <filesystem>

namespace ds
{
    using FileInfo = std::pair<std::string, std::string>;

    class FileManager
    {
    public:
        static std::vector<FileInfo> GetImages();

    private:
        FileManager();
        ~FileManager();
        FileManager(const FileManager&) = delete;
        FileManager& operator=(const FileManager&) = delete;
        FileManager(FileManager&&) = delete;
        FileManager& operator=(FileManager&&) = delete;
    };
}

#endif // FILE_MANAGER_H