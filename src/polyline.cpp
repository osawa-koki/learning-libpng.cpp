#include <png.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>

#define OUTPUT_FILE "polyline.png"
#define WIDTH 250
#define HEIGHT 250
#define COLOR 0xff0000
#define BACKGROUND_COLOR 0x00f000
#define POINTS {50, 50, 100, 200, 200, 150}

int main() {
  int color = COLOR;
  int background_color = BACKGROUND_COLOR;
  std::vector<int> points = POINTS;

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

      for (int i = 0, j = points.size() - 2; i < points.size(); j = i, i += 2) {
        float x1 = points[i], y1 = points[i + 1], x2 = points[j], y2 = points[j + 1];

        // check if this pixel is on the line
        if ((x == x1 && y == y1) || (x == x2 && y == y2)) { // pixel is a vertex
          inside_line = true;
          break;
        }

        // calculate slope
        float slope = (y2 - y1) / (x2 - x1);

        // calculate y-intercept
        float y_intercept = y1 - slope * x1;

        // calculate y value at this x coordinate
        float y_on_line = slope * x + y_intercept;

        // check if this pixel is on the line
        if (std::abs(y_on_line - y) <= 1.0f && x >= std::min(x1, x2) && x <= std::max(x1, x2)) {
          inside_line = true;
          break;
        }
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
