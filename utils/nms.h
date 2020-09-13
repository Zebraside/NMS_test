#pragma once

#include <vector>
#include <algorithm>

/*
    NOTE: (x,y) - coordinates of left bottom corner of the rect. w, h - width and height respectively
    /------\
    |      |
    X------|

*/
struct Rect {
    int x;
    int y;
    int w;
    int h;
};

namespace NMSUtils {

// nms - performs non-maximum suppression operation
// boxes - bounding boxes
// scores - corresponding scores
// threshold - suppress box with lower score if intersection over union between two boxes > threshold
// returns indices of non suppressed bounding boxes
std::vector<size_t> nms(const std::vector<Rect> &boxes, const std::vector<float>& scores, float threshold);

} // NMSUtils
