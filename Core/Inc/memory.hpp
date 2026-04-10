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

struct MemoryMap
{
    struct SystemData
    {
        int8_t new_vma_statuses[10]; // 1 = "OK"; 0 = "ERROR"
        int8_t light_status;         // 1 = ON; 0 = OFF
        int8_t current_mission;      // current mision's number
        int16_t batL_voltage;        // 1250 => 12.50
        int16_t batR_voltage;        // 1250 => 12.50
        int8_t mission_names[4][16]; // <= 4 mission names (length = 16)
        int8_t error_logs[4][16];    // <= 4 errror logs (length = 16)
    } __attribute__((__packed__)) system_data;
    int16_t free_bytes;
} __attribute__((__packed__));

class Memory
{
public:
    hydrolib::ReturnCode Read(void *read_buffer, int address, int length);
    hydrolib::ReturnCode Write(const void *write_buffer, int address, int length);
};

#endif