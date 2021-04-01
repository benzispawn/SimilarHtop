#include "ncurses_display.h"
#include "system.h"
#include <iostream>

int main() {
  System system;
  // for (auto proc : system.Processes()) {
  //   std::cout << proc.User() << std::endl;
  // }
  NCursesDisplay::Display(system);
}