#include "PCUnit/PCUnit.h"
#include "mock_target.h"
#include <string.h>

static int setup(void)
{
	mock_target_init();
	return 0;
}


static void test_f_int_args(void)
{
	f_int_args_Expectation e[1] = {{0}};
	e[0].expected.a = 1;
	e[0].expected.b = 2;
	e[0].expected.c = 3;
	e[0].expected.d = 4;
	e[0].retval = 5;
	f_int_args_expect(e, sizeof e / sizeof e[0]);
	PCU_ASSERT_EQUAL(5, f_int_args(1, 2, 3, 4));
	mock_target_verify();
}

static void test_f_int_args_all_params_ignored(void)
{
	f_int_args_Expectation e[1] = {{0}};
	memset(&e[0].ignored, 1, sizeof e[0].ignored);
	e[0].retval = 5;
	f_int_args_expect(e, sizeof e / sizeof e[0]);
	PCU_ASSERT_EQUAL(5, f_int_args(1, 2, 3, 4));
	mock_target_verify();
}

static void test_f_int_args_param_ignored(void)
{
	f_int_args_Expectation e[1] = {{0}};
	e[0].expected.a = 1;
	e[0].ignored.b = 1;
	e[0].expected.c = 3;
	e[0].ignored.d = 1;
	e[0].retval = 5;
	f_int_args_expect(e, sizeof e / sizeof e[0]);
	PCU_ASSERT_EQUAL(5, f_int_args(1, 1000, 3, 1000));
	mock_target_verify();
}

static void test_f_int_args_2times(void)
{
	f_int_args_Expectation e[2] = {{0}};
	e[0].expected.a = 1;
	e[0].expected.b = 2;
	e[0].expected.c = 3;
	e[0].expected.d = 4;
	e[0].retval = 5;
	e[1].expected.a = 10;
	e[1].expected.b = 20;
	e[1].expected.c = 30;
	e[1].expected.d = 40;
	e[1].retval = 50;
	f_int_args_expect(e, sizeof e / sizeof e[0]);
	PCU_ASSERT_EQUAL(5, f_int_args(1, 2, 3, 4));
	PCU_ASSERT_EQUAL(50, f_int_args(10, 20, 30, 40));
	mock_target_verify();
}

static void test_f_const_char_ptr(void)
{
	f_const_char_ptr_Expectation e[1] = {{0}};
	e[0].expected.a = "hoge";
	e[0].retval = "foo";
	f_const_char_ptr_expect(e, sizeof e / sizeof e[0]);
	PCU_ASSERT_STRING_EQUAL("foo", f_const_char_ptr("hoge"));
	mock_target_verify();
}

static void test_f_void(void)
{
	f_void_omitted_Expectation e[1] = {{0}};
	f_void_omitted_expect(e, sizeof e / sizeof e[0]);
	f_void_omitted();
	mock_target_verify();
}

static void test_f_STRUCT_t_ptr(void)
{
	STRUCT_t st1 = {100};
	STRUCT_t st2 = {10};
	STRUCT_t st3 = {10};
	f_STRUCT_t_ptr_Expectation e[1] = {{0}};
	e[0].expected.a = &st1;
	e[0].retval = &st2;
	f_STRUCT_t_ptr_expect(e, sizeof e / sizeof e[0]);
	PCU_ASSERT_MEMORY_EQUAL(&st3, f_STRUCT_t_ptr(&st1), sizeof(STRUCT_t), 1);
	mock_target_verify();
}


static int f_int_args_callback(int a, int b, int c, int d)
{
	const int expected_a[] = {1, 10, 100, 1000};
	const int expected_b[] = {2, 20, 200, 2000};
	const int expected_c[] = {3, 30, 300, 3000};
	const int expected_d[] = {4, 40, 400, 4000};
	const int retval[]     = {5, 50, 500, 5000};
	PCU_ASSERT_OPERATOR(f_int_args_num_calls(), <, sizeof retval / sizeof retval[0]);
	PCU_ASSERT_EQUAL(expected_a[f_int_args_num_calls()], a);
	PCU_ASSERT_EQUAL(expected_b[f_int_args_num_calls()], b);
	PCU_ASSERT_EQUAL(expected_c[f_int_args_num_calls()], c);
	PCU_ASSERT_EQUAL(expected_d[f_int_args_num_calls()], d);
	return retval[f_int_args_num_calls()];
}

static void test_f_int_args_callback(void)
{
	f_int_args_set_callback(f_int_args_callback);
	PCU_ASSERT_EQUAL(5, f_int_args(1, 2, 3, 4));
	PCU_ASSERT_EQUAL(50, f_int_args(10, 20, 30, 40));
	PCU_ASSERT_EQUAL(500, f_int_args(100, 200, 300, 400));
	PCU_ASSERT_EQUAL(5000, f_int_args(1000, 2000, 3000, 4000));
	PCU_ASSERT_EQUAL(4, f_int_args_num_calls());
}


PCU_Suite *MockTest_suite(void)
{
	static PCU_Test tests[] = {
		PCU_TEST(test_f_int_args),
		PCU_TEST(test_f_int_args_all_params_ignored),
		PCU_TEST(test_f_int_args_param_ignored),
		PCU_TEST(test_f_int_args_2times),
		PCU_TEST(test_f_const_char_ptr),
		PCU_TEST(test_f_void),
		PCU_TEST(test_f_STRUCT_t_ptr),
		PCU_TEST(test_f_int_args_callback),
	};
	static PCU_Suite suite = {
		"MockTest", tests, sizeof tests / sizeof tests[0], setup
	};
	return &suite;
}

