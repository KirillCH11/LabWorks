/* Churkin Kirill st129364@student.spbu.ru
	Lab work 1
*/

#include "Image.h"
#include <chrono>
#include <iostream>

int main() {
    const std::vector<std::string> test_images = {"Panda_small.bmp", "Panda_middle.bmp", "Panda_large.bmp"};
    
    for (const auto& image_name : test_images) {
        std::cout << "Testing image: " << image_name << std::endl;
        
        {
            Image_BMP image(image_name);
            
            auto start = std::chrono::high_resolution_clock::now();
            image.rotate_clockwise();
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            std::cout << "  Rotate clockwise: " << elapsed.count() << "s" << std::endl;
            
            image.save("rotated_" + image_name);
        }
        
        {
            Image_BMP image(image_name);
            
            auto start = std::chrono::high_resolution_clock::now();
            image.Gauss_filter();
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            std::cout << "  Gauss filter (sequential): " << elapsed.count() << "s" << std::endl;
            
            start = std::chrono::high_resolution_clock::now();
            image.Gauss_filter_parallel();
            end = std::chrono::high_resolution_clock::now();
            elapsed = end - start;
            std::cout << "  Gauss filter (parallel): " << elapsed.count() << "s" << std::endl;
            
            image.save("filtered_" + image_name);
        }
    }

    return 0;
}
