/* Churkin Kirill st129364@student.spbu.ru
	Lab work 1
*/

#include "Image.h"
#include <cstdint>
#include <cmath>
#include <stdexcept>

Image_BMP::Image_BMP(const std::string& file_name) 
{
    load(file_name);
}

void Image_BMP::load(const std::string& filename) 
{
    std::ifstream file(filename, std::ios::binary);

    file.read(reinterpret_cast<char*>(&file_header), sizeof(BMP_Header));
    file.read(reinterpret_cast<char*>(&info_header), sizeof(BMP_Info));

    const int padding = (4 - (info_header.width * sizeof(Pixel)) % 4) % 4;

    lst.resize(info_header.width * abs(info_header.height));
    file.seekg(file_header.data_offset, std::ios::beg);

    for (int y = 0; y < abs(info_header.height); ++y) 
    {
        for (int x = 0; x < info_header.width; ++x) 
        {
            Pixel pixel;
            file.read(reinterpret_cast<char*>(&pixel), sizeof(Pixel));

            int dst_y = info_header.height > 0 ? (info_header.height - 1 - y) : y;
            lst[dst_y * info_header.width + x] = pixel;
        }
        file.seekg(padding, std::ios::cur);
    }
}

void Image_BMP::save(const std::string& filename) 
{
    std::ofstream file(filename, std::ios::binary);

    const int padding = (4 - (info_header.width * sizeof(Pixel)) % 4) % 4;
    const char padding_data[4] = {0, 0, 0, 0};

    file_header.file_size = sizeof(BMP_Header) + sizeof(BMP_Info) + 
                          (info_header.width * sizeof(Pixel) + padding) * abs(info_header.height);
    file_header.data_offset = sizeof(BMP_Header) + sizeof(BMP_Info);
    
    info_header.image_size = (info_header.width * sizeof(Pixel) + padding) * abs(info_header.height);
    info_header.height = abs(info_header.height);

    file.write(reinterpret_cast<const char*>(&file_header), sizeof(BMP_Header));
    file.write(reinterpret_cast<const char*>(&info_header), sizeof(BMP_Info));

    for (int y = info_header.height - 1; y >= 0; --y) 
    {
        const int row_start = y * info_header.width;
        file.write(reinterpret_cast<const char*>(&lst[row_start]), info_header.width * sizeof(Pixel));
        if (padding > 0) {
            file.write(padding_data, padding);
        }
    }
}

void Image_BMP::rotate_clockwise() 
{
    std::vector<Pixel> lst_new(abs(info_header.width * info_header.height));
    const int new_width = abs(info_header.height);
    const int new_height = info_header.width;
    const int height = abs(info_header.height);
    const int width = info_header.width;
    
    #pragma omp parallel for schedule(static)
    for (int y = 0; y < height; ++y) 
    {
        const int new_x = y;
        const int src_y_offset = y * width;
        for (int x = 0; x < width; ++x) 
        {
            const int new_y = new_width - 1 - x;
            lst_new[new_y * new_height + new_x] = lst[src_y_offset + x];
        }
    }

    lst = std::move(lst_new);
    info_header.width = new_width;
    info_header.height = new_height;
}

void Image_BMP::rotate_counter_clockwise() 
{
    std::vector<Pixel> lst_new(abs(info_header.width * info_header.height));
    const int new_width = abs(info_header.height);
    const int new_height = info_header.width;
    const int height = abs(info_header.height);
    const int width = info_header.width;

    #pragma omp parallel for schedule(static)
    for (int y = 0; y < height; ++y) 
    {
        const int new_y = y;
        const int src_y_offset = y * width;
        for (int x = 0; x < width; ++x) 
        {
            const int new_x = new_height - 1 - y;
            lst_new[new_y * new_height + new_x] = lst[src_y_offset + x];
        }
    }
    
    lst = std::move(lst_new);
    info_header.width = new_width;
    info_header.height = new_height;
}

void Image_BMP::Gauss_filter() 
{
    const float kernel[3][3] = {
        {1/16.0f, 2/16.0f, 1/16.0f},
        {2/16.0f, 4/16.0f, 2/16.0f},
        {1/16.0f, 2/16.0f, 1/16.0f}
    };

    std::vector<Pixel> lst_new(lst.size());
    const int height = info_header.height;
    const int width = info_header.width;

    for (int y = 1; y < height - 1; ++y) 
    {
        const int y_offset = y * width;
        for (int x = 1; x < width - 1; ++x) 
        {
            float r = 0, g = 0, b = 0;
            for (int ky = -1; ky <= 1; ++ky) 
            {
                const int ky_offset = (y + ky) * width;
                for (int kx = -1; kx <= 1; ++kx) 
                {
                    const int idx = ky_offset + (x + kx);
                    const float k = kernel[ky + 1][kx + 1];
                    r += lst[idx].red * k;
                    g += lst[idx].green * k;
                    b += lst[idx].blue * k;
                }
            }
            const int new_idx = y_offset + x;
            lst_new[new_idx].red   = static_cast<unsigned char>(std::min(std::max(r, 0.0f), 255.0f));
            lst_new[new_idx].green = static_cast<unsigned char>(std::min(std::max(g, 0.0f), 255.0f));
            lst_new[new_idx].blue  = static_cast<unsigned char>(std::min(std::max(b, 0.0f), 255.0f));
        }
    }

    lst = std::move(lst_new);
}

void Image_BMP::Gauss_filter_parallel() 
{
    const float kernel[3][3] = {
        {1/16.0f, 2/16.0f, 1/16.0f},
        {2/16.0f, 4/16.0f, 2/16.0f},
        {1/16.0f, 2/16.0f, 1/16.0f}
    };

    std::vector<Pixel> lst_new(lst.size());
    const int height = info_header.height;
    const int width = info_header.width;

    #pragma omp parallel for schedule(dynamic, 16)
    for (int y = 1; y < height - 1; ++y) 
    {
        const int y_offset = y * width;
        for (int x = 1; x < width - 1; ++x) 
        {
            float r = 0, g = 0, b = 0;
            for (int ky = -1; ky <= 1; ++ky) 
            {
                const int ky_offset = (y + ky) * width;
                for (int kx = -1; kx <= 1; ++kx) 
                {
                    const int idx = ky_offset + (x + kx);
                    const float k = kernel[ky + 1][kx + 1];
                    r += lst[idx].red * k;
                    g += lst[idx].green * k;
                    b += lst[idx].blue * k;
                }
            }
            const int new_idx = y_offset + x;
            lst_new[new_idx].red   = static_cast<unsigned char>(std::min(std::max(r, 0.0f), 255.0f));
            lst_new[new_idx].green = static_cast<unsigned char>(std::min(std::max(g, 0.0f), 255.0f));
            lst_new[new_idx].blue  = static_cast<unsigned char>(std::min(std::max(b, 0.0f), 255.0f));
        }
    }

    lst = std::move(lst_new);
}
