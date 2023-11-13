#ifndef MENUITEM_HPP
#define MENUITEM_HPP

#include <string>

class MenuItem {
public:
    MenuItem(const std::string& selector, const std::string& label, int enabled)
        : m_selector(selector), m_label(label), m_enabled(enabled) {}

    const std::string& getLabel() const { return m_label; }
    const std::string& getSelector() const { return m_selector; }
    int getEnabled() const { return m_enabled; }
    virtual std::string execute() = 0;

private:
    std::string m_selector;
    std::string m_label;
    int m_enabled;
};

#endif