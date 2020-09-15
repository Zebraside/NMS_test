#include <random>

#include "nms_test.h"

TEST_P(NMSTest, test_nms) {
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

float iou(const NMSUtils::Rect &a, const NMSUtils::Rect &b) {
    int minX = std::max(a.x, b.x);
    int minY = std::max(a.y, b.y);
    int maxX = std::min(a.x + a.w, b.x + b.w);
    int maxY = std::min(a.y + a.h, b.y + b.h);

    float inersect_area = std::max(0, maxX - minX) * std::max(0, maxY - minY);

    return inersect_area / ((a.w * a.h) + (b.w * b.h) - inersect_area);
}

std::vector<size_t> referenceNMS(const std::vector<NMSUtils::Rect>& boxes, const std::vector<float>& scores, float threshold) {
    assert(boxes.size() == scores.size());

    std::vector<std::pair<int, float>> sorted_idxs;
    for (size_t i = 0; i < boxes.size(); ++i) {
        sorted_idxs.push_back({i, scores[i]});
    }

    std::sort(sorted_idxs.begin(), sorted_idxs.end(), [](const std::pair<int, float>& left, const std::pair<int, float>& right) {
        return left.second > right.second;
    });

    std::vector<size_t> idxs;
    std::vector<bool> keep(sorted_idxs.size(), true);
    for (size_t i = 0; i < sorted_idxs.size(); ++i) {
        size_t high_idx = sorted_idxs[i].first;
        if (!keep[high_idx])
            continue;

        for (size_t j = i + 1; j < sorted_idxs.size(); ++j) {
            size_t cur_idx = sorted_idxs[j].first;
            if (!keep[cur_idx])
                continue;

            keep[cur_idx] = keep[cur_idx] && (iou(boxes[high_idx], boxes[cur_idx]) <= threshold);
        }

        if (keep[high_idx])
            idxs.push_back(high_idx);
    }

    return idxs;
}

std::vector<NMSTestParams> generateRandomNMSTestParams() {
    std::default_random_engine engine;
    std::uniform_int_distribution<int> uniform_int_dist(1, 1000);
    std::normal_distribution<float> norm_float_dist(0.6, 0.4);
    auto random_uniform_int = std::bind(uniform_int_dist, engine);
    auto random_norm_float = std::bind(norm_float_dist, engine);

    std::vector<NMSTestParams> params;
    for (auto test_size : {10, 100, 500, 1000, 5000, 10000}) {
        NMSTestParams param;
        std::vector<NMSUtils::Rect> boxes;
        std::vector<float> scores;
        for (int i = 0; i < test_size; ++i) {
            boxes.push_back({random_uniform_int(), random_uniform_int(), random_uniform_int(), random_uniform_int()});
            scores.push_back({random_norm_float()});
        }

        param.boxes = boxes;
        param.scores = scores;
        param.threshhold = std::max(random_norm_float(), 0.0f);
        param.expected = referenceNMS(boxes, scores, param.threshhold);

        params.push_back(param);
    }

    return params;
}
