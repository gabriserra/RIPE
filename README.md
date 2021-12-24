# RIPE: Runtime Intrusion Prevention Evaluator

This repository presents a newer version of the [RIPE benchmark](https://github.com/johnwilander/RIPE).

RIPE was originally developed by John Wilander and Nick Nikiforakis and the [paper](papers/ripe_paper.pdf) was presented at The 27th Annual Computer Security Applications Conference (ACSAC) in Orlando, Florida, December 5-9, 2011.

# Follow-ups

Originally, in 2011, the [RIPE benchmark](https://github.com/johnwilander/RIPE) was developed for `x86` architecture. 

In 2018, Draper Labs realized a port for `risc-V` architecture, called [hope-RIPE](https://github.com/draperlaboratory/hope-RIPE)

> The RIPE port to RISC-V was developed by John Merrill. RIPE for RISC-V is designed for use with the Spike simulator.

Hubert Rosier realized a port for `x86-64`, called [RIPE64](https://github.com/hrosier/ripe64).

> This port was developed by Hubert ROSIER for an academic project in the National University of Singapore. The project was supervised by Professor Roland YAP and co-supervisor senior research fellow Gregory James DUCK of the School of Computing of the National University of Singapore.

Gabriele Serra realized improved the benchmark implementing a hardware abstraction layer and re-implementing attacks for ARM 64 architecture.

> The HAL and the `aarch64` port was developer by Gabriele Serra a PhD student at Scuola Superiore Sant'Anna, Pisa.

This repository aims at merge the best of all ports and let developers use a common repo for all architectures.

## Getting started: build and run

### Dependencies

To build and run RIPE benchmark you need (at least) the following binaries/environments:
- python3 (for the frontend benchmark tool)
- gcc
- clang

### Build

To build the benchmark just run the `make` command.
It will create the `build` folder and two executable file in it. 
Indeed executable files are compiled both using *clang* and *gcc*

By default, both binaries are compiled without stack protector (`-fno-stack-protector`) and with executable stack (`-z execstack`).
### Individual test with attack parameters

The RIPE testbed is based upon five attack parameters which combine to yield approximately 900 buffer overflow attacks. 

To run a specific attack, you need to specify all the dimensions like this:
```bash
 ./build/[gcc|clang]_attack_gen -l location -c code_ptr -i inject_param -t [direct|indirect] -f func_abused [-d t]
```
where:  
  - __location__ can be `stack`, `heap`, `bss` or `data`
  - __code\_ptr__ can be `ret`, `baseptr`, `funcptrstackvar`, `funcptrstackparam`, `funcptrheap`, `funcptrbss`, `funcptrdata`, `structfuncptrstack`, `structfuncptrheap`, `structfuncptrbss`, `structfuncptrdata`, `longjmpstackvar`, `longjmpstackparam`, `longjmpheap`, `longjmpbss` or `longjmpdata`
  - __inject\_params__ can be `nonop`,`simplenop`, `simplenopequival`, `r2libc` or `rop`
  - __func\_abused__ can be `memcpy`, `strcpy`, `strncpy`, `sprintf`, `snprintf`, `strcat`, `strncat`, `sscanf`, `fscanf` or `homebrew`

The attacks is successful if a shell has been spawned.
#### Location

The attack location describes the memory section in which the target buffer is located. RIPE supports attacks on the `stack`, `heap`, `data`, and `bss` sections.

#### Attack code

// TODO

#### Target Code Pointer

// TODO

#### Overflow Technique

Buffer overflows can be performed with or without indirection. 
- The `direct` technique simply overwrites a target pointer in the same memory location as the overflow buffer. Direct, data-only attacks overwrite a target pointer with an integer value.
- The `indirect` technique initially targets a generic pointer that is adjacent to the buffer. A dereference redirects this pointer to the attack code. Indirect overflows work between memory regions (i.e. from a stack buffer to a heap pointer).

#### Function

There are nine vulnerable functions available as attack vectors:
- `str(n)cpy`, `str(n)cat`, `s(n)printf`, C library string functions
- `memcpy`
- `homebrew`, a loop-based, memcpy() equivalent
- `sscanf` via a format string vulnerability

### Full benchmark

You can run all the possible attack forms by running the script `ripe_tester.py`:  
```bash
 ./ripe_tester.py [direct|indirect|both] n (gcc|clang|both) (VERBOSE_OPTIONS)
```

It accepts at least 2 pararameters, the first one to launch direct attacks, indirect or both; the second is the number of times each attack should be launched. The other parameters are optional, the third parameter specifies to use the gcc or clang executables or both.

The last one controls the output format:    
- `--only-ok`: only prints the functional attacks  
- `--only-some`: only prints the partly functional attacks  
- `--only-fail`: only prints the non-functional attacks  
- `--no-ok`: don't print the functional attacks  
- `--no-fail`: don't print the non-functional attacks  
- `--only-summary`: only prints the summary  
- `--format-bash`(default): prints the summary in plain text  
- `--format-latex`: prints the summary as a latex table  
- `--format-bash-latex`: prints the summary in plain text and as a latex table  

### Test results

RIPE produces and performs a series of exploits based on its five attack parameters:
- technique
- attack code
- target code pointer
- memory location
- vulnerable function.

The result log generated by the frontend labels each test as one of the following:
- `OK`: the attack executes successfully
- `FAILED`: the attack encounters an error before running to completion
- `SOME`: attacks that didn't succeed each round
- `NOT POSSIBLE`: the attack is not practically possible (eg. a direct attack on a stack buffer targeting a global pointer)

## Notes
### ASLR

Address Space Layout Randomization is a computer security technique that randomly arranges the address space positions of key data areas of a process, including the base of the executable and the positions of the stack, heap and libraries. ASLR makes a number of attacks unfeasible.

ASLR can be disabled temporary with:  
```bash
echo 0 | sudo tee /proc/sys/kernel/randomize_va_space
```
### Ropper

[Ropper](https://github.com/sashs/Ropper) can find gadgets to build rop chains for different architectures.
It was used before the wanted gadgets were hardcoded in functions.

### Metasploit

The one byte NOP equivalent sled has been generated using the metasploit framework 
with the command:  
```
generate 40 -s rsp -t c
```
the `-s rsp` tells that we don't want to change the RSP register (I got errors without).  

### Shellcodes

Shellcodes can be find anywhere on the internet. However, a good place to start finding shellcodes is [ShellStorm](http://shell-storm.org/shellcode/). Shellcodes are grouped by architecture.

# License & Citation

RIPE was originally released under the MIT licence. Subsequent modification and porting to other architectures were still released under the MIT license.

If you use, extend or build upon RIPE we kindly ask you to cite the original ACSAC paper in which RIPE is presented. Here's the BibTeX:

```
@inproceedings{ wilander.ripe,
    author = {John Wilander and Nick Nikiforakis and Yves Younan and Mariam Kamkar and Wouter Joosen},
    title = {{RIPE}: Runtime Intrusion Prevention Evaluator},
    booktitle = {In Proceedings of the 27th Annual Computer Security Applications Conference, {ACSAC}},
    year = {2011},
    publisher = {ACM}
}
```

# Greetings

May your buffer overflow protection prove ripe!