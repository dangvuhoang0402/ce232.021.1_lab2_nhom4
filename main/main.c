/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "ssd1306.h"

static const char *TAG = "OLED";


uint8_t uit_logo_map[] = {
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xd7, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfa, 0xab, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x21, 0x7f, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0x04, 0x57, 0x4f, 0xff, 0xff, 0xff, 0xf9, 0x14, 0xbf, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xc2, 0x51, 0xb9, 0x79, 0x3f, 0xff, 0xff, 0xda, 0xa2, 0x5f, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xfd, 0x28, 0x8f, 0xcf, 0xa7, 0xff, 0xfd, 0x40, 0x20, 0x11, 0x3f, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xf2, 0x05, 0x7a, 0x7a, 0x7f, 0xfe, 0x82, 0x2a, 0x8a, 0xa4, 0x8f, 0xff, 
0xff, 0xff, 0xff, 0xff, 0x88, 0xa9, 0xa5, 0xcb, 0xff, 0xd1, 0x29, 0x57, 0x78, 0x89, 0x23, 0xff, 
0xff, 0xff, 0xff, 0xfe, 0x52, 0x2f, 0x5e, 0x7f, 0xfa, 0x04, 0x55, 0xff, 0xfa, 0x24, 0x50, 0x7f, 
0xff, 0xff, 0xff, 0xfa, 0x84, 0xd9, 0x73, 0xff, 0xc4, 0xab, 0xfe, 0x7f, 0xfe, 0x91, 0x7d, 0x3f, 
0xff, 0xff, 0xff, 0xf0, 0x53, 0xe7, 0x9f, 0xfa, 0x28, 0x5f, 0xff, 0x9f, 0xff, 0xd7, 0xfe, 0x8f, 
0xff, 0xff, 0xff, 0xca, 0x8e, 0x3c, 0xff, 0xc9, 0x07, 0xff, 0xff, 0xef, 0xff, 0xff, 0xff, 0xaf, 
0xff, 0xff, 0xff, 0xa1, 0x3a, 0xe7, 0xfe, 0x24, 0xbf, 0xff, 0xff, 0xf3, 0xff, 0xff, 0xff, 0xc3, 
0xff, 0xff, 0xff, 0x0a, 0xeb, 0x9f, 0xf1, 0x42, 0xff, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xeb, 
0xff, 0xff, 0xfc, 0x57, 0x5d, 0x7f, 0x8a, 0x2f, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xf3, 
0xff, 0xff, 0xfd, 0x1a, 0xb3, 0xfe, 0x51, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xf5, 
0xff, 0xff, 0xf0, 0xf5, 0xdf, 0xf4, 0x8b, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xf9, 
0xff, 0xff, 0xf5, 0xce, 0xbf, 0xd1, 0x2f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xff, 0xff, 0xf5, 
0xff, 0xff, 0xe7, 0x3a, 0xfe, 0x08, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xf9, 
0xff, 0xff, 0xdc, 0xeb, 0xf9, 0x53, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0xff, 0xff, 0xfb, 
0xff, 0xff, 0xf5, 0xaf, 0xe4, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xfb, 
0xff, 0xff, 0xd7, 0x7f, 0x92, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xff, 0xff, 0xf3, 
0xff, 0xff, 0xac, 0xfe, 0x49, 0x7f, 0xaf, 0xff, 0xef, 0xff, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xfb, 
0xff, 0xff, 0x77, 0xf9, 0x25, 0xff, 0x15, 0xff, 0x43, 0xff, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xf7, 
0xff, 0xff, 0xd7, 0xe4, 0x97, 0xfc, 0xa2, 0xfc, 0x28, 0xff, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xef, 
0xff, 0xff, 0xbf, 0xa9, 0x3f, 0xfa, 0x0b, 0xef, 0x84, 0xbf, 0xff, 0xff, 0xf9, 0xff, 0xff, 0xef, 
0xff, 0xfe, 0x7f, 0x04, 0xff, 0xf4, 0xaf, 0x17, 0xd2, 0x7f, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xdf, 
0xff, 0xff, 0xfc, 0xa9, 0xff, 0xf1, 0x3c, 0xa1, 0x69, 0x1f, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xbf, 
0xff, 0xff, 0xf2, 0x27, 0xff, 0xc4, 0xb5, 0x08, 0xf8, 0x9f, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xbf, 
0xff, 0xff, 0xe9, 0x4f, 0xff, 0xd2, 0xf0, 0xa5, 0x1e, 0x47, 0xff, 0xff, 0xfd, 0xff, 0xfe, 0xff, 
0xff, 0xff, 0xc4, 0x3f, 0xff, 0xa5, 0xc4, 0x10, 0x4f, 0x17, 0xff, 0xff, 0xfd, 0xff, 0xfe, 0xff, 
0xff, 0xff, 0x11, 0x5f, 0xff, 0x8b, 0x92, 0xa5, 0x27, 0xa7, 0xff, 0xff, 0xf9, 0xff, 0xfd, 0xff, 
0xff, 0xfe, 0xa4, 0xff, 0xff, 0xaf, 0x24, 0x48, 0x91, 0xd7, 0xff, 0xff, 0xfd, 0xff, 0xfb, 0xff, 
0xff, 0xfc, 0x13, 0xfd, 0x5f, 0xff, 0x51, 0x12, 0x4f, 0xff, 0xdb, 0x7f, 0xfb, 0xff, 0xf7, 0xff, 
0xff, 0xf2, 0xa5, 0xfa, 0x46, 0x7d, 0xfe, 0x85, 0x7e, 0xf3, 0xa4, 0xff, 0xfb, 0xff, 0xff, 0xff, 
0xff, 0xf4, 0x4f, 0xf9, 0x27, 0x32, 0x5f, 0xf7, 0xe9, 0x39, 0x81, 0x3f, 0xfb, 0xff, 0xff, 0xff, 
0xff, 0xc2, 0x17, 0xf8, 0x97, 0x74, 0x81, 0xfe, 0x82, 0x3d, 0xd4, 0x7f, 0xf3, 0xff, 0xff, 0xff, 
0xff, 0xa9, 0x5f, 0xfa, 0x4e, 0xe2, 0x2b, 0xff, 0x28, 0x9f, 0xa2, 0xbf, 0xf7, 0xff, 0xff, 0xff, 
0xff, 0x12, 0x3f, 0xfc, 0x87, 0xd1, 0x5f, 0xff, 0xe5, 0x2f, 0x94, 0x7f, 0xef, 0xff, 0xff, 0xff, 
0xfe, 0xa1, 0x7f, 0xfc, 0x2f, 0xca, 0x7f, 0xff, 0xf8, 0x87, 0xc1, 0xff, 0xcf, 0xff, 0xff, 0xff, 
0xfe, 0x14, 0xff, 0xff, 0xf7, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 
0xf9, 0x42, 0xff, 0xff, 0xff, 0x6f, 0xff, 0xff, 0xff, 0x7d, 0xbf, 0xff, 0xbf, 0xff, 0xff, 0xff, 
0xfa, 0x29, 0xff, 0xff, 0xd0, 0x91, 0xff, 0xff, 0xfe, 0x82, 0x2f, 0xff, 0x7f, 0xff, 0xff, 0xff, 
0xf1, 0x13, 0xff, 0xff, 0xfa, 0x04, 0x7f, 0xff, 0xf8, 0x21, 0x7f, 0xfe, 0xbf, 0xff, 0xff, 0xff, 
0xf4, 0xa5, 0xff, 0xff, 0xff, 0x52, 0xbf, 0xff, 0xf5, 0x4b, 0xff, 0xfd, 0xff, 0xff, 0xff, 0xff, 
0xe2, 0x0b, 0xff, 0x00, 0x3f, 0xc8, 0x1f, 0xff, 0xe4, 0x2f, 0xf0, 0x03, 0xff, 0xff, 0xff, 0xff, 
0xd1, 0x53, 0xff, 0xaa, 0x9f, 0xf5, 0x4f, 0xff, 0x92, 0xbf, 0xd5, 0x2b, 0xff, 0xff, 0xff, 0xff, 
0xca, 0x0b, 0xff, 0xc0, 0x43, 0xfc, 0x93, 0xff, 0xa4, 0xff, 0x20, 0x9f, 0xff, 0xff, 0xff, 0xff, 
0xd0, 0xa7, 0xff, 0xfa, 0x94, 0xff, 0x45, 0xfe, 0x13, 0xfa, 0x4a, 0x5f, 0xff, 0xff, 0xff, 0xff, 
0xc5, 0x13, 0xff, 0xfc, 0x41, 0x1f, 0xe8, 0xfc, 0xaf, 0xd1, 0x11, 0x7f, 0xff, 0xff, 0xff, 0xff, 
0x90, 0xa5, 0xff, 0xff, 0x14, 0x42, 0xf4, 0x72, 0x7e, 0x88, 0x8b, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xca, 0x13, 0xff, 0xff, 0xc9, 0x29, 0x15, 0x14, 0x90, 0x52, 0x57, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xa2, 0xa4, 0xff, 0xff, 0xf4, 0x84, 0x42, 0x49, 0x45, 0x09, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xc8, 0x09, 0xff, 0xff, 0xfe, 0x51, 0x29, 0x22, 0x10, 0xa5, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xd2, 0xa2, 0x3f, 0xff, 0xff, 0xca, 0x44, 0x48, 0xa5, 0x17, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xc9, 0x14, 0x5f, 0xff, 0xff, 0xfd, 0x29, 0x12, 0x14, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xe4, 0x41, 0x0b, 0xff, 0xff, 0xff, 0xf6, 0xed, 0xeb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xf2, 0x94, 0xa5, 0xff, 0xff, 0xff, 0xff, 0xf6, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xf8, 0x49, 0x10, 0x17, 0xff, 0xff, 0xfa, 0x89, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0x24, 0x4a, 0x88, 0x55, 0x55, 0x04, 0x2f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0x42, 0x90, 0x52, 0x84, 0x88, 0x53, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xd4, 0x45, 0x08, 0x29, 0x22, 0xaf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xfd, 0x10, 0xa2, 0x82, 0x5b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xf6, 0x95, 0x7d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};
void app_main(void)
{   
    Screen_t Screen;
    i2c_master_init();
    ssd1306_init();
    ssd1306_bitmap_picture(&Screen, uit_logo_map);
    ssd1306_display_picture(&Screen);
    ESP_LOGI(TAG, "Show picture!");
}
