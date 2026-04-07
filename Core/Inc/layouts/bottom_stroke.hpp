#ifdef __cplusplus
extern "C"
{
#endif

#include <cstdlib>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
}
#endif

#include "main_menu.hpp"

#ifndef INC_BOTTOM_STROKE_HPP_
#define INC_BOTTOM_STROKE_HPP_

class BottomSTR
{
private:
    void UpdateSingleBattery(int16_t raw_data, char *out_v, char *out_p);

public:
    uint8_t Y_btm;
    char batL_voltage[16];
    char batL_procent[10];
    char batR_voltage[16];
    char batR_procent[10];
    bool status;
    void Draw();
    void DataUpdate(const SystemData *system_data);
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
    const int MIN_VOLTAGE = 0;        // полностью разряжен
    const int MAX_VOLTAGE = 17 * 100; // полностью заряжен

    if (voltage_scale < MIN_VOLTAGE)
        voltage_scale = MIN_VOLTAGE;
    if (voltage_scale > MAX_VOLTAGE)
        voltage_scale = MAX_VOLTAGE;

    int percent = static_cast<int>((voltage_scale * 100) / (MAX_VOLTAGE - MIN_VOLTAGE));

    return percent;
}

void BottomSTR::UpdateSingleBattery(int16_t raw_value, char *out_v, char *out_p)
{
    // Если значение 0 или отрицательное, считаем, что данных нет
    if (raw_value <= 0)
    {
        strcpy(out_v, "?");
        strcpy(out_p, "-%");
    }
    else
    {
        // Формируем строку напряжения: 1250 -> "12.50"
        snprintf(out_v, 16, "%d.%02d", raw_value / 100, raw_value % 100);

        // Считаем проценты
        int percent = CalculatePercent(raw_value);
        snprintf(out_p, 10, "%d%%", percent);
    }
}

void BottomSTR::DataUpdate(const SystemData *system_data)
{
    if (!system_data)
        return;

    // Передаем числа напрямую из структуры
    UpdateSingleBattery(system_data->batL_voltage, this->batL_voltage, this->batL_procent);
    UpdateSingleBattery(system_data->batR_voltage, this->batR_voltage, this->batR_procent);
}

void BottomSTR::Draw()
{
    ssd1306_SetCursor(3, Y_btm);
    if (status)
        ssd1306_WriteString("OK", Font_6x8, White);
    else
        ssd1306_WriteString("ERROR", Font_6x8, White);

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
