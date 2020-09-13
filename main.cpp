#include <random>
#include <functional>

#include <benchmark/benchmark.h>
#include <opencv2/dnn/dnn.hpp>

#include "nms.h"

std::vector<BBox> generateBBox(size_t size) {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> box_dist(0,1000);
    std::normal_distribution<float> prob_dist(0.7,0.3);
    auto box_param = std::bind ( box_dist, generator );
    auto prob_param = std::bind ( prob_dist, generator );
    std::vector<BBox> bboxes;
    for (size_t i = 0; i < size; ++i) {
        bboxes.push_back({{box_param(), box_param(), box_param(), box_param()}, prob_param()});
    }

    return bboxes;
}

static void BM_nms_naive(benchmark::State& state) {

    auto bboxes = generateBBox(state.range(0));

    for (auto _ : state) {
        auto result = Utils::nms_naive(bboxes, 0.7f);
    }
}

static void BM_nms_fast(benchmark::State& state) {

    auto bboxes = generateBBox(state.range(0));

    for (auto _ : state) {
        auto result =  Utils::nms_fast(bboxes, 0.7f);
    }
}

static void BM_nms_real_fast(benchmark::State& state) {

    auto bboxes = generateBBox(state.range(0));

    for (auto _ : state) {
        auto result =  Utils::nms_real_fast(bboxes, 0.7f);
    }
}

static void BM_nms_opencv(benchmark::State& state) {
    auto bboxs = generateBBox(state.range(0));

    std::vector<cv::Rect> boxes;
    std::vector<float> scores;
    for(auto& bbox : bboxs) {
        boxes.push_back(cv::Rect(bbox.box.x,
                                 bbox.box.y,
                                 bbox.box.w,
                                 bbox.box.h));

        scores.push_back(bbox.prob);
    }

    std::vector<int> indices;
    for (auto _ : state) {
        cv::dnn::NMSBoxes(boxes, scores, 0.0f, 0.7f, indices);
    }
}

BENCHMARK(BM_nms_naive)->RangeMultiplier(10)->Range(10, 100'000)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_nms_fast)->RangeMultiplier(10)->Range(10, 100'000)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_nms_real_fast)->RangeMultiplier(10)->Range(10, 100'000)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_nms_opencv)->RangeMultiplier(10)->Range(10, 100'000)->Unit(benchmark::kMicrosecond);

// Run the benchmark
BENCHMARK_MAIN();
