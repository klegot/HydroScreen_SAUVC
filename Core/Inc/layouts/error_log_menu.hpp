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

class ErrorLogMenu : public BaseMenu
{
private:
    static constexpr uint8_t MAX_LOGS = 4;
    char error_logs[MAX_LOGS][20];

public:
    void Draw() override;
    void CursorUp() override;
    void CursorDown() override;
    void DrawCursor() override;
    int Enter() override;
    void DataUpdate(const MemoryMap::SystemData *system_data) override;
    MenuType GetType() const override { return ERROR_LOG_MENU; }

    ErrorLogMenu()
    {
        for (int i = 0; i < MAX_LOGS; i++)
        {
            snprintf(error_logs[i], 16, "---");
        }
    }
};

void ErrorLogMenu::DataUpdate(const MemoryMap::SystemData *system_data)
{
    if (!system_data)
        return;

    for (int i = 0; i < MAX_LOGS; i++)
    {
        memcpy(this->error_logs[i], system_data->error_logs[i], 16);
        this->error_logs[i][16] = '\0';
    }
}

void ErrorLogMenu::CursorUp() {}

void ErrorLogMenu::CursorDown() {}

int ErrorLogMenu::Enter() {}

void ErrorLogMenu::DrawCursor() {}

void ErrorLogMenu::Draw()
{
    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("error logs:", Font_7x10, White);

    for (int i = 0; i < MAX_LOGS; i++)
    {
        ssd1306_SetCursor(0, 12 + i * 12);
        ssd1306_WriteString(error_logs[i], Font_7x10, White);
    }
}