#include "ImageHelper.h"

namespace ds
{
    ImageHelper::ImageHelper() {}
    ImageHelper::~ImageHelper() {}

    ImageHelper& ImageHelper::Instance()
    {
        static ImageHelper instance;
        return instance;
    }

    const ImageData ImageHelper::ReadImage(const std::string& file_name) const
    {
        cv::Mat image = cv::imread(file_name);
        if (image.empty())
        {
            std::cerr << "Failed to load image '" << file_name.data() << "'\n";
            return ImageData();
        }

        cv::cvtColor(image, image, cv::COLOR_BGR2RGBA);
        
        ImageData data{
            .width = image.size().width,
            .height = image.size().height,
        };

        data.bytes.resize(image.total() * image.elemSize());
        memcpy(data.bytes.data(), image.data, data.bytes.size());

        image.release();

        return data;
    }

    void ImageHelper::SaveImage(const std::string_view file_path, const ImageData& image_data) const
    {
        cv::Mat image(image_data.height, image_data.width, CV_8UC(CHANNELS), (void*)image_data.bytes.data());

        if (image.empty())
        {
            std::cerr << "Failed to save image '" << file_path << "'\n";
            image.release();
            return;
        }

        cv::cvtColor(image, image, cv::COLOR_RGBA2BGR);

        if (!cv::imwrite(file_path.data(), image))
        {
            std::cerr << "Failed to save the image at '" << file_path << "'\n";
        }
        else
        {
            std::cout << "Image saved successfully at '" << file_path << "'\n";
        }

        image.release();
    }
}