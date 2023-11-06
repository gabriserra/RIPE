/**
 * @brief
 *
 * 55           push    %ebp
 * 89 e5        mov     %esp,%ebp
 *
 */

char *function_prologue[] =
    {0x55, 0x89, 0xE5};

size_t function_signature_count = sizeof(function_signature) / sizeof(function_signature[0]);

/*
 Dummy functions used to create gadgets for the ROP attack
*/

void gadget1(int a, int b)
{
    int arthur, dent, j;
    arthur = a + b / 42;

    for (j = 0; j < 10; j++)
        ;
    // Gadget 1, locate at Gadget + 62 bytes
    asm("nop");      // Using this to find it easier in dissas code
    asm("pop %eax"); // FFFFFFFF => 8
    asm("add $9, %eax");
    asm("ret");

    return;
}

void gadget2(int a, int b)
{
    int ford, prefect, j;
    ford = a + b / 43;
    // Gadget 1, locate at gadget2 + 62 bytes
    for (j = 0; j < 10; j++)
        ;
    asm("nop");
    asm("pop %ebx");
    asm("pop %ecx"); // FFFFFFFF => 448
    asm("add $449, %ecx");
    asm("ret");

    return;
}
int gadget3(int a, int b)
{
    int i, j;
    i = a + b / 33;

    for (j = 0; j < 10; j++)
        ;
    // Gadget3 starts here, located at gadget3 + 62 bytes
    asm("nop");
    asm("int $0x80");

    return 42;
}