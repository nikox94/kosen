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
  Vect a_c(1, 0, 0);
  Vect b(1, 1, 1);
  Vect b_c(sqrt(1/3.0), sqrt(1/3.0), sqrt(1/3.0));
  Vect c(-1, 0, -1);
  Vect c_c(-sqrt(1/2.0), 0, -sqrt(1/2.0));
  Vect d(-1000, 2500, -1500);
  Vect d_c(-1000/sqrt(95E5), 2500/sqrt(95E5), -1500/sqrt(95E5));
  EXPECT_EQ(a_c, a.normalise());
  EXPECT_EQ(b_c, b.normalise());
  EXPECT_EQ(c_c, c.normalise());
  EXPECT_EQ(d_c, d.normalise());
}
