SRC += tap_dances.c hud.c trackpad.c

RAW_ENABLE = yes

MOUSEKEY_ENABLE = yes
CAPS_WORD_ENABLE = yes
TAP_DANCE_ENABLE = yes
ENCODER_MAP_ENABLE = yes

# Cirque Trackpad I²C configuration
POINTING_DEVICE_ENABLE = yes
POINTING_DEVICE_DRIVER = custom
# QMK's Cirque driver code, with trackpad.c as the driver on top of it
I2C_DRIVER_REQUIRED = yes
OPT_DEFS += -DPOINTING_DEVICE_DRIVER_cirque_pinnacle_i2c
SRC += drivers/sensors/cirque_pinnacle.c drivers/sensors/cirque_pinnacle_i2c.c
SRC += drivers/sensors/cirque_pinnacle_gestures.c $(QUANTUM_DIR)/pointing_device/pointing_device_gestures.c
