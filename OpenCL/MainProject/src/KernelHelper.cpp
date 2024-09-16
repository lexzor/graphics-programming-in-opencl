#include "KernelHelper.h"

namespace ds
{
    KernelHelper::KernelHelper() : m_Context(nullptr), m_DeviceID(nullptr), m_PlatformID(nullptr) {}

    KernelHelper::~KernelHelper()
    {
        if (m_Context)
        {
            // Releasing context
            clReleaseContext(m_Context);
        }
    }

    KernelHelper& KernelHelper::Instance()
    {
        static KernelHelper instance;
        return instance;
    }

    void KernelHelper::Init()
    {
        cl_uint retNumDevices = 0;
        cl_uint retNumPlatforms = 0;

        cl_int ret = clGetPlatformIDs(1, &m_PlatformID, &retNumPlatforms);
        if (ret != CL_SUCCESS || retNumPlatforms == 0)
        {
            throw std::runtime_error(std::string("Failed to find OpenCL platform. Error code: ") + std::to_string(ret));
        }

        ret = clGetDeviceIDs(m_PlatformID, CL_DEVICE_TYPE_DEFAULT, 1, &m_DeviceID, &retNumDevices);
        if (ret != CL_SUCCESS || retNumDevices == 0)
        {
            throw std::runtime_error(std::string("Failed to find OpenCL device. Error code: ") + std::to_string(ret));
        }

        m_Context = clCreateContext(nullptr, 1, &m_DeviceID, nullptr, nullptr, &ret);
        if (ret != CL_SUCCESS || !m_Context)
        {
            throw std::runtime_error(std::string("Failed to create OpenCL context. Error code: ") + std::to_string(ret));
        }

        std::cout << "KernelLoader initialized succesfully\n";
    }

    cl_program KernelHelper::CreateCLProgramByKernelSourceFile(const char* file_name) const
    {
        std::string path = "kernels/" + std::string(file_name);

        std::ifstream file(path);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open kernel file.");
        }

        std::stringstream buffer;
        buffer << file.rdbuf();

        std::string kernel_content = buffer.str();

        const char* source = kernel_content.c_str();
        const size_t sourceSize = kernel_content.size();

        cl_int ret;
        cl_program program = clCreateProgramWithSource(m_Context, 1, &source, &sourceSize, &ret);
        if (ret != CL_SUCCESS || !program)
        {
            throw std::runtime_error("Failed to create OpenCL program. Error code: " + std::to_string(ret));
        }

        ret = clBuildProgram(program, 1, &m_DeviceID, NULL, NULL, NULL);
        if (ret != CL_SUCCESS)
        {
            throw std::runtime_error("Failed to build program. Error code: " + std::to_string(ret));
        }

        return program;
    }

    void KernelHelper::DisplayDeviceInformations() const
    {
        char platform_name[128];
        char device_name[128];
        cl_uint compute_units;
        size_t global_mem_size;
        size_t max_work_group_size;

        CLRUN(clGetPlatformInfo(m_PlatformID, CL_PLATFORM_NAME, sizeof(platform_name), platform_name, nullptr));
        CLRUN(clGetDeviceInfo(m_DeviceID, CL_DEVICE_NAME, sizeof(device_name), device_name, nullptr));
        CLRUN(clGetDeviceInfo(m_DeviceID, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(compute_units), &compute_units, nullptr));
        CLRUN(clGetDeviceInfo(m_DeviceID, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(global_mem_size), &global_mem_size, nullptr));
        CLRUN(clGetDeviceInfo(m_DeviceID, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(max_work_group_size), &max_work_group_size, nullptr));

        std::cout << "Current device informations:\n\n";

        std::cout << "- Platform: " << platform_name << "\n";
        std::cout << "- Device: " << device_name << "\n";
        std::cout << "- Global Memory Size: " << global_mem_size / (1024 * 1024) << " MB" << "\n";
        std::cout << "- Compute Units: " << compute_units << "\n";
        std::cout << "- Max Work Group Size: " << max_work_group_size << "\n\n";
    }
}