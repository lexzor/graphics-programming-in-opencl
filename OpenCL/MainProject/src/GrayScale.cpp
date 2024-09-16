#include "GrayScale.h"

namespace ds
{
    GrayScale::GrayScale()
    {
        cl_int ret = CL_SUCCESS;
        m_CommandQueue = clCreateCommandQueueWithProperties(klh().m_Context, klh().m_DeviceID, nullptr, &ret);
        if (ret != CL_SUCCESS)
        {
            throw new std::runtime_error("Failed to create GrayScale::m_CommandQueue");
        }

        try {
            m_Program = klh().CreateCLProgramByKernelSourceFile(m_KernelSourceFile);
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << "\n";
            throw new std::runtime_error("Failed to create GrayScale filter");
        }    

        m_Kernel = clCreateKernel(m_Program, m_KernelName, &ret);
        if (ret != CL_SUCCESS)
        {
            throw std::runtime_error("Failed to create kernel. Error code: " + std::to_string(ret));
        }
    }

    GrayScale::~GrayScale()
    {
        clReleaseKernel(m_Kernel);
        clReleaseProgram(m_Program);
        clReleaseCommandQueue(m_CommandQueue);
    }

    void GrayScale::ProcessImage(ImageData* image_data) const
    {
        cl_int ret = 0;
        cl_mem imageBuffer = clCreateBuffer(klh().m_Context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, image_data->bytes.size(), (void*)image_data->bytes.data(), &ret);
        if (ret != CL_SUCCESS)
        {
            std::cerr << "Failed to create OpenCL buffer\n";
            return;
        }

        CLRUN(clSetKernelArg(m_Kernel, 0, sizeof(cl_mem), &imageBuffer));
        CLRUN(clSetKernelArg(m_Kernel, 1, sizeof(int), &image_data->width));
        CLRUN(clSetKernelArg(m_Kernel, 2, sizeof(int), &image_data->height));

        size_t global_work_size[2] = { image_data->width, image_data->height };
        CLRUN(clEnqueueNDRangeKernel(m_CommandQueue, m_Kernel, 2, nullptr, global_work_size, nullptr, 0, nullptr, nullptr));

        CLRUN(clFinish(m_CommandQueue));
        CLRUN(clEnqueueReadBuffer(m_CommandQueue, imageBuffer, CL_TRUE, 0, image_data->bytes.size(), image_data->bytes.data(), 0, nullptr, nullptr));
        CLRUN(clReleaseMemObject(imageBuffer));
    }
}