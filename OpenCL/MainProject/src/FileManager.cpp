#include "FileManager.h"
#include <iostream>

namespace ds
{
    std::vector<FileInfo> FileManager::GetImages()
    {
        const std::filesystem::path IMAGES_PATH = "assets/images/";

        std::vector<FileInfo> imagesList;
        
        for (const auto& image : std::filesystem::directory_iterator(IMAGES_PATH))
        {
            std::cout << "Found '" << image.path().string() << "'\n";
            imagesList.emplace_back(image.path().filename().string(), image.path().string());
        }

        return imagesList;
    }
}