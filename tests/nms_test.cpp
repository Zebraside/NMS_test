#include "nms_test.h"
#include "nms.h"

bool isEqual(const Rect& left, const Rect& right) {
    return (left.x == right.x) &&
           (left.y == right.y) &&
           (left.w == right.w) &&
           (left.h == right.h);
}

bool isEqual(const BBox& left, const BBox& right) {
    return isEqual(left.box, right.box) &&
           std::abs(left.prob - right.prob) < 0.00001f;
}

auto bbox_sort = [](const BBox& left, const BBox& right) {
    if (left.box.x != right.box.x) {
        return left.box.x < right.box.x;
    }
    if (left.box.y != right.box.y) {
        return left.box.y < right.box.y;
    }
    if (left.box.w != right.box.w) {
        return left.box.w < right.box.w;
    }
    if (left.box.h != right.box.h) {
        return left.box.x < right.box.x;
    }

    if (std::abs(left.prob - right.prob) > 0.00001) {
        return left.prob < right.prob;
    }

    return false;
};

TEST_P(NMSTest, test_nms_naive) {
    auto params = GetParam();
    auto result = Utils::nms_naive(params.input, params.threshhold);

    auto expected = params.expected;
    ASSERT_EQ(result.size(), expected.size());


    std::sort(result.begin(), result.end(), bbox_sort);
    std::sort(expected.begin(), expected.end(), bbox_sort);

    for (size_t i = 0; i < result.size(); ++i) {
        ASSERT_TRUE(isEqual(result[i], expected[i]));
    }
}

TEST_P(NMSTest, test_nms_fast) {
    auto params = GetParam();
    auto result = Utils::nms_fast(params.input, params.threshhold);

    auto expected = params.expected;
    ASSERT_EQ(result.size(), expected.size());


    std::sort(result.begin(), result.end(), bbox_sort);
    std::sort(expected.begin(), expected.end(), bbox_sort);

    for (size_t i = 0; i < result.size(); ++i) {
        ASSERT_TRUE(isEqual(result[i], expected[i]));
    }
}

TEST_P(NMSTest, test_nms_real_fast) {
    auto params = GetParam();
    auto result = Utils::nms_real_fast(params.input, params.threshhold);

    auto expected = params.expected;
    ASSERT_EQ(result.size(), expected.size());


    std::sort(result.begin(), result.end(), bbox_sort);
    std::sort(expected.begin(), expected.end(), bbox_sort);

    for (size_t i = 0; i < result.size(); ++i) {
        ASSERT_TRUE(isEqual(result[i], expected[i]));
    }
}
