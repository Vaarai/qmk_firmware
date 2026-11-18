SRC += keyboards/chocozor/common/tap_dances.c

AUTOCORRECT_ENABLE = yes
MOUSEKEY_ENABLE = yes
CAPS_WORD_ENABLE = yes
TAP_DANCE_ENABLE = yes

# Cirque Trackpad SPI configuration
POINTING_DEVICE_ENABLE = yes
SPI_DRIVER_ENABLE = yes
POINTING_DEVICE_DRIVER = cirque_pinnacle_spi

HAPTIC_ENABLE = yes
HAPTIC_DRIVER = solenoid
SOLENOID_ENABLE = yes