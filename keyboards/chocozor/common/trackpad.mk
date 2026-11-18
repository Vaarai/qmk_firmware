# Chocozor trackpad: QMK's Cirque Pinnacle code, with common/trackpad.c as the
# pointing device driver on top of it (absolute mode, our own taps, tap-drag,
# corner clicks, NAV scroll, glide). Include it from a keymap's rules.mk:
#     include keyboards/chocozor/common/trackpad.mk
# The bus comes from the revision's rules.mk: TRACKPAD_BUS = i2c (v1.3) or spi (v2.9).

POINTING_DEVICE_ENABLE = yes
POINTING_DEVICE_DRIVER = custom

ifeq ($(strip $(TRACKPAD_BUS)), spi)
    SPI_DRIVER_REQUIRED = yes
else ifeq ($(strip $(TRACKPAD_BUS)), i2c)
    I2C_DRIVER_REQUIRED = yes
else
    $(error TRACKPAD_BUS must be i2c or spi, set in the revision's rules.mk)
endif

OPT_DEFS += -DPOINTING_DEVICE_DRIVER_cirque_pinnacle_$(strip $(TRACKPAD_BUS))
SRC += drivers/sensors/cirque_pinnacle.c drivers/sensors/cirque_pinnacle_$(strip $(TRACKPAD_BUS)).c
SRC += drivers/sensors/cirque_pinnacle_gestures.c $(QUANTUM_DIR)/pointing_device/pointing_device_gestures.c
SRC += keyboards/chocozor/common/trackpad.c
