#ifndef ARCH_ARCH_H
#define ARCH_ARCH_H

#ifdef __i386__
#include "i386/shellcode.h"
#include "i386/gadget.h"
#include "i386/vulnerable.h"
#endif

#if __x86_64__
#include "x86-64/shellcode.h"
#include "x86-64/gadget.h"
#include "x86-64/vulnerable.h"
#endif

#ifdef __aarch64__
#include "aarch64/shellcode.h"
#include "aarch64/gadget.h"
#include "aarch64/vulnerable.h"
#endif

#endif /* ARCH_ARCH_H */