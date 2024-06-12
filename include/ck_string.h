/*
    String interning library.
    Pure C library that allows you to intern strings. C99 compatible.
    Use this library to intern strings, which means that you can compare strings by comparing their pointers.
    Strings are never deallocated, so you should only intern strings that are used frequently.
*/
#ifndef CK_STRING_H
#define CK_STRING_H

const char *ck_string_intern(const char *str);
const char *ck_string_format(const char *fmt, ...);

#endif // CK_STRING_H

#ifdef CK_STRING_IMPLEMENTATION

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INTERNED_STRINGS 4096
#define MAX_BUFFER_SIZE 1024
static const char* ck_interned_strings[MAX_INTERNED_STRINGS];
static int ck_num_interned_strings = 0;
static char ck_buffer[MAX_BUFFER_SIZE];

const char* ck_string_intern(const char* str) {
    // Check if the string is already interned
    for (int i = 0; i < ck_num_interned_strings; i++) {
        if (strcmp(ck_interned_strings[i], str) == 0) {
            return ck_interned_strings[i];
        }
    }

    // Intern the string
    if (ck_num_interned_strings < MAX_INTERNED_STRINGS) {
        ck_interned_strings[ck_num_interned_strings] = str;
        ck_num_interned_strings++;
    } else {
        // Array is full, assert an error
        assert(false && "Maximum number of interned strings reached");
    }

    return str;
}

const char *ck_string_format(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    // Get the length of the formatted string
    int len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    // Create a fixed-length buffer to hold the formatted string
    if (len >= MAX_BUFFER_SIZE) {
        // Handle the case where the formatted string is too long
        assert(false && "Formatted string is too long");
        return NULL;
    }

    va_start(args, fmt);
    vsnprintf(ck_buffer, sizeof(ck_buffer), fmt, args);
    va_end(args);

    const char *interned_str = ck_string_intern(buffer);

    return interned_str;
}

#endif // CK_STRING_IMPLEMENTATION