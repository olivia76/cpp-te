#include <iostream>

#include <vector>

#include "animal2.hpp"

using namespace pipeline;
using Animals = std::vector<Animal>;

void make_sounds(const Animals &animals) {
  std::cout << "Is it my cow?" << std::endl;
  for (const auto &animal : animals)
    animal.make_sound();
}

#include "animal2_visitor.hpp"

void draw_animals(const Animals &animals, const Visitor &drawer_visitor) {
  std::cout << std::endl << "Let's draw our animals..." << std::endl;
  for (const auto &animal : animals) {
    visit(drawer_visitor, animal);
  }
}

#include "opengldrawer.hpp"

int main() {
  std::cout << "Running demo..." << std::endl;

  auto animals = Animals({Sheep{}, Cow{}});

  make_sounds(animals);

  OpenGlDrawer drawer;
  auto openGLdrawer_visitor =
      Visitor::create([&drawer](const auto &shape) { drawer(shape); });
  draw_animals(animals, openGLdrawer_visitor);

  return 0;
}
