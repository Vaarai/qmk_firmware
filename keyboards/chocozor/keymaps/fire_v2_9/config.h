#pragma once

/* Split transactions for the keymap: the USB half sends the accents' OS mode
 * to the other half (common/accents.c), and MAGIC pointing's toggle, reading
 * back the pad's rest (keymap.c) */
#define SPLIT_TRANSACTION_IDS_USER RPC_ID_USER_OS_SYNC, RPC_ID_USER_MAGIC
