#ifndef COMMON_H
#define COMMON_H

#define GROUP_NAME_LENGTH 12

// Create a static array of const char* so each element is a string literal:
static const char *POSSIBLE_CHARS[] = {
    " ",
    "A", "B", "C", "D", "E", "F", "G",
    "H", "I", "J", "K", "L", "M", "N",
    "O", "P", "Q", "R", "S", "T", "U",
    "V", "W", "X", "Y", "Z"};

// If you want a macro for the length, calculate it from the array:
#define POSSIBLE_CHARS_LENGTH (sizeof(POSSIBLE_CHARS) / sizeof(POSSIBLE_CHARS[0]))

#endif