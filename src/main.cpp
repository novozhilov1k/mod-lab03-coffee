// Copyright 2025 GOSTOUH

#include "Automata.h"
#include <iostream>
#include <thread>
#include <chrono>

void printSeparator() {
  std::cout << "\n========================================\n";
}

void wait(int seconds) {
  std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

int main() {
  std::cout << "=== ДЕМОНСТРАЦИЯ РАБОТЫ АВТОМАТА ПО ПРОДАЖЕ НАПИТКОВ ===\n";

  Automata aut("menu.txt");

  printSeparator();
  std::cout << "СЦЕНАРИЙ 1: Успешная покупка кофе\n";
  printSeparator();

  aut.on();
  wait(1);

  aut.coin(100);
  wait(1);

  aut.coin(50);
  wait(1);

  aut.choice(2);
  wait(6);

  printSeparator();
  std::cout << "СЦЕНАРИЙ 2: Недостаточно средств\n";
  printSeparator();

  aut.coin(100);
  wait(1);

  aut.choice(4);
  wait(1);

  aut.coin(100);
  wait(1);

  aut.choice(4);
  wait(6);

  printSeparator();
  std::cout << "СЦЕНАРИЙ 3: Отмена операции\n";
  printSeparator();

  aut.coin(200);
  wait(1);

  aut.cancel();
  wait(1);

  printSeparator();
  std::cout << "СЦЕНАРИЙ 4: Покупка чая и сдача\n";
  printSeparator();

  aut.coin(100);
  wait(1);

  aut.choice(7);
  wait(6);

  printSeparator();
  std::cout << "СЦЕНАРИЙ 5: Выключение автомата\n";
  printSeparator();

  aut.off();
  wait(1);

  aut.on();
  wait(1);

  aut.off();

  printSeparator();
  std::cout << "ДЕМОНСТРАЦИЯ ЗАВЕРШЕНА!\n";
  printSeparator();

  return 0;
}
