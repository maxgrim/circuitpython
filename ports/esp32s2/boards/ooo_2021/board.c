/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Scott Shawcroft for Adafruit Industries
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "supervisor/board.h"
#include "mpconfigboard.h"
#include "shared-bindings/microcontroller/Pin.h"

// Display
#include "shared-bindings/displayio/FourWire.h"
#include "shared-module/displayio/__init__.h"
#include "shared-module/displayio/mipi_constants.h"

displayio_fourwire_obj_t board_display_obj;

uint8_t display_init_sequence[] = {
    0x01, 0x80, 0x80,  // Software reset then delay 0x80 (128ms)
    0xEF, 0x03, 0x03, 0x80, 0x02,
    0xCF, 0x03, 0x00, 0xC1, 0x30,
    0xED, 0x04, 0x64, 0x03, 0x12, 0x81,
    0xE8, 0x03, 0x85, 0x00, 0x78,
    0xCB, 0x05, 0x39, 0x2C, 0x00, 0x34, 0x02,
    0xF7, 0x01, 0x20,
    0xEA, 0x02, 0x00, 0x00,
    0xc0, 0x01, 0x23,  // Power control VRH[5:0]
    0xc1, 0x01, 0x10,  // Power control SAP[2:0];BT[3:0]
    0xc5, 0x02, 0x3e, 0x28,  // VCM control
    0xc7, 0x01, 0x86,  // VCM control2
    0x36, 0x01, 0x38,  // Memory Access Control
    0x37, 0x01, 0x00,  // Vertical scroll zero
    0x3a, 0x01, 0x55,  // COLMOD: Pixel Format Set
    0xb1, 0x02, 0x00, 0x18,  // Frame Rate Control (In Normal Mode/Full Colors)
    0xb6, 0x03, 0x08, 0x82, 0x27,  // Display Function Control
    0xF2, 0x01, 0x00,  // 3Gamma Function Disable
    0x26, 0x01, 0x01,  // Gamma curve selected
    0xe0, 0x0f, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,  // Set Gamma
    0xe1, 0x0f, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,  // Set Gamma
    0x11, 0x80, 0x78,  // Exit Sleep then delay 0x78 (120ms)
    0x29, 0x80, 0x78,  // Display on then delay 0x78 (120ms)
};

void board_init(void) {
    // Display (ILI9341)
    busio_spi_obj_t *spi = &displays[0].fourwire_bus.inline_bus;
    common_hal_busio_spi_construct(spi, &pin_GPIO17, &pin_GPIO15, NULL);
    common_hal_busio_spi_never_reset(spi);
    displayio_fourwire_obj_t *bus = &displays[0].fourwire_bus;
    bus->base.type = &displayio_fourwire_type;
    common_hal_displayio_fourwire_construct(bus,
        spi,
        &pin_GPIO16, // TFT_DC Command or data
        &pin_GPIO8, // TFT_CS Chip select
        &pin_GPIO21, // TFT_RST Reset
        50000000, // Baudrate
        0, // Polarity
        0); // Phase

    pwmout_reset();

    displayio_display_obj_t *display = &displays[0].display;
    display->base.type = &displayio_display_type;
    common_hal_displayio_display_construct(display,
        bus,
        320, // Width (after rotation)
        240, // Height (after rotation)
        0, // column start
        0, // row start
        180, // rotation
        16, // Color depth
        false, // Grayscale
        false, // Pixels in a byte share a row. Only used for depth < 8
        1, // bytes per cell. Only valid for depths < 8
        false, // reverse_pixels_in_byte. Only valid for depths < 8
        true, // reverse_pixels_in_word
        MIPI_COMMAND_SET_COLUMN_ADDRESS, // Set column command
        MIPI_COMMAND_SET_PAGE_ADDRESS, // Set row command
        MIPI_COMMAND_WRITE_MEMORY_START, // Write memory command
        0x37, // set vertical scroll command
        display_init_sequence,
        sizeof(display_init_sequence),
        &pin_GPIO6,  // backlight pin
        NO_BRIGHTNESS_COMMAND,
        0.6f, // brightness (ignored)
        false, // auto_brightness
        false, // single_byte_bounds
        false, // data_as_commands
        true, // auto_refresh
        60, // native_frames_per_second
        true, // backlight_on_high
        false); // not SH1107
    
    // USB
    common_hal_never_reset_pin(&pin_GPIO19);
    common_hal_never_reset_pin(&pin_GPIO20);

    // Debug UART
    #ifdef DEBUG
    common_hal_never_reset_pin(&pin_GPIO43);
    common_hal_never_reset_pin(&pin_GPIO44);
    #endif /* DEBUG */
}

bool board_requests_safe_mode(void) {
    return false;
}

void reset_board(void) {

}

void board_deinit(void) {
}
