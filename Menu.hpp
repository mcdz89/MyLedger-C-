#ifndef MENU_HPP
#define MENU_HPP

#include <string>
#include <map>
#include <memory>
#include "MenuItem.hpp"

class Menu : public MenuItem {
public:
    Menu(const std::string& selector, const std::string& label, int enabled, std::string cmdExit)
        : MenuItem(selector, label, enabled) {m_cmdExit = cmdExit;}
    ~Menu(){}

    int addMenuItem(MenuItem *menuItem);
    std::string execute();

private:
    std::string m_cmdExit;		// Selector to exit this menu
    std::map<std::string, std::unique_ptr<MenuItem>> m_menuItemList;
};

#endif