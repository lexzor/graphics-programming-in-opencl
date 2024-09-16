#ifndef KERNEL_LOADER_H
#define KERNEL_LOADER_H

#include <CL/cl.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <source_location>

#include "ImageHelper.h"

#define klh KernelHelper::Instance

#define CLRUN(x) \
{ \
    cl_int _clFunctionReturnCode = (x); \
    if(_clFunctionReturnCode != CL_SUCCESS)\
    { \
        std::cerr << "[CLRUN] " << std::source_location::current().function_name() << "::" << std::source_location::current().line() << ". Error code: " << _clFunctionReturnCode << "\n"; \
    } \
}

namespace ds
{
    class KernelHelper
    {
    public:
        KernelHelper();
        ~KernelHelper();

        static KernelHelper& Instance();

        void Init();

        cl_program CreateCLProgramByKernelSourceFile(const char* file_name) const;
        void DisplayDeviceInformations() const;

        cl_context m_Context;
        cl_device_id m_DeviceID;
        cl_platform_id m_PlatformID;

    private:
        KernelHelper(const KernelHelper&) = delete;
        KernelHelper& operator=(const KernelHelper&) = delete;
        KernelHelper(KernelHelper&&) = delete;
        KernelHelper& operator=(KernelHelper&&) = delete;
    };
}
#endif // KERNEL_LOADER_H