/*
sphynxboot - A bootloader for modern systems.
Written in 2024 by Kevin Alavik <kevin@alavik.se>

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
*/

#include <efi.h>
#include <common.h>
#include <fs/sfs.h>
#include <lib/print.h>
#include <lib/alloc.h>
#include <lib/string.h>
#include <boot/kernel.h>
#include <utils/wchar.h>

#define KERNEL_PATH "kernel.elf"

EFI_HANDLE *imageHandle;
EFI_SYSTEM_TABLE *systemTable;

EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *stdout;
EFI_SIMPLE_TEXT_INPUT_PROTOCOL *stdin;
EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *stderr;

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

    char *assaved_ii_banner[] = {
        "   _____                              ____              _              ",
        "  / ____|                            |  _ \\            | |            ",
        " | (___  _ __  _ __  _   _ _ __ __  _| |_) | ___   ___ | |_            ",
        "  \\___ \\| '_ \\| '_ \\| | | | '_ \\ \\/ /  _ < / _ \\ / _ \\| __|  ",
        "  ____) | |_) | | | | |_| | | | |>  <| |_) | (_) | (_) | |_            ",
        " |_____/| .__/|_| |_|\\__, |_| |_/_/\\_\\____/ \\___/ \\___/ \\__|     ",
        "        | |           __/ |                                            ",
        "        |_|          |___/                                             ",
    };

    CHAR16 *menu[] = {
        L"╔══════════ Welcome to Sphynx ══════════╗\r\n",
        L"║                                       ║\r\n",
        L"║ 1. Boot Sphynx Normally (Enter)       ║\r\n",
        L"║ 2. Reboot                             ║\r\n",
        L"║                                       ║\r\n",
        L"║ 3. Enter Firmware Settings (Esc)      ║\r\n",
        L"║                                       ║\r\n",
        L"║                                       ║\r\n",
        L"║                                       ║\r\n",
        L"║ sphynxboot v0.0.1                     ║\r\n",
        L"╚═══════════════════════════════════════╝\r\n",
    };

    for (int i = 0; i < sizeof(assaved_ii_banner) / sizeof(assaved_ii_banner[0]); i++) {
        printf("%s\n", assaved_ii_banner[i]);
    }

    printf("\n");

    int are_we_done = 0;
    int saved_i = 0;
    int word_len = 0;

    for (int i = 0; i < sizeof(menu) / sizeof(menu[0]); i++) {
        for (int j = 0; j < wchar_strlen(menu[i]); j++) {
            CHAR16 ch = menu[i][j];

            if(are_we_done == word_len) {
                saved_i = 0;
                are_we_done = 0;
            }

            stdout->SetAttribute(stdout, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
            
            if (ch == L'B') {
                stdout->SetAttribute(stdout, EFI_WHITE | EFI_BACKGROUND_BLACK);
            } else if (ch == L'R') {
                stdout->SetAttribute(stdout, EFI_WHITE | EFI_BACKGROUND_BLACK);
            } else if ((menu[i][j-1] == L'(' && ch == L'E' && menu[i][j+1] == L'n' && menu[i][j+2] == L't' && menu[i][j+3] == L'e' && menu[i][j+4] == L'r' && menu[i][j+5] == L')') || (are_we_done != word_len && saved_i == i && word_len == 5)) {
                if(are_we_done == 0) {
                    saved_i = i;
                    word_len = 5;
                }

                are_we_done++;
                stdout->SetAttribute(stdout, EFI_WHITE | EFI_BACKGROUND_BLACK);
            } else if ((menu[i][j-1] == L'(' && ch == L'E' && menu[i][j+1] == L's' && menu[i][j+2] == L'c' && menu[i][j+3] == L')') || (are_we_done != word_len && saved_i == i && word_len == 3)) {
                if(are_we_done == 0) {
                    saved_i = i;
                    word_len = 3;
                }

                are_we_done++;
                stdout->SetAttribute(stdout, EFI_WHITE | EFI_BACKGROUND_BLACK);
            } else {
                stdout->SetAttribute(stdout, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
            }

            CHAR16 str[2] = {ch, L'\0'};
            stdout->OutputString(stdout, str);
        }
    }

    printf("Autobooting in 5 seconds (space to cancel)");

    EFI_INPUT_KEY key;
    EFI_UINTN key_event = 0;

    int countdown = 5;
    int auto_boot = 1;
    while (1) {
        stdout->SetAttribute(stdout, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
        stdout->SetCursorPosition(stdout, 0, 20);

        SystemTable->BootServices->WaitForEvent(1, &(SystemTable->ConIn->WaitForKey), &key_event);
        stdin->ReadKeyStroke(stdin, &key);

        switch (key.ScanCode)
        {
        case EFI_SCANCODE_ESC:
            return 0;
        default:
            switch (key.UnicodeChar)
            {
            case '\r':
            case 'b':
                load_kernel(KERNEL_PATH);
                SystemTable->BootServices->Exit(ImageHandle, 0, 0, NULL);
                break;
            case ' ':
                printf("Auto-boot canceled. Press Enter to boot normally.\n");
                auto_boot = 0;
                break;
            default:       
               break;
            }
        }

        if (auto_boot) {
            stdout->SetAttribute(stdout, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
            stdout->SetCursorPosition(stdout, 0, 20);
            printf("Autobooting in %d seconds (space to cancel)\n", countdown);

            countdown--;
            if (countdown > 0) {
                SystemTable->BootServices->Stall(1000000);
            }

            if(countdown == 0) {   
                stdout->SetAttribute(stdout, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
                stdout->SetCursorPosition(stdout, 0, 20);
                printf("Autobooting in %d seconds (space to cancel)\n", countdown);                     
                if (auto_boot) {
                    load_kernel(KERNEL_PATH);
                }
            }
        }; 

    }


    return EFI_SUCCESS;
}
