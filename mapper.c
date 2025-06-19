#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stddef.h>

// Function prototypes for large integer arithmetic (from large_int_arithmetic.h)
int calculate_large_mod(const char* large_num_str, int divisor);
int calculate_large_div_to_string(
    const char* large_num_str,
    int divisor,
    char* quotient_str_out,
    size_t quotient_buffer_size
);

// Helper function (from large_int_arithmetic.c)
// NOTE: This was static in large_int_arithmetic.c.
// It can remain static if only used by the arithmetic functions within this consolidated file.
// If map_to_cartesian or main also needed it directly (they don't seem to), it would need to be non-static.
// For now, keeping it static as its usage is localized to the arithmetic functions.
static int is_valid_number_string(const char* s) {
    if (!s || *s == '\0') return 0; // NULL or empty
    // Check if it's "0"
    if (s[0] == '0' && s[1] == '\0') return 1;
    // Check for leading zeros if not "0"
    // This check was originally in the helper but calculate_large_mod/div do their own full digit checks.
    // For strict equivalence to the original, it's included.
    // However, the actual arithmetic functions below iterate and check all digits,
    // and handle "0" or numbers with leading zeros before this helper would be strictly necessary
    // for their internal logic if they called it.
    // The primary validation is done by direct iteration and isdigit checks in the arithmetic functions.
    // This helper isn't directly called by the provided arithmetic functions, so it's effectively unused as is.
    // To simplify and match the provided arithmetic code (which doesn't call this helper),
    // this helper could be omitted if not used. For now, keeping it as it was in the source file.
    if (s[0] == '0' && strlen(s) > 1) return 0;

    for (size_t i = 0; s[i] != '\0'; ++i) {
        if (!isdigit(s[i])) return 0; // Not a digit
    }
    return 1;
}

// Function implementation for calculate_large_mod (from large_int_arithmetic.c)
int calculate_large_mod(const char* large_num_str, int divisor) {
    if (!large_num_str) return -1;
    size_t len = strlen(large_num_str);
    if (len == 0) return -1;

    for (size_t i = 0; i < len; ++i) {
        if (!isdigit(large_num_str[i])) return -1;
    }

    if (divisor <= 0) return -1;

    long long remainder = 0;

    for (size_t i = 0; i < len; ++i) {
        int digit_val = large_num_str[i] - '0';
        remainder = (remainder * 10 + digit_val) % divisor;
    }
    return (int)remainder;
}

// Function implementation for calculate_large_div_to_string (from large_int_arithmetic.c)
int calculate_large_div_to_string(
    const char* large_num_str,
    int divisor,
    char* quotient_str_out,
    size_t quotient_buffer_size
) {
    if (!quotient_str_out) return -1;
    if (quotient_buffer_size == 0) {
        if (quotient_str_out) *quotient_str_out = '\0';
        return -1;
    }
    *quotient_str_out = '\0';

    if (!large_num_str) return -1;
    size_t len = strlen(large_num_str);
    if (len == 0) return -1;

    for (size_t i = 0; i < len; ++i) {
        if (!isdigit(large_num_str[i])) return -1;
    }

    if (divisor <= 0) return -1;

    if (quotient_buffer_size < 2) return -1; // Min for "0" + null terminator


    char temp_quotient_buffer[len + 1];
    size_t temp_idx = 0;
    long long current_dividend_segment = 0;
    int has_started_quotient = 0;

    for (size_t i = 0; i < len; ++i) {
        int digit_val = large_num_str[i] - '0';
        current_dividend_segment = current_dividend_segment * 10 + digit_val;

        int quotient_digit = current_dividend_segment / divisor;

        if (quotient_digit > 0 || has_started_quotient) {
            if (temp_idx < len) {
                 temp_quotient_buffer[temp_idx++] = quotient_digit + '0';
            } else {
                // This case implies quotient is longer than input, which shouldn't happen for divisor > 1.
                // If divisor is 1, quotient length can be same as input.
                // If len is 0, this block is not entered.
                // If temp_idx reaches len, it means quotient has 'len' digits.
            }
            has_started_quotient = 1;
        }
        current_dividend_segment = current_dividend_segment % divisor;
    }

    if (temp_idx == 0) {
        // This handles cases like "5" / 10 = "0" or "0" / N = "0"
        // temp_quotient_buffer must have space for at least "0" and '\0'
        if (len >= 0 && temp_idx < (len +1) ) { // Check temp_idx is within bounds for the buffer
             temp_quotient_buffer[temp_idx++] = '0';
        } else {
            // This should not be reached if quotient_buffer_size >= 2 and len+1 buffer is used
            return -1; // Should not happen
        }
    }

    // Null terminate the temporary buffer
    if (temp_idx < (len + 1)) { // Ensure temp_idx is within bounds of temp_quotient_buffer
        temp_quotient_buffer[temp_idx] = '\0';
    } else {
        // This means temp_idx has gone out of bounds for temp_quotient_buffer.
        // This would be an error in logic if temp_quotient_buffer is size len+1.
        return -1;
    }

    if (strlen(temp_quotient_buffer) + 1 > quotient_buffer_size) {
        return -1;
    }
    strcpy(quotient_str_out, temp_quotient_buffer);

    return 0;
}

// Pre-calculated sine and cosine tables (from mapper.c)
const float sin_table[40] = {
    0.00000000,  0.15643447,  0.30901699,  0.45399050,  0.58778525,
    0.70710677,  0.80901699,  0.89100651,  0.95105652,  0.98768834,
    1.00000000,  0.98768834,  0.95105652,  0.89100651,  0.80901699,
    0.70710677,  0.58778525,  0.45399050,  0.30901699,  0.15643447,
    0.00000000, -0.15643445, -0.30901702, -0.45399053, -0.58778528,
   -0.70710683, -0.80901702, -0.89100657, -0.95105658, -0.98768837,
   -1.00000000, -0.98768837, -0.95105658, -0.89100657, -0.80901702,
   -0.70710683, -0.58778528, -0.45399053, -0.30901702, -0.15643445
};

const float cos_table[40] = {
    1.00000000,  0.98768834,  0.95105652,  0.89100651,  0.80901699,
    0.70710677,  0.58778525,  0.45399050,  0.30901699,  0.15643447,
    0.00000000, -0.15643447, -0.30901699, -0.45399050, -0.58778525,
   -0.70710677, -0.80901699, -0.89100651, -0.95105652, -0.98768834,
   -1.00000000, -0.98768834, -0.95105652, -0.89100651, -0.80901699,
   -0.70710677, -0.58778525, -0.45399050, -0.30901699, -0.15643447,
    0.00000000,  0.15643447,  0.30901699,  0.45399050,  0.58778525,
    0.70710677,  0.80901699,  0.89100651,  0.95105652,  0.98768834
};

// Function to map a large number string to Cartesian coordinates (from mapper.c)
int map_to_cartesian(const char* number_str, float *x_out, float *y_out) {
    if (number_str == NULL || x_out == NULL || y_out == NULL) {
        fprintf(stderr, "Error: NULL argument passed to map_to_cartesian.\n");
        return -1;
    }
    size_t num_len = strlen(number_str);
    if (num_len == 0) {
        fprintf(stderr, "Error: Empty number string passed to map_to_cartesian.\n");
        return -1;
    }

    int theta_index = calculate_large_mod(number_str, 40);
    if (theta_index == -1) {
        fprintf(stderr, "Error: calculate_large_mod failed for input %s\n", number_str);
        return -1;
    }

    size_t radius_buf_size = num_len + 1;
    if (radius_buf_size < 2) {
        radius_buf_size = 2;
    }
    char actual_radius_buffer[radius_buf_size];

    if (calculate_large_div_to_string(number_str, 40, actual_radius_buffer, radius_buf_size) == -1) {
        fprintf(stderr, "Error: calculate_large_div_to_string failed for input %s (gross radius calculation)\n", number_str);
        return -1;
    }

    int indice_circunferencia = calculate_large_mod(actual_radius_buffer, 77);
    if (indice_circunferencia == -1) {
        fprintf(stderr, "Error: Failed to calculate indice_circunferencia from radius_str '%s'\n", actual_radius_buffer);
        return -1;
    }

    float R_efetivo = (float)indice_circunferencia + 1.0f;

    if (theta_index < 0 || theta_index >= 40) {
        fprintf(stderr, "Error: Invalid theta_index %d calculated for input %s. This should not happen.\n", theta_index, number_str);
        return -1;
    }

    *x_out = R_efetivo * cos_table[theta_index];
    *y_out = R_efetivo * sin_table[theta_index];

    return 0;
}

// Main function (from mapper.c)
int main() {
    const char* large_num_input = "9210836494447108270027136741376870869791784014198948301625976867708124077590";
    float x_coord, y_coord;

    printf("Attempting to map large number: %s\n", large_num_input);

    int status = map_to_cartesian(large_num_input, &x_coord, &y_coord);

    if (status == 0) {
        printf("Main: map_to_cartesian successful.\n");
        printf("Main: Input Number String: %s\n", large_num_input);

        int theta_main = calculate_large_mod(large_num_input, 40);
        printf("Main: Theta Index (from calculate_large_mod): %d\n", theta_main);
        if (theta_main != -1) {
             printf("Main: Angle (degrees): %d\n", theta_main * 9);
             if (theta_main >= 0 && theta_main < 40) {
                printf("Main: Cos_val from table at index %d: %.8f\n", theta_main, cos_table[theta_main]);
                printf("Main: Sin_val from table at index %d: %.8f\n", theta_main, sin_table[theta_main]);
            } else {
                printf("Main: Theta index %d is out of bounds for table lookup.\n", theta_main);
            }
        } else {
            printf("Main: Error calculating theta_index for verification.\n");
        }

        size_t input_len = strlen(large_num_input);
        size_t gross_radius_buf_size = input_len + 1;
        if (gross_radius_buf_size < 2) gross_radius_buf_size = 2;

        char gross_radius_str_main[gross_radius_buf_size];

        if (calculate_large_div_to_string(large_num_input, 40, gross_radius_str_main, gross_radius_buf_size) == 0) {
            printf("Main: Gross Radius String (calculated in main): %s\n", gross_radius_str_main);

            float gross_radius_as_float = strtof(gross_radius_str_main, NULL);
            printf("Main: Gross Radius String '%s' as float (strtof): %f (scientific: %e)\n",
                   gross_radius_str_main, gross_radius_as_float, gross_radius_as_float);

            int indice_circunferencia_main = calculate_large_mod(gross_radius_str_main, 77);
            if (indice_circunferencia_main != -1) {
                printf("Main: Indice da Circunferencia (calculated in main from gross radius string): %d\n", indice_circunferencia_main);
                float R_efetivo_main = (float)indice_circunferencia_main + 1.0f;
                printf("Main: R_efetivo (calculated in main): %.8f\n", R_efetivo_main);
            } else {
                printf("Main: Error calculating Indice da Circunferencia for verification.\n");
            }
        } else {
            printf("Main: Error calculating Gross Radius String for verification.\n");
        }

        printf("Main: X Coordinate (from map_to_cartesian with R_efetivo): %.8f\n", x_coord);
        printf("Main: Y Coordinate (from map_to_cartesian with R_efetivo): %.8f\n", y_coord);
    } else {
        printf("Main: map_to_cartesian failed with status %d. Check stderr for details from the function.\n", status);
    }

    return status;
}
