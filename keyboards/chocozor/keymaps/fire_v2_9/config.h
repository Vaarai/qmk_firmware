#pragma once

/* Split transactions for the keymap: the USB half sends the accents' OS mode
 * to the other half (common/accents.c) */
#define SPLIT_TRANSACTION_IDS_USER RPC_ID_USER_OS_SYNC
