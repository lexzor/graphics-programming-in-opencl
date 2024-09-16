#ifndef BOX_BLUR_H
#define BOX_BLUR_H

#include "KernelHelper.h"
#include "ImageHelper.h"

#include <string>
#include <filesystem>

namespace ds
{
	class BoxBlur
	{
	public:
		BoxBlur();
		~BoxBlur();

		void ProcessImage(ImageData* image_data) const;

	private:
		const char* m_KernelSourceFile = "box_blur.cl";
		const char* m_KernelName = "box_blur";

		cl_command_queue m_CommandQueue;
		cl_program m_Program;
		cl_kernel m_Kernel;

		const int m_KernelFilterSize = 50; // 5x5 box blur
	};
}

#endif // BOX_BLUR_H