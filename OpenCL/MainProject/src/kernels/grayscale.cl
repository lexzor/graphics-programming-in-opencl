__kernel void grayscale(__global uchar4* image, int width, int height) {
    int x = get_global_id(0);
    int y = get_global_id(1);
    
    int idx = y * width + x;

    if (x < width && y < height) {
        uchar4 color = image[idx];
        uchar gray = (uchar)(0.299f * color.x + 0.587f * color.y + 0.114f * color.z);
        image[idx] = (uchar4)(gray, gray, gray, color.w);
    }
}
