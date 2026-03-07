/*
 * mission_launch_menu.hpp
 *
 *  Created on: Mar 7, 2026
 *      Author: andrey
 */
#include <layouts/main_menu.hpp>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#ifndef INC_LAYOUTS_MISSION_LAUNCH_MENU_HPP_
#define INC_LAYOUTS_MISSION_LAUNCH_MENU_HPP_

#define MISSION_COUNT 5

class MissionsMenu : public BaseMenu
{
private:

	char mission_names[MISSION_COUNT][15];

public:
	uint8_t Y_curs;
	void Draw() override;
	void CursorUp() override;
	void CursorDown() override;
	void DrawCursor() override;
	int Enter() override;
	void Cancel();
	void DataUpdate(const SystemData* system_data) override;
	MenuType GetType() const override {return MISSIONS_MENU;}

	MissionsMenu()
	{
	    for (int i = 0; i < MISSION_COUNT; i++) {
	        strcpy(mission_names[i], "--no name--");
	    }
	    Y_curs = 0;
	}
};

void MissionsMenu::DataUpdate(const SystemData* system_data)
{
    for (int i = 0; i < MISSION_COUNT; i++)
    {
    	strcpy(this->mission_names[i], system_data->new_mission_names[i]);
    }
}

void MissionsMenu::CursorUp()
{
	if (Y_curs < count_way - 1) Y_curs++;
	else Y_curs = 0;
	MissionsMenu::Draw();
}

void MissionsMenu::CursorDown()
{
	if (Y_curs > 0) Y_curs--;
	else Y_curs = MISSION_COUNT - 1;
	MissionsMenu::Draw();
}

int MissionsMenu::Enter()
{
	return Y_curs + 1;
}

void MissionsMenu::Cancel()
{
}

void MissionsMenu::DrawCursor()
{
	ssd1306_SetCursor(0, Y_curs * 12);
	ssd1306_WriteString(">", Font_7x10, White);
}

void MissionsMenu::Draw()
{
	ssd1306_Fill(Black);
	ssd1306_SetCursor(0, 0);
	ssd1306_WriteString("missions:", Font_7x10, White);
	for (int i = 0; i < MISSION_COUNT; i ++)
	{
		ssd1306_SetCursor(10, 12 + i*10);
		ssd1306_WriteString(mission_names[i], Font_6x8, White);
	};
}

#endif /* INC_LAYOUTS_MISSION_LAUNCH_MENU_HPP_ */
