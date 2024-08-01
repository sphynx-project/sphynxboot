#include <common.h>
#include <fs/sfs.h>
#include <lib/print.h>
#include <lib/alloc.h>
#include <lib/string.h>
#include <boot/kernel.h>
#include <efi.h>

EFI_HANDLE *imageHandle;
EFI_SYSTEM_TABLE *systemTable;

EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *stdout;
EFI_SIMPLE_TEXT_INPUT_PROTOCOL *stdin;
EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *stderr;

void delay_seconds(int seconds) {
    EFI_STATUS status;
    EFI_UINT64 triggerTime = seconds * 10000000ULL;
    EFI_EVENT timerEvent;
    
    systemTable->BootServices->CreateEvent(
        EVT_TIMER,
        TPL_CALLBACK,
        NULL,
        NULL,
        &timerEvent
    );
    
    status = systemTable->BootServices->SetTimer(
        timerEvent,
        TimerRelative,
        triggerTime
    );
    if (EFI_ERROR(status)) {
        printf("Error setting timer.\n");
        systemTable->BootServices->CloseEvent(timerEvent);
        return;
    }

    EFI_UINTN index;
    status = systemTable->BootServices->WaitForEvent(1, &timerEvent, &index);
    if (EFI_ERROR(status)) {
        printf("Error waiting for event.\n");
    }

    systemTable->BootServices->CloseEvent(timerEvent);
}

EFI_STATUS sphynxboot_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    imageHandle = &ImageHandle;
    systemTable = &*SystemTable;
    stdout = systemTable->ConOut;
    stdin = systemTable->ConIn;
    stderr = systemTable->StdErr;

    stdout->SetAttribute(stdout, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
    stdout->SetCursorPosition(stdout, 0, 0);
    stdout->ClearScreen(stdout);

    char *logo[] = { 
        " _._     _,-'\"\"`-._",
        "(,-.`._,'(       |\\`-/|   Sphynxboot v0.0.1",
        "    `-.-' \\ )-`( , o o)",
        "          `-    \\`_`\"'-"
    };

    for (int i = 0; i < sizeof(logo) / sizeof(logo[0]); i++) {
        for(int j = 0; j < strlen(logo[i]); j++) {
            if (i == 1 && j >= 26) {
                stdout->SetAttribute(stdout, EFI_CYAN | EFI_BACKGROUND_BLACK);
            } else {
                stdout->SetAttribute(stdout, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
            }
            printf("%c", logo[i][j]);
        }
        printf("\n");
    }

    printf("\n\n");

    int countdown = 3;
    printf("Booting in 3...");
    while (countdown > 1) {
        stdout->SetAttribute(stdout, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
        delay_seconds(1);
        countdown--;
        printf("%d...", countdown);
    }

    delay_seconds(1);
    printf("0\n");

    load_kernel();

    return EFI_SUCCESS;
}
