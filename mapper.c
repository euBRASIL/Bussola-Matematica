#include <stdio.h>
#include <string.h> // For strlen
#include <stdlib.h> // For strtof
#include <math.h>   // For isinf, isnan
#include "large_int_arithmetic.h" // New include

// Pre-calculated sine and cosine tables
// Values from the issue description for angles 0, 9, 18, ..., 351 degrees
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

// Function to map a large number string to Cartesian coordinates
// Parameters:
//   const char* number_str: The input number as a string
//   float *x_out: Pointer to store the calculated x-coordinate
//   float *y_out: Pointer to store the calculated y-coordinate
// Returns 0 on success, -1 on error.
int map_to_cartesian(const char* number_str, float *x_out, float *y_out) {
    if (number_str == NULL || x_out == NULL || y_out == NULL) {
        fprintf(stderr, "Error: NULL argument passed to map_to_cartesian.\n");
        return -1; // Invalid arguments
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

    // Buffer for radius string: length of number_str + 1 for null terminator is max possible.
    // Minimum size is 2 (for "0" + null terminator).
    size_t radius_buf_size = num_len + 1;
    if (radius_buf_size < 2) { // Should only happen if num_len is 0, already checked.
        radius_buf_size = 2;
    }
    char radius_str[radius_buf_size];

    if (calculate_large_div_to_string(number_str, 40, radius_str, radius_buf_size) == -1) {
        fprintf(stderr, "Error: calculate_large_div_to_string failed for input %s\n", number_str);
        return -1;
    }

    float float_radius = strtof(radius_str, NULL);
    // Check for float overflow/NaN
    if (isinf(float_radius) || isnan(float_radius)) {
        fprintf(stderr, "Error in map_to_cartesian: Radius string '%s' is too large and resulted in float overflow/NaN.\n", radius_str);
        return -2; // New error code for overflow
    }

    // calculate_large_mod should ensure theta_index is within 0-39 for divisor 40.
    // Defensive check:
    if (theta_index < 0 || theta_index >= 40) {
        fprintf(stderr, "Error: Invalid theta_index %d calculated for input %s. This should not happen.\n", theta_index, number_str);
        return -1; // Keep -1 for other internal errors
    }

    *x_out = float_radius * cos_table[theta_index];
    *y_out = float_radius * sin_table[theta_index];

    return 0; // Success
}

int main() {
    const char* large_num_input = "9210836494447108270027136741376870869791784014198948301625976867708124077590";
    float x_coord, y_coord;

    printf("Attempting to map large number: %s\n", large_num_input);

    int status = map_to_cartesian(large_num_input, &x_coord, &y_coord);

    // Removed raw status print here, handled in if/else block

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
        size_t rsm_buf_size = input_len + 1; // For radius string + null terminator
        if (rsm_buf_size < 2) rsm_buf_size = 2; // Min for "0" and '\0'

        char actual_rsm_buffer[rsm_buf_size];

        if (calculate_large_div_to_string(large_num_input, 40, actual_rsm_buffer, rsm_buf_size) == 0) {
            printf("Main: Radius String (from calculate_large_div_to_string): %s\n", actual_rsm_buffer);
            float float_radius_main = strtof(actual_rsm_buffer, NULL);
            // Check for strtof errors (optional, but good practice)
            // if (errno == ERANGE) {
            //     perror("strtof error for radius_str_main");
            // }
            printf("Main: Float Radius (from strtof): %f\n", float_radius_main);
            printf("Main: Float Radius (scientific notation): %e\n", float_radius_main);
        } else {
            printf("Main: Error calculating radius string for verification.\n");
        }

        printf("Main: X Coordinate (from map_to_cartesian): %.8f\n", x_coord);
        printf("Main: Y Coordinate (from map_to_cartesian): %.8f\n", y_coord);
    } else if (status == -2) {
        // map_to_cartesian already printed detailed error to stderr for this case
        printf("Main: map_to_cartesian failed with status %d (Radius overflow or NaN detected during float conversion).\n", status);
    } else { // Handles -1 or any other non-zero status
        // map_to_cartesian should have printed details to stderr for other errors too
        printf("Main: map_to_cartesian failed with status %d (General error, check stderr for details from function).\n", status);
    }

    return status; // Return 0 on success, non-zero on failure
}
