/// @brief Classic quicksort implementation.
void quicksort(double *array, int length);

/// @brief Quicksort implementation where values are spread along two separate arrays.
/// @param array1 Array with lower values once sorting is done.
/// @param array2 Array with higher values once sorting is done.
/// @param length1 Length of [array1].
/// @param length2 Length of [array2].
void quicksort_split(double *array1, double *array2, int length1, int length2);
