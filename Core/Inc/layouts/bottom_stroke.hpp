/*
 * bottom_stroke.hpp
 *
 *  Created on: Dec 16, 2025
 *      Author: andrey
 */

#include <layouts/main_menu.hpp>


#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <cstdlib>

#ifndef INC_BOTTOM_STROKE_HPP_
#define INC_BOTTOM_STROKE_HPP_


class BottomSTR
{
public:
	uint8_t Y_btm;
	char batL_voltage[16];
	char batL_procent[10];
	char batR_voltage[16];
	char batR_procent[10];
	bool status;
	void Draw();
	void DataUpdate(const SystemData* system_data);
	int CalculatePercent(int voltage_str);

	BottomSTR()
	{
		strcpy(batL_voltage, "?");
	    strcpy(batR_voltage, "?");
	    strcpy(batL_procent, "-%");
	    strcpy(batR_procent, "-%");
		status = true;
		Y_btm = 55;
	}
};

int BottomSTR::CalculatePercent(int voltage_scale)
{
    const int MIN_VOLTAGE = 0;  // полностью разряжен
    const int MAX_VOLTAGE = 17 * 100;  // полностью заряжен

    if (voltage_scale < MIN_VOLTAGE) voltage_scale = MIN_VOLTAGE;
    if (voltage_scale > MAX_VOLTAGE) voltage_scale = MAX_VOLTAGE;

    int percent = static_cast<int>((voltage_scale * 100) / (MAX_VOLTAGE - MIN_VOLTAGE));

    return percent;
}

void BottomSTR::DataUpdate(const SystemData* system_data)
{
    int batL_scale = std::atoi(system_data->batL_voltage);
    if ((batL_scale == 0 && strcmp(system_data->batL_voltage, "0") != 0) ||
        strlen(system_data->batL_voltage) == 0)
    {
        strcpy(batL_voltage, "err");
        strcpy(batL_procent, "-%");
    }
    else
    {
    	int integer_batL_voltage = batL_scale / 100;
    	int fractional_batL_voltage = batL_scale % 100;
    	sprintf(batL_voltage, "%d.%02d", integer_batL_voltage, fractional_batL_voltage);
        int percentL = CalculatePercent(batL_scale);
        snprintf(batL_procent, sizeof(batL_procent), "%d%%", percentL);
    }

    int batR_scale = std::atoi(system_data->batR_voltage);
    if ((batR_scale == 0 && strcmp(system_data->batR_voltage, "0") != 0) ||
        strlen(system_data->batR_voltage) == 0)
        {
            strcpy(batR_voltage, "err");
            strcpy(batR_procent, "-%");
        }
    else
    {
    	int integer_batR_voltage = batR_scale / 100;
    	int fractional_batR_voltage = batR_scale % 100;
    	sprintf(batR_voltage, "%d.%02d", integer_batR_voltage, fractional_batR_voltage);
        int percentR = CalculatePercent(batR_scale);
        snprintf(batR_procent, sizeof(batR_procent), "%d%%", percentR);
    } // TODO убрать это в отдельный метод
}

void BottomSTR::Draw()
{
    ssd1306_SetCursor(3, Y_btm);
    if (status) ssd1306_WriteString("OK", Font_6x8, White);
    else ssd1306_WriteString("ERROR", Font_6x8, White);

    ssd1306_SetCursor(30, Y_btm);
    ssd1306_WriteString("BatL", Font_6x8, White);
    ssd1306_SetCursor(55, Y_btm);
    ssd1306_WriteString(batL_procent, Font_6x8, White);

    ssd1306_SetCursor(80, Y_btm);
    ssd1306_WriteString("BatR", Font_6x8, White);
    ssd1306_SetCursor(105, Y_btm);
    ssd1306_WriteString(batR_procent, Font_6x8, White);
}

#endif /* INC_BOTTOM_STROKE_HPP_ */
