#pragma once

#include <vector>
#include <algorithm>


struct Rect {
    int x; // left
    int y; // bottom
    int w;
    int h;
};

struct BBox {
    Rect box;
    float prob;
};

namespace Utils {

    std::vector<BBox> nms_naive(const std::vector<BBox> &bboxes, float threshold);
    std::vector<BBox> nms_fast(const std::vector<BBox> &bboxes, float threshold);
    std::vector<BBox> nms_real_fast(const std::vector<BBox> &bboxes, float threshold);

} // Utils
