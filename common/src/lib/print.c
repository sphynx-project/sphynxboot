#include <lib/print.h>
#include <utils/wchar.h>

void putc(char character)
{
    CHAR16 str[2];
    str[0] = (CHAR16)character;
    str[1] = '\0';

    if (character == '\n')
    {
        stdout->OutputString(stdout, L"\r");
    }

    stdout->OutputString(stdout, str);
}

void putwc(wchar_t character)
{
    CHAR16 str[2];
    str[0] = (CHAR16)character;
    str[1] = '\0';

    if (character == L'\n')
    {
        stdout->OutputString(stdout, L"\r");
    }

    stdout->OutputString(stdout, str);
}

void putstr(const char *str)
{
    while (*str)
    {
        if ((unsigned char)*str < 128)
        {
            putc(*str);
        }
        else
        {
            wchar_t wch;
            int len = mbtowc(&wch, str, MB_CUR_MAX);
            if (len > 0)
            {
                putwc(wch);
                str += len - 1;
            }
            else
            {
                putc(*str);
            }
        }
        str++;
    }
}

void put_int(int num, int width, bool zero_padding, bool left_justify,
             bool force_sign, bool space_before_positive)
{
    char buffer[12];
    int i = 0;
    bool is_negative = false;

    if (num < 0)
    {
        is_negative = true;
        num = -num;
    }

    if (num == 0)
    {
        buffer[i++] = '0';
    }
    else
    {
        while (num)
        {
            buffer[i++] = (num % 10) + '0';
            num /= 10;
        }
    }

    if (is_negative)
    {
        putc('-');
        width--;
    }
    else if (force_sign)
    {
        putc('+');
        width--;
    }
    else if (space_before_positive)
    {
        putc(' ');
        width--;
    }

    while (i < width && !left_justify)
    {
        putc(zero_padding ? '0' : ' ');
        width--;
    }

    while (i > 0)
    {
        putc(buffer[--i]);
    }

    while (i < width && left_justify)
    {
        putc(' ');
        width--;
    }
}

void put_uint(unsigned int num, int width, bool zero_padding, bool left_justify)
{
    char buffer[12];
    int i = 0;

    if (num == 0)
    {
        buffer[i++] = '0';
    }
    else
    {
        while (num)
        {
            buffer[i++] = (num % 10) + '0';
            num /= 10;
        }
    }

    while (i < width && !left_justify)
    {
        putc(zero_padding ? '0' : ' ');
        width--;
    }

    while (i > 0)
    {
        putc(buffer[--i]);
    }

    while (i < width && left_justify)
    {
        putc(' ');
        width--;
    }
}

void put_hex(unsigned long long num, bool uppercase, bool alternative_form,
             int width, bool zero_padding, bool left_justify)
{
    char buffer[17];
    int i = 0;

    if (num == 0)
    {
        buffer[i++] = '0';
    }
    else
    {
        while (num)
        {
            int rem = num % 16;
            if (rem < 10)
            {
                buffer[i++] = '0' + rem;
            }
            else
            {
                buffer[i++] = (uppercase ? 'A' : 'a') + (rem - 10);
            }
            num /= 16;
        }
    }

    if (alternative_form)
    {
        putstr("0x");
        width -= 2;
    }

    while (i < 16)
    {
        buffer[i++] = '0';
    }

    while (i > 0)
    {
        putc(buffer[--i]);
    }

    while (i < width && left_justify)
    {
        putc(' ');
        width--;
    }
}

void printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    while (*fmt)
    {
        if (*fmt == '%')
        {
            fmt++;
            bool left_justify = false;
            bool force_sign = false;
            bool space_before_positive = false;
            bool alternative_form = false;
            bool zero_padding = false;
            int width = 0;
            int precision = -1;
            int length = 0;
            bool uppercase = false;

            while (*fmt == '-' || *fmt == '+' || *fmt == ' ' || *fmt == '#' ||
                   *fmt == '0')
            {
                if (*fmt == '-')
                    left_justify = true;
                if (*fmt == '+')
                    force_sign = true;
                if (*fmt == ' ')
                    space_before_positive = true;
                if (*fmt == '#')
                    alternative_form = true;
                if (*fmt == '0')
                    zero_padding = true;
                fmt++;
            }

            if (*fmt >= '0' && *fmt <= '9')
            {
                width = 0;
                while (*fmt >= '0' && *fmt <= '9')
                {
                    width = width * 10 + (*fmt++ - '0');
                }
            }
            else if (*fmt == '*')
            {
                width = va_arg(args, int);
                fmt++;
            }

            if (*fmt == '.')
            {
                fmt++;
                if (*fmt >= '0' && *fmt <= '9')
                {
                    precision = 0;
                    while (*fmt >= '0' && *fmt <= '9')
                    {
                        precision = precision * 10 + (*fmt++ - '0');
                    }
                }
                else if (*fmt == '*')
                {
                    precision = va_arg(args, int);
                    fmt++;
                }
                else
                {
                    precision = 0;
                }
            }

            if (*fmt == 'h')
            {
                fmt++;
                if (*fmt == 'h')
                {
                    length = 1;
                    fmt++;
                }
                else
                {
                    length = 2;
                }
            }
            else if (*fmt == 'l')
            {
                fmt++;
                if (*fmt == 'l')
                {
                    length = 4;
                    fmt++;
                }
                else
                {
                    length = 3;
                }
            }
            else if (*fmt == 'j')
            {
                length = 5;
                fmt++;
            }
            else if (*fmt == 'z')
            {
                length = 6;
                fmt++;
            }
            else if (*fmt == 't')
            {
                length = 7;
                fmt++;
            }
            else if (*fmt == 'L')
            {
                length = 8;
                fmt++;
            }

            switch (*fmt)
            {
            case 's':
            {
                const char *str_arg = va_arg(args, const char *);
                putstr(str_arg);
                break;
            }
            case 'c':
            {
                int ch = va_arg(args, int);
                putc((char)ch);
                break;
            }
            case 'd':
            case 'i':
            {
                int int_arg;
                if (length == 1)
                {
                    int_arg = (signed char)va_arg(args, int);
                }
                else if (length == 2)
                {
                    int_arg = (short int)va_arg(args, int);
                }
                else if (length == 3)
                {
                    int_arg = va_arg(args, long int);
                }
                else if (length == 4)
                {
                    int_arg = va_arg(args, long long int);
                }
                else if (length == 5)
                {
                    int_arg = va_arg(args, intmax_t);
                }
                else if (length == 6)
                {
                    int_arg = va_arg(args, size_t);
                }
                else if (length == 7)
                {
                    int_arg = va_arg(args, ptrdiff_t);
                }
                else
                {
                    int_arg = va_arg(args, int);
                }
                put_int(int_arg, width, zero_padding, left_justify, force_sign,
                        space_before_positive);
                break;
            }
            case 'u':
            {
                unsigned int uint_arg;
                if (length == 1)
                {
                    uint_arg = (unsigned char)va_arg(args, unsigned int);
                }
                else if (length == 2)
                {
                    uint_arg = (unsigned short int)va_arg(args, unsigned int);
                }
                else if (length == 3)
                {
                    uint_arg = va_arg(args, unsigned long int);
                }
                else if (length == 4)
                {
                    uint_arg = va_arg(args, unsigned long long int);
                }
                else if (length == 5)
                {
                    uint_arg = va_arg(args, uintmax_t);
                }
                else if (length == 6)
                {
                    uint_arg = va_arg(args, size_t);
                }
                else if (length == 7)
                {
                    uint_arg = va_arg(args, ptrdiff_t);
                }
                else
                {
                    uint_arg = va_arg(args, unsigned int);
                }
                put_uint(uint_arg, width, zero_padding, left_justify);
                break;
            }
            case 'x':
            case 'X':
            {
                if (*fmt == 'X')
                {
                    uppercase = true;
                }
                unsigned long long hex_arg;
                if (length == 1)
                {
                    hex_arg = (unsigned char)va_arg(args, unsigned int);
                }
                else if (length == 2)
                {
                    hex_arg = (unsigned short int)va_arg(args, unsigned int);
                }
                else if (length == 3)
                {
                    hex_arg = va_arg(args, unsigned long int);
                }
                else if (length == 4)
                {
                    hex_arg = va_arg(args, unsigned long long int);
                }
                else if (length == 5)
                {
                    hex_arg = va_arg(args, uintmax_t);
                }
                else if (length == 6)
                {
                    hex_arg = va_arg(args, size_t);
                }
                else if (length == 7)
                {
                    hex_arg = va_arg(args, ptrdiff_t);
                }
                else
                {
                    hex_arg = va_arg(args, unsigned int);
                }
                put_hex(hex_arg, uppercase, alternative_form, width,
                        zero_padding, left_justify);
                break;
            }
            case 'f':
            case 'F':
            {
                break;
            }
            case 'p':
            {
                void *ptr_arg = va_arg(args, void *);
                put_hex((unsigned long long)ptr_arg, false, true, width,
                        zero_padding, left_justify);
                break;
            }
            case '%':
            {
                putc('%');
                break;
            }
            default:
                putc('%');
                putc(*fmt);
                break;
            }
        }
        else
        {
            putc(*fmt);
        }
        fmt++;
    }

    va_end(args);
}
