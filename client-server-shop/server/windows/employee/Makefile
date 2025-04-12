.PHONY: bins

ASM_FILES := $(wildcard *.asm)

BIN_FILES := $(patsubst %.asm,%.bin,$(subst windows/,windows/bins/,${ASM_FILES}))



server: server.c $(BIN_FILES)
	cc -I/usr/local/include/ -L/usr/local/lib -lpq server.c -o server -std=gnu17 -Wno-gnu-designator




bins: $(BIN_FILES)
	@echo $(BIN_FILES)
./windows/bins/%.bin: ./windows/%.asm
	nasm -f bin $< -o $@




#server: server.o 




# smth: 
# 	@echo ${BIN_FILES}

# http://elm-chan.org/junk/32bit/binclude.html