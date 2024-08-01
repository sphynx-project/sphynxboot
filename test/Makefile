CC = clang
LD = ld.lld
CFLAGS = -O0 -Wall -Werror -g -ffreestanding -Wno-infinite-recursion \
         -fno-stack-protector -fno-stack-check -fno-omit-frame-pointer -fno-lto -fPIE -m64 \
         -march=x86-64 -mabi=sysv -mno-80387 -mno-mmx -msse -msse2 -mno-red-zone --target=x86_64-pc-none-elf
CFLAGS += -I../common/src/protocol
LDFLAGS = -nostdlib -static -m elf_x86_64 -z max-page-size=0x1000 -T linker.ld
TARGET = kernel.elf
SOURCES = $(shell find src -name '*.c')
HEADERS = $(shell find src -name '*.h')
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo " + $(LD) $(LDFLAGS) $(OBJECTS) -o $@"
	@$(LD) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.c $(HEADERS)
	@echo " + $(CC) $(CFLAGS) -c $< -o $@"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo " + rm -f $(OBJECTS) $(TARGET)"
	@rm -f $(OBJECTS) $(TARGET)
