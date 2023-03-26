#include <png.h>
#include <iostream>
#include <vector>
#include <cstring>

#define OUTPUT_FILE "polygon.png"
#define WIDTH 250
#define HEIGHT 250
#define COLOR 0x000ff
#define BACKGROUND_COLOR 0xff0000
#define POLYGON_POINTS {50, 180, 100, 80, 150, 80, 200, 180}

int main() {
  int color = COLOR;
  int background_color = BACKGROUND_COLOR;

  std::vector<int> polygon_points = POLYGON_POINTS;

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
    bool inside_polygon = false;
    for (int x = 0; x < WIDTH; ++x) {
      inside_polygon = false;
      for (int i = 0, j = polygon_points.size() - 2; i < polygon_points.size(); j = i, i += 2) {
        if ((polygon_points[i + 1] > y) != (polygon_points[j + 1] > y) &&
          x < (polygon_points[j] - polygon_points[i]) * (y - polygon_points[i + 1]) /
              (polygon_points[j + 1] - polygon_points[i + 1]) +
            polygon_points[i]) {
          inside_polygon = !inside_polygon;
        }
      }

      if (inside_polygon) {
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

  std::cout << "Polygon saved to " << OUTPUT_FILE << std::endl;

  return 0;
}
