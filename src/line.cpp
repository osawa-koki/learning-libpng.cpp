#include <png.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>

#define OUTPUT_FILE "line.png"
#define WIDTH 250
#define HEIGHT 250
#define COLOR 0xff0000
#define BACKGROUND_COLOR 0x00f000
#define X1 50
#define Y1 50
#define X2 200
#define Y2 200

int main() {
  int color = COLOR;
  int background_color = BACKGROUND_COLOR;

  int x1 = X1, y1 = Y1, x2 = X2, y2 = Y2;

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
      bool inside_line = false;

      // calculate slope
      float slope = (float)(y2 - y1) / (float)(x2 - x1);

      // calculate y-intercept
      float y_intercept = y1 - slope * x1;

      // calculate y value at this x coordinate
      float y_on_line = slope * x + y_intercept;

      // check if this pixel is on the line
      if (std::abs(y_on_line - y) <= 1.0f) {
        inside_line = true;
      }

      if (inside_line) {
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

  std::cout << "Line saved to " << OUTPUT_FILE << std::endl;

  return 0;
}
