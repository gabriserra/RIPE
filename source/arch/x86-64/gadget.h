
/**
 * @brief
 *
 * 55           push    %rbp
 * 48 89 e5     mov     %rsp,%rbp
 *
 */

char *function_prologue[] =
    {0x55, 0x48, 0x89, 0xE5};

size_t function_signature_count = sizeof(function_signature) / sizeof(function_signature[0]);

// Dummy functions used to create gadgets for the ROP attack
void gadget1(int a, int b)
{
    int arthur, dent, j;
    arthur = a + b / 42;

    char buffer[256];
    for (j = 0; j < 10; j++)
        ;
    __asm__(
        "mov $0x3b, %rax\n"
        "pop %rdi;\n"
        "ret");

    return;
}

void gadget2(int a, int b)
{
    int ford, prefect, j;
    ford = a + b / 43;
    for (j = 0; j < 10; j++)
        ;
    __asm__(
        "mov $0, %rsi\n"
        "mov $0, %rdx\n"
        "ret");

    return;
}

int gadget3(int a, int b)
{
    int i, j;
    i = a + b / 33;

    for (j = 0; j < 10; j++)
        ;
    __asm__("syscall");
    return 42;
}

void gadget4(int a, int b)
{
    int i, j;
    i = a + 4 * b / 12;

    for (j = 0; j < 5; j++)
        ;
    __asm__("nop; pop %rdi; ret;");
    return;
}