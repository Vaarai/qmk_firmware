SRC += keyboards/chocozor/common/tap_dances.c
SRC += keyboards/chocozor/common/accents.c
SRC += keyboards/chocozor/common/pcstatus.c

# PC status from the host over Raw HID, on the left screen (common/pcstatus.c)
RAW_ENABLE = yes

# Detect the host OS to type accents the right way (see common/accents.c)
OS_DETECTION_ENABLE = yes

AUTOCORRECT_ENABLE = yes
MOUSEKEY_ENABLE = yes
CAPS_WORD_ENABLE = yes
TAP_DANCE_ENABLE = yes

# Trackpad: the shared driver (common/trackpad.c), bus from the revision
include keyboards/chocozor/common/trackpad.mk

HAPTIC_ENABLE = yes
HAPTIC_DRIVER = solenoid
SOLENOID_ENABLE = yes