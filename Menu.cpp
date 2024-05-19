#include "Menu.hpp"
#include "Account.hpp"
#include <iostream>
#include <string>
#include <exception>

// Add menu items
int Menu::addMenuItem(MenuItem *pMenuItem) {
    if (pMenuItem == nullptr) {
        std::cerr << "ERROR: Menu::addMenuItem cannot add menu item value with null pointer" << std::endl;
        return -1;
    }

    std::string selector = pMenuItem->getSelector();
    auto result = m_menuItemList.insert({selector, std::unique_ptr<MenuItem>(pMenuItem)});
    if (!result.second) {
        std::cerr << "ERROR: A menu item with selector \"" << selector << "\" already exists." << std::endl;
        return -1;
    }

    return 0;
}


std::string Menu::execute() {
    std::string selection;

    // Utilize the stack rather than heap where practical and safe
    Account acc;
    int accMenuOption = acc.checkAcc();

    while (selection != m_cmdExit) {
        std::cout << "\n" << this->getLabel() << "\n";

        for (const auto& [key, menuItem] : m_menuItemList) {
            // Skip nullptr check as it should be handled during insertion 
            if (accMenuOption == 1 && 
                (menuItem->getLabel() == "View Bank Account" || menuItem->getLabel() == "Edit Bank Account")) {
                menuItem->setEnabled(true);
            }
            if (menuItem->getEnabled()) {
                std::cout << menuItem->getSelector() << " - " << menuItem->getLabel() << "\n";
            }
        }

        std::cout << "Enter selection (or " << m_cmdExit << " to exit menu): ";
        std::cin >> selection;
        std::cout << "\n";

        if (selection == m_cmdExit) {
            break;
        }

        auto iter = m_menuItemList.find(selection);
        if (iter == m_menuItemList.end()) {
            std::cout << selection << " is not a valid menu selection. Try again.\n";
        } else {
            iter->second->execute();
        }
    }

    return selection;
}
