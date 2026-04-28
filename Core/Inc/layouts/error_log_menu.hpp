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
    void CursorDown() override;
    void CursorUp() override;
    void DrawCursor() override;
    void Reset() override {};
    int Enter() override;
    void DataUpdate(const MemoryMap *system_data) override;
    MenuType GetType() const override { return ERROR_LOG_MENU; }

    ErrorLogMenu()
    {
        for (int i = 0; i < MAX_LOGS - 1; i++)
        {
            snprintf(error_logs[i], 16, "---");
        }
    }
};

void ErrorLogMenu::DataUpdate(const MemoryMap *system_data)
{
    if (!system_data)
        return;

    for (int i = 0; i < MAX_LOGS; i++)
    {
        memcpy(this->error_logs[i], system_data->error_logs[i], 16);
        this->error_logs[i][16] = '\0';
    }
}

void ErrorLogMenu::CursorDown() {}

void ErrorLogMenu::CursorUp() {}

int ErrorLogMenu::Enter() {}

void ErrorLogMenu::DrawCursor() {}

void ErrorLogMenu::Draw()
{
    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("error logs:", Font_7x10, White);

    for (int i = 0; i < MAX_LOGS - 1; i++)
    {
        ssd1306_SetCursor(0, 12 + i * 12);
        ssd1306_WriteString(error_logs[i], Font_7x10, White);
    }
}