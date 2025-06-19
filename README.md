# Cartesian Number Mapper in C

## Overview

This project provides a C program (`mapper.c`) that maps large unsigned integer numbers to 2D Cartesian coordinates (x, y). The mapping is based on a geometric interpretation where each number corresponds to a polar vector, which is then converted to Cartesian coordinates. This approach utilizes a predefined angular spiral and pre-calculated trigonometric values to avoid external mathematical libraries.

The core logic is encapsulated in the `map_to_cartesian` function within `mapper.c`.

## Mathematical Process

The mapping from an input number to Cartesian coordinates follows these steps:

1.  **Input Number (`N`)**: An `unsigned long` integer.

2.  **Angular Component (Theta Determination)**:
    *   The system uses a spiral of 40 discrete angular vectors, each separated by 9 degrees (40 * 9° = 360°).
    *   The specific vector for the input number `N` is determined by its remainder when divided by 40:
        `theta_index = N % 40`
    *   This `theta_index` is then used to look up pre-calculated sine and cosine values. The actual angle in degrees would be `theta = theta_index * 9`.

3.  **Radial Component (Radius Determination)**:
    *   The magnitude (or radius `r`) of the vector is determined by the integer quotient of the input number `N` divided by 40:
        `radius = N / 40`

4.  **Coordinate Calculation**:
    *   With the `theta_index` and `radius`, the Cartesian coordinates (x, y) are calculated using:
        *   `x_coordinate = radius * cos_table[theta_index]`
        *   `y_coordinate = radius * sin_table[theta_index]`
    *   `cos_table` and `sin_table` are pre-calculated arrays storing the cosine and sine values for each of the 40 angles (0°, 9°, 18°, ..., 351°).

## Implementation Details

*   **Language**: C (C99 standard assumed for `unsigned long` and `float` types).
*   **Input Number Type**: `unsigned long`.
*   **Trigonometric Values**: `float` type, stored in `sin_table` and `cos_table`.
*   **Output Coordinates**: `float` type.
*   **Dependencies**: Standard C library (`stdio.h` for printing). No external math libraries (like `math.h` for `sin`/`cos` functions at runtime) are used for the core conversion; trigonometric values are from tables. The `-lm` flag during compilation is good practice if `math.h` functions were to be added later but is not strictly needed for the current table-based `sin`/`cos` lookup.

## Compilation

To compile the `mapper.c` program, use a C compiler like GCC:

```bash
gcc mapper.c -o mapper -lm
```
(The `-lm` links the math library, which isn't strictly necessary for the current version using only tables but is harmless.)

## Execution

After successful compilation, run the executable:

```bash
./mapper
```

The program will use a hardcoded example number (`123456789`) and print the mapping details to the console.

## Example Output

Running the `mapper` executable will produce output similar to the following (exact floating-point precision may vary slightly based on system/compiler):

```
Input Number: 123456789
Theta Index: 29 (Angle: 261 degrees)
Radius: 3086419
Cos_val from table at index 29: -0.15643448
Sin_val from table at index 29: -0.98768836
Calculated X Coordinate: -482822.34375000
Calculated Y Coordinate: -3048420.25000000
```

This output shows the input number, its corresponding theta index and angle, the calculated radius, the sine/cosine values retrieved from the tables for that index, and the final computed X and Y Cartesian coordinates.
