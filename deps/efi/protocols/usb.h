#ifndef EFI_PROTOCOLS_USB_H
#define EFI_PROTOCOLS_USB_H

////
// USB I/O Protocol
///

#define EFI_USB_IO_PROTOCOL_GUID {0x2b2f68d6,0x0cd2,0x44cf, {0x8e,0x8b,0xbb,0xa2,0x0b,0x1b,0x5b,0x75}}

// USB Transfer result error codes
#define EFI_USB_NOERROR 0x0000
#define EFI_USB_ERR_NOTEXECUTE 0x0001
#define EFI_USB_ERR_STALL 0x0002
#define EFI_USB_ERR_BUFFER 0x0004
#define EFI_USB_ERR_BABBLE 0x0008
#define EFI_USB_ERR_NAK 0x0010
#define EFI_USB_ERR_CRC 0x0020
#define EFI_USB_ERR_TIMEOUT 0x0040
#define EFI_USB_ERR_BITSTUFF 0x0080
#define EFI_USB_ERR_SYSTEM 0x0100

typedef struct _EFI_USB_IO_PROTOCOL EFI_USB_IO_PROTOCOL;

typedef enum {
	EfiUsbDataIn,
	EfiUsbDataOut,
	EfiUsbNoData
} EFI_USB_DATA_DIRECTION;

typedef struct {
	EFI_UINT8 RequestType;
	EFI_UINT8 Request;
	EFI_UINT16 Value;
	EFI_UINT16 Index;
	EFI_UINT16 Length;
} EFI_USB_DEVICE_REQUEST;

typedef struct {
	EFI_UINT8 Length;
	EFI_UINT8 DescriptorType;
	EFI_UINT16 TotalLength;
	EFI_UINT8 NumInterfaces;
	EFI_UINT8 ConfigurationValue;
	EFI_UINT8 Configuration;
	EFI_UINT8 Attributes;
	EFI_UINT8 MaxPower;
} EFI_USB_CONFIG_DESCRIPTOR;

typedef struct {
	EFI_UINT8 Length;
	EFI_UINT8 DescriptorType;
	EFI_UINT8 InterfaceNumber;
	EFI_UINT8 AlternateSetting;
	EFI_UINT8 NumEndpoints;
	EFI_UINT8 InterfaceClass;
	EFI_UINT8 InterfaceSubClass;
	EFI_UINT8 InterfaceProtocol;
	EFI_UINT8 Interface;
} EFI_USB_INTERFACE_DESCRIPTOR;

typedef struct {
	EFI_UINT8 Length;
	EFI_UINT8 DescriptorType;
	EFI_UINT8 EndpointAddress;
	EFI_UINT8 Attributes;
	EFI_UINT16 MaxPacketSize;
	EFI_UINT8 Interval;
} EFI_USB_ENDPOINT_DESCRIPTOR;

typedef struct {
	EFI_UINT8 Length;
	EFI_UINT8 DescriptorType;
	EFI_UINT16 BcdUSB;
	EFI_UINT8 DeviceClass;
	EFI_UINT8 DeviceSubClass;
	EFI_UINT8 DeviceProtocol;
	EFI_UINT8 MaxPacketSize0;
	EFI_UINT16 IdVendor;
	EFI_UINT16 IdProduct;
	EFI_UINT16 BcdDevice;
	EFI_UINT8 StrManufacturer;
	EFI_UINT8 StrProduct;
	EFI_UINT8 StrSerialNumber;
	EFI_UINT8 NumConfigurations;
} EFI_USB_DEVICE_DESCRIPTOR;

typedef
EFI_STATUS
(EFIAPI *EFI_USB_IO_CONTROL_TRANSFER)(
	IN EFI_USB_IO_PROTOCOL *This,
	IN EFI_USB_DEVICE_REQUEST *Request,
	IN EFI_USB_DATA_DIRECTION Direction,
	IN EFI_UINT32 Timeout,
	IN OUT VOID *Data OPTIONAL,
	IN EFI_UINTN DataLength OPTIONAL,
	OUT EFI_UINT32 *Status
);

typedef
EFI_STATUS
(EFIAPI *EFI_USB_IO_BULK_TRANSFER)(
	IN EFI_USB_IO_PROTOCOL *This,
	IN EFI_UINT8 DeviceEndpoint,
	IN OUT VOID *Data,
	IN OUT EFI_UINTN *DataLength,
	IN EFI_UINTN Timeout,
	OUT EFI_UINT32 *Status
);

typedef
EFI_STATUS
(EFIAPI *EFI_ASYNC_USB_TRANSFER_CALLBACK)(
	IN VOID *Data,
	IN EFI_UINTN DataLength,
	IN VOID *Context,
	IN EFI_UINT32 Status
);

typedef
EFI_STATUS
(EFIAPI *EFI_USB_IO_ASYNC_INTERRUPT_TRANSFER)(
	IN EFI_USB_IO_PROTOCOL *This,
	IN EFI_UINT8 DeviceEndpoint,
	IN EFI_BOOLEAN IsNewTransfer,
	IN EFI_UINTN PollingInterval OPTIONAL,
	IN EFI_UINTN DataLength OPTIONAL,
	IN EFI_ASYNC_USB_TRANSFER_CALLBACK InterruptCallBack OPTIONAL,
	IN VOID *Context OPTIONAL
);

typedef
EFI_STATUS
(EFIAPI *EFI_USB_IO_SYNC_INTERRUPT_TRANSFER)(
	IN EFI_USB_IO_PROTOCOL *This,
	IN EFI_UINT8 DeviceEndpoint,
	IN OUT VOID *Data,
	IN OUT EFI_UINTN *DataLength,
	IN EFI_UINTN Timeout,
	OUT EFI_UINT32 *Status
);

typedef
EFI_STATUS
(EFIAPI *EFI_USB_IO_ISOCHRONOUS_TRANSFER)(
	IN EFI_USB_IO_PROTOCOL *This,
	IN EFI_UINT8 DeviceEndpoint,
	IN OUT VOID *Data,
	IN EFI_UINTN DataLength,
	OUT EFI_UINT32 *Status
);

typedef
EFI_STATUS
(EFIAPI *EFI_USB_IO_ASYNC_ISOCHRONOUS_TRANSFER)(
	IN EFI_USB_IO_PROTOCOL *This,
	IN EFI_UINT8 DeviceEndpoint,
	IN OUT VOID *Data,
	IN EFI_UINTN DataLength,
	IN EFI_ASYNC_USB_TRANSFER_CALLBACK IsochronousCallBack,
	IN VOID *Context OPTIONAL
);

typedef
EFI_STATUS
(EFIAPI *EFI_USB_IO_GET_DEVICE_DESCRIPTOR)(
	IN EFI_USB_IO_PROTOCOL *This,
	OUT EFI_USB_DEVICE_DESCRIPTOR *DeviceDescriptor
);

typedef
EFI_STATUS
(EFIAPI *EFI_USB_IO_GET_CONFIG_DESCRIPTOR)(
	IN EFI_USB_IO_PROTOCOL *This,
	OUT EFI_USB_CONFIG_DESCRIPTOR *ConfigurationDescriptor
);

typedef
EFI_STATUS
(EFIAPI *EFI_USB_IO_GET_INTERFACE_DESCRIPTOR)(
	IN EFI_USB_IO_PROTOCOL *This,
	OUT EFI_USB_INTERFACE_DESCRIPTOR *InterfaceDescriptor
);

typedef
EFI_STATUS
(EFIAPI *EFI_USB_IO_GET_ENDPOINT_DESCRIPTOR)(
	IN EFI_USB_IO_PROTOCOL *This,
	IN EFI_UINT8 EndpointIndex,
	OUT EFI_USB_ENDPOINT_DESCRIPTOR *EndpointDescriptor
);

typedef
EFI_STATUS
(EFIAPI *EFI_USB_IO_GET_STRING_DESCRIPTOR)(
	IN EFI_USB_IO_PROTOCOL *This,
	IN EFI_UINT16 LangID,
	IN EFI_UINT8 StringID,
	OUT CHAR16 **String
);

typedef
EFI_STATUS
(EFIAPI *EFI_USB_IO_GET_SUPPORTED_LANGUAGES)(
	IN EFI_USB_IO_PROTOCOL *This,
	OUT EFI_UINT16 **LangIDTable,
	OUT EFI_UINT16 *TableSize
);

typedef
EFI_STATUS
(EFIAPI *EFI_USB_IO_PORT_RESET)(
	IN EFI_USB_IO_PROTOCOL *This
);

typedef struct _EFI_USB_IO_PROTOCOL {
	EFI_USB_IO_CONTROL_TRANSFER UsbControlTransfer;
	EFI_USB_IO_BULK_TRANSFER UsbBulkTransfer;
	EFI_USB_IO_ASYNC_INTERRUPT_TRANSFER UsbAsyncInterruptTransfer;
	EFI_USB_IO_SYNC_INTERRUPT_TRANSFER UsbSyncInterruptTransfer;
	EFI_USB_IO_ISOCHRONOUS_TRANSFER UsbIsochronousTransfer;
	EFI_USB_IO_ASYNC_ISOCHRONOUS_TRANSFER UsbAsyncIsochronousTransfer;
	EFI_USB_IO_GET_DEVICE_DESCRIPTOR UsbGetDeviceDescriptor;
	EFI_USB_IO_GET_CONFIG_DESCRIPTOR UsbGetConfigDescriptor;
	EFI_USB_IO_GET_INTERFACE_DESCRIPTOR UsbGetInterfaceDescriptor;
	EFI_USB_IO_GET_ENDPOINT_DESCRIPTOR UsbGetEndpointDescriptor;
	EFI_USB_IO_GET_STRING_DESCRIPTOR UsbGetStringDescriptor;
	EFI_USB_IO_GET_SUPPORTED_LANGUAGES UsbGetSupportedLanguages;
	EFI_USB_IO_PORT_RESET UsbPortReset;
} EFI_USB_IO_PROTOCOL;

////
// USB2 Host Controller Protocol
///

#define EFI_USB2_HC_PROTOCOL_GUID {0x3e745226,0x9818,0x45b6, {0xa2,0xac,0xd7,0xcd,0x0e,0x8b,0xa2,0xbc}}

#define EFI_USB_MAX_BULK_BUFFER_NUM 10

#define EFI_USB_SPEED_FULL 0x0000
#define EFI_USB_SPEED_LOW 0x0001
#define EFI_USB_SPEED_HIGH 0x0002
#define EFI_USB_SPEED_SUPER 0x0003

#define EFI_USB_HC_RESET_GLOBAL 0x0001
#define EFI_USB_HC_RESET_HOST_CONTROLLER 0x0002
#define EFI_USB_HC_RESET_GLOBAL_WITH_DEBUG 0x0004
#define EFI_USB_HC_RESET_HOST_WITH_DEBUG 0x0008

#define EFI_USB_MAX_ISO_BUFFER_NUM 7
#define EFI_USB_MAX_ISO_BUFFER_NUM1 2

// EFI_USB_PORT_STATUS.PortStatus bit
#define USB_PORT_STAT_CONNECTION 0x0001
#define USB_PORT_STAT_ENABLE 0x0002
#define USB_PORT_STAT_SUSPEND 0x0004
#define USB_PORT_STAT_OVERCURRENT 0x0008
#define USB_PORT_STAT_RESET 0x0010
#define USB_PORT_STAT_POWER 0x0100
#define USB_PORT_STAT_LOW_SPEED 0x0200
#define USB_PORT_STAT_HIGH_SPEED 0x0400
#define USB_PORT_STAT_SUPER_SPEED 0x0800
#define USB_PORT_STAT_OWNER 0x2000

// EFI_USB_PORT_STATUS.PortChangeStatus bit
#define USB_PORT_STAT_C_CONNECTION 0x0001
#define USB_PORT_STAT_C_ENABLE 0x0002
#define USB_PORT_STAT_C_SUSPEND 0x0004
#define USB_PORT_STAT_C_OVERCURRENT 0x0008
#define USB_PORT_STAT_C_RESET 0x0010

typedef struct _EFI_USB2_HC_PROTOCOL EFI_USB2_HC_PROTOCOL;

typedef enum {
	EfiUsbHcStateHalt,
	EfiUsbHcStateOperational,
	EfiUsbHcStateSuspend,
	EfiUsbHcStateMaximum
} EFI_USB_HC_STATE;

typedef enum {
	EfiUsbPortEnable = 1,
	EfiUsbPortSuspend = 2,
	EfiUsbPortReset = 4,
	EfiUsbPortPower = 8,
	EfiUsbPortOwner = 13,
	EfiUsbPortConnectChange = 16,
	EfiUsbPortEnableChange = 17,
	EfiUsbPortSuspendChange = 18,
	EfiUsbPortOverCurrentChange = 19,
	EfiUsbPortResetChange = 20
} EFI_USB_PORT_FEATURE;

typedef struct {
	EFI_UINT8 TranslatorHubAddress;
	EFI_UINT8 TranslatorPortNumber;
} EFI_USB2_HC_TRANSACTION_TRANSLATOR;

typedef struct {
	EFI_UINT16 PortStatus;
	EFI_UINT16 PortChangeStatus;
} EFI_USB_PORT_STATUS;

typedef
EFI_STATUS
(EFIAPI *EFI_USB2_HC_PROTOCOL_GET_CAPABILITY)(
	IN EFI_USB2_HC_PROTOCOL *This,
	OUT EFI_UINT8 *MaxSpeed,
	OUT EFI_UINT8 *PortNumber,
	OUT EFI_UINT8 *Is64BitCapable
);

typedef
EFI_STATUS
(EFIAPI *EFI_USB2_HC_PROTOCOL_RESET)(
	IN EFI_USB2_HC_PROTOCOL *This,
	IN EFI_UINT16 Attributes
);

typedef
EFI_STATUS
(EFIAPI *EFI_USB2_HC_PROTOCOL_GET_STATE)(
	IN EFI_USB2_HC_PROTOCOL *This,
	OUT EFI_USB_HC_STATE *State
);

typedef
EFI_STATUS
(EFIAPI *EFI_USB2_HC_PROTOCOL_SET_STATE)(
	IN EFI_USB2_HC_PROTOCOL *This,
	IN EFI_USB_HC_STATE State
);

typedef
EFI_STATUS
(EFIAPI *EFI_USB2_HC_PROTOCOL_CONTROL_TRANSFER)(
	IN EFI_USB2_HC_PROTOCOL *This,
	IN EFI_UINT8 DeviceAddress,
	IN EFI_UINT8 DeviceSpeed,
	IN EFI_UINTN MaximumPacketLength,
	IN EFI_USB_DEVICE_REQUEST *Request,
	IN EFI_USB_DATA_DIRECTION TransferDirection,
	IN OUT VOID *Data OPTIONAL,
	IN OUT EFI_UINTN *DataLength OPTIONAL,
	IN EFI_UINTN TimeOut,
	IN EFI_USB2_HC_TRANSACTION_TRANSLATOR *Translator,
	OUT EFI_UINT32 *TransferResult
);

typedef
EFI_STATUS
(EFIAPI *EFI_USB2_HC_PROTOCOL_BULK_TRANSFER)(
	IN EFI_USB2_HC_PROTOCOL *This,
	IN EFI_UINT8 DeviceAddress,
	IN EFI_UINT8 EndPointAddress,
	IN EFI_UINT8 DeviceSpeed,
	IN EFI_UINTN MaximumPacketLength,
	IN EFI_UINT8 DataBuffersNumber,
	IN OUT VOID *Data[EFI_USB_MAX_BULK_BUFFER_NUM],
	IN OUT EFI_UINTN *DataLength,
	IN OUT EFI_UINT8 *DataToggle,
	IN EFI_UINTN TimeOut,
	IN EFI_USB2_HC_TRANSACTION_TRANSLATOR *Translator,
	OUT EFI_UINT32 *TransferResult
);

typedef
EFI_STATUS
(EFIAPI *EFI_USB2_HC_PROTOCOL_ASYNC_INTERRUPT_TRANSFER)(
	IN EFI_USB2_HC_PROTOCOL *This,
	IN EFI_UINT8 DeviceAddress,
	IN EFI_UINT8 EndPointAddress,
	IN EFI_UINT8 DeviceSpeed,
	IN EFI_UINTN MaximumPacketLength,
	IN EFI_BOOLEAN IsNewTransfer,
	IN OUT EFI_UINT8 *DataToggle,
	IN EFI_UINTN PollingInterval OPTIONAL,
	IN EFI_UINTN DataLength OPTIONAL,
	IN EFI_USB2_HC_TRANSACTION_TRANSLATOR *Translator OPTIONAL,
	IN EFI_ASYNC_USB_TRANSFER_CALLBACK CallBackFunction OPTIONAL,
	IN VOID *Context OPTIONAL
);

typedef
EFI_STATUS
(EFIAPI *EFI_USB2_HC_PROTOCOL_SYNC_INTERRUPT_TRANSFER)(
	IN EFI_USB2_HC_PROTOCOL *This,
	IN EFI_UINT8 DeviceAddress,
	IN EFI_UINT8 EndPointAddress,
	IN EFI_UINT8 DeviceSpeed,
	IN EFI_UINTN MaximumPacketLength,
	IN OUT VOID *Data,
	IN OUT EFI_UINTN *DataLength,
	IN OUT EFI_UINT8 *DataToggle,
	IN EFI_UINTN TimeOut,
	IN EFI_USB2_HC_TRANSACTION_TRANSLATOR *Translator,
	OUT EFI_UINT32 *TransferResult
);

typedef
EFI_STATUS
(EFIAPI *EFI_USB2_HC_PROTOCOL_ISOCHRONOUS_TRANSFER)(
	IN EFI_USB2_HC_PROTOCOL *This,
	IN EFI_UINT8 DeviceAddress,
	IN EFI_UINT8 EndPointAddress,
	IN EFI_UINT8 DeviceSpeed,
	IN EFI_UINTN MaximumPacketLength,
	IN EFI_UINT8 DataBuffersNumber,
	IN OUT VOID *Data[EFI_USB_MAX_ISO_BUFFER_NUM],
	IN EFI_UINTN DataLength,
	IN EFI_USB2_HC_TRANSACTION_TRANSLATOR *Translator,
	OUT EFI_UINT32 *TransferResult
);

typedef
EFI_STATUS
(EFIAPI *EFI_USB2_HC_PROTOCOL_ASYNC_ISOCHRONOUS_TRANSFER)(
	IN EFI_USB2_HC_PROTOCOL *This,
	IN EFI_UINT8 DeviceAddress,
	IN EFI_UINT8 EndPointAddress,
	IN EFI_UINT8 DeviceSpeed,
	IN EFI_UINTN MaximumPacketLength,
	IN EFI_UINT8 DataBuffersNumber,
	IN OUT VOID *Data[EFI_USB_MAX_ISO_BUFFER_NUM],
	IN EFI_UINTN DataLength,
	IN EFI_USB2_HC_TRANSACTION_TRANSLATOR *Translator,
	IN EFI_ASYNC_USB_TRANSFER_CALLBACK IsochronousCallBack,
	IN VOID *Context OPTIONAL
);

typedef
EFI_STATUS
(EFIAPI *EFI_USB2_HC_PROTOCOL_GET_ROOTHUB_PORT_STATUS)(
	IN EFI_USB2_HC_PROTOCOL *This,
	IN EFI_UINT8 PortNumber,
	OUT EFI_USB_PORT_STATUS *PortStatus
);

typedef
EFI_STATUS
(EFIAPI *EFI_USB2_HC_PROTOCOL_SET_ROOTHUB_PORT_FEATURE)(
	IN EFI_USB2_HC_PROTOCOL *This,
	IN EFI_UINT8 PortNumber,
	IN EFI_USB_PORT_FEATURE PortFeature
);

typedef
EFI_STATUS
(EFIAPI *EFI_USB2_HC_PROTOCOL_CLEAR_ROOTHUB_PORT_FEATURE)(
	IN EFI_USB2_HC_PROTOCOL *This,
	IN EFI_UINT8 PortNumber,
	IN EFI_USB_PORT_FEATURE PortFeature
);

typedef struct _EFI_USB2_HC_PROTOCOL {
	EFI_USB2_HC_PROTOCOL_GET_CAPABILITY GetCapability;
	EFI_USB2_HC_PROTOCOL_RESET Reset;
	EFI_USB2_HC_PROTOCOL_GET_STATE GetState;
	EFI_USB2_HC_PROTOCOL_SET_STATE SetState;
	EFI_USB2_HC_PROTOCOL_CONTROL_TRANSFER ControlTransfer;
	EFI_USB2_HC_PROTOCOL_BULK_TRANSFER BulkTransfer;
	EFI_USB2_HC_PROTOCOL_ASYNC_INTERRUPT_TRANSFER AsyncInterruptTransfer;
	EFI_USB2_HC_PROTOCOL_ASYNC_INTERRUPT_TRANSFER SyncInterruptTransfer;
	EFI_USB2_HC_PROTOCOL_ISOCHRONOUS_TRANSFER IsochronousTransfer;
	EFI_USB2_HC_PROTOCOL_ASYNC_ISOCHRONOUS_TRANSFER AsyncIsochronousTransfer;
	EFI_USB2_HC_PROTOCOL_GET_ROOTHUB_PORT_STATUS GetRootHubPortStatus;
	EFI_USB2_HC_PROTOCOL_SET_ROOTHUB_PORT_FEATURE SetRootHubPortFeature;
	EFI_USB2_HC_PROTOCOL_CLEAR_ROOTHUB_PORT_FEATURE ClearRootHubPortFeature;
	EFI_UINT16 MajorRevision;
	EFI_UINT16 MinorRevision;
} EFI_USB2_HC_PROTOCOL;

////
// USB Function Protocol
///

#define EFI_USBFN_IO_PROTOCOL_GUID {0x32d2963a,0xfe5d, 0x4f30, {0xb6,0x33,0x6e,0x5d,0xc5,0x58,0x3,0xcc}}

#define EFI_USBFN_IO_PROTOCOL_REVISION 0x00010001

typedef struct _EFI_USBFN_IO_PROTOCOL EFI_USBFN_IO_PROTOCOL;

typedef enum {
	EfiUsbUnknownPort = 0,
	EfiUsbStandardDownstreamPort,
	EfiUsbChargingDownstreamPort,
	EfiUsbDedicatedChargingPort,
	EfiUsbInvalidDedicatedChargingPort
} EFI_USBFN_PORT_TYPE;

typedef enum {
	UsbEndpointControl = 0x00,
	// UsbEndpointIsochronous = 0x01,
	UsbEndpointBulk = 0x02,
	// UsbEndpointInterrupt = 0x03
} EFI_USB_ENDPOINT_TYPE;

typedef enum {
	EfiUsbDeviceInfoUnknown = 0,
	EfiUsbDeviceInfoSerialNumber,
	EfiUsbDeviceInfoManufacturerName,
	EfiUsbDeviceInfoProductName
} EFI_USBFN_DEVICE_INFO_ID;

typedef enum {
	EfiUsbEndpointDirectionHostOut = 0,
	EfiUsbEndpointDirectionHostIn,
	EfiUsbEndpointDirectionDeviceTx = EfiUsbEndpointDirectionHostIn,
	EfiUsbEndpointDirectionDeviceRx = EfiUsbEndpointDirectionHostOut
} EFI_USBFN_ENDPOINT_DIRECTION;

typedef enum {
	// Nothing
	EfiUsbMsgNone = 0,
	// SETUP packet is received, returned Buffer contains
	// EFI_USB_DEVICE_REQUEST struct
	EfiUsbMsgSetupPacket,
	// Indicates that some of the requested data has been
	// received from the host. It is the responsibility of the
	// class driver to determine if it needs to wait for any
	// remaining data. Returned Buffer contains
	// EFI_USBFN_TRANSFER_RESULT struct containing endpoint
	// number, transfer status and count of bytes received.
	EfiUsbMsgEndpointStatusChangedRx,
	// Indicates that some of the requested data has been
	// transmitted to the host. It is the responsibility of the
	// class driver to determine if anyremaining data needs to be
	// resent. Returned Buffer contains
	// EFI_USBFN_TRANSFER_RESULT struct containing endpoint
	// number, transferstatus andcount of bytes sent.
	EfiUsbMsgEndpointStatusChangedTx,
	// DETACH bus event signaled
	EfiUsbMsgBusEventDetach,
	// ATTACH bus event signaled
	EfiUsbMsgBusEventAttach,
	// RESET bus event signaled
	EfiUsbMsgBusEventReset,
	// SUSPEND bus event signaled
	EfiUsbMsgBusEventSuspend,
	// RESUME bus event signaled
	EfiUsbMsgBusEventResume,
	// Bus speed updated, returned buffer indicated bus speed
	// using following enumeration named EFI_USB_BUS_SPEED
	EfiUsbMsgBusEventSpeed
} EFI_USBFN_MESSAGE;

typedef enum {
	UsbTransferStatusUnknown = 0,
	UsbTransferStatusComplete,
	UsbTransferStatusAborted,
	UsbTransferStatusActive,
	UsbTransferStatusNone
} EFI_USBFN_TRANSFER_STATUS;

typedef enum _EFI_USB_BUS_SPEED {
	UsbBusSpeedUnknown = 0,
	UsbBusSpeedLow,
	UsbBusSpeedFull,
	UsbBusSpeedHigh,
	UsbBusSpeedSuper,
	UsbBusSpeedMaximum = UsbBusSpeedSuper
} EFI_USB_BUS_SPEED;

typedef enum {
	EfiUsbPolicyUndefined = 0,
	EfiUsbPolicyMaxTransactionSize,
	EfiUsbPolicyZeroLengthTerminationSupport,
	EfiUsbPolicyZeroLengthTermination
} EFI_USBFN_POLICY_TYPE;

typedef struct _EFI_USBFN_TRANSFER_RESULT {
	EFI_UINTN BytesTransferred;
	EFI_USBFN_TRANSFER_STATUS TransferStatus;
	EFI_UINT8 EndpointIndex;
	EFI_USBFN_ENDPOINT_DIRECTION Direction;
	VOID *Buffer;
} EFI_USBFN_TRANSFER_RESULT;

typedef union _EFI_USBFN_MESSAGE_PAYLOAD {
	EFI_USB_DEVICE_REQUEST udr;
	EFI_USBFN_TRANSFER_RESULT utr;
	EFI_USB_BUS_SPEED ubs;
} EFI_USBFN_MESSAGE_PAYLOAD;

typedef struct {
	EFI_USB_INTERFACE_DESCRIPTOR *InterfaceDescriptor;
	EFI_USB_ENDPOINT_DESCRIPTOR **EndpointDescriptorTable;
} EFI_USB_INTERFACE_INFO;

typedef struct {
	EFI_USB_CONFIG_DESCRIPTOR *ConfigDescriptor;
	EFI_USB_INTERFACE_INFO **InterfaceInfoTable;
} EFI_USB_CONFIG_INFO;

typedef struct {
	EFI_USB_DEVICE_DESCRIPTOR *DeviceDescriptor;
	EFI_USB_CONFIG_INFO **ConfigInfoTable;
} EFI_USB_DEVICE_INFO;

typedef
EFI_STATUS
(EFIAPI *EFI_USBFN_IO_DETECT_PORT)(
	IN EFI_USBFN_IO_PROTOCOL *This,
	OUT EFI_USBFN_PORT_TYPE *PortType
);

typedef
EFI_STATUS
(EFIAPI *EFI_USBFN_IO_CONFIGURE_ENABLE_ENDPOINTS)(
	IN EFI_USBFN_IO_PROTOCOL *This,
	IN EFI_USB_DEVICE_INFO *DeviceInfo
);

typedef
EFI_STATUS
(EFIAPI *EFI_USBFN_IO_GET_ENDPOINT_MAXPACKET_SIZE)(
	IN EFI_USBFN_IO_PROTOCOL *This,
	IN EFI_USB_ENDPOINT_TYPE EndpointType,
	IN EFI_USB_BUS_SPEED BusSpeed,
	OUT EFI_UINT16 *MaxPacketSize
);

typedef
EFI_STATUS
(EFIAPI *EFI_USBFN_IO_GET_DEVICE_INFO)(
	IN EFI_USBFN_IO_PROTOCOL *This,
	IN EFI_USBFN_DEVICE_INFO_ID Id,
	IN OUT EFI_UINTN *BufferSize,
	OUT VOID *Buffer OPTIONAL
);

typedef
EFI_STATUS
(EFIAPI *EFI_USBFN_IO_GET_VENDOR_ID_PRODUCT_ID)(
	IN EFI_USBFN_IO_PROTOCOL *This,
	OUT EFI_UINT16 *Vid,
	OUT EFI_UINT16 *Pid
);

typedef
EFI_STATUS
(EFIAPI *EFI_USBFN_IO_ABORT_TRANSFER)(
	IN EFI_USBFN_IO_PROTOCOL *This,
	IN EFI_UINT8 EndpointIndex,
	IN EFI_USBFN_ENDPOINT_DIRECTION Direction
);

typedef
EFI_STATUS
(EFIAPI *EFI_USBFN_IO_GET_ENDPOINT_STALL_STATE)(
	IN EFI_USBFN_IO_PROTOCOL *This,
	IN EFI_UINT8 EndpointIndex,
	IN EFI_USBFN_ENDPOINT_DIRECTION Direction,
	IN OUT EFI_BOOLEAN *State
);

typedef
EFI_STATUS
(EFIAPI *EFI_USBFN_IO_SET_ENDPOINT_STALL_STATE)(
	IN EFI_USBFN_IO_PROTOCOL *This,
	IN EFI_UINT8 EndpointIndex,
	IN EFI_USBFN_ENDPOINT_DIRECTION Direction,
	IN EFI_BOOLEAN State
);

typedef
EFI_STATUS
(EFIAPI *EFI_USBFN_IO_EVENTHANDLER)(
	IN EFI_USBFN_IO_PROTOCOL *This,
	OUT EFI_USBFN_MESSAGE *Message,
	IN OUT EFI_UINTN *PayloadSize,
	OUT EFI_USBFN_MESSAGE_PAYLOAD *Payload
);

typedef
EFI_STATUS
(EFIAPI *EFI_USBFN_IO_TRANSFER)(
	IN EFI_USBFN_IO_PROTOCOL *This,
	IN EFI_UINT8 EndpointIndex,
	IN EFI_USBFN_ENDPOINT_DIRECTION Direction,
	IN OUT EFI_UINTN *BufferSize,
	IN OUT VOID *Buffer
);

typedef
EFI_STATUS
(EFIAPI *EFI_USBFN_IO_GET_MAXTRANSFER_SIZE)(
	IN EFI_USBFN_IO_PROTOCOL *This,
	OUT EFI_UINTN *MaxTransferSize
);

typedef
EFI_STATUS
(EFIAPI *EFI_USBFN_IO_ALLOCATE_TRANSFER_BUFFER)(
	IN EFI_USBFN_IO_PROTOCOL *This,
	IN EFI_UINTN Size,
	OUT VOID **Buffer
);

typedef
EFI_STATUS
(EFIAPI *EFI_USBFN_IO_FREE_TRANSFER_BUFFER)(
	IN EFI_USBFN_IO_PROTOCOL *This,
	IN VOID *Buffer
);

typedef
EFI_STATUS
(EFIAPI *EFI_USBFN_IO_START_CONTROLLER)(
	IN EFI_USBFN_IO_PROTOCOL *This
);

typedef
EFI_STATUS
(EFIAPI *EFI_USBFN_IO_STOP_CONTROLLER)(
	IN EFI_USBFN_IO_PROTOCOL *This
);

typedef
EFI_STATUS
(EFIAPI *EFI_USBFN_IO_SET_ENDPOINT_POLICY)(
	IN EFI_USBFN_IO_PROTOCOL *This,
	IN EFI_UINT8 EndpointIndex,
	IN EFI_USBFN_ENDPOINT_DIRECTION Direction,
	IN EFI_USBFN_POLICY_TYPE PolicyType,
	IN EFI_UINTN BufferSize,
	IN VOID *Buffer
);

typedef
EFI_STATUS
(EFIAPI *EFI_USBFN_IO_GET_ENDPOINT_POLICY)(
	IN EFI_USBFN_IO_PROTOCOL *This,
	IN EFI_UINT8 EndpointIndex,
	IN EFI_USBFN_ENDPOINT_DIRECTION Direction,
	IN EFI_USBFN_POLICY_TYPE PolicyType,
	IN OUT EFI_UINTN *BufferSize,
	IN OUT VOID *Buffer
);

typedef struct _EFI_USBFN_IO_PROTOCOL {
	EFI_UINT32 Revision;
	EFI_USBFN_IO_DETECT_PORT DetectPort;
	EFI_USBFN_IO_CONFIGURE_ENABLE_ENDPOINTS ConfigureEnableEndpoints;
	EFI_USBFN_IO_GET_ENDPOINT_MAXPACKET_SIZE GetEndpointMaxPacketSize;
	EFI_USBFN_IO_GET_DEVICE_INFO GetDeviceInfo;
	EFI_USBFN_IO_GET_VENDOR_ID_PRODUCT_ID GetVendorIdProductId;
	EFI_USBFN_IO_ABORT_TRANSFER AbortTransfer;
	EFI_USBFN_IO_GET_ENDPOINT_STALL_STATE GetEndpointStallState;
	EFI_USBFN_IO_SET_ENDPOINT_STALL_STATE SetEndpointStallState;
	EFI_USBFN_IO_EVENTHANDLER EventHandler;
	EFI_USBFN_IO_TRANSFER Transfer;
	EFI_USBFN_IO_GET_MAXTRANSFER_SIZE GetMaxTransferSize;
	EFI_USBFN_IO_ALLOCATE_TRANSFER_BUFFER AllocateTransferBuffer;
	EFI_USBFN_IO_FREE_TRANSFER_BUFFER FreeTransferBuffer;
	EFI_USBFN_IO_START_CONTROLLER StartController;
	EFI_USBFN_IO_STOP_CONTROLLER StopController;
	EFI_USBFN_IO_SET_ENDPOINT_POLICY SetEndpointPolicy;
	EFI_USBFN_IO_GET_ENDPOINT_POLICY GetEndpointPolicy;
} EFI_USBFN_IO_PROTOCOL;

#endif /* EFI_PROTOCOLS_USB_H */
