/* Churkin Kirill st129364@student.spbu.ru
	Lab work 1
*/

#include "Image.h"

int main() {
    Image_BMP image("Panda.bmp");

    image.rotate_clockwise();
    image.save("image_clockwise.bmp");

    image.rotate_counter_clockwise();
    image.rotate_counter_clockwise();
    image.save("image_counter_clockwise.bmp");

    image.rotate_clockwise();
    image.Gauss_filter();
    image.save("gaussian_filtered_image.bmp");

    std::cout << "Processing has been completed successfully!" << std::endl;
    return 0;
}
