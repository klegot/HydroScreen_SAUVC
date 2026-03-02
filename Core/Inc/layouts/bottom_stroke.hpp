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
	char batL_procent[5];
	char batR_voltage[16];
	char batR_procent[5];
	bool status;
	void Draw();
	void DataUpdate(const SystemData* system_data);
	int CalculatePercent(const char* voltage_str);

	BottomSTR()
	{
		strcpy(batL_voltage, "?");
	    strcpy(batR_voltage, "?");
	    strcpy(batL_procent, "99");
	    strcpy(batR_procent, "99");
		status = true;
		Y_btm = 55;
	}
};

int BottomSTR::CalculatePercent(const char* voltage_str)
{
    float voltage = atof(voltage_str);

    const float MIN_VOLTAGE = 0;  // полностью разряжен
    const float MAX_VOLTAGE = 16.8;  // полностью заряжен

    if (voltage < MIN_VOLTAGE) voltage = MIN_VOLTAGE;
    if (voltage > MAX_VOLTAGE) voltage = MAX_VOLTAGE;

    float percent_float = ((voltage - MIN_VOLTAGE) / (MAX_VOLTAGE - MIN_VOLTAGE)) * 100.0f;

    int percent = (int)(percent_float + 0.5f);

    if (percent < 0) percent = 0;
    if (percent > 100) percent = 100;

    return percent;
}

void BottomSTR::DataUpdate(const SystemData* system_data)
{
	strncpy(this->batL_voltage, system_data->batL_voltage, sizeof(this->batL_voltage) - 1);

	strncpy(this->batR_voltage, system_data->batR_voltage, sizeof(this->batR_voltage) - 1);

	int percentL = CalculatePercent(this->batL_voltage);
	int percentR = CalculatePercent(this->batR_voltage);

	snprintf(batL_procent, sizeof(batL_procent), "%d", percentL);
	snprintf(batR_procent, sizeof(batR_procent), "%d", percentR);
}

void BottomSTR::Draw()
{
	char percent_buffer[5];
	ssd1306_SetCursor(3, Y_btm);
	if (status) ssd1306_WriteString("OK", Font_6x8, White);
	else ssd1306_WriteString("ERROR", Font_6x8, White);
	ssd1306_SetCursor(30, Y_btm);
	ssd1306_WriteString("BatL", Font_6x8, White);
	ssd1306_SetCursor(55, Y_btm);
	strcpy(percent_buffer, batL_procent);
	strcat(percent_buffer, " ?");
	ssd1306_WriteString(batL_procent, Font_6x8, White);
	ssd1306_SetCursor(80, Y_btm);
	ssd1306_WriteString("BatR", Font_6x8, White);
	ssd1306_SetCursor(105, Y_btm);
	strcpy(percent_buffer, batR_procent);
	strcat(percent_buffer, " ?");
	ssd1306_WriteString(batR_procent, Font_6x8, White);
};

#endif /* INC_BOTTOM_STROKE_HPP_ */
