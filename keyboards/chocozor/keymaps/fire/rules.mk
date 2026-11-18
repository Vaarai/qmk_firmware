SRC += keyboards/chocozor/common/tap_dances.c

AUTOCORRECT_ENABLE = yes
MOUSEKEY_ENABLE = yes
CAPS_WORD_ENABLE = yes
TAP_DANCE_ENABLE = yes

# Trackpad: the shared driver (common/trackpad.c), bus from the revision
include keyboards/chocozor/common/trackpad.mk
