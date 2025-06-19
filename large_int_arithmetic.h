#ifndef LARGE_INT_ARITHMETIC_H
#define LARGE_INT_ARITHMETIC_H

#include <stddef.h> // For size_t

// Calculates (large_num_str % divisor)
// Returns the integer remainder.
// Returns -1 if large_num_str is NULL, empty, contains non-digits, or if divisor is <= 0.
int calculate_large_mod(const char* large_num_str, int divisor);

// Calculates (large_num_str / divisor) and stores the result in quotient_str_out.
// quotient_str_out must be a buffer large enough to hold the result + null terminator
// (e.g., strlen(large_num_str) + 1 is generally safe for the quotient string length,
// but quotient_buffer_size should be checked).
// Returns 0 on success.
// Returns -1 if any input pointer is NULL, if large_num_str is empty or contains non-digits,
// if divisor is <= 0, or if quotient_buffer_size is too small for the result including null terminator.
// The output quotient_str_out will not have unnecessary leading zeros (e.g., "003" becomes "3").
// If the result of division is 0 (e.g. "5" / 10 = "0"), quotient_str_out will be "0".
int calculate_large_div_to_string(
    const char* large_num_str,
    int divisor,
    char* quotient_str_out,
    size_t quotient_buffer_size
);

#endif // LARGE_INT_ARITHMETIC_H
