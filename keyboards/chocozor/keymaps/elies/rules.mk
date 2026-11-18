SRC += tap_dances.c hud.c

RAW_ENABLE = yes

MOUSEKEY_ENABLE = yes
CAPS_WORD_ENABLE = yes
TAP_DANCE_ENABLE = yes
ENCODER_MAP_ENABLE = yes

# Trackpad: the shared driver (common/trackpad.c), bus from the revision
include keyboards/chocozor/common/trackpad.mk
