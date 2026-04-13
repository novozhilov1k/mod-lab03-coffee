#ifndef INCLUDE_AUTOMATA_H_
#define INCLUDE_AUTOMATA_H_

#include <vector>
#include <string>

enum class STATES {
  OFF,
  WAIT,
  ACCEPT,
  CHECK,
  COOK
};

class Automata {
 private:
  STATES state;
  int cash;
  std::vector<std::string> menu;
  std::vector<int> prices;
  int selectedDrink;

  void setState(STATES newState);
  void printMessage(const std::string& msg) const;
  void returnChange();
  void loadMenuFromFile(const std::string& filename);
  void showCookingProgress();

 public:
  Automata();
  explicit Automata(const std::string& menuFile);

  void on();
  void off();
  void coin(int value);
  void cancel();
  void choice(int index);
  void check();
  void cook();
  void finish();

  std::vector<std::string> getMenu() const;
  STATES getState() const;
  int getCash() const;
  std::string getStateString() const;
};

#endif  // INCLUDE_AUTOMATA_H_
