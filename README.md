# sphynxboot

A simple x86_64 bootloader for the Sphynx OS. It only supports UEFI and wont ever support anything else.

## Building

To build the bootloader simply run `make` in the root directory. This will build sphynxboot into `bin/BOOTX64.efi`

## Usage

Follow the example in `test` or look at [sphynx](https:://github.com/sphynxos/sphynx)'s code. The entry for the kernel should take in a `boot_t*` (check `sphynxboot.h` for `boot_t`).

### Config file

For sphynxboot to know where the kernel is located it needs to have a config file and this file has to be in `/boot.conf`. Example:
```ini
kernel=/sphynx/kernel.elf

```
(*Note: This has a new line in the end since the config file has to end with a `\n` and the values also cant have spaces in them.*)

## License

sphynxboot is licensed under the MIT license see [LICENSE](https://github.com/sphynxos/sphynxboot/tree/main/LICENSE).

## Credits

sphynxboot uses aurixos's EFI library which you can find [here](https://github.com/aurixos/efi)
