#ifndef MEMORY_HPP
#define MEMORY_HPP

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "ssd1306.h"
#include "ssd1306_conf.h"
#include "ssd1306_fonts.h"
#include "ssd1306_tests.h"

#ifdef __cplusplus
}
#endif

struct SystemData
{
    int8_t new_vma_statuses[10]; // 1 = "OK"; 0 = "ERROR"
    int8_t light_status;         // 1 = ON; 0 = OFF
    int8_t current_mission;      // current mision's number
    int8_t batL_voltage[8];      // 1250 => 12.50
    int8_t batR_voltage[8];      // 1250 => 12.50
    int8_t mission_names[5][16]; // <= 5 mission names (length = 16)
    int8_t error_logs[5][16];    // <= 5 errror logs (length = 16)
} __attribute__((__packed__));

#endif