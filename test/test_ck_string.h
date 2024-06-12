// Test for ck_string_intern function
#include "ck_string.h"
#include <assert.h>

void test_ck_string_intern() {
    const char* str1 = "Hello";
    const char* str2 = "World";
    const char* str3 = "Hello";

    const char* interned_str1 = ck_string_intern(str1);
    const char* interned_str2 = ck_string_intern(str2);
    const char* interned_str3 = ck_string_intern(str3);

    assert(interned_str1 == str1);
    assert(interned_str2 == str2);
    assert(interned_str3 == str1); // str3 should be interned as str1

    // Add more test cases here
}

// Test for ck_string_format function
void test_ck_string_format() {
    const char* formatted_str1 = ck_string_format("Hello, %s!", "GitHub Copilot");
    const char* formatted_str2 = ck_string_format("The answer is %d.", 42);

    assert(strcmp(formatted_str1, "Hello, GitHub Copilot!") == 0);
    assert(strcmp(formatted_str2, "The answer is 42.") == 0);

    // Add more test cases here
}

// Run all the tests
void run_tests() {
    test_ck_string_intern();
    test_ck_string_format();

    // Add more test functions here
}