#ifndef MENUCOMMAND_HPP
#define MENUCOMMAND_HPP

#include "MenuItem.hpp"

class MenuCommand : public MenuItem
{
public:
	MenuCommand(std::string selector, std::string label, int enabled, void(*execFunc)()) : MenuItem(selector, label, enabled) { m_execFunc = execFunc; }
	~MenuCommand(){}

	std::string execute(){ m_execFunc(); return ""; }

private:
	void(*m_execFunc)();	// Function to call when menu command is executed
};

#endif