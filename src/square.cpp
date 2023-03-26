#include <png.h>
#include <iostream>
#include <vector>
#include <cstring>

#define OUTPUT_FILE "square.png"
#define WIDTH 250
#define HEIGHT 250
#define COLOR 0xff0000  // purple color
#define BACKGROUND_COLOR 0xffff00 // yellow color
#define SQUARE_X 50     // X-coordinate of top-left corner of square
#define SQUARE_Y 50     // Y-coordinate of top-left corner of square
#define SQUARE_SIZE 100 // Size of square in pixels

int main() {
  int color = COLOR;
  int background_color = BACKGROUND_COLOR;
  
  FILE* fp = fopen(OUTPUT_FILE, "wb");
  if (!fp) {
    std::cout << "Error: Failed to create file" << std::endl;
    return -1;
  }

  png_structp png_ptr;
  png_infop info_ptr;
  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  info_ptr = png_create_info_struct(png_ptr);
  png_init_io(png_ptr, fp);

  png_set_IHDR(png_ptr, info_ptr, WIDTH, HEIGHT, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
         PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
  png_write_info(png_ptr, info_ptr);

  std::vector<png_byte> row(3 * WIDTH);
  for (int y = 0; y < HEIGHT; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      if (x >= SQUARE_X && x < SQUARE_X + SQUARE_SIZE && y >= SQUARE_Y && y < SQUARE_Y + SQUARE_SIZE) {
        png_bytep pixel = &(row[x * 3]);
        pixel[0] = (color >> 16) & 0xff; // Red
        pixel[1] = (color >> 8) & 0xff;  // Green
        pixel[2] = (color >> 0) & 0xff;         // Blue
      } else {
        png_bytep pixel = &(row[x * 3]);
        pixel[0] = (background_color >> 16) & 0xff; // Red
        pixel[1] = (background_color >> 8) & 0xff; // Green
        pixel[2] = (background_color >> 0) & 0xff; // Blue
      }
    }

    png_write_row(png_ptr, &row[0]);
  }

  png_write_end(png_ptr, NULL);
  png_destroy_write_struct(&png_ptr, &info_ptr);
  fclose(fp);

  std::cout << "Square saved to " << OUTPUT_FILE << std::endl;

  return 0;
}
