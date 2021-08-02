struct Point {
  unsigned int x;
  unsigned int y;
  unsigned int z;

  Point set(unsigned int x, unsigned int y, unsigned int z=0) {
    this->x=x; this->y=y; this->z=z;
    return *this;
  }
  Point(unsigned int x, unsigned int y, unsigned int z=0)
  : x(x), y(y), z(z) {}
};