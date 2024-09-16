#include <iostream>
#include <Windows.h>
#include <CL/cl.h>
#include <vector>
#include <memory>
#include <unordered_map>

#include "ImageHelper.h"
#include "KernelHelper.h"
#include "GrayScale.h"
#include "BoxBlur.h"
#include "FileManager.h"

int main()
{
    try {
        ds::klh().Init();
        ds::klh().DisplayDeviceInformations();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return -1;
    }

    std::unique_ptr<ds::GrayScale> grayScale;
    std::unique_ptr<ds::BoxBlur> boxBlur;

    try {
        grayScale = std::make_unique<ds::GrayScale>();
        boxBlur = std::make_unique<ds::BoxBlur>();
    }
    catch (std::exception& e) {
        std::cout << e.what() << "\n";
        return -1;
    }

    std::vector<ds::FileInfo> imagesList = ds::FileManager::GetImages();

    std::vector<ds::ImageData> imagesData;
    unsigned long long totalBytes = 0;

    for (ds::ImageData image_data{}; const auto& [file_name, file_path] : imagesList)
    {
        image_data.bytes.clear();
        image_data = ds::ieh().ReadImage(file_path);

        std::cout << "Loaded image '" << file_name << "'\n";

        totalBytes += image_data.bytes.size();
        imagesData.emplace_back(image_data);
    }

    std::cout << "----Loaded " << imagesData.size() << " images\n";

    enum
    {
        GRAY_SCALE_KEY = 1,
        BOX_BLUR_KEY,
        BLURRED_GRAY_KEY,
    };

    ds::ImageData processedData{};

    unsigned long long totalSize = 0;

    auto start_counter = std::chrono::high_resolution_clock::now();
    std::unordered_multimap<int, ds::ImageData> imagesMap;

    std::cout << "Loaded " << totalBytes << " bytes from images\n";
    std::cout << "Total data going to be processed on GPU: " << totalBytes * 3 << "\n";

    for (ds::ImageData& image_data : imagesData)
    {
        processedData = image_data;
        boxBlur->ProcessImage(&processedData);
        imagesMap.emplace(BOX_BLUR_KEY, processedData);
        
        processedData = image_data;
        grayScale->ProcessImage(&processedData);
        imagesMap.emplace(GRAY_SCALE_KEY, processedData);

        boxBlur->ProcessImage(&processedData);
        imagesMap.emplace(BLURRED_GRAY_KEY, processedData);
    }

    auto end_counter = std::chrono::high_resolution_clock::now();
    auto difference = end_counter - start_counter;

    std::chrono::duration<double> elapsed_seconds = difference;
    std::cout << "Processing Time: " << elapsed_seconds.count() << " seconds\n";

    grayScale.release();
    boxBlur.release();
    imagesData.clear();

    for (const auto& [key, image_data] : imagesMap)
    {
        switch (key)
        {
            case GRAY_SCALE_KEY:
            {
                static size_t grayCounter = 0;
                ds::ieh().SaveImage("output/images/gray_scale_" + imagesList.at(grayCounter++).first, image_data);
                break;
            }

            case BOX_BLUR_KEY:
            {
                static size_t boxBlurCounter = 0;
                ds::ieh().SaveImage("output/images/box_blur_" + imagesList.at(boxBlurCounter++).first, image_data);
                break;
            }

            case BLURRED_GRAY_KEY:
            {
                static size_t blurredGrayCounter = 0;
                ds::ieh().SaveImage("output/images/blurred_gray_" + imagesList.at(blurredGrayCounter++).first, image_data);
                break;
            }
        }
    }

    imagesList.clear();
    imagesMap.clear();

    std::string input;
    std::cin >> input;
}