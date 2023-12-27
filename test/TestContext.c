#include <Context.h>

#include <stdio.h>
#include <malloc.h>

#define TEST_STACK_SIZE 128
static WThreadContext contexts[2];

static void testFunc() {
	char arr[100] = { 0 };
	for (int i = 0; i < 100; i++) {
		arr[i] = i;
	}

	for (float i = 0; i < 10; i++) {
		printf("Test Start:%d\n", (int)i);
		wThreadSwapContext(&contexts[1], &contexts[0]);
		printf("Test End:%d\n", (int)i);
	}
}

void mainTest(float i) {
	for (; i < 10; i++) {
		printf("Main Start:%d\n", (int)i);
		wThreadSwapContext(&contexts[0], &contexts[1]);
		printf("Main End:%d\n", (int)i);
	}
}

int main(int argc, char* argv[]) {
	int arr[100] = { 0 };
	for (int i = 0; i < 100; i++) {
		arr[i] = i;
	}

	WThreadContext c[2];
	wThreadGetContext(&c[0]);

	WThreadStack st;
	wThreadCreateStack(&st, TEST_STACK_SIZE);

	wThreadInitContext(&contexts[1], testFunc, &st);

	mainTest(0.f);

	wThreadFreeStack(&st);
	return 0;
}
