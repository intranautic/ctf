#include <iostream>

class polygon {
public:
  polygon(int width, int height)
    : m_width(width)
    , m_height(height)
  {
  }

  virtual ~polygon()=default;

  void set_width(int width) {
    m_width = width;
    return;
  }

  void set_height(int height) {
    m_height = height;
    return;
  }

  int get_width() const { return m_width; }

  int get_height() const { return m_height; }

  virtual int area() const=0;
  virtual int sides() const=0;
private:
  int m_width;
  int m_height;
};

class quad : public polygon {
  using _base = polygon;
public:
  quad(int width, int height)
    : _base(width, height)
  {
  }

  virtual int area() const override {
    return get_width() * get_height();
  }

  virtual int sides() const override {
    return 4;
  }
};

class triangle : public polygon {
  using _base = polygon;
public:
  triangle(int width, int height)
    : _base(width, height)
  {
  }

  virtual int area() const override {
    return (get_width() * get_height()) / 2;
  }

  virtual int sides() const override {
    return 3;
  }
};

void print_area(polygon& p) {
  std::cout << p.area() << std::endl;
  return;
}

void print_sides(polygon& p) {
  std::cout << p.sides() << std::endl;
  return;
}

int main(int argc, char** argv) {
  quad q(4, 5);
  triangle t(4, 5);

  print_area(q);
  print_sides(q);

  print_area(t);
  print_sides(t);
  return 0;
}
