/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include "gtest/gtest.h"
#include "xtensor/xarray.hpp"
#include "xtensor/xtensor.hpp"
#include "xtensor/xrandom.hpp"
#include "xtensor/xview.hpp"
#include "xtensor/xmath.hpp"
#include "test_common.hpp"

namespace xt
{
    TEST(xlayout, xarray_initializer_constructors)
    {
        xarray<double, layout_type::column_major> cm = {{1,2,3}, {4,5,6}, {7,8,9}};
        xarray<double, layout_type::row_major> rm = {{1,2,3}, {4,5,6}, {7,8,9}};
        xarray<double, layout_type::dynamic> dm = {{1,2,3}, {4,5,6}, {7,8,9}};
        EXPECT_EQ(rm(0, 2), cm(0, 2));
        EXPECT_EQ(dm(0, 2), cm(0, 2));
        EXPECT_EQ(rm(2, 2), cm(2, 2));
        auto rm1 = rm[{2, 0}];
        auto cm1 = cm[{2, 0}];
        EXPECT_EQ(rm1, cm1);
        std::vector<double> cm_data = {1,4,7,2,5,8,3,6,9};
        std::vector<double> rm_data = {1,2,3,4,5,6,7,8,9};
        EXPECT_FALSE(cm.data() == rm.data());
        EXPECT_EQ(cm_data, cm.data());
        EXPECT_EQ(rm_data, rm.data());
        EXPECT_EQ(rm_data, dm.data());
        EXPECT_TRUE(cm.data()[1] == 4);
        EXPECT_TRUE(rm.data()[1] == 2);

        xarray<double, layout_type::column_major> cm_assigned = rm;
        EXPECT_EQ(cm.data(), cm_assigned.data());
    }

    TEST(xlayout, xtensor_initializer_constructors)
    {
        xtensor<double, 2, layout_type::column_major> cm = {{1,2,3}, {4,5,6}, {7,8,9}};
        xtensor<double, 2, layout_type::row_major> rm = {{1,2,3}, {4,5,6}, {7,8,9}};
        xtensor<double, 2, layout_type::dynamic> dm = {{1,2,3}, {4,5,6}, {7,8,9}};
        EXPECT_EQ(rm(0, 2), cm(0, 2));
        EXPECT_EQ(dm(0, 2), cm(0, 2));
        EXPECT_EQ(rm(2, 2), cm(2, 2));
        auto rm1 = rm[{2, 0}];
        auto cm1 = cm[{2, 0}];
        EXPECT_EQ(rm1, cm1);
        std::vector<double> cm_data = {1,4,7,2,5,8,3,6,9};
        std::vector<double> rm_data = {1,2,3,4,5,6,7,8,9};
        EXPECT_FALSE(cm.data() == rm.data());
        EXPECT_EQ(cm_data, cm.data());
        EXPECT_EQ(rm_data, rm.data());
        EXPECT_EQ(rm_data, dm.data());
        EXPECT_TRUE(cm.data()[1] == 4);
        EXPECT_TRUE(rm.data()[1] == 2);

        xtensor<double, 2, layout_type::column_major> cm_assigned = rm;
        EXPECT_EQ(cm.data(), cm_assigned.data());
    }

    TEST(xlayout, xfunctions_mix_layouts)
    {
        xarray<double, layout_type::column_major> cm = {{1,2,3}, {4,5,6}, {7,8,9}};
        xarray<double, layout_type::row_major> rm = {{1,2,3}, {4,5,6}, {7,8,9}};

        auto f = cm * rm + 5;
        EXPECT_EQ(6, f(0, 0));
        EXPECT_EQ(9, f(0, 1));

        xarray<double> res = f;
        EXPECT_EQ(6, res(0, 0));
        EXPECT_EQ(9, res(0, 1));

        auto e = equal(cm, rm);
        EXPECT_TRUE(e(0, 1));
        EXPECT_TRUE(e(1, 1));

        for (auto it = e.xbegin(); it != e.xend(); it++)
        {
            EXPECT_TRUE(*it);
        }

        xarray<bool> ree = e;
        EXPECT_TRUE(ree(0, 1));
        EXPECT_TRUE(ree(1, 1));

        EXPECT_TRUE(all(ree));
    }

    TEST(xlayout, xgenerator_assignment)
    {
        xarray<double, layout_type::column_major> cm = {{1,2,3}, {4,5,6}, {7,8,9}};
        xarray<double, layout_type::row_major> rm = {{1,2,3}, {4,5,6}, {7,8,9}};

        xarray<double, layout_type::column_major> cmt1 = triu(cm);
        xarray<double, layout_type::column_major> cmt2 = triu(rm);
        xarray<double, layout_type::row_major> rmt1 = triu(cm);
        xarray<double, layout_type::row_major> rmt2 = triu(rm);

        EXPECT_EQ(cmt1.data(), cmt2.data());
        EXPECT_EQ(rmt1.data(), rmt2.data());
        EXPECT_TRUE(all(equal(cmt1, cmt2)));
        EXPECT_TRUE(all(equal(rmt1, rmt2)));

        random::seed(0);
        xarray<double, layout_type::column_major> crand = random::rand<double>({5, 5});
        random::seed(0);
        xarray<double, layout_type::row_major> rrand = random::rand<double>({5, 5});
        EXPECT_EQ(rrand(2, 3), crand(2, 3));
    }

    TEST(xlayout, xview)
    {
        xarray<double, layout_type::column_major> cm = {{1,2,3}, {4,5,6}, {7,8,9}};
        xarray<double, layout_type::row_major> rm = {{1,2,3}, {4,5,6}, {7,8,9}};

        auto vcm = view(cm, range(0, 2), range(1, 3));
        auto vrm = view(rm, range(0, 2), range(1, 3));

        EXPECT_EQ(vrm(0, 0), vcm(0, 0));
        EXPECT_EQ(vrm(0, 1), vcm(0, 1));
        EXPECT_EQ(3, vcm(0, 1));
        EXPECT_EQ(3, vrm(0, 1));
    }

    TEST(xlayout, xreducer)
    {
        xarray<double, layout_type::column_major> cm = {{1,2,3}, {4,5,6}, {7,8,9}};
        xarray<double, layout_type::row_major> rm = {{1,2,3}, {4,5,6}, {7,8,9}};

        auto rcm = sum(cm, {1});
        auto rrm = sum(rm, {1});
        auto rrm_wrong = sum(rm, {0});

        EXPECT_EQ(rrm, rcm);
        EXPECT_NE(rrm_wrong, rcm);
    }


    TEST(xlayout, DISABLED_equal_iterator)
    {
        xarray<double, layout_type::column_major> cm = {{1,2,3}, {4,5,6}, {7,8,9}};
        xarray<double, layout_type::row_major> rm = {{1,2,3}, {4,5,6}, {7,8,9}};
        auto e = equal(cm, rm);
        for (auto el : e)
        {
            EXPECT_TRUE(el);
        }

    }
}