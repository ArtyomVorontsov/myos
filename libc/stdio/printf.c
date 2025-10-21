#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "types.h"

static bool print(const char *data, size_t length)
{
    const unsigned char *bytes = (const unsigned char *)data;
    for (size_t i = 0; i < length; i++)
        if (putchar(bytes[i]) == EOF)
            return false;
    return true;
}

// Converts int16_t to string, returns length of the string
size_t int16_to_str(int16_t value, char *buffer)
{
    char temp[6]; // Max int16_t is -32768 (5 digits + sign)
    size_t i = 0;
    int is_negative = 0;

    if (value == 0)
    {
        buffer[0] = '0';
        buffer[1] = '\0';
        return 1;
    }

    if (value < 0)
    {
        is_negative = 1;
        // Handle edge case: -32768
        if (value == -32768)
        {
            buffer[0] = '-';
            buffer[1] = '3';
            buffer[2] = '2';
            buffer[3] = '7';
            buffer[4] = '6';
            buffer[5] = '8';
            buffer[6] = '\0';
            return 6;
        }
        value = -value;
    }

    // Extract digits
    while (value != 0)
    {
        temp[i++] = '0' + (value % 10);
        value /= 10;
    }

    size_t len = 0;
    if (is_negative)
    {
        buffer[len++] = '-';
    }

    // Reverse digits into final buffer
    while (i > 0)
    {
        buffer[len++] = temp[--i];
    }
    buffer[len] = '\0';
    return len;
}

int printf(const char *__restrict__ format, ...)
{
    va_list parameters;
    va_start(parameters, format);

    int written = 0;

    while (*format != '\0')
    {
        size_t maxrem = INT_MAX - written;

        if (format[0] != '%' || format[1] == '%')
        {
            if (format[0] == '%')
                format++;
            size_t amount = 1;
            while (format[amount] && format[amount] != '%')
                amount++;
            if (maxrem < amount)
            {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!print(format, amount))
                return -1;
            format += amount;
            written += amount;
            continue;
        }

        const char *format_begun_at = format++;

        if (*format == 'c')
        {
            format++;
            char c = (char)va_arg(parameters, int /* char promotes to int */);
            if (!maxrem)
            {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!print(&c, sizeof(c)))
                return -1;
            written++;
        }
        else if (*format == 's')
        {
            format++;
            const char *str = va_arg(parameters, const char *);
            size_t len = strlen(str);
            if (maxrem < len)
            {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!print(str, len))
                return -1;
            written += len;
        }
        else if (*format == 'd')
        {
            format++;
            int num = (int)va_arg(parameters, int /* char promotes to int */);
            if (!maxrem)
            {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }

            char buf[8];
            int16_to_str(num, buf);
            buf[7] = '\0';
            size_t len = strlen(buf);

            if (!print(buf, len))
                return -1;
            written++;
        }
        else
        {
            format = format_begun_at;
            size_t len = strlen(format);
            if (maxrem < len)
            {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!print(format, len))
                return -1;
            written += len;
            format += len;
        }
    }

    va_end(parameters);
    return written;
}
