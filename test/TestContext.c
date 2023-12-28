#include <Context.h>

#include <stdio.h>
#include <malloc.h>

#define TEST_STACK_SIZE 1024 * 1024
static WThreadContext contexts[2];
static int testValue = -2;

static void testFunc(void) {
	char arr[100] = { 0 };
	for (int i = 0; i < 100; i++) {
		arr[i] = i;
	}

	for (int i = 0; i < 10; i++) {
		printf("Test Start:%d\n", i);
		testValue += 2;

		wThreadSwapContext(&contexts[1], &contexts[0]);

		printf("Test End:%d\n", i);
		testValue -= 2;
	}
}

void mainTest(float i) {
	for (; i < 10; i++) {
		printf("Main Start:%d\n", (int)i);
		testValue += 3;

		wThreadSwapContext(&contexts[0], &contexts[1]);

		printf("Main End:%d\n", (int)i);
		testValue -= 3;
	}
}

int main(int argc, char* argv[]) {
	int arr[100] = { 0 };
	for (int i = 0; i < 100; i++) {
		arr[i] = i;
	}

	WThreadStack st;
	wThreadCreateStack(&st, TEST_STACK_SIZE);

	wThreadInitContext(&contexts[1], testFunc, &st);

	mainTest(0.f);

	wThreadFreeStack(&st);
	return testValue;
}
