/*
 * diagnostics_menu.hpp
 *
 *  Created on: Dec 19, 2025
 *      Author: andrey
 */

#ifndef INC_LAYOUTS_DIAGNOSTICS_MENU_HPP_
#define INC_LAYOUTS_DIAGNOSTICS_MENU_HPP_


#include <layouts/main_menu.hpp>

#include <stdbool.h>
#include <string.h>
#include <stdio.h>

class DiagnosticsMenu : public BaseMenu
{
public:
	char batL_voltage[16];
	char batR_voltage[16];
	bool light_status;
	uint8_t Y_curs;
	void Draw() override;
	void CursorUp() override;
	void CursorDown() override;
	void DrawCursor() override;
	int Enter() override;
	void Cancel();
	void DataUpdate(const SystemData* system_data) override;
	MenuType GetType() const override {return DIAGNOSTICS_MENU;}


	DiagnosticsMenu()
	{
		Y_curs = 0;
		strcpy(batL_voltage, "?");
		strcpy(batR_voltage, "?");
		light_status = false;
	}
};

void DiagnosticsMenu::DataUpdate(const SystemData* system_data)
{
	strncpy(this->batL_voltage, system_data->batL_voltage, sizeof(this->batL_voltage) - 1);

	strncpy(this->batR_voltage, system_data->batR_voltage, sizeof(this->batR_voltage) - 1);

	light_status = system_data->light_status;
}

void DiagnosticsMenu::CursorUp()
{
	/*if (Y_curs < count_way - 1) Y_curs++;
	else Y_curs = 0;
	DiagnosticsMenu::Draw();*/
}

void DiagnosticsMenu::CursorDown()
{
	/*if (Y_curs > 0) Y_curs--;
	else Y_curs = count_way - 1;
	DiagnosticsMenu::Draw();*/
}

int DiagnosticsMenu::Enter()
{
	return 1;
}

void DiagnosticsMenu::Cancel()
{

}

void DiagnosticsMenu::DrawCursor()
{
	ssd1306_SetCursor(0, Y_curs * 12);
	ssd1306_WriteString(">", Font_7x10, White);
}

void DiagnosticsMenu::Draw()
{
	char voltage_buffer[32];
	ssd1306_Fill(Black);
	ssd1306_SetCursor(10, 0);
	ssd1306_WriteString("VMA status", Font_7x10, White);
	ssd1306_SetCursor(10, 12);
	ssd1306_WriteString("lights", Font_7x10, White);
	ssd1306_SetCursor(60, 12);
	if (light_status) {ssd1306_WriteString("ON", Font_7x10, White);}
	else {ssd1306_WriteString("OFF", Font_7x10, White);}
	ssd1306_SetCursor(10, 24);
	ssd1306_WriteString("BatL", Font_7x10, White);
	ssd1306_SetCursor(60, 24);
	strcpy(voltage_buffer, batL_voltage);
	strcat(voltage_buffer, " V");
	ssd1306_WriteString(voltage_buffer, Font_7x10, White);
	ssd1306_SetCursor(10, 36);
	ssd1306_WriteString("BatR", Font_7x10, White);
	ssd1306_SetCursor(60, 36);
	strcpy(voltage_buffer, batR_voltage);
	strcat(voltage_buffer, " V");
	ssd1306_WriteString(voltage_buffer, Font_7x10, White);
	DiagnosticsMenu::DrawCursor();
}


#endif /* INC_LAYOUTS_DIAGNOSTICS_MENU_HPP_ */
