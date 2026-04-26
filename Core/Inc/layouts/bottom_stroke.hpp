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
    static constexpr uint8_t Y_btm = 55;
    void UpdateSingleBattery(int16_t raw_data, char *out_v, char *out_p);

public:
    char batL_voltage[10];
    char batL_procent[10];
    char batR_voltage[10];
    char batR_procent[10];
    bool status;
    void Draw();
    void DataUpdate(const MemoryMap *system_data);
    int CalculatePercent(int voltage_str);

    BottomSTR()
    {
        strcpy(batL_voltage, "?");
        strcpy(batR_voltage, "?");
        strcpy(batL_procent, "-%");
        strcpy(batR_procent, "-%");
        status = true;
    }
};

int BottomSTR::CalculatePercent(int voltage_scale)
{
    int percent;
    if (voltage_scale >= 1680)
    {
        percent = 100;
    }
    else if (1640 <= voltage_scale)
    {
        percent = static_cast<int>(((voltage_scale - 1640) / 4) + 90);
    }
    else if (1600 <= voltage_scale)
    {
        percent = static_cast<int>(((voltage_scale - 1600) / 4) + 80);
    }
    else if (1560 <= voltage_scale)
    {
        percent = static_cast<int>(((voltage_scale - 1560) / 2) + 60);
    }
    else if (1530 <= voltage_scale)
    {
        percent = static_cast<int>(((voltage_scale - 1530) / 3) + 50);
    }
    else if (1500 <= voltage_scale)
    {
        percent = static_cast<int>(((voltage_scale - 1500) / 3) + 40);
    }
    else if (1480 <= voltage_scale)
    {
        percent = static_cast<int>(((voltage_scale - 1480) / 2) + 30);
    }
    else if (1440 <= voltage_scale)
    {
        percent = static_cast<int>(((voltage_scale - 1440) / 4) + 20);
    }
    else if (1200 <= voltage_scale)
    {
        percent = static_cast<int>(((voltage_scale - 1200) / 12) + 0);
    }
    else
    {
        percent = 0;
    }

    return percent;
}

void BottomSTR::UpdateSingleBattery(int16_t raw_value, char *out_v, char *out_p)
{
    if (raw_value <= 0)
    {
        strcpy(out_v, "?");
        strcpy(out_p, "-%");
    }
    else
    {
        snprintf(out_v, 16, "%d.%02d", raw_value / 100, raw_value % 100);

        int percent = CalculatePercent(raw_value);
        snprintf(out_p, 10, "%d%%", percent);
    }
}

void BottomSTR::DataUpdate(const MemoryMap *system_data)
{
    if (!system_data)
        return;

    UpdateSingleBattery(system_data->batL_voltage, this->batL_voltage, this->batL_procent);
    UpdateSingleBattery(system_data->batR_voltage, this->batR_voltage, this->batR_procent);
}

void BottomSTR::Draw()
{
    ssd1306_SetCursor(0, Y_btm);
    if (status)
        ssd1306_WriteString("OK", Font_6x8, White);
    else
        ssd1306_WriteString("ERR", Font_6x8, White);

    ssd1306_SetCursor(23, Y_btm);
    ssd1306_WriteString("BatL ", Font_6x8, White);
    ssd1306_SetCursor(49, Y_btm);
    ssd1306_WriteString(batL_procent, Font_6x8, White);

    ssd1306_SetCursor(76, Y_btm);
    ssd1306_WriteString("BatR ", Font_6x8, White);
    ssd1306_SetCursor(103, Y_btm);
    ssd1306_WriteString(batR_procent, Font_6x8, White);
}

#endif /* INC_BOTTOM_STROKE_HPP_ */
