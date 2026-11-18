SRC += tap_dances.c hud.c

RAW_ENABLE = yes

MOUSEKEY_ENABLE = yes
CAPS_WORD_ENABLE = yes
TAP_DANCE_ENABLE = yes
ENCODER_MAP_ENABLE = yes

# Cirque Trackpad I²C configuration
POINTING_DEVICE_ENABLE = yes
POINTING_DEVICE_DRIVER = cirque_pinnacle_i2c
