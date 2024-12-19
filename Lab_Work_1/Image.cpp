/* Churkin Kirill st129364@student.spbu.ru
	Lab work 1
*/

#include "Image.h"

Image_BMP::Image_BMP(const std::string& file_name) 
{
    load(file_name);
}

void Image_BMP::load(const std::string& filename) 
{
    std::ifstream file(filename, std::ios::binary);
    file.read(reinterpret_cast<char*>(&file_header), sizeof(BMP_Header));
    file.read(reinterpret_cast<char*>(&info_header), sizeof(BMP_Info));

    lst.resize(info_header.width * info_header.height);
    file.seekg(file_header.data_offset, std::ios::beg);

    for (int y = 0; y < info_header.height; ++y) 
    {
        for (int x = 0; x < info_header.width; ++x) 
        {
            Pixel pixel;
            file.read(reinterpret_cast<char*>(&pixel), sizeof(Pixel));
            lst[(y * info_header.width) + x] = pixel;
        }
    }
}

void Image_BMP::save(const std::string& filename) 
{
    std::ofstream file(filename, std::ios::binary);
    file_header.file_size = sizeof(BMP_Header) + sizeof(BMP_Info) + lst.size() * sizeof(Pixel);
    info_header.image_size = lst.size() * sizeof(Pixel);

    file.write(reinterpret_cast<const char*>(&file_header), sizeof(BMP_Header));
    file.write(reinterpret_cast<const char*>(&info_header), sizeof(BMP_Info));
    file.write(reinterpret_cast<const char*>(lst.data()), lst.size() * sizeof(Pixel));
}

void Image_BMP::rotate_clockwise() 
{
    std::vector<Pixel> lst_new(info_header.width * info_header.height);
    int new_width = info_header.height;
    int new_height = info_header.width;

    for (int y = 0; y < info_header.height; ++y) 
    {
        for (int x = 0; x < info_header.width; ++x) 
        {
            int new_x = y;
            int new_y = new_width - 1 - x;
            lst_new[new_y * new_height + new_x] = lst[y * info_header.width + x];
        }
    }

    lst = std::move(lst_new);
    info_header.width = new_width;
    info_header.height = new_height;
}

void Image_BMP::rotate_counter_clockwise() 
{
    std::vector<Pixel> lst_new(info_header.width * info_header.height);
    int new_width = info_header.height;
    int new_height = info_header.width;

    for (int y = 0; y < info_header.height; ++y) 
    {
        for (int x = 0; x < info_header.width; ++x) 
        {
            int new_x = new_height - 1 - y;
            int new_y = x;
            lst_new[new_y * new_height + new_x] = lst[y * info_header.width + x];
        }
    }
    
    lst = std::move(lst_new);
    info_header.width = new_width;
    info_header.height = new_height;
}


void Image_BMP::Gauss_filter() 
{
    const float yadr[3][3] = {
        {1/16.0f, 2/16.0f, 1/16.0f},
        {2/16.0f, 4/16.0f, 2/16.0f},
        {1/16.0f, 2/16.0f, 1/16.0f}
    };

    std::vector<Pixel> lst_new(lst.size());

    for (int y = 1; y < info_header.height - 1; ++y) 
    {
        for (int x = 1; x < info_header.width - 1; ++x) 
        {
            float r = 0, g = 0, b = 0;
            for (int ky = -1; ky <= 1; ++ky) 
            {
                for (int kx = -1; kx <= 1; ++kx) 
                {
                    int pix_ind = ((y + ky) * info_header.width + (x + kx));
                    r += lst[pix_ind].red * yadr[ky + 1][kx + 1];
                    g += lst[pix_ind].green * yadr[ky + 1][kx + 1];
                    b += lst[pix_ind].blue * yadr[ky + 1][kx + 1];
                }
            }

            int new_pix_ind = (y * info_header.width + x);
            lst_new[new_pix_ind].red = std::min(std::max(static_cast<int>(r), 0), 255);
            lst_new[new_pix_ind].green = std::min(std::max(static_cast<int>(g), 0), 255);
            lst_new[new_pix_ind].blue = std::min(std::max(static_cast<int>(b), 0), 255);
        }
    }

    lst = std::move(lst_new);
}
