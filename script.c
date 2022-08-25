extern void someFunction(int i);

int add(int length, int* array) {
    int result = 0;
	for (int i = 0; i < length; i++) {
		result += array[i];
		array[i]++;
	}
	return result;
}
