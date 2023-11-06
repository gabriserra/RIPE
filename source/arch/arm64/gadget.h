

/**
 * @brief
 *
 * a9xx7bfd     stp	x29, x30, [sp, #-xx]!
 * 910003fd 	mov	x29, sp
 *
 * The second byte of the first instruction encodes the post increment of
 * the stack pointer. Thus for prologue we just skip the first two bytes
 *
 */

char *function_prologue[] =
    {0x7b, 0xfd, 0x91, 0x00, 0x03, 0xfd};

size_t function_signature_count = sizeof(function_signature) / sizeof(function_signature[0]);