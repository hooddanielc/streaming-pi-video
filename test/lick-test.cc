#include <lick/lick.h>

FIXTURE(is_equal) {
  EXPECT_EQ(0, 0) << "asserts equal";
}

FIXTURE(not_equal) {
  EXPECT_NE(0, 1) << "asserts not equal";
}

FIXTURE(falsy_values) {
  EXPECT_FALSE(false) << "asserts false";
}

FIXTURE(true_values) {
  EXPECT_TRUE(true) << "asserts true";
}

FIXTURE(greater_than) {
  EXPECT_GT(0, -1) << "asserts greater than";
}

FIXTURE(lesser_than) {
  EXPECT_LT(0, 1) << "asserts lesser than";
}

FIXTURE(greater_than_or_equal) {
  EXPECT_GE(1, 0) << "asserts greater or equal with not equal";
  EXPECT_GE(-1, -1) << "asserts greather than or equal with equal";
}

FIXTURE(lesser_than_or_equal) {
  EXPECT_LE(0, 1) << "asserts lesser or equal with not equal";
  EXPECT_LE(-1, -1) << "asserts lesser than or equal with equal";
}

int main(int argc, char *argv[]) {
  return dj::lick::main(argc, argv);
}
