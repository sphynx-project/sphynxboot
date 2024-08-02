MAKEFLAGS += --no-print-directory

DEPS := ovmf=https://retrage.github.io/edk2-nightly/bin/RELEASEX64_OVMF.fd efi=https://github.com/aurixos/efi/archive/refs/heads/main.zip
DEPS_DIR := deps
TMP_DIR := $(shell mktemp -d)

BIN_DIR := bin
COMMON_DIR := common
TEST_DIR := test

TARGET_TEST := $(TEST_DIR)/kernel.elf
TARGET_COMMON := $(BIN_DIR)/BOOTX64.efi

OVMF := $(DEPS_DIR)/ovmf/RELEASEX64_OVMF.fd
ROOT_DIR := $(shell pwd)

all: $(TARGET_COMMON)

.PHONY: cleanup_tmp
cleanup_tmp:
	@echo " + rm -rf $(TMP_DIR)"
	@rm -rf $(TMP_DIR)

define DOWNLOAD_template
deps-download-$(1):
	@if [ ! -d $(DEPS_DIR)/$(1) ]; then \
		echo " + Creating directory $(DEPS_DIR)/$(1)"; \
		mkdir -p $(DEPS_DIR)/$(1); \
		if [ "$(2)" = ".zip" ]; then \
			echo " + Downloading and unzipping $(3)"; \
			curl -Ls $(3) -o $(TMP_DIR)/$(1).zip && \
			unzip -o -q $(TMP_DIR)/$(1).zip -d $(TMP_DIR) && \
			mv $(TMP_DIR)/*/* $(DEPS_DIR)/$(1) && \
			$(MAKE) cleanup_tmp; \
		else \
			echo " + Downloading $(3)"; \
			curl -Ls $(3) -o $(DEPS_DIR)/$(1)/$(notdir $(3)) || { echo "Error downloading $(3)"; exit 1; }; \
		fi; \
	else \
		echo " + Directory $(DEPS_DIR)/$(1) already exists, skipping download"; \
	fi
endef

$(foreach DEP,$(DEPS),$(eval $(call DOWNLOAD_template,$(word 1,$(subst =, ,$(DEP))),$(suffix $(word 2,$(subst =, ,$(DEP)))),$(word 2,$(subst =, ,$(DEP))))))

.PHONY: deps-download-all
deps-download-all: $(foreach DEP,$(DEPS),deps-download-$(word 1,$(subst =, ,$(DEP))))

$(TARGET_COMMON): $(COMMON_DIR)/sphynxboot.efi | $(BIN_DIR)
	@echo " + cp $(COMMON_DIR)/sphynxboot.efi $(TARGET_COMMON)"
	@cp $(COMMON_DIR)/sphynxboot.efi $(TARGET_COMMON)

$(TARGET_TEST): kernel

$(OVMF): deps-download-ovmf

$(COMMON_DIR)/sphynxboot.efi: common
	@$(MAKE) -C $(COMMON_DIR) ROOT=$(ROOT_DIR) DEPS_DIR=$(DEPS_DIR)

$(BIN_DIR):
	@echo " + mkdir -p $(BIN_DIR)"
	@mkdir -p $(BIN_DIR)

.PHONY: setup
setup: $(BIN_DIR)

.PHONY: kernel
kernel:
	@$(MAKE) -C $(TEST_DIR)

.PHONY: common
common: deps-download-efi
	@$(MAKE) -C $(COMMON_DIR)

.PHONY: test
test: | $(TARGET_COMMON) $(TARGET_TEST) $(OVMF)
	@if [ "$(shell uname -s)" = "Darwin" ]; then \
	    dd if=/dev/zero of=boot.img bs=1m count=64; \
	    mkfs.fat -F 32 -n EFI_SYSTEM boot.img; \
	    mmd -i boot.img ::/EFI ::/EFI/BOOT; \
	    mcopy -i boot.img $(TARGET_COMMON) ::/EFI/BOOT/BOOTX64.efi; \
	    mcopy -i $(TARGET_TEST) ::/boot/kernel/kernel; \
	else \
	    dd if=/dev/zero of=boot.img bs=1M count=64; \
	    mkfs.fat -F 32 -n EFI_SYSTEM boot.img; \
	    mkdir -p mnt; \
	    sudo mount -o loop boot.img mnt; \
	    sudo mkdir -p mnt/EFI/BOOT; \
	    sudo cp $(TARGET_COMMON) mnt/EFI/BOOT/BOOTX64.efi; \
	    sudo mkdir -p mnt/boot/kernel; \
	    sudo cp $(TARGET_TEST) mnt/boot/kernel/kernel; \
	    sudo umount mnt; \
	    rm -rf mnt; \
	fi
	@qemu-system-x86_64 -m 2G -drive if=pflash,format=raw,readonly=on,file=$(OVMF) -drive if=ide,format=raw,file=boot.img -serial stdio

.PHONY: clean
clean:
	@$(MAKE) -C $(COMMON_DIR) clean
	@$(MAKE) -C $(TEST_DIR) clean
	@echo " + Removing $(BIN_DIR) mnt boot.img $(DEPS_DIR)"
	@rm -rf $(BIN_DIR) mnt boot.img $(DEPS_DIR)
