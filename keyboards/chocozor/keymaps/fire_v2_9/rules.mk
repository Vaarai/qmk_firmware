SRC += keyboards/chocozor/common/tap_dances.c
SRC += keyboards/chocozor/common/accents.c

# Detect the host OS to type accents the right way (see common/accents.c)
OS_DETECTION_ENABLE = yes

AUTOCORRECT_ENABLE = yes
MOUSEKEY_ENABLE = yes
CAPS_WORD_ENABLE = yes
TAP_DANCE_ENABLE = yes

# Cirque Trackpad SPI configuration
POINTING_DEVICE_ENABLE = yes
SPI_DRIVER_ENABLE = yes
SPI_DRIVER_REQUIRED = yes
# QMK's Cirque driver code, with common/trackpad.c as the driver on top of it
# (absolute mode, our own taps, tap-drag, corner clicks, NAV scroll, glide)
POINTING_DEVICE_DRIVER = custom
OPT_DEFS += -DPOINTING_DEVICE_DRIVER_cirque_pinnacle_spi
SRC += drivers/sensors/cirque_pinnacle.c drivers/sensors/cirque_pinnacle_spi.c
SRC += drivers/sensors/cirque_pinnacle_gestures.c $(QUANTUM_DIR)/pointing_device/pointing_device_gestures.c
SRC += keyboards/chocozor/common/trackpad.c

HAPTIC_ENABLE = yes
HAPTIC_DRIVER = solenoid
SOLENOID_ENABLE = yes