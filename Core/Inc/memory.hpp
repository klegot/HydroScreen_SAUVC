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
    int8_t new_vma_statuses[10][4]; // "OK" или "ERR"
    int8_t light_status;
    int8_t batL_voltage[16];
    int8_t batR_voltage[16];
    int8_t new_mission_names[5][16];
} __attribute__((__packed__));

#endif