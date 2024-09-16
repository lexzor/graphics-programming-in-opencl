__kernel void box_blur(__global uchar4* image, int width, int height, int kernelSize) {
    int x = get_global_id(0);
    int y = get_global_id(1);
    int idx = y * width + x;

    if (x < width && y < height) {
        float4 sum = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
        int count = 0;
        int halfKernel = kernelSize / 2;

        for (int ky = -halfKernel; ky <= halfKernel; ky++) {
            for (int kx = -halfKernel; kx <= halfKernel; kx++) {
                int nx = clamp(x + kx, 0, width - 1);
                int ny = clamp(y + ky, 0, height - 1);
                int nidx = ny * width + nx;

                sum += (float4)(image[nidx].x, image[nidx].y, image[nidx].z, 0.0f);
                count++;
            }
        }

        uchar4 blurredPixel = (uchar4)(
            (uchar)(sum.x / count),
            (uchar)(sum.y / count),
            (uchar)(sum.z / count),
            image[idx].w 
        );

        image[idx] = blurredPixel;
    }
}