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

#ifndef INC_LAYOUTS_DIAGNOSTICS_MENU_HPP_
#define INC_LAYOUTS_DIAGNOSTICS_MENU_HPP_

class DiagnosticsMenu : public BaseMenu
{
private:
    void FormatVoltage(int16_t raw_data, char *out_v);

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
    void DataUpdate(const SystemData *system_data) override;
    MenuType GetType() const override { return DIAGNOSTICS_MENU; }

    DiagnosticsMenu()
    {
        Y_curs = 0;
        strcpy(batL_voltage, "?");
        strcpy(batR_voltage, "?");
        light_status = false;
    }
};

void DiagnosticsMenu::FormatVoltage(int16_t raw_value, char *out_v)
{
    // Если используем 0 как признак отсутствия данных
    if (raw_value <= 0)
    {
        strcpy(out_v, "?");
    }
    else
    {
        // Форматируем число 1250 -> "12.50"
        // Делим на 100 для целой части, остаток от деления на 100 для сотых
        snprintf(out_v, 16, "%d.%02d", raw_value / 100, raw_value % 100);
    }
}

void DiagnosticsMenu::DataUpdate(const SystemData *system_data)
{
    if (!system_data)
        return;

    FormatVoltage(system_data->batL_voltage, this->batL_voltage);
    FormatVoltage(system_data->batR_voltage, this->batR_voltage);

    this->light_status = system_data->light_status;
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

int DiagnosticsMenu::Enter() { return 1; }

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
    if (light_status)
    {
        ssd1306_WriteString("ON", Font_7x10, White);
    }
    else
    {
        ssd1306_WriteString("OFF", Font_7x10, White);
    }
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
