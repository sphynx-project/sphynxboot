MAKEFLAGS += --no-print-directory

DEPS := ovmf=https://retrage.github.io/edk2-nightly/bin/RELEASEX64_OVMF.fd efi=https://github.com/aurixos/efi/archive/refs/heads/main.zip
DEPS_DIR := deps
TMP_DIR := /tmp

BIN_DIR := bin
COMMON_DIR := common
TEST_DIR := test

TARGET_TEST := $(TEST_DIR)/kernel.elf
TARGET_COMMON := $(BIN_DIR)/BOOTX64.elf

OVMF := $(DEPS_DIR)/ovmf/RELEASEX64_OVMF.fd
ROOT_DIR := $(shell pwd)

.PHONY: all
all: setup deps kernel $(TARGET_COMMON)

define DOWNLOAD_template
$(DEPS_DIR)/$(1):
	@echo "+ mkdir -p $(TMP_DIR)/$(1)"
	@mkdir -p $(TMP_DIR)/$(1)
	@echo "+ mkdir -p $(DEPS_DIR)/$(1)"
	@mkdir -p $(DEPS_DIR)/$(1)
ifeq ($(2),.zip)
	@echo "+ curl -Ls $(3) -o $(TMP_DIR)/$(1)/$(1).zip"
	@curl -Ls $(3) -o $(TMP_DIR)/$(1)/$(1).zip
	@echo "+ unzip -q $(TMP_DIR)/$(1)/$(1).zip -d $(TMP_DIR)/$(1)"
	@unzip -o -q $(TMP_DIR)/$(1)/$(1).zip -d $(TMP_DIR)/$(1)
	@echo "+ mv $(TMP_DIR)/$(1)/*/* $(DEPS_DIR)/$(1)"
	@mv $(TMP_DIR)/$(1)/*/* $(DEPS_DIR)/$(1)
	@echo "+ rm -rf $(TMP_DIR)/$(1)"
	@rm -rf $(TMP_DIR)/$(1)
else
	@echo "+ curl -Ls $(3) -o $(DEPS_DIR)/$(1)/$(notdir $(3))"
	@curl -Ls $(3) -o $(DEPS_DIR)/$(1)/$(notdir $(3))
endif

.PHONY: $(1)
$(1): $(DEPS_DIR)/$(1)
endef

$(foreach DEP,$(DEPS),$(eval $(call DOWNLOAD_template,$(word 1,$(subst =, ,$(DEP))),$(suffix $(word 2,$(subst =, ,$(DEP)))),$(word 2,$(subst =, ,$(DEP))))))

.PHONY: deps
deps: $(foreach DEP,$(DEPS),$(word 1,$(subst =, ,$(DEP))))

$(TARGET_COMMON): $(COMMON_DIR)/sphynxboot.efi | $(BIN_DIR)
	@cp $(COMMON_DIR)/sphynxboot.efi $(TARGET_COMMON)

$(TARGET_TEST): kernel

$(COMMON_DIR)/sphynxboot.efi: common
	@$(MAKE) -C $(COMMON_DIR) ROOT=$(ROOT_DIR) DEPS_DIR=$(DEPS_DIR)

$(BIN_DIR):
	@echo " + mkdir -p $(BIN_DIR)"
	@mkdir -p $(BIN_DIR)

.PHONY: setup
setup: $(BIN_DIR)

.PHONY: kernel
kernel: deps
	@$(MAKE) -C $(TEST_DIR)

.PHONY: common
common: deps
	@$(MAKE) -C $(COMMON_DIR)

.PHONY: test
test: | $(TARGET_COMMON) $(TARGET_TEST) $(OVMF)
	@if [ "$(shell uname -s)" = "Darwin" ]; then \
	    dd if=/dev/zero of=boot.img bs=1m count=64; \
	    mkfs.fat -F 32 -n EFI_SYSTEM boot.img; \
	    mmd -i boot.img ::/EFI ::/EFI/BOOT; \
	    mcopy -i boot.img $(TARGET_COMMON) ::/EFI/BOOT/BOOTX64.efi; \
		mcopy -i $(TARGET_TEST) ::kernel.elf; \
	else \
	    dd if=/dev/zero of=boot.img bs=1M count=64; \
	    mkfs.fat -F 32 -n EFI_SYSTEM boot.img; \
	    mkdir -p mnt; \
	    sudo mount -o loop boot.img mnt; \
	    sudo mkdir -p mnt/EFI/BOOT; \
	    sudo cp $(TARGET_COMMON) mnt/EFI/BOOT/BOOTX64.efi; \
		sudo cp $(TARGET_TEST) mnt/kernel.elf; \
	    sudo umount mnt; \
	    rm -rf mnt; \
	fi
	@qemu-system-x86_64 -m 2G -drive if=pflash,format=raw,readonly=on,file=$(OVMF) -drive if=ide,format=raw,file=boot.img -debugcon stdio

.PHONY: clean
clean:
	@$(MAKE) -C $(COMMON_DIR) clean
	@$(MAKE) -C $(TEST_DIR) clean
	@rm -rf $(BIN_DIR) mnt boot.img $(DEPS_DIR)
