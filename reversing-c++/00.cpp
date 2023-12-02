#include <iostream>
#include <sstream>
class Human {
  int _age;
public:
  Human() { _age =0; }
  int get_age() const { return _age; }
  void set_age(int age) { this->_age = age; }
  virtual std::string speak() const {
    std::stringstream ss;
    ss << "I am " << _age << " years old.";
    return ss.str();
  }
};

int main(int argc, char** argv) {
  auto x = new Human();
  x->set_age(24);
  std::cout<<x->speak()<<std::endl;
  return 0;
}
