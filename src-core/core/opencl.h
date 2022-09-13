#pragma once

#ifdef USE_OPENCL
#define CL_TARGET_OPENCL_VERSION 110
#include <CL/cl.h>
#include <string>
#include <vector>

namespace satdump
{
    namespace opencl
    {
        struct OCLDevice
        {
            int platform_id;
            int device_id;
            std::string name;
        };

        std::vector<OCLDevice> getAllDevices();

        void initOpenCL();

        extern cl_context ocl_context;
        extern cl_device_id ocl_device;

        void setupOCLContext();

        cl_program buildCLKernel(std::string path);
    }
}
#endif