void swap(double *a, double *b);

int min(int a, int b);

int max(int a, int b);

/// @brief Create a random array with [length] values ranging from [min] to [max].
double *random_double_array(int length, double min, double max);

/// @brief Reads an [array] of doubles and it's [length] from [path].
/// @param array An array of <double>.
/// @param length Length of [array].
/// @param path Path to file, containing filename and extension.
void read_double_array(double **array, int *length, char *path);

/// @brief Writes an [array] of doubles preceded by it's [length] to [path].
/// @param array An array of <double>.
/// @param length Length of [array].
/// @param path Path to file, containing filename and extension.
void save_double_array(double *array, int length, char *path);

/// @brief Prints [array] to the console.
void print_double_array(double *array, int length);

/// @brief Returns index of first element out of order in [array]. Returns -1 if [array] is sorted.
int sorted(double *array, int length);
