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
    static constexpr uint8_t MAX_MISSION_COUNT = 4;
    static constexpr uint8_t MAX_LINES_ON_SCREEN = 3;
    char mission_names[MAX_MISSION_COUNT][20];

public:
    uint8_t Y_curs;
    uint8_t offset;
    void Draw() override;
    void CursorDown() override;
    void CursorUp() override;
    void DrawCursor() override;
    void Reset() override;
    int Enter() override;
    void DataUpdate(const MemoryMap *system_data) override;
    MenuType GetType() const override { return MISSIONS_MENU; }

    MissionsMenu()
    {
        for (int i = 0; i < MAX_MISSION_COUNT; i++)
        {
            snprintf(mission_names[i], 16, "--no name--");
        }
        Y_curs = 0;
        offset = 0;
    }
};

void MissionsMenu::DataUpdate(const MemoryMap *system_data)
{
    for (int i = 0; i < MAX_MISSION_COUNT; i++)
    {
        strncpy(this->mission_names[i], (char *)system_data->mission_names[i], 16);
        this->mission_names[i][15] = '\0';
    }
}

void MissionsMenu::CursorDown()
{
    if (Y_curs < MAX_LINES_ON_SCREEN - 1 && (Y_curs + offset * MAX_LINES_ON_SCREEN) < MAX_MISSION_COUNT - 1)
    {
        Y_curs++;
    }
    else if (offset < 1)
    {
        Y_curs = 0;
        offset++;
    }
    else
    {
        Y_curs = 0;
        offset = 0;
    }
    MissionsMenu::Draw();
}

void MissionsMenu::CursorUp()
{
    if (Y_curs > 0 && offset < 1)
    {
        Y_curs--;
    }
    else if (Y_curs == 0 && offset > 0)
    {
        Y_curs = MAX_LINES_ON_SCREEN - 1;
        offset--;
    }
    MissionsMenu::Draw();
}

void MissionsMenu::Reset()
{
    Y_curs = 0;
    offset = 0;
}

int MissionsMenu::Enter() { return Y_curs + 1 + offset * MAX_LINES_ON_SCREEN; }

void MissionsMenu::DrawCursor()
{
    ssd1306_SetCursor(0, 12 + Y_curs * 12);
    ssd1306_WriteString(">", Font_7x10, White);
}

void MissionsMenu::Draw()
{
    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("missions:", Font_7x10, White);
    if (offset < 1)
    {
        for (int i = 0; i < MAX_LINES_ON_SCREEN; i++)
        {
            ssd1306_SetCursor(10, 12 + i * 12);
            ssd1306_WriteString(mission_names[i], Font_7x10, White);
        }
    }
    else if (offset > 0)
    {
        uint8_t j = 0;
        for (int i = offset * MAX_LINES_ON_SCREEN; i < MAX_MISSION_COUNT; i++)
        {
            ssd1306_SetCursor(10, 12 + j * 12);
            ssd1306_WriteString(mission_names[i], Font_7x10, White);
            j++;
        }
    }
    MissionsMenu::DrawCursor();
}

#endif /* INC_LAYOUTS_MISSION_LAUNCH_MENU_HPP_ */
