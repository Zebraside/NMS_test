#pragma once

#include <vector>

#include <gtest/gtest.h>

#include "nms.h"



struct NMSTestParams {
    std::vector<BBox> input;
    std::vector<BBox> expected;
    float threshhold;
};

class NMSTest : public ::testing::TestWithParam<NMSTestParams> {

};

std::vector<std::vector<BBox>> input = {
    {

    },
    {
        {{5, 5, 1, 1}, 1.0f}
    },
    // intersection
    {
        {{5, 5, 3, 1}, 0.9f},
        {{6, 5, 2, 1}, 0.7f},
    },
    {
        {{5, 5, 1, 3}, 0.9f},
        {{5, 6, 1, 2}, 0.7f},
    },
    {
        {{5, 5, 10, 10}, 0.9f},
        {{10, 5, 5, 10}, 0.7f},
    },
    // no intersection
    {
        {{5, 5, 2, 1}, 0.9f},
        {{8, 5, 1, 1}, 0.7f},
    },
    // multiple intersections
    {
        {{4, 5, 3, 1}, 0.5f},
        {{6, 5, 1, 1}, 0.7f},
        {{5, 5, 2, 1}, 0.9f},
    },
};

std::vector<std::vector<BBox>> expected = {
    {

    },
    {
        {{5, 5, 1, 1}, 1.0f}
    },
    // intersection
    {
        {{5, 5, 3, 1}, 0.9f},
    },
    {
        {{5, 5, 1, 3}, 0.9f},
    },
    {
        {{5, 5, 10, 10}, 0.9f},
    },
    // no intersection
    {
        {{5, 5, 2, 1}, 0.9f},
        {{8, 5, 1, 1}, 0.7f},
    },
    // multiple intersections
    {
        {{5, 5, 2, 1}, 0.9f},
    }
};

std::vector<float> thresholds = {
        0.7f,
        0.7f,
        0.6f,
        0.6f,
        0.4f,
        0.99f,
        0.7f
};

std::vector<NMSTestParams> generateParams() {
    assert(input.size() == expected.size());
    assert(input.size() == thresholds.size());

    std::vector<NMSTestParams> params;
    for (size_t i = 0; i < input.size(); ++i) {
        params.push_back({input[i], expected[i], thresholds[i]});
    }

    return params;
}

INSTANTIATE_TEST_CASE_P(
        NMSTests,
        NMSTest,
        ::testing::ValuesIn(generateParams()));
