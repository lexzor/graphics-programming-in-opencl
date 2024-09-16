#ifndef GRAY_SCALE_H
#define GRAY_SCALE_H

#include "KernelHelper.h"
#include "ImageHelper.h"

#include <string>
#include <filesystem>

namespace ds
{
	class GrayScale
	{
	public:
		GrayScale();
		~GrayScale();

		void ProcessImage(ImageData* image_data) const;

	private:
		const char* m_KernelSourceFile = "grayscale.cl";
		const char* m_KernelName = "grayscale";

		cl_command_queue m_CommandQueue;
		cl_program m_Program;
		cl_kernel m_Kernel;
	};
}

#endif // GRAY_SCALE_H