/*
 * main_menu.hpp
 *
 *  Created on: Dec 16, 2025
 *      Author: andrey
 */
#include <main.h>
#include <ssd1306.h>
#include <ssd1306_conf.h>
#include <ssd1306_fonts.h>
#include <ssd1306_tests.h>

#ifndef INC_MAIN_MENU_HPP_
#define INC_MAIN_MENU_HPP_

typedef struct {
    char new_vma_statuses[10][4];  // "OK" или "ERR"
    bool light_status;       // 0 или 1
    char batL_voltage[16];     // например 12.5
    char batR_voltage[16];     // например 11.8
    char new_mission_names[10][15];
} SystemData;

class BaseMenu {
  public:
	enum MenuType {
		        MAIN_MENU,
		        DIAGNOSTICS_MENU,
				VMA_REV_MENU,
				MISSIONS_MENU
		    };
      virtual void Draw() = 0;
      virtual void CursorUp() = 0;
      virtual void CursorDown() = 0;
      virtual void DrawCursor() = 0;
      virtual int Enter() = 0;
      virtual void DataUpdate(const SystemData* system_data) = 0;
      virtual MenuType GetType() const = 0;
      virtual ~BaseMenu() {}
  };

class MainMenu : public BaseMenu
{
private:
	static constexpr uint8_t MENU_ITEMS_COUNT = 3;
public:
	uint8_t Y_curs;
	void Draw() override ;
	void CursorUp() override ;
	void CursorDown() override ;
	void DrawCursor() override ;
	int Enter() override ;
	void DataUpdate(const SystemData* system_data) override {}
	MenuType GetType() const override {return MAIN_MENU;}
	MainMenu()
	{
		Y_curs = 0;
	}
};

void MainMenu::CursorUp()
{
	if (Y_curs < MENU_ITEMS_COUNT - 1) Y_curs++;
	else Y_curs = 0;
	MainMenu::Draw();
}

void MainMenu::CursorDown()
{
	if (Y_curs > 0) Y_curs--;
	else Y_curs = MENU_ITEMS_COUNT - 1;
	MainMenu::Draw();
}

int MainMenu::Enter()
{
	return Y_curs + 1;
}

void MainMenu::DrawCursor()
{
	ssd1306_SetCursor(0, Y_curs * 12);
	ssd1306_WriteString(">", Font_7x10, White);
}

void MainMenu::Draw()
{
	ssd1306_Fill(Black);
	ssd1306_SetCursor(10, 0);
	ssd1306_WriteString("mission launch", Font_7x10, White);
	ssd1306_SetCursor(10, 12);
	ssd1306_WriteString("diagnostics", Font_7x10, White);
	ssd1306_SetCursor(10, 24);
	ssd1306_WriteString("error log", Font_7x10, White);
	MainMenu::DrawCursor();
}

#endif /* INC_MAIN_MENU_HPP_ */
