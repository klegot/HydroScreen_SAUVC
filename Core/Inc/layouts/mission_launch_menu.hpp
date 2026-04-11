#ifdef __cplusplus
extern "C"
{
#endif

#include "stdbool.h"
#include "string.h"

#ifdef __cplusplus
}
#endif

#include "main_menu.hpp"

#ifndef INC_LAYOUTS_MISSION_LAUNCH_MENU_HPP_
#define INC_LAYOUTS_MISSION_LAUNCH_MENU_HPP_

class MissionsMenu : public BaseMenu
{
private:
    static constexpr uint8_t MAX_MISSIONS = 4;
    char mission_names[MAX_MISSIONS][20];

public:
    uint8_t Y_curs;
    void Draw() override;
    void CursorUp() override;
    void CursorDown() override;
    void DrawCursor() override;
    int Enter() override;
    void DataUpdate(const MemoryMap *system_data) override;
    MenuType GetType() const override { return MISSIONS_MENU; }

    MissionsMenu()
    {
        for (int i = 0; i < MAX_MISSIONS; i++)
        {
            snprintf(mission_names[i], 16, "--no name--");
        }
        Y_curs = 0;
    }
};

void MissionsMenu::DataUpdate(const MemoryMap *system_data)
{
    for (int i = 0; i < MAX_MISSIONS; i++)
    {
        strncpy(this->mission_names[i], (char *)system_data->mission_names[i], 16);
        this->mission_names[i][15] = '\0';
    }
}

void MissionsMenu::CursorUp()
{
    if (Y_curs < MAX_MISSIONS - 1)
        Y_curs++;
    else
        Y_curs = 0;
    MissionsMenu::Draw();
}

void MissionsMenu::CursorDown()
{
    if (Y_curs > 0)
        Y_curs--;
    else
        Y_curs = MAX_MISSIONS - 1;
    MissionsMenu::Draw();
}

int MissionsMenu::Enter() { return Y_curs + 1; }

void MissionsMenu::DrawCursor()
{
    ssd1306_SetCursor(0, 12 + Y_curs * 10);
    ssd1306_WriteString(">", Font_7x10, White);
}

void MissionsMenu::Draw()
{
    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("missions:", Font_7x10, White);
    for (int i = 0; i < MAX_MISSIONS; i++)
    {
        ssd1306_SetCursor(10, 12 + i * 10);
        ssd1306_WriteString(mission_names[i], Font_6x8, White);
    }
    MissionsMenu::DrawCursor();
}

#endif /* INC_LAYOUTS_MISSION_LAUNCH_MENU_HPP_ */
