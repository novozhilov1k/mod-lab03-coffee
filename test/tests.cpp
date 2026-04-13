#include <gtest/gtest.h>
#include <fstream>
#include "../include/Automata.h"

void createTestMenuFile() {
    std::ofstream file("test_menu.txt");
    file << "Эспрессо:100\n";
    file << "Американо:120\n";
    file << "Капучино:150\n";
    file << "Латте:150\n";
    file << "Чай:80\n";
    file.close();
}

TEST(AutomataTest, InitialStateIsOff) {
    Automata a("test_menu.txt");
    EXPECT_EQ(a.getState(), STATES::OFF);
}

TEST(AutomataTest, OnChangesStateToWait) {
    Automata a("test_menu.txt");
    a.on();
    EXPECT_EQ(a.getState(), STATES::WAIT);
}

TEST(AutomataTest, OffFromWaitChangesStateToOff) {
    Automata a("test_menu.txt");
    a.on();
    a.off();
    EXPECT_EQ(a.getState(), STATES::OFF);
}

TEST(AutomataTest, CoinFromWaitChangesStateToAccept) {
    Automata a("test_menu.txt");
    a.on();
    a.coin(50);
    EXPECT_EQ(a.getState(), STATES::ACCEPT);
}

TEST(AutomataTest, MultipleCoinsAccumulateCash) {
    Automata a("test_menu.txt");
    a.on();
    a.coin(50);
    a.coin(30);
    EXPECT_EQ(a.getCash(), 80);
}

TEST(AutomataTest, ChoiceWithEnoughMoneyGoesToCook) {
    Automata a("test_menu.txt");
    a.on();
    a.coin(100);
    a.choice(0);
    EXPECT_EQ(a.getState(), STATES::COOK);
}

TEST(AutomataTest, ChoiceWithNotEnoughMoneyStaysAccept) {
    Automata a("test_menu.txt");
    a.on();
    a.coin(50);
    a.choice(2);
    EXPECT_EQ(a.getState(), STATES::ACCEPT);
}

TEST(AutomataTest, CancelFromAcceptReturnsCashAndGoesToWait) {
    Automata a("test_menu.txt");
    a.on();
    a.coin(100);
    a.cancel();
    EXPECT_EQ(a.getState(), STATES::WAIT);
    EXPECT_EQ(a.getCash(), 0);
}

TEST(AutomataTest, CancelFromCheckReturnsCashAndGoesToWait) {
    Automata a("test_menu.txt");
    a.on();
    a.coin(50);
    a.choice(0);
    a.cancel();
    EXPECT_EQ(a.getState(), STATES::WAIT);
    EXPECT_EQ(a.getCash(), 0);
}

TEST(AutomataTest, FinishResetsStateToWait) {
    Automata a("test_menu.txt");
    a.on();
    a.coin(100);
    a.choice(0);
    a.finish();
    EXPECT_EQ(a.getState(), STATES::WAIT);
    EXPECT_EQ(a.getCash(), 0);
}

TEST(AutomataTest, ChoiceWithoutCoinDoesNothing) {
    Automata a("test_menu.txt");
    a.on();
    a.choice(0);
    EXPECT_EQ(a.getState(), STATES::WAIT);
}

TEST(AutomataTest, CoinWhenOffDoesNothing) {
    Automata a("test_menu.txt");
    a.coin(50);
    EXPECT_EQ(a.getState(), STATES::OFF);
    EXPECT_EQ(a.getCash(), 0);
}

TEST(AutomataTest, MenuLoadedCorrectly) {
    Automata a("test_menu.txt");
    auto menu = a.getMenu();
    EXPECT_EQ(menu.size(), 5);
}

TEST(AutomataTest, GetStateStringReturnsCorrectValue) {
    Automata a("test_menu.txt");
    EXPECT_EQ(a.getStateString(), "OFF");
    a.on();
    EXPECT_EQ(a.getStateString(), "WAIT");
}

TEST(AutomataTest, FullPurchaseSequence) {
    Automata a("test_menu.txt");
    
    a.on();
    EXPECT_EQ(a.getState(), STATES::WAIT);
    
    a.coin(100);
    EXPECT_EQ(a.getState(), STATES::ACCEPT);
    
    a.coin(50);
    EXPECT_EQ(a.getCash(), 150);
    
    a.choice(2);
    EXPECT_EQ(a.getState(), STATES::COOK);
    
    a.finish();
    EXPECT_EQ(a.getState(), STATES::WAIT);
    EXPECT_EQ(a.getCash(), 0);
}

int main(int argc, char **argv) {
    createTestMenuFile();
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    std::remove("test_menu.txt");
    return result;
}