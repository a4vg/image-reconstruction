#ifndef COLORS_CPP
#define COLORS_CPP

struct Color {float r; float g; float b;}; 

namespace colors {
  static Color pink     = {1.0f, 0.0f, 1.0f};
  static Color blue     = {0.0f, 0.0f, 1.0f};
  static Color darkBlue = {0.0f, 0.0f, 0.4f};
  static Color red      = {1.0f, 0.0f, 0.0f};
  static Color green    = {0.0f, 1.0f, 0.0f};
  static Color yellow   = {1.0f, 1.0f, 0.0f};
  static Color white    = {1.0f, 1.0f, 1.0f};
  static Color brown    = {0.7f, 0.6f, 0.6f};
  static Color black    = {0.0f, 0.0f, 0.0f};
}

#endif // COLORS_CPP
