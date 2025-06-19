#include <stdio.h>

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

// Function to map a large number to Cartesian coordinates
// Parameters:
//   unsigned long x: The input number
//   float *x_out: Pointer to store the calculated x-coordinate
//   float *y_out: Pointer to store the calculated y-coordinate
void map_to_cartesian(unsigned long x, float *x_out, float *y_out) {
    if (x_out == NULL || y_out == NULL) {
        // Or handle error appropriately
        return;
    }

    int theta_index = x % 40;
    unsigned long radius = x / 40; // Integer division

    float cos_val = cos_table[theta_index];
    float sin_val = sin_table[theta_index];

    *x_out = (float)radius * cos_val;
    *y_out = (float)radius * sin_val;
}

int main() {
    unsigned long number_to_map = 123456789;
    float x_coord, y_coord;

    // Calculate theta_index and radius for verification printing
    // These are also calculated inside map_to_cartesian, but we want to print them from main
    int theta_index_main = number_to_map % 40;
    unsigned long radius_main = number_to_map / 40;

    map_to_cartesian(number_to_map, &x_coord, &y_coord);

    printf("Input Number: %lu\n", number_to_map);
    printf("Theta Index: %d (Angle: %d degrees)\n", theta_index_main, theta_index_main * 9);
    printf("Radius: %lu\n", radius_main);

    // Accessing table values directly for printing verification
    // Ensure these are the values used by the function for the given index
    if (theta_index_main >= 0 && theta_index_main < 40) {
        printf("Cos_val from table at index %d: %.8f\n", theta_index_main, cos_table[theta_index_main]);
        printf("Sin_val from table at index %d: %.8f\n", theta_index_main, sin_table[theta_index_main]);
    }

    printf("Calculated X Coordinate: %.8f\n", x_coord);
    printf("Calculated Y Coordinate: %.8f\n", y_coord);

    return 0;
}
