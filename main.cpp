#include <random>
#include <functional>

#include <benchmark/benchmark.h>
#include <opencv2/dnn/dnn.hpp>

#include "nms.h"

std::vector<Rect> generateBoxes(size_t size) {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> box_dist(0,1000);
    auto box_param = std::bind ( box_dist, generator );

    std::vector<Rect> bboxes;
    for (size_t i = 0; i < size; ++i) {
        bboxes.push_back({box_param(), box_param(), box_param(), box_param()});
    }

    return bboxes;
}

std::vector<float> generateScores(size_t size) {
    std::default_random_engine generator;
    std::normal_distribution<float> score_dist(0.5,0.3);
    auto score_param = std::bind ( score_dist, generator );

    std::vector<float> scores;
    for (size_t i = 0; i < size; ++i) {
        scores.push_back(score_param());
    }

    return scores;
}

static void BM_nms_real_fast(benchmark::State& state) {

    auto boxes = generateBoxes(state.range(0));
    auto scores = generateScores(state.range(0));

    for (auto _ : state) {
        auto result = NMSUtils::nms(boxes, scores, 0.7);
    }
}

static void BM_nms_opencv(benchmark::State& state) {
    auto boxes = generateBoxes(state.range(0));
    auto scores = generateScores(state.range(0));
    std::vector<cv::Rect> rects;
    for(auto& bbox : boxes) {
        rects.push_back(cv::Rect(bbox.x,
                                 bbox.y,
                                 bbox.w,
                                 bbox.h));
    }

    std::vector<int> indices;
    for (auto _ : state) {
        cv::dnn::NMSBoxes(rects, scores, 0.0f, 0.7f, indices);
    }
}

BENCHMARK(BM_nms_real_fast)->RangeMultiplier(10)->Range(10, 100'000)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_nms_opencv)->RangeMultiplier(10)->Range(10, 100'000)->Unit(benchmark::kMicrosecond);

// Run the benchmark
BENCHMARK_MAIN();
