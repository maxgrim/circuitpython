
USB_VID = 0x1209
USB_PID = 0x0001
USB_PRODUCT = "OOO 2021"
USB_MANUFACTURER = "Deloitte"

INTERNAL_FLASH_FILESYSTEM = 1
#SPI_FLASH_FILESYSTEM = 1
#EXTERNAL_FLASH_DEVICES = "W25Q128JVxQ"

LONGINT_IMPL = MPZ

# The default queue depth of 16 overflows on release builds,
# so increase it to 32.
CFLAGS += -DCFG_TUD_TASK_QUEUE_SZ=32

CIRCUITPY_ESP_FLASH_MODE=dio
CIRCUITPY_ESP_FLASH_FREQ=80m
CIRCUITPY_ESP_FLASH_SIZE=4MB

# We only have enough endpoints available in hardware to
# enable ONE of these at a time.
CIRCUITPY_USB_MIDI = 0
CIRCUITPY_USB_HID = 1
CIRCUITPY_USB_VENDOR = 0

CIRCUITPY_MODULE=wrover

CIRCUITPY_STAGE = 1

# Include these Python libraries in the firmware
FROZEN_MPY_DIRS += $(TOP)/frozen/Adafruit_CircuitPython_Bitmap_Font
FROZEN_MPY_DIRS += $(TOP)/frozen/Adafruit_CircuitPython_Debouncer
FROZEN_MPY_DIRS += $(TOP)/frozen/Adafruit_CircuitPython_Display_Shapes
FROZEN_MPY_DIRS += $(TOP)/frozen/Adafruit_CircuitPython_Display_Text
FROZEN_MPY_DIRS += $(TOP)/frozen/Adafruit_CircuitPython_HID
FROZEN_MPY_DIRS += $(TOP)/frozen/Adafruit_CircuitPython_ImageLoad
FROZEN_MPY_DIRS += $(TOP)/frozen/Adafruit_CircuitPython_IRRemote
FROZEN_MPY_DIRS += $(TOP)/frozen/Adafruit_CircuitPython_NeoPixel
FROZEN_MPY_DIRS += $(TOP)/frozen/Adafruit_CircuitPython_PCF8563
FROZEN_MPY_DIRS += $(TOP)/frozen/Adafruit_CircuitPython_Register
FROZEN_MPY_DIRS += $(TOP)/frozen/Adafruit_CircuitPython_Requests
FROZEN_MPY_DIRS += $(TOP)/frozen/circuitpython-stage
