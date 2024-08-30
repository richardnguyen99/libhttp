#include <unity/unity_fixture.h>

TEST_GROUP(TEST_DUMMY);

TEST_SETUP(TEST_DUMMY) {}

TEST_TEAR_DOWN(TEST_DUMMY) {}

TEST(TEST_DUMMY, DummyAddingTest)
{
	TEST_ASSERT_EQUAL(2, 1 + 1);
	TEST_ASSERT_EQUAL(4, 2 + 2);

	TEST_PASS_MESSAGE("DummyAddingTest passed");
}

TEST_GROUP_RUNNER(TEST_DUMMY)
{
	RUN_TEST_CASE(TEST_DUMMY, DummyAddingTest);
}

static void RunAllTests(void)
{
	RUN_TEST_GROUP(TEST_DUMMY);
}

int main(int argc, const char *argv[])
{
	return UnityMain(argc, argv, RunAllTests);
}
