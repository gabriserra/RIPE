# Makefile for RIPE
GCC=gcc
CLG=clang

# Depending on how you test your system you may want to comment, or uncomment the following
# Disable stack protection, make the stack executable and add debug info
CFLAGS=-fno-stack-protector -z execstack -g

all: dir gcc_attack_gen clang_attack_gen

dir:
	if [ ! -d "build" ]; then mkdir build; fi

gcc_attack_gen: ./source/attack_gen.c
	${GCC} ${CFLAGS} -Isource/arch ./source/attack_gen.c -o ./build/gcc_attack_gen

clang_attack_gen: ./source/attack_gen.c
	${GCC} ${CFLAGS} -Isource/arch ./source/attack_gen.c -o ./build/clang_attack_gen

clean:
	rm -rf build
	

