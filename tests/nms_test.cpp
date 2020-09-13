#include "nms_test.h"
#include "nms.h"

TEST_P(NMSTest, test_nms_real_fast) {
    auto params = GetParam();
    auto indices = NMSUtils::nms(params.boxes, params.scores, params.threshhold);

    auto expected = params.expected;

    ASSERT_EQ(indices.size(), expected.size());

    std::sort(indices.begin(), indices.end());
    std::sort(expected.begin(), expected.end());

    for (size_t i = 0; i < indices.size(); ++i) {
        ASSERT_EQ(indices[i], expected[i]);
    }
}
