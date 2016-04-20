#include "Vect.h"
#include "gtest/gtest.h"
#include <math.h>

TEST(VectTest, Magnitude) {
  Vect a(1, 0, 0);
  Vect b(0, -1, 0);
  Vect c(1, 1, 1);
  Vect d(-100, 5000, -200);
  EXPECT_EQ(1, a.magnitude());
  EXPECT_EQ(1, b.magnitude());
  EXPECT_EQ(sqrt(3), c.magnitude());
  EXPECT_EQ(sqrt(25050000), d.magnitude());
}

TEST(VectTest, Normalise) {
  Vect a(5, 0, 0);
  Vect b(1, 0, 0);
  EXPECT_EQ(b, a.normalise());
}
