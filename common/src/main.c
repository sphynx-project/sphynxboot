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

#define CONFIG_PATH "\\boot.conf"

EFI_HANDLE *imageHandle;
EFI_SYSTEM_TABLE *systemTable;

EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *stdout;
EFI_SIMPLE_TEXT_INPUT_PROTOCOL *stdin;
EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *stderr;

void cfg_get_key(const char* buffer, const char* key, char** value) {
    if (buffer == NULL || key == NULL || value == NULL) {
        return;
    }

    const char* key_start = strstr(buffer, key);
    if (key_start == NULL) {
        *value = NULL;
        return;
    }

    key_start += strlen(key);
    if (*key_start != '=') {
        *value = NULL;
        return;
    }
    key_start++;

    const char* value_end = key_start;
    while (*value_end != '\0' && *value_end != ' ' && *value_end != '\n' && *value_end != '\r') {
        value_end++;
    }

    size_t value_length = value_end - key_start;
    *value = (char*)malloc(value_length + 1);
    if (*value == NULL) {
        return;
    }

    strncpy(*value, key_start, value_length);
    (*value)[value_length] = '\0';
}

EFI_STATUS sphynxboot_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    imageHandle = &ImageHandle;
    systemTable = SystemTable;
    stdout = systemTable->ConOut;
    stdin = systemTable->ConIn;
    stderr = systemTable->StdErr;

    stdout->SetAttribute(stdout, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
    stdout->SetCursorPosition(stdout, 0, 0);
    stdout->ClearScreen(stdout);

    char *kernel_path;

    // parse config
    
    CHAR16 *path_wide = malloc(strlen(CONFIG_PATH) * sizeof(CHAR16) + 2);
    utf8_char_to_wchar(CONFIG_PATH, path_wide);

    SimpleFile cfg = sfs_open(path_wide);
    if(EFI_ERROR(cfg.status)) {
        printf(" - Error: Failed to open \"/boot.conf\"\n");
        for(;;)
            ;
    }

    char *cfg_data = malloc(cfg.info.PhysicalSize);
    if (cfg_data == NULL)
    {
        printf(" - Error: Failed to allocate memory for kernel data buffer\n");
        for (;;)
            ;
    }

    sfs_read(&cfg, &*cfg_data);
    if (EFI_ERROR(cfg.status))
    {
        free(path_wide);
        printf(" - Error: Failed to read config data: %d\n", cfg.status);
        for (;;)
            ;
    }

    cfg_get_key(cfg_data, "kernel", &kernel_path);
    if (kernel_path == NULL) {
        sfs_close(&cfg);
        free(cfg_data);
        free(path_wide);

        printf(" - Error: Failed to find kernel in boot config (A new line at the EOF the file '\\n' is required)? Expected = \"kernel=...\" \n");
        for (;;)
            ;
    }

    sfs_close(&cfg);
    free(cfg_data);
    free(path_wide);

    // end

    CHAR16 *banner[] = {
        L"  ____        _                        ____              _            \r\n",
        L" / ___| _ __ | |__  _   _ _ __ __  __ | __ )  ___   ___ | |_          \r\n",
        L" \\___ \\| '_ \\| '_ \\| | | | '_ \\\\ \\/ / |  _ \\ / _ \\ / _ \\| __| \r\n",
        L"  ___) | |_) | | | | |_| | | | |>  <  | |_) | (_) | (_) | |_          \r\n",
        L" |____/| .__/|_| |_|\\__, |_| |_/_/\\_\\ |____/ \\___/ \\___/ \\__|   \r\n",
        L"       |_|          |___/                                             \r\n",
    };

    CHAR16 *menu[] = {
        L"                                                          _,--._.-,                     \r\n",
        L"                                                         /\\_r-,\\_ )                   \r\n",
        L"╔══════════ Welcome to Sphynx ══════════╗             .-.) _;='_/ (.;                   \r\n",
        L"║                                       ║              \\ \'     \\/S )                 \r\n",
        L"║ 1. Boot Sphynx Normally (Enter)       ║               L.'-. _.'|-'                    \r\n",
        L"║ 2. Reboot                             ║              <_`-'\'_.'/                      \r\n",
        L"║                                       ║                `'-._( \\                      \r\n",
        L"║ 3. Enter Firmware Settings (Esc)      ║                 ___   \\\\,      ___          \r\n",
        L"║                                       ║                 \\ .'-. \\\\   .-'_. /        \r\n",
        L"║                                       ║                  '._' '.\\\\/.-'_.'           \r\n",
        L"║                                       ║                     '--``\\('--'              \r\n",
        L"║ sphynxboot v1.0.1 (Rose alpha)        ║                           \\\\                \r\n",
        L"╚═══════════════════════════════════════╝                           `\\\\,              \r\n",
        L"                                                                      \\|               \r\n",
    };

     for (int i = 0; i < sizeof(banner) / sizeof(banner[0]); i++) {
        for (int j = 0; j < wchar_strlen(banner[i]); j++) {
            CHAR16 ch = banner[i][j];
            CHAR16 str[2] = {ch, L'\0'};
            stdout->OutputString(stdout, str);
        }
    }

    printf("\n");

    int are_we_done = 0;
    int saved_i = 0;
    int word_len = 0;

    for (int i = 0; i < sizeof(menu) / sizeof(menu[0]); i++) {
        for (int j = 0; j < wchar_strlen(menu[i]); j++) {
            CHAR16 ch = menu[i][j];

            if (are_we_done == word_len) {
                saved_i = 0;
                are_we_done = 0;
            }

            stdout->SetAttribute(stdout, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);


            if (ch == L'B' && i <= 8) {
                stdout->SetAttribute(stdout, EFI_WHITE | EFI_BACKGROUND_BLACK);
            } else if (ch == L'R' && i <= 8) {
                stdout->SetAttribute(stdout, EFI_WHITE | EFI_BACKGROUND_BLACK);
            } else if ((menu[i][j-1] == L'(' && ch == L'E' && menu[i][j+1] == L'n' && menu[i][j+2] == L't' && menu[i][j+3] == L'e' && menu[i][j+4] == L'r' && menu[i][j+5] == L')') || (are_we_done != word_len && saved_i == i && word_len == 5)) {
                if (are_we_done == 0) {
                    saved_i = i;
                    word_len = 5;
                }

                are_we_done++;
                stdout->SetAttribute(stdout, EFI_WHITE | EFI_BACKGROUND_BLACK);
            } else if ((menu[i][j-1] == L'(' && ch == L'E' && menu[i][j+1] == L's' && menu[i][j+2] == L'c' && menu[i][j+3] == L')') || (are_we_done != word_len && saved_i == i && word_len == 3)) {
                if (are_we_done == 0) {
                    saved_i = i;
                    word_len = 3;
                }

                are_we_done++;
                stdout->SetAttribute(stdout, EFI_WHITE | EFI_BACKGROUND_BLACK);
            } else if(j >= 42 && i <= 6) {
                stdout->SetAttribute(stdout, EFI_LIGHTRED | EFI_BACKGROUND_BLACK);
            } else if(j >= 42 & i >= 7) {
                stdout->SetAttribute(stdout, EFI_GREEN | EFI_BACKGROUND_BLACK);
            } else {
                stdout->SetAttribute(stdout, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
            }

            CHAR16 str[2] = {ch, L'\0'};
            stdout->OutputString(stdout, str);
        }
    }

    EFI_INPUT_KEY key;
    EFI_UINTN index;

    stdout->SetAttribute(stdout, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);

    while (1) {
        EFI_EVENT WaitList[1] = {systemTable->ConIn->WaitForKey};  // Only wait for key events now
        EFI_STATUS status = systemTable->BootServices->WaitForEvent(1, WaitList, &index);

        if (EFI_ERROR(status)) {
            printf("WaitForEvent failed: %r\n", status);
            return status;
        }

        if (index == 0) {
            stdin->ReadKeyStroke(stdin, &key);

            switch (key.ScanCode) {
            case EFI_SCANCODE_ESC:
                return EFI_SUCCESS;
            default:
                switch (key.UnicodeChar) {
                case '\r':
                case 'b':
                    load_kernel(kernel_path);
                    systemTable->BootServices->Exit(imageHandle, 0, 0, NULL);
                    break;
                case 'r':
                    systemTable->RuntimeServices->ResetSystem(EfiResetCold, EFI_SUCCESS, 0, NULL);
                    break;
                default:
                    break;
                }
            }
        }
    }

    return EFI_SUCCESS;
}
