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
#include <cstdlib>

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
	int batL_scale = std::atoi(system_data->batL_voltage);
	if ((batL_scale == 0 && strcmp(system_data->batL_voltage, "0") != 0) || strlen(system_data->batL_voltage) == 0)
	    {
	        strcpy(batL_voltage, "err");
	    }
	else
	    {
	    int integer_batL_voltage = batL_scale / 100;
	    int fractional_batL_voltage = batL_scale % 100;
	    sprintf(batL_voltage, "%d.%02d", integer_batL_voltage, fractional_batL_voltage);
	    }

	int batR_scale = std::atoi(system_data->batR_voltage);
	if ((batR_scale == 0 && strcmp(system_data->batR_voltage, "0") != 0) || strlen(system_data->batR_voltage) == 0)
	    {
	    strcpy(batR_voltage, "err");
	    }
	else
	    {
	    int integer_batR_voltage = batR_scale / 100;
	    int fractional_batR_voltage = batR_scale % 100;
	    sprintf(batR_voltage, "%d.%02d", integer_batR_voltage, fractional_batR_voltage);
	    } // TODO вынести это в отдельный метод
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
	snprintf(voltage_buffer, sizeof(voltage_buffer), "%s V", batL_voltage);
	ssd1306_WriteString(voltage_buffer, Font_7x10, White);
	ssd1306_SetCursor(10, 36);
	ssd1306_WriteString("BatR", Font_7x10, White);
	ssd1306_SetCursor(60, 36);
	snprintf(voltage_buffer, sizeof(voltage_buffer), "%s V", batR_voltage);
	ssd1306_WriteString(voltage_buffer, Font_7x10, White);
	DiagnosticsMenu::DrawCursor();
}


#endif /* INC_LAYOUTS_DIAGNOSTICS_MENU_HPP_ */
