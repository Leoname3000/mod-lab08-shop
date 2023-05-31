// Copyright 2021 GHA Test Team
#include <gtest/gtest.h>
#include "../include/task.h"

TEST(ShopTest, FactorialTest) {
    Shop shop = Shop(2,4,100,10,10);
    int factorial = shop.Factorial(5);
    EXPECT_EQ(120, factorial);
}

TEST(ShopTest, LambdaTest) {
    Shop shop = Shop(2,4,100,10,10);
    double lambda = shop.TheoreticalLambda();
    EXPECT_EQ(4.0, lambda);
}

TEST(ShopTest, MuTest) {
    Shop shop = Shop(2,4,100,10,10);
    double mu = shop.TheoreticalMu();
    EXPECT_EQ(2.0, mu);
}

TEST(ShopTest, RhoTest) {
    Shop shop = Shop(2,4,100,10,10);
    double lambda = shop.TheoreticalLambda();
    double mu = shop.TheoreticalMu();
    double rho = shop.Rho(lambda, mu);
    EXPECT_EQ(2.0, rho);
}

TEST(ShopTest, IdlingProbabilityTest) {
    Shop shop = Shop(2,4,100,10,10);
    double idlingProbability = shop.TheoreticalIdlingProbability();
    EXPECT_EQ(0.04, idlingProbability);
}

TEST(ShopTest, RejectionProbabilityTest) {
    Shop shop = Shop(2,4,100,10,10);
    double rejectionProbability = shop.TheoreticalRejectionProbability();
    EXPECT_EQ(0.08, rejectionProbability);
}

TEST(ShopTest, RelativeThroughputTest) {
    Shop shop = Shop(2,4,100,10,10);
    double rejectionProbability = shop.TheoreticalRejectionProbability();
    double relativeThroughput = shop.RelativeThroughput(rejectionProbability);
    EXPECT_EQ(0.92, relativeThroughput);
}

TEST(ShopTest, AbsoluteThroughputTest) {
    Shop shop = Shop(2,4,100,10,10);
    double lambda = shop.TheoreticalLambda();
    double rejectionProbability = shop.TheoreticalRejectionProbability();
    double relativeThroughput = shop.RelativeThroughput(rejectionProbability);
    double absoluteThroughput = shop.AbsoluteThroughput(lambda, relativeThroughput);
    EXPECT_EQ(3.68, absoluteThroughput);
}