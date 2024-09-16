#include "BoxBlur.h"

namespace ds
{
    BoxBlur::BoxBlur()
    {
        cl_int ret = CL_SUCCESS;
        m_CommandQueue = clCreateCommandQueueWithProperties(klh().m_Context, klh().m_DeviceID, nullptr, &ret);
        if (ret != CL_SUCCESS)
        {
            throw new std::runtime_error("Failed to create GaussianBlur::m_CommandQueue");
        }

        try {
            m_Program = klh().CreateCLProgramByKernelSourceFile(m_KernelSourceFile);
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << "\n";
            throw new std::runtime_error("Failed to create GaussianBlur filter");
        }

        m_Kernel = clCreateKernel(m_Program, m_KernelName, &ret);
        if (ret != CL_SUCCESS)
        {
            throw std::runtime_error("Failed to create kernel. Error code: " + std::to_string(ret));
        }
    }

    BoxBlur::~BoxBlur()
    {
        clReleaseKernel(m_Kernel);
        clReleaseProgram(m_Program);
        clReleaseCommandQueue(m_CommandQueue);
    }

    void BoxBlur::ProcessImage(ImageData* image_data) const
    {
        cl_int ret = 0;
        cl_mem imageBuffer = clCreateBuffer(klh().m_Context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, image_data->bytes.size(), (void*)image_data->bytes.data(), &ret);
        if (ret != CL_SUCCESS)
        {
            std::cerr << "Failed to create OpenCL buffer\n";
            return;
        }

        // Set kernel arguments
        CLRUN(clSetKernelArg(m_Kernel, 0, sizeof(cl_mem), &imageBuffer));
        CLRUN(clSetKernelArg(m_Kernel, 1, sizeof(int), &image_data->width));
        CLRUN(clSetKernelArg(m_Kernel, 2, sizeof(int), &image_data->height));
        CLRUN(clSetKernelArg(m_Kernel, 3, sizeof(int), &m_KernelFilterSize)); // Pass the kernel size

        // Define global work size
        size_t global_work_size[2] = { image_data->width, image_data->height };
        CLRUN(clEnqueueNDRangeKernel(m_CommandQueue, m_Kernel, 2, nullptr, global_work_size, nullptr, 0, nullptr, nullptr));

        // Finish command queue and read back the processed image
        CLRUN(clFinish(m_CommandQueue));
        CLRUN(clEnqueueReadBuffer(m_CommandQueue, imageBuffer, CL_TRUE, 0, image_data->bytes.size(), image_data->bytes.data(), 0, nullptr, nullptr));

        // Release memory objects
        CLRUN(clReleaseMemObject(imageBuffer));
    }
}