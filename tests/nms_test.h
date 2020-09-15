#pragma once

#include <vector>

#include <gtest/gtest.h>

#include "nms.h"

struct NMSTestParams {
    std::vector<NMSUtils::Rect> boxes;
    std::vector<float> scores;
    float threshhold;

    std::vector<size_t> expected;
};

class NMSTest : public ::testing::TestWithParam<NMSTestParams> {

};

std::vector<NMSTestParams> test_params = {
        {
            {},
            {},
            0.7f,
            {}
        },
        {
            {{5, 5, 1, 1}},
            {1.0f},
            0.7f,
            {0}
        },
        // intersection
        {
            {{5, 5, 3, 1}, {6, 5, 2, 1}},
            {0.9f, 0.7f},
            0.6f,
            {0}
        },
        {
            {{5, 5, 1, 3}, {5, 6, 1, 2}},
            {0.9f, 0.7f},
            0.6f,
            {0}
        },
        {
            {{5, 5, 10, 10}, {10, 5, 5, 10}},
            {0.9f, 0.7f},
            0.4f,
            {0}
        },
        // no intersection
        {
            {{5, 5, 2, 1}, {8, 5, 1, 1}},
            {0.9f, 0.7f},
            0.99f,
            {0, 1}
        },
        {
            {{5, 5, 2, 1}, {5, 9, 1, 1}},
            {0.9f, 0.7f},
            0.99f,
            {0, 1}
        },
        // multiple intersections
        {
            {{4, 5, 3, 1}, {6, 5, 1, 1}, {5, 5, 2, 1}},
            {0.5f, 0.7f, 0.9f},
            0.3f,
            {2}
        }
};

INSTANTIATE_TEST_CASE_P(
        NMSTestsManual,
        NMSTest,
        ::testing::ValuesIn(test_params));

std::vector<NMSTestParams> generateRandomNMSTestParams();

INSTANTIATE_TEST_CASE_P(
        NMSTestsRandom,
        NMSTest,
        ::testing::ValuesIn(generateRandomNMSTestParams()));
