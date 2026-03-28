/*
 * vma_revolutions_menu.hpp
 *
 *  Created on: Dec 21, 2025
 *      Author: andrey
 */

#ifndef INC_LAYOUTS_VMA_REVOLUTIONS_MENU_HPP_
#define INC_LAYOUTS_VMA_REVOLUTIONS_MENU_HPP_


#include <layouts/main_menu.hpp>

#include <stdbool.h>
#include <string.h>
#include <stdio.h>


class VmaMenu : public BaseMenu
{
private:
    static constexpr uint8_t VMA_COUNT = 10;
	char vma_numbers[VMA_COUNT][4];
	char vma_statuses[VMA_COUNT][10];

public:

	void Draw() override;
	void CursorUp() override;
	void CursorDown() override;
	void DrawCursor() override;
	int Enter() override;
	void DataUpdate(const SystemData* system_data) override;
	MenuType GetType() const override {return VMA_REV_MENU;}

	VmaMenu()
	{
	    for (int i = 0; i < 10; i++) {
	        snprintf(vma_numbers[i], 4, "%02d:", i+1);
	    }

	    for (int i = 0; i < 10; i++) {
	        strcpy(vma_statuses[i], "---");
	    }
	}
};

void VmaMenu::DataUpdate(const SystemData* system_data)
{
    for (int i = 0; i < 10; i++)
    {
    	strcpy(this->vma_statuses[i], system_data->new_vma_statuses[i]);
    }
}

void VmaMenu::CursorUp()
{
}

void VmaMenu::CursorDown()
{
}

int VmaMenu::Enter()
{
	return 1;
}

void VmaMenu::DrawCursor()
{
}

void VmaMenu::Draw()
{
	ssd1306_Fill(Black);
	ssd1306_SetCursor(0, 0);
	ssd1306_WriteString("eng-s:", Font_7x10, White);
	for (int i = 0; i <= 4; i ++)
	{
		ssd1306_SetCursor(45, i*10);
		ssd1306_WriteString(vma_numbers[i], Font_6x8, White);
	};
	for (int i = 5; i <= 9; i ++)
	{
		ssd1306_SetCursor(90, (i-5)*10);
		ssd1306_WriteString(vma_numbers[i], Font_6x8, White);
	};
	for (int i = 0; i <= 4; i ++)
	{
		ssd1306_SetCursor(45+18, i*10);
		ssd1306_WriteString(vma_statuses[i], Font_6x8, White);
	};
	for (int i = 5; i <= 9; i ++)
	{
		ssd1306_SetCursor(90+18, (i-5)*10);
		ssd1306_WriteString(vma_statuses[i], Font_6x8, White);
	};
}



#endif /* INC_LAYOUTS_VMA_REVOLUTIONS_MENU_HPP_ */
