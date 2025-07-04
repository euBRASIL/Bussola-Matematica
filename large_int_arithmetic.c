#include "large_int_arithmetic.h"
#include <string.h> // For strlen, strcpy
#include <ctype.h>  // For isdigit

// Helper to check if string is valid number string (contains only digits and is not empty)
static int is_valid_number_string(const char* s) {
    if (!s || *s == '\0') return 0; // NULL or empty
    // Check if it's "0"
    if (s[0] == '0' && s[1] == '\0') return 1;
    // Check for leading zeros if not "0"
    if (s[0] == '0' && strlen(s) > 1) return 0; 

    for (size_t i = 0; s[i] != '\0'; ++i) {
        if (!isdigit(s[i])) return 0; // Not a digit
    }
    return 1;
}

int calculate_large_mod(const char* large_num_str, int divisor) {
    if (!large_num_str) return -1; // Explicit NULL check
    size_t len = strlen(large_num_str);
    if (len == 0) return -1; // Empty string check

    for (size_t i = 0; i < len; ++i) {
        if (!isdigit(large_num_str[i])) return -1; // Contains non-digits
    }
    // After individual checks, is_valid_number_string might be redundant here if checks are comprehensive.
    // For simplicity, let's rely on the loop above for digit check.

    if (divisor <= 0) return -1;

    long long remainder = 0; // Use long long for remainder to avoid overflow with remainder * 10
    
    for (size_t i = 0; i < len; ++i) {
        int digit_val = large_num_str[i] - '0';
        remainder = (remainder * 10 + digit_val) % divisor;
    }
    return (int)remainder;
}

int calculate_large_div_to_string(
    const char* large_num_str,
    int divisor,
    char* quotient_str_out,
    size_t quotient_buffer_size
) {
    if (!quotient_str_out) return -1;
    if (quotient_buffer_size == 0) { // Must have space for at least null terminator
        if (quotient_str_out) *quotient_str_out = '\0'; // Clear if possible
        return -1;
    }
    *quotient_str_out = '\0'; // Initialize output to empty string for safety

    if (!large_num_str) return -1;
    size_t len = strlen(large_num_str);
    if (len == 0) return -1;

    for (size_t i = 0; i < len; ++i) {
        if (!isdigit(large_num_str[i])) return -1;
    }
    
    if (divisor <= 0) return -1;

    // Max possible quotient length is len + 1 (for null terminator), or 2 for "0" + null.
    // If quotient_buffer_size is less than 2 (for "0" and ' '), it's too small.
    if (quotient_buffer_size < 2) return -1;


    char temp_quotient_buffer[len + 1]; // Max quotient string length (excluding null) is len
    size_t temp_idx = 0;
    long long current_dividend_segment = 0;
    int has_started_quotient = 0; // To handle leading zeros in quotient

    for (size_t i = 0; i < len; ++i) {
        int digit_val = large_num_str[i] - '0';
        current_dividend_segment = current_dividend_segment * 10 + digit_val;
        
        int quotient_digit = current_dividend_segment / divisor;
        
        if (quotient_digit > 0 || has_started_quotient) {
            if (temp_idx < len) { // Protect temp_quotient_buffer bounds
                 temp_quotient_buffer[temp_idx++] = quotient_digit + '0';
            } else {
                // Should not happen if logic is correct and len is appropriate for buffer
                // This indicates an issue if quotient becomes longer than input (not possible for div > 1)
            }
            has_started_quotient = 1;
        }
        current_dividend_segment = current_dividend_segment % divisor;
    }

    if (temp_idx == 0) { // Result is 0 (e.g. "5" / 10, or "0" / 10)
        if (len > 0 && temp_idx < len) { // Ensure space for "0"
            temp_quotient_buffer[temp_idx++] = '0';
        } else if (len == 0 && temp_idx == 0) { // Input ""
             // Already handled by initial checks
        }
    }
    
    if (temp_idx < len +1 ) { // ensure null termination is within bounds
        temp_quotient_buffer[temp_idx] = '\0';
    } else {
        // This case implies temp_idx went out of bounds for temp_quotient_buffer
        // which means quotient string is longer than original string (error for div > 1)
        // For safety, if this unexpected case happens, indicate error
        return -1; 
    }


    // Final check on buffer size for actual result string
    if (strlen(temp_quotient_buffer) + 1 > quotient_buffer_size) {
        return -1; // Not enough space in user-provided buffer
    }
    strcpy(quotient_str_out, temp_quotient_buffer);

    return 0;
}

#ifdef TEST_LARGE_ARITHMETIC
#include <stdio.h> 
#include <string.h> // For memset, strcmp in tests
#include <stdlib.h> // Potentially for future use, not strictly needed by this test code

// Test function for calculate_large_mod
static int test_mod_function() {
    int tests_passed = 0;
    int total_tests = 0;
    char test_desc[100];

    struct mod_test_case {
        const char* num_str;
        int divisor;
        int expected;
        const char* desc;
    } mod_cases[] = {
        {"123", 40, 3, "Simple mod"},
        {"40", 40, 0, "Divisible"},
        {"39", 40, 39, "Smaller than divisor"},
        {"0", 40, 0, "Zero input"},
        {"7", 3, 1, "7 mod 3"},
        {"100", 10, 0, "100 mod 10"},
        {"100", 3, 1, "100 mod 3"},
        {"921083649", 40, 9, "Larger number mod 40 (ends in 9)"},
        {NULL, 40, -1, "NULL input string"},
        {"", 40, -1, "Empty input string"},
        {"12a3", 40, -1, "Non-digit in string"},
        {"123", 0, -1, "Divisor zero"},
        {"123", -5, -1, "Divisor negative"}
    };

    total_tests = sizeof(mod_cases) / sizeof(mod_cases[0]);
    printf("--- Testing calculate_large_mod ---\n");
    for (int i = 0; i < total_tests; ++i) {
        int result = calculate_large_mod(mod_cases[i].num_str, mod_cases[i].divisor);
        // For NULL num_str, use "NULL" literal in printf
        char num_str_print[32];
        if (mod_cases[i].num_str == NULL) {
            strcpy(num_str_print, "NULL");
        } else {
            strncpy(num_str_print, mod_cases[i].num_str, sizeof(num_str_print) -1);
            num_str_print[sizeof(num_str_print)-1] = '\0';
        }
        snprintf(test_desc, sizeof(test_desc), "Test: %s (Input: \"%s\" %% %d)", 
                 mod_cases[i].desc, num_str_print, mod_cases[i].divisor);

        if (result == mod_cases[i].expected) {
            printf("[PASS] %s -> Expected: %d, Got: %d\n", test_desc, mod_cases[i].expected, result);
            tests_passed++;
        } else {
            printf("[FAIL] %s -> Expected: %d, Got: %d\n", test_desc, mod_cases[i].expected, result);
        }
    }
    printf("--- calculate_large_mod tests finished: %d/%d passed ---\n\n", tests_passed, total_tests);
    return (tests_passed == total_tests);
}

// Test function for calculate_large_div_to_string
static int test_div_function() {
    int tests_passed = 0;
    int total_tests = 0;
    char result_buf[256]; 
    char test_desc[200];

    struct div_test_case {
        const char* num_str;
        int divisor;
        const char* expected_q_str;
        int expected_ret;
        size_t buf_size; 
        const char* desc;
    } div_cases[] = {
        {"123", 40, "3", 0, 10, "Simple div '123'/40"},
        {"39", 40, "0", 0, 10, "'39'/40 (result 0)"},
        {"40", 40, "1", 0, 10, "'40'/40 (result 1)"},
        {"80", 40, "2", 0, 10, "'80'/40"},
        {"0", 40, "0", 0, 10, "'0'/40"},
        {"799", 40, "19", 0, 10, "'799'/40"},
        {"12345", 10, "1234", 0, 10, "'12345'/10"},
        {"12345", 12345, "1", 0, 10, "'12345'/12345"},
        {"12344", 12345, "0", 0, 10, "'12344'/12345"},
        {"1000", 3, "333", 0, 10, "'1000'/3"},
        {"921083640", 40, "23027091", 0, 30, "Large num div by 40 (ends in 0)"},
        {"921083649", 40, "23027091", 0, 30, "Large num div by 40 (ends in 9)"},
        {NULL, 40, "", -1, 10, "NULL num_str"},
        {"123", 0, "", -1, 10, "Divisor zero"},
        {"123", -5, "", -1, 10, "Divisor negative"},
        {"", 40, "", -1, 10, "Empty num_str"},
        {"12a3", 40, "", -1, 10, "Non-digit in num_str"},
        {"123", 10, "", -1, 0, "Buffer size 0"},
        {"123", 10, "", -1, 1, "Buffer size 1 (too small for '1'+\0)"}, // "12" -> "1" + ' ' needs 2
        {"12345", 10, "1234", 0, 5, "Buffer exact size for '1234'+\0"},
        {"12345", 10, "", -1, 4, "Buffer too small for '1234'+\0"},
        {"0", 10, "0", 0, 2, "Buffer exact for '0'+\0"},
        {"0", 10, "", -1, 1, "Buffer too small for '0'+\0"},
    };

    total_tests = sizeof(div_cases) / sizeof(div_cases[0]);
    printf("--- Testing calculate_large_div_to_string ---\n");
    for (int i = 0; i < total_tests; ++i) {
        memset(result_buf, 0xAA, sizeof(result_buf)); 
        // To prevent reading uninitialized memory if function fails to write/null-terminate:
        if (div_cases[i].buf_size > 0 && div_cases[i].buf_size <= sizeof(result_buf)) {
             result_buf[0] = '\0'; 
             if (div_cases[i].buf_size > 1) result_buf[div_cases[i].buf_size-1] = 'X'; // Canary
        } else if (div_cases[i].buf_size == 0 && sizeof(result_buf) > 0) {
            result_buf[0] = '\0';
        }


        int ret_val = calculate_large_div_to_string(div_cases[i].num_str, div_cases[i].divisor, result_buf, div_cases[i].buf_size);
        
        char num_str_print[32];
        if (div_cases[i].num_str == NULL) {
            strcpy(num_str_print, "NULL");
        } else {
            strncpy(num_str_print, div_cases[i].num_str, sizeof(num_str_print) -1);
            num_str_print[sizeof(num_str_print)-1] = '\0';
        }
        snprintf(test_desc, sizeof(test_desc), "Test: %s (Input: \"%s\" / %d, BufSize: %zu)", 
                 div_cases[i].desc, num_str_print, 
                 div_cases[i].divisor, div_cases[i].buf_size);

        int pass = 0;
        if (ret_val == div_cases[i].expected_ret) {
            if (ret_val == 0) { 
                if (strcmp(result_buf, div_cases[i].expected_q_str) == 0) {
                    pass = 1;
                }
            } else { 
                pass = 1;
            }
        }

        if (pass) {
            printf("[PASS] %s -> Expected Ret: %d, Got Ret: %d. Expected Q: \"%s\", Got Q: \"%s\"\n", 
                   test_desc, div_cases[i].expected_ret, ret_val, 
                   div_cases[i].expected_q_str, (ret_val==0 ? result_buf : "N/A"));
            tests_passed++;
        } else {
            // Ensure result_buf is null-terminated for printing if it came from a bad state
            result_buf[sizeof(result_buf)-1] = '\0';
            printf("[FAIL] %s -> Expected Ret: %d, Got Ret: %d. Expected Q: \"%s\", Got Q: \"%s\"\n", 
                   test_desc, div_cases[i].expected_ret, ret_val, 
                   div_cases[i].expected_q_str, result_buf);
        }
    }
    printf("--- calculate_large_div_to_string tests finished: %d/%d passed ---\n\n", tests_passed, total_tests);
    return (tests_passed == total_tests);
}

int main() {
    printf("=== Running Arithmetic Tests ===\n");
    int mod_ok = test_mod_function();
    int div_ok = test_div_function();

    if (mod_ok && div_ok) {
        printf("=== All Arithmetic Tests Passed ===\n");
        return 0;
    } else {
        printf("=== SOME ARITHMETIC TESTS FAILED ===\n");
        return 1;
    }
}
#endif // TEST_LARGE_ARITHMETIC
