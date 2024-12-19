/* Churkin Kirill st129364@student.spbu.ru
	Lab work 1
*/

#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

#pragma pack(push,1)

struct BMP_Header 
{
    uint16_t file_type = 0x4D42;
    uint16_t reserve_1 = 0;
    uint16_t reserve_2 = 0;
    uint32_t file_size = 0;
    uint32_t data_offset = 0;
};

struct BMP_Info 
{
    uint32_t header_size = 0;
    int32_t width = 0;
    int32_t height = 0;
    uint16_t color_planes = 1;
    uint16_t bit_count = 24;
    uint32_t compression = 0;
    uint32_t image_size = 0;
    int32_t pixels_per_meter_x = 0;
    int32_t pixels_per_meter_y = 0;
    uint32_t colors_count = 0;
    uint32_t imp_colors_count = 0;
};

struct Pixel {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
};

#pragma pack(pop)

class Image_BMP 
{
public:
    Image_BMP(const std::string& file_name);
    
    void load(const std::string& file_name);
    void save(const std::string& file_name);
    void rotate_clockwise();
    void rotate_counter_clockwise();
    void Gauss_filter();
    
private:
    BMP_Header file_header;
    BMP_Info info_header;
    std::vector<Pixel> lst; 
};

#endif 
