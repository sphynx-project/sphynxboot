#ifndef EFI_TYPES_H
#define EFI_TYPES_H

#include <stdint.h>

// Common UEFI Data types
typedef int8_t EFI_INT8;
typedef uint8_t EFI_UINT8;
typedef int16_t EFI_INT16;
typedef uint16_t EFI_UINT16;
typedef int32_t EFI_INT32;
typedef uint32_t EFI_UINT32;
typedef int64_t EFI_INT64;
typedef uint64_t EFI_UINT64;
typedef int64_t EFI_INTN;
typedef uint64_t EFI_UINTN;
typedef char CHAR8;
typedef uint16_t CHAR16;
typedef void VOID;

typedef EFI_INT32 EFI_INT;

typedef enum _EFI_BOOLEAN {
	EFI_TRUE = 1,
	EFI_FALSE = 0
} EFI_BOOLEAN;

typedef struct {
	EFI_UINT64 Uint64;
	EFI_UINT64 Uint64_1;
} EFI_UINT128;

typedef long EFI_INTPTR;
typedef unsigned long EFI_UINTPTR;

typedef long SSIZE;
typedef unsigned long SIZE;

typedef __builtin_va_list VA_LIST;
#define va_start(ap, arg) __builtin_va_start(ap, arg)
#define va_arg(ap, type) __builtin_va_arg(ap, type)
#define va_end(ap) __builtin_va_end(ap)

typedef EFI_UINTN EFI_STATUS;
typedef VOID *EFI_HANDLE;
typedef VOID *EFI_EVENT;
typedef EFI_UINT64 EFI_LBA;
typedef EFI_UINTN EFI_TPL;

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Base.h
#define IN
#define OUT
#define OPTIONAL
#define CONST const

// x86_64 Microsoft calling convention
// https://github.com/vathpela/gnu-efi/blob/master/inc/x86_64/efibind.h
#define EFIAPI __attribute__((ms_abi))

// EFI Physical Address
typedef EFI_UINT64 EFI_PHYSICAL_ADDRESS;

// EFI Virtual Address
typedef EFI_UINT64 EFI_VIRTUAL_ADDRESS;

// Task Priority Level
typedef EFI_UINTN EFI_TPL;

// String ID
typedef EFI_UINT16 EFI_STRING_ID;

// HII Handle
typedef VOID *EFI_HII_HANDLE;

// LBA
typedef EFI_UINT64 EFI_LBA;

// EFI IPv4 Address
typedef struct {
	EFI_UINT8 Addr[4];
} EFI_IPv4_ADDRESS;

// EFI IPv6 Address
typedef struct {
	EFI_UINT8 Addr[16];
} EFI_IPv6_ADDRESS;

// EFI IP Address
typedef union {
	EFI_UINT32 Addr[4];
	EFI_IPv4_ADDRESS v4;
	EFI_IPv6_ADDRESS v6;
} EFI_IP_ADDRESS;

// EFI Mac Address
typedef struct {
	EFI_UINT8 Addr[32];
} EFI_MAC_ADDRESS;

// EFI Status Codes
#define EFI_SUCCESS													(0x00)
#define EFI_LOAD_ERROR												(0x8000000000000001)
#define EFI_INVALID_PARAMETER										(0x8000000000000002)
#define EFI_UNSUPPORTED												(0x8000000000000003)
#define EFI_BAD_BUFFER_SIZE											(0x8000000000000004)
#define EFI_BUFFER_TOO_SMALL										(0x8000000000000005)
#define EFI_NOT_READY												(0x8000000000000006)
#define EFI_DEVICE_ERROR											(0x8000000000000007)
#define EFI_WRITE_PROTECTED											(0x8000000000000008)
#define EFI_OUT_OF_RESOURCES										(0x8000000000000009)
#define EFI_VOLUME_CORRUPTED										(0x800000000000000A)
#define EFI_VOLUME_FULL												(0x800000000000000B)
#define EFI_NO_MEDIA												(0x800000000000000C)
#define EFI_MEDIA_CHANGED											(0x800000000000000D)
#define EFI_NOT_FOUND												(0x800000000000000E)
#define EFI_ACCESS_DENIED											(0x800000000000000F)
#define EFI_NO_RESPONSE												(0x8000000000000010)
#define EFI_NO_MAPPING												(0x8000000000000011)
#define EFI_TIMEOUT													(0x8000000000000012)
#define EFI_NOT_STARTED												(0x8000000000000013)
#define EFI_ALREADY_STARTED											(0x8000000000000014)
#define EFI_ABORTED													(0x8000000000000015)
#define EFI_ICMP_ERROR												(0x8000000000000016)
#define EFI_TFTP_ERROR												(0x8000000000000017)
#define EFI_PROTOCOL_ERROR											(0x8000000000000018)
#define EFI_INCOMPATIBLE_VERSION									(0x8000000000000019)
#define EFI_SECURITY_VIOLATION										(0x800000000000001A)
#define EFI_CRC_ERROR												(0x800000000000001B)
#define EFI_END_OF_MEDIA											(0x800000000000001C)
#define EFI_END_OF_FILE												(0x800000000000001F)
#define EFI_INVALID_LANGUAGE										(0x8000000000000020)
#define EFI_COMPROMISED_DATA										(0x8000000000000021)
#define EFI_IP_ADDRESS_CONFLICT										(0x8000000000000022)
#define EFI_HTTP_ERROR												(0x8000000000000023)

// Text color attributes
#define EFI_BLACK										0x00
#define EFI_BLUE										0x01
#define EFI_GREEN										0x02
#define EFI_CYAN										0x03
#define EFI_RED											0x04
#define EFI_MAGENTA										0x05
#define EFI_BROWN										0x06
#define EFI_LIGHTGRAY									0x07
#define EFI_BRIGHT										0x08
#define EFI_DARKGRAY									(EFI_BLACK | EFI_BRIGHT) 0x08
#define EFI_LIGHTBLUE									0x09
#define EFI_LIGHTGREEN									0x0A
#define EFI_LIGHTCYAN									0x0B
#define EFI_LIGHTRED									0x0C
#define EFI_LIGHTMAGENTA								0x0D
#define EFI_YELLOW										0x0E
#define EFI_WHITE										0x0F

// Text background colors
#define EFI_BACKGROUND_BLACK 0x00
#define EFI_BACKGROUND_BLUE 0x10
#define EFI_BACKGROUND_GREEN 0x20
#define EFI_BACKGROUND_CYAN 0x30
#define EFI_BACKGROUND_RED 0x40
#define EFI_BACKGROUND_MAGENTA 0x50
#define EFI_BACKGROUND_BROWN 0x60
#define EFI_BACKGROUND_LIGHTGRAY 0x70

// Converts Foreground and Background colors into a single value
#define EFI_TEXT_ATTR(Foreground,Background) \
	((Foreground) | ((Background) << 4))

// Box Drawings
#define BOXDRAW_HORIZONTAL 0x2500
#define BOXDRAW_VERTICAL 0x2502
#define BOXDRAW_DOWN_RIGHT 0x250c
#define BOXDRAW_DOWN_LEFT 0x2510
#define BOXDRAW_UP_RIGHT 0x2514
#define BOXDRAW_UP_LEFT 0x2518
#define BOXDRAW_VERTICAL_RIGHT 0x251c
#define BOXDRAW_VERTICAL_LEFT 0x2524
#define BOXDRAW_DOWN_HORIZONTAL 0x252c
#define BOXDRAW_UP_HORIZONTAL 0x2534
#define BOXDRAW_VERTICAL_HORIZONTAL 0x253c
#define BOXDRAW_DOUBLE_HORIZONTAL 0x2550
#define BOXDRAW_DOUBLE_VERTICAL 0x2551
#define BOXDRAW_DOWN_RIGHT_DOUBLE 0x2552
#define BOXDRAW_DOWN_DOUBLE_RIGHT 0x2553
#define BOXDRAW_DOUBLE_DOWN_RIGHT 0x2554
#define BOXDRAW_DOWN_LEFT_DOUBLE 0x2555
#define BOXDRAW_DOWN_DOUBLE_LEFT 0x2556
#define BOXDRAW_DOUBLE_DOWN_LEFT 0x2557
#define BOXDRAW_UP_RIGHT_DOUBLE 0x2558
#define BOXDRAW_UP_DOUBLE_RIGHT 0x2559
#define BOXDRAW_DOUBLE_UP_RIGHT 0x255a
#define BOXDRAW_UP_LEFT_DOUBLE 0x255b
#define BOXDRAW_UP_DOUBLE_LEFT 0x255c
#define BOXDRAW_DOUBLE_UP_LEFT 0x255d
#define BOXDRAW_VERTICAL_RIGHT_DOUBLE 0x255e
#define BOXDRAW_VERTICAL_DOUBLE_RIGHT 0x255f
#define BOXDRAW_DOUBLE_VERTICAL_RIGHT 0x2560
#define BOXDRAW_VERTICAL_LEFT_DOUBLE 0x2561
#define BOXDRAW_VERTICAL_DOUBLE_LEFT 0x2562
#define BOXDRAW_DOUBLE_VERTICAL_LEFT 0x2563
#define BOXDRAW_DOWN_HORIZONTAL_DOUBLE 0x2564
#define BOXDRAW_DOWN_DOUBLE_HORIZONTAL 0x2565
#define BOXDRAW_DOUBLE_DOWN_HORIZONTAL 0x2566
#define BOXDRAW_UP_HORIZONTAL_DOUBLE 0x2567
#define BOXDRAW_UP_DOUBLE_HORIZONTAL 0x2568
#define BOXDRAW_DOUBLE_UP_HORIZONTAL 0x2569
#define BOXDRAW_VERTICAL_HORIZONTAL_DOUBLE 0x256a
#define BOXDRAW_VERTICAL_DOUBLE_HORIZONTAL 0x256b
#define BOXDRAW_DOUBLE_VERTICAL_HORIZONTAL 0x256c

// Block elements
#define BLOCKELEMENT_FULL_BLOCK 0x2588
#define BLOCKELEMENT_LIGHT_SHADE 0x2591

// Geometric Shapes
#define GEOMETRICSHAPE_UP_TRIANGLE 0x25b2
#define GEOMETRICSHAPE_RIGHT_TRIANGLE 0x25ba
#define GEOMETRICSHAPE_DOWN_TRIANGLE 0x25bc
#define GEOMETRICSHAPE_LEFT_TRIANGLE 0x25c4

// Arrows
#define ARROW_UP 0x2191
#define ARROW_DOWN 0x2193

// EFI_GUID
typedef struct {
	EFI_UINT32 Data1;
	EFI_UINT16 Data2;
	EFI_UINT16 Data3;
	EFI_UINT8 Data4[8];
} EFI_GUID;

// MBR Partition Entry
#pragma pack(1)
typedef struct {
	EFI_UINT8 BootIndicator;
	EFI_UINT8 StartHead;
	EFI_UINT8 StartSector;
	EFI_UINT8 StartTrack;
	EFI_UINT8 OSIndicator;
	EFI_UINT8 EndHead;
	EFI_UINT8 EndSector;
	EFI_UINT8 EndTrack;
	EFI_UINT8 StartingLBA[4];
	EFI_UINT8 SizeInLBA[4];
} MBR_PARTITION_RECORD;
#pragma pack()

// Master Boot Record
#pragma pack(1)
typedef struct {
	EFI_UINT8 BootStrapCode[440];
	EFI_UINT8 UniqueMbrSignature[4];
	EFI_UINT8 Unknown[2];
	MBR_PARTITION_RECORD Partition[4];
	EFI_UINT16 Signature;
} MASTER_BOOT_RECORD;
#pragma pack()

// GPT Partition Entry
#pragma pack(1)
typedef struct {
	EFI_GUID PartitionTypeGUID;
	EFI_GUID UniquePartitionGUID;
	EFI_LBA StartingLBA;
	EFI_LBA EndingLBA;
	EFI_UINT64 Attributes;
	CHAR16 PartitionName[36];
} EFI_PARTITION_ENTRY;
#pragma pack()

#endif /* EFI_TYPES_H */
