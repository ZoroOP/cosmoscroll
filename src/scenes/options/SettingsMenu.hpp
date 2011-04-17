#ifndef SETTINGSMENU_HPP
#define SETTINGSMENU_HPP

#include "scenes/BaseMenu.hpp"

class SettingsMenu: public BaseMenu
{
public:
	SettingsMenu();

private:
	void EventCallback(int id);

	gui::CheckBox* cb_fullscreen_;
	gui::OptionList* opt_languages_;
};

#endif // SETTINGSMENU_HPP
