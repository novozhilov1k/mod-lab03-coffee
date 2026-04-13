#include "Automata.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <sstream>
#include <iomanip>

Automata::Automata() : state(STATES::OFF), cash(0), selectedDrink(-1) {
    loadMenuFromFile("menu.txt");
}

Automata::Automata(const std::string& menuFile) : state(STATES::OFF), cash(0), selectedDrink(-1) {
    loadMenuFromFile(menuFile);
}

void Automata::setState(STATES newState) {
    state = newState;
}

void Automata::printMessage(const std::string& msg) const {
    std::cout << "[АВТОМАТ] " << msg << std::endl;
}

void Automata::returnChange() {
    if (cash > 0) {
        printMessage("Заберите сдачу: " + std::to_string(cash) + " руб.");
        cash = 0;
    }
}

void Automata::loadMenuFromFile(const std::string& filename) {
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        printMessage("Ошибка: не удалось загрузить меню из файла " + filename);
        menu = {
            "Эспрессо", "Американо", "Капучино", "Латте", 
            "Флэт Уайт", "Раф", "Моккачино", "Чай чёрный",
            "Чай зелёный", "Чай с бергамотом", "Матча латте",
            "Какао", "Горячий шоколад", "Молоко"
        };
        prices = {100, 120, 150, 150, 160, 180, 170, 80, 80, 90, 200, 130, 140, 70};
        return;
    }
    
    menu.clear();
    prices.clear();
    
    std::string line;
    while (std::getline(file, line)) {
        size_t delimiterPos = line.find(':');
        if (delimiterPos != std::string::npos) {
            std::string name = line.substr(0, delimiterPos);
            int price = std::stoi(line.substr(delimiterPos + 1));
            menu.push_back(name);
            prices.push_back(price);
        }
    }
    
    file.close();
    
    if (menu.empty()) {
        printMessage("Ошибка: меню пустое. Загружено меню по умолчанию.");
        menu = {
            "Эспрессо", "Американо", "Капучино", "Латте",
            "Флэт Уайт", "Раф", "Моккачино", "Чай чёрный",
            "Чай зелёный", "Чай с бергамотом", "Матча латте",
            "Какао", "Горячий шоколад", "Молоко"
        };
        prices = {100, 120, 150, 150, 160, 180, 170, 80, 80, 90, 200, 130, 140, 70};
    }
}

void Automata::showCookingProgress() {
    const int totalSteps = 20;
    const int stepDelay = 250;
    
    std::cout << "\n[АВТОМАТ] Приготовление напитка \"" << menu[selectedDrink] << "\"" << std::endl;
    
    for (int i = 1; i <= totalSteps; ++i) {
        int percentage = (i * 100) / totalSteps;
        
        std::cout << "\r[";
        int pos = (i * 20) / totalSteps;
        for (int j = 0; j < 20; ++j) {
            if (j < pos) std::cout << "=";
            else if (j == pos) std::cout << ">";
            else std::cout << " ";
        }
        std::cout << "] " << std::setw(3) << percentage << "%" << std::flush;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(stepDelay));
    }
    
    std::cout << std::endl;
}

void Automata::on() {
    if (state == STATES::OFF) {
        setState(STATES::WAIT);
        printMessage("Автомат включён. Внесите деньги.");
    } else {
        printMessage("Автомат уже включён.");
    }
}

void Automata::off() {
    if (state == STATES::WAIT) {
        setState(STATES::OFF);
        printMessage("Автомат выключен.");
    } else {
        printMessage("Нельзя выключить автомат во время обслуживания.");
    }
}

void Automata::coin(int value) {
    if (state == STATES::WAIT || state == STATES::ACCEPT) {
        if (value <= 0) {
            printMessage("Некорректная сумма.");
            return;
        }
        
        cash += value;
        printMessage("Внесено: " + std::to_string(value) + " руб. Всего: " + std::to_string(cash) + " руб.");
        
        if (state == STATES::WAIT) {
            setState(STATES::ACCEPT);
        }
    } else {
        printMessage("Сейчас нельзя внести деньги.");
    }
}

void Automata::cancel() {
    if (state == STATES::WAIT) {
        printMessage("Нет активной операции.");
    } else if (state == STATES::ACCEPT || state == STATES::CHECK) {
        printMessage("Операция отменена.");
        returnChange();
        setState(STATES::WAIT);
    } else {
        printMessage("Невозможно отменить операцию.");
    }
}

void Automata::choice(int index) {
    if (state != STATES::ACCEPT) {
        printMessage("Сначала внесите деньги.");
        return;
    }
    
    if (index < 0 || index >= static_cast<int>(menu.size())) {
        printMessage("Неверный номер напитка.");
        return;
    }
    
    selectedDrink = index;
    printMessage("Выбран напиток: " + menu[selectedDrink] + " (Цена: " + std::to_string(prices[selectedDrink]) + " руб.)");
    
    setState(STATES::CHECK);
    check();
}

void Automata::check() {
    if (state != STATES::CHECK) {
        return;
    }
    
    if (cash >= prices[selectedDrink]) {
        printMessage("Средств достаточно. Начинаю приготовление...");
        setState(STATES::COOK);
        cook();
    } else {
        int needed = prices[selectedDrink] - cash;
        printMessage("Недостаточно средств. Не хватает: " + std::to_string(needed) + " руб.");
        setState(STATES::ACCEPT);
    }
}

void Automata::cook() {
    if (state != STATES::COOK) {
        return;
    }
    
    showCookingProgress();
    finish();
}

void Automata::finish() {
    if (state != STATES::COOK) {
        return;
    }
    
    printMessage("Напиток \"" + menu[selectedDrink] + "\" готов! Можно забирать.");
    
    int change = cash - prices[selectedDrink];
    cash = 0;
    
    if (change > 0) {
        printMessage("Заберите сдачу: " + std::to_string(change) + " руб.");
    }
    
    selectedDrink = -1;
    setState(STATES::WAIT);
}

std::vector<std::string> Automata::getMenu() const {
    std::vector<std::string> result;
    for (size_t i = 0; i < menu.size(); ++i) {
        result.push_back(std::to_string(i + 1) + ". " + menu[i] + " - " + std::to_string(prices[i]) + " руб.");
    }
    return result;
}

STATES Automata::getState() const {
    return state;
}

int Automata::getCash() const {
    return cash;
}

std::string Automata::getStateString() const {
    switch (state) {
        case STATES::OFF:    return "OFF";
        case STATES::WAIT:   return "WAIT";
        case STATES::ACCEPT: return "ACCEPT";
        case STATES::CHECK:  return "CHECK";
        case STATES::COOK:   return "COOK";
        default:             return "UNKNOWN";
    }
}