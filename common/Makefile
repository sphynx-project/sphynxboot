COMMON_ENTRY := sphynxboot_main

CC := clang
CFLAGS := -target x86_64-unknown-windows -ffreestanding -fshort-wchar -Wno-unused-command-line-argument -Wno-void-pointer-to-int-cast -Wno-int-to-void-pointer-cast -Wno-int-to-pointer-cast
CFLAGS += -I../deps/efi -Isrc
LDFLAGS := -target x86_64-unknown-windows -nostdlib -Wl,-entry:$(COMMON_ENTRY) -Wl,-subsystem:efi_application -fuse-ld=lld-link

SRC_DIR := src
SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(SRCS:.c=.o)
TARGET := sphynxboot.efi

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	@echo " + $(CC) $(LDFLAGS) -o $@ $^"
	@$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	@echo " + rm -f $(OBJS) $(TARGET)"
	@rm -f $(OBJS) $(TARGET)
