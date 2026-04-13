// Copyright 2025 GOSTOUH

#include <gtest/gtest.h>
#include <fstream>
#include "../include/Automata.h"

class AutomataTest : public ::testing::Test {
 protected:
  void SetUp() override {
    std::ofstream file("test_menu.txt");
    file << "Эспрессо:100\n";
    file << "Американо:120\n";
    file << "Капучино:150\n";
    file << "Латте:150\n";
    file << "Чай:80\n";
    file.close();
  }

  void TearDown() override {
    std::remove("test_menu.txt");
  }
};

TEST_F(AutomataTest, InitialStateIsOff) {
  Automata a("test_menu.txt");
  EXPECT_EQ(a.getState(), STATES::OFF);
}

TEST_F(AutomataTest, OnChangesStateToWait) {
  Automata a("test_menu.txt");
  a.on();
  EXPECT_EQ(a.getState(), STATES::WAIT);
}

TEST_F(AutomataTest, OffFromWaitChangesStateToOff) {
  Automata a("test_menu.txt");
  a.on();
  a.off();
  EXPECT_EQ(a.getState(), STATES::OFF);
}

TEST_F(AutomataTest, CoinFromWaitChangesStateToAccept) {
  Automata a("test_menu.txt");
  a.on();
  a.coin(50);
  EXPECT_EQ(a.getState(), STATES::ACCEPT);
}

TEST_F(AutomataTest, MultipleCoinsAccumulateCash) {
  Automata a("test_menu.txt");
  a.on();
  a.coin(50);
  a.coin(30);
  EXPECT_EQ(a.getCash(), 80);
}

TEST_F(AutomataTest, ChoiceWithNotEnoughMoneyStaysAccept) {
  Automata a("test_menu.txt");
  a.on();
  a.coin(50);
  a.choice(2);
  EXPECT_EQ(a.getState(), STATES::ACCEPT);
}

TEST_F(AutomataTest, CancelFromAcceptReturnsCashAndGoesToWait) {
  Automata a("test_menu.txt");
  a.on();
  a.coin(100);
  a.cancel();
  EXPECT_EQ(a.getState(), STATES::WAIT);
  EXPECT_EQ(a.getCash(), 0);
}

TEST_F(AutomataTest, CancelFromCheckReturnsCashAndGoesToWait) {
  Automata a("test_menu.txt");
  a.on();
  a.coin(50);
  a.choice(0);
  a.cancel();
  EXPECT_EQ(a.getState(), STATES::WAIT);
  EXPECT_EQ(a.getCash(), 0);
}

TEST_F(AutomataTest, ChoiceWithoutCoinDoesNothing) {
  Automata a("test_menu.txt");
  a.on();
  a.choice(0);
  EXPECT_EQ(a.getState(), STATES::WAIT);
}

TEST_F(AutomataTest, CoinWhenOffDoesNothing) {
  Automata a("test_menu.txt");
  a.coin(50);
  EXPECT_EQ(a.getState(), STATES::OFF);
  EXPECT_EQ(a.getCash(), 0);
}

TEST_F(AutomataTest, MenuLoadedCorrectly) {
  Automata a("test_menu.txt");
  auto menu = a.getMenu();
  EXPECT_EQ(menu.size(), 5);
}

TEST_F(AutomataTest, GetStateStringReturnsCorrectValue) {
  Automata a("test_menu.txt");
  EXPECT_EQ(a.getStateString(), "OFF");
  a.on();
  EXPECT_EQ(a.getStateString(), "WAIT");
}

TEST_F(AutomataTest, CheckTransitionsToCookWhenEnoughMoney) {
  Automata a("test_menu.txt");
  a.on();
  a.coin(100);
  a.choice(0);
  EXPECT_EQ(a.getState(), STATES::WAIT);
  EXPECT_EQ(a.getCash(), 0);
}

TEST_F(AutomataTest, FullPurchaseSequence) {
  Automata a("test_menu.txt");

  a.on();
  EXPECT_EQ(a.getState(), STATES::WAIT);

  a.coin(100);
  EXPECT_EQ(a.getState(), STATES::ACCEPT);

  a.coin(50);
  EXPECT_EQ(a.getCash(), 150);

  a.choice(2);
  EXPECT_EQ(a.getState(), STATES::WAIT);
  EXPECT_EQ(a.getCash(), 0);
}

TEST_F(AutomataTest, PurchaseWithChangeReturnsCorrectAmount) {
  Automata a("test_menu.txt");

  a.on();
  a.coin(200);
  a.choice(1);

  EXPECT_EQ(a.getState(), STATES::WAIT);
  EXPECT_EQ(a.getCash(), 0);
}
