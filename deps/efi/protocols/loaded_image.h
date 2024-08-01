#ifndef EFI_PROTOCOLS_LOADED_IMAGE_H
#define EFI_PROTOCOLS_LOADED_IMAGE_H

////
// EFI Loaded Image Protocol
///

#define EFI_LOADED_IMAGE_PROTOCOL_GUID {0x5b1b31a1,0x9562,0x11d2, {0x8e,0x3f,0x00,0xa0,0xc9,0x69,0x72,0x3b}}

#define EFI_LOADED_IMAGE_PROTOCOL_VERSION 0x1000

typedef struct _EFI_LOADED_IMAGE_PROTOCOL EFI_LOADED_IMAGE_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *EFI_IMAGE_UNLOAD)(
	IN EFI_HANDLE ImageHandle
);

typedef struct _EFI_LOADED_IMAGE_PROTOCOL {
	EFI_UINT32 Revision;
	EFI_HANDLE ParentHandle;
	EFI_SYSTEM_TABLE *SystemTable;

	// Source location of the image
	EFI_HANDLE DeviceHandle;
	EFI_DEVICE_PATH_PROTOCOL *FilePath;
	VOID *Reserved;

	EFI_UINT32 LoadOptionsSize;
	VOID *LoadOptions;

	VOID *ImageBase;
	EFI_UINT64 ImageSize;
	EFI_MEMORY_TYPE ImageCodeType;
	EFI_MEMORY_TYPE ImageDataType;
	EFI_IMAGE_UNLOAD Unload;
} EFI_LOADED_IMAGE_PROTOCOL;

////
// EFI Loaded Image Device Path Protocol
///

#define EFI_LOADED_IMAGE_DEVICE_PATH_PROTOCOL_GUID {0xbc62157e,0x3e33,0x4fec, {0x99,0x20,0x2d,0x3b,0x36,0xd7,0x50,0xdf}}

// UEFI Spec 2.10, 9.2.1:
// The Loaded Image Device Path Protocol uses the same protocol interface structure as the Device Path Protocol defined
// in Chapter 9. The only difference between the Device Path Protocol and the Loaded Image Device Path Protocol is
// the protocol GUID value.
typedef EFI_DEVICE_PATH_PROTOCOL EFI_LOADED_IMAGE_DEVICE_PATH_PROTOCOL;

#endif /* EFI_PROTOCOLS_LOADED_IMAGE_H */
