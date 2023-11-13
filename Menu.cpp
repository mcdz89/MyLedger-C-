#include "Menu.hpp"
#include <iostream>
#include <string>
#include <exception>

// Add menu items
int Menu::addMenuItem(MenuItem *pMenuItem) {
    try {
        if (pMenuItem == nullptr) {
            throw std::invalid_argument("ERROR: Menu::addMenuItem cannot add menu item value with null pointer");
        }

        std::string selector = pMenuItem->getSelector();
        if (m_menuItemList.find(selector) == m_menuItemList.end() && pMenuItem->getEnabled() == 1)
            m_menuItemList.insert({selector, std::unique_ptr<MenuItem>(pMenuItem)});

        return 0;
    } catch(const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
        return -1;
    }
}


std::string Menu::execute() {
    std::string selection = "";
    MenuItem *pMenuItem;

    while (selection != m_cmdExit) {
        std::cout << "\n" << this->getLabel() << "\n";

        for (const auto& [key, value] : m_menuItemList) {
            pMenuItem = value.get();
            if (pMenuItem == nullptr) {
                std::cout << "ERROR: Menu::execute() menu item is null on menu " << this->getSelector() << "\n";
            } else {
                std::cout << pMenuItem->getSelector() << " - " << pMenuItem->getLabel() << "\n";
            }
        }

        std::cout << "Enter selection (or " << m_cmdExit << " to exit menu): ";
        std::cin >> selection;
        std::cout << "\n";

        if (selection == "q" || selection == "Q") {
            continue;
        }

        auto iter = m_menuItemList.find(selection);
        if (iter == m_menuItemList.end()) {
            std::cout << selection << " is not a valid menu selection. Try again.\n";
        } else {
            pMenuItem = iter->second.get();
            pMenuItem->execute();
        }
    }

    return selection;
}
