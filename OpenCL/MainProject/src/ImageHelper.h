#ifndef IMAGE_HELPER_H
#define IMAGE_HELPER_H

#define ieh ImageHelper::Instance

#include "opencv2/opencv.hpp"

#include <string>
#include <vector>

namespace ds
{
    constexpr uint8_t CHANNELS = 4;

    struct ImageData
    {
        int width{0};
        int height{0};
        std::vector<unsigned char> bytes;
    };

    class ImageHelper
    {
    public:
        ImageHelper();
        ~ImageHelper();
        static ImageHelper& Instance();

        const ImageData ReadImage(const std::string& file_name) const;
        void SaveImage(const std::string_view file_path, const ImageData& image_data) const;

    private:
        ImageHelper(const ImageHelper&) = delete;
        ImageHelper& operator=(const ImageHelper&) = delete;
        ImageHelper(ImageHelper&&) = delete;
        ImageHelper& operator=(ImageHelper&&) = delete;

        const std::string m_ImagesPath = "assets/images/";
    };
}

#endif // IMAGE_HELPER_H