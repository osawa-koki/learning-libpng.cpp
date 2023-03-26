#include <png.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>

#define OUTPUT_FILE "ellipse.png"
#define WIDTH 250
#define HEIGHT 250
#define COLOR 0xff0000
#define BACKGROUND_COLOR 0x00f000
#define RX 100
#define RY 50
#define CX 125
#define CY 125

int main() {
  int color = COLOR;
  int background_color = BACKGROUND_COLOR;

  int cx = CX;
  int cy = CY;
  int rx = RX;
  int ry = RY;

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
      bool inside_ellipse = pow(x - cx, 2) / pow(rx, 2) + pow(y - cy, 2) / pow(ry, 2) <= 1.0;

      if (inside_ellipse) {
        png_bytep pixel = &(row[x * 3]);
        pixel[0] = (color >> 16) & 0xff; // Red
        pixel[1] = (color >> 8) & 0xff; // Green
        pixel[2] = (color >> 0) & 0xff; // Blue
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

  std::cout << "Ellipse saved to " << OUTPUT_FILE << std::endl;

  return 0;
}
