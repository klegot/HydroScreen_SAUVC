// Made by klegot

#include <cstddef>
#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "ssd1306.h"
#include "ssd1306_conf.h"
#include "ssd1306_fonts.h"
#include "ssd1306_tests.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
}
#endif

#include "hydrolib_bus_application_slave.hpp"
#include "hydrolib_bus_datalink_stream.hpp"
#include "hydrv_gpio_low.hpp"
#include "hydrv_rs_485.hpp"
#include "layouts/bottom_stroke.hpp"
#include "layouts/diagnostics_menu.hpp"
#include "layouts/error_log_menu.hpp"
#include "layouts/main_menu.hpp"
#include "layouts/mission_launch_menu.hpp"
#include "layouts/vma_revolutions_menu.hpp"
#include "memory.hpp"

#define BUFFER_LENGTH 5
constinit hydrv::GPIO::GPIOLow rx_pin(hydrv::GPIO::GPIOLow::GPIOA_port, 10, hydrv::GPIO::GPIOLow::GPIO_UART_RX);

constinit hydrv::GPIO::GPIOLow tx_pin(hydrv::GPIO::GPIOLow::GPIOA_port, 9, hydrv::GPIO::GPIOLow::GPIO_UART_TX);

constinit hydrv::GPIO::GPIOLow direction_pin(hydrv::GPIO::GPIOLow::GPIOA_port, 8, hydrv::GPIO::GPIOLow::GPIO_Output);

static constexpr hydrv::UART::UARTLow::UARTPreset USART1_115200_LOW{
    USART1_BASE, 7, RCC_APB2ENR_USART1EN, RCC_BASE + offsetof(RCC_TypeDef, APB2ENR), USART1_IRQn, 39, 1};

constinit hydrv::UART::RS485<255, 255> RS(USART1_115200_LOW, rx_pin, tx_pin, direction_pin, 7);

static constexpr void *kLoggerStab = nullptr;
static inline Memory memory{};
static inline hydrolib::bus::datalink::StreamManager manager(3, RS, kLoggerStab);
static inline hydrolib::bus::datalink::Stream stream(manager, 2);
static inline hydrolib::bus::application::Slave slave(stream, memory, kLoggerStab);

MemoryMap current_system_data = {.vma_statuses = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                 .light_status = 0,
                                 .current_mission = 0,
                                 .batL_voltage = -1,
                                 .batR_voltage = -1,
                                 .mission_names = {"--no name--", "--no name--", "--no name--", "--no name--"},
                                 .error_logs = {"--no logs--", "", "", ""}};

static BaseMenu *current_window = nullptr;
static BottomSTR bottom_str;

inline hydrolib::ReturnCode Memory::Read(void *read_buffer, int address, int length)
{
    switch (address)
    {
    case 0:
        memcpy(read_buffer, &current_system_data, sizeof(MemoryMap));
        break;
    default:
        return hydrolib::ReturnCode::FAIL;
    }
    length -= sizeof(MemoryMap);
    if (length > 0)
    {

        void *next_read_buffer = static_cast<uint8_t *>(read_buffer) + sizeof(MemoryMap);
        return Read(next_read_buffer, address + sizeof(MemoryMap), length);
    }
    return hydrolib::ReturnCode::OK;
}

inline hydrolib::ReturnCode Memory::Write(const void *write_buffer, int address, int length)
{
    switch (address)
    {
    case 0:
        memcpy(&current_system_data, write_buffer, sizeof(MemoryMap));
        break;

    default:
        return hydrolib::ReturnCode::FAIL;
    }
    current_window->DataUpdate(&current_system_data);
    current_window->Draw();
    bottom_str.DataUpdate(&current_system_data);
    bottom_str.Draw();
    length -= sizeof(MemoryMap);
    if (length > 0)
    {
        const void *next_write_buffer = static_cast<const uint8_t *>(write_buffer) + sizeof(MemoryMap);
        return Write(next_write_buffer, address + sizeof(MemoryMap), length);
    }
    return hydrolib::ReturnCode::OK;
}

I2C_HandleTypeDef hi2c1;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);

void DataRead()
{
    SystemData temp_data = {};
    master.Read(&temp_data, SLAVE_DATA_REGISTR, sizeof(SystemData));
    for (int i = 0; i < 10; i++)
    {
        manager.Process();
        hydrolib::ReturnCode result = master.Process();
        if (result == hydrolib::ReturnCode::NO_DATA || result == hydrolib::ReturnCode::TIMEOUT)
        {
            HAL_Delay(10);
            continue;
        }
        if (result == hydrolib::ReturnCode::OK)
        {
            if (std::memcmp(&system_data, &temp_data, sizeof(SystemData)) != 0)
            {
                std::memcpy(&system_data, &temp_data, sizeof(SystemData));
                current_window->DataUpdate(&system_data);
                current_window->Draw();
                bottom_str.DataUpdate(&system_data);
                bottom_str.Draw();
            }
            break;
        }
    }
}

int main(void)
{
    HAL_Init();

    SystemClock_Config();

    MX_GPIO_Init();
    MX_I2C1_Init();

    ssd1306_Init();

    static MainMenu main_menu;
    static MissionsMenu missions_menu;
    static DiagnosticsMenu diagnostics_menu;
    static ErrorLogMenu error_menu;
    static VmaMenu vma_menu;

    current_window = &main_menu;
    current_window->Draw();
    bottom_str.Draw();

    RS.Init();

    uint32_t last_send_time = 0;

    while (1)
    {
        bottom_str.Draw();
        if (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4)) // выбор
        {
            if (current_window->GetType() == BaseMenu::MAIN_MENU)
            {
                if (current_window->Enter() == 1)
                {
                    current_window = &missions_menu;
                    current_window->DataUpdate(&current_system_data);
                    current_window->Draw();
                }
                else if (current_window->Enter() == 2)
                {
                    current_window = &diagnostics_menu;
                    current_window->DataUpdate(&current_system_data);
                    current_window->Draw();
                }
                else if (current_window->Enter() == 3)
                {
                    current_window = &error_menu;
                    current_window->DataUpdate(&current_system_data);
                    current_window->Draw();
                }
            }
            else if (current_window->GetType() == BaseMenu::DIAGNOSTICS_MENU)
            {
                current_window = &vma_menu;
                current_window->DataUpdate(&current_system_data);
                current_window->Draw();
            }
            else if (current_window->GetType() == BaseMenu::MISSIONS_MENU)
            {
                current_system_data.current_mission = current_window->Enter();
            }

            HAL_Delay(200);
        }
        if (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5)) // вверх
        {
            current_window->CursorUp();
            HAL_Delay(200);
        }
        if (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3)) // вниз
        {
            current_window->CursorDown();
            HAL_Delay(200);
        }
        if (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)) // отмена
        {
            current_window = &main_menu;
            current_window->Draw();
            HAL_Delay(200);
        }
        if (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)) // левая средняя
        {
            HAL_Delay(200);
        }
        if (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)) // левая верхняя
        {
            HAL_Delay(200);
        }
        ssd1306_UpdateScreen();
    }
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

static void MX_I2C1_Init(void)
{
    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 100000;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        Error_Handler();
    }
}

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pins : PA0 PA1 PA2 PA3
                             PA4 PA5 */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

extern "C" void RS485_IRQHandler(void)
{
    RS.IRQCallback();
    manager.Process();
    slave.Process();
}

void Error_Handler(void)
{
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
}
#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
#endif /* USE_FULL_ASSERT */
