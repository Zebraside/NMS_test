#include <algorithm>
#include <vector>
#include <cassert>
#include "nms.h"

namespace NMSUtils {
    int intersect_area(const Rect &a, const Rect &b) {
        int minX = std::max(a.x, b.x);
        int minY = std::max(a.y, b.y);
        int maxX = std::min(a.x + a.w, b.x + b.w);
        int maxY = std::min(a.y + a.h, b.y + b.h);

        return std::max(0, maxX - minX) * std::max(0, maxY - minY);
    }

    static inline int rect_area(const Rect& a) {
        return a.w * a.h;
    }

    float iou(const Rect &a, const Rect &b) {
        float inersect_area = intersect_area(a, b);
        return inersect_area / (rect_area(a) + rect_area(b) - inersect_area);
    }

    std::vector<size_t> nms(const std::vector<Rect> &boxes, const std::vector<float>& scores, float threshold) {
        assert(boxes.size() == scores.size());

        std::vector<std::pair<int, float>> sorted_boxes;
        for (size_t i = 0; i < scores.size(); ++i) {
            sorted_boxes.push_back({i, scores[i]});
        }

        std::sort(sorted_boxes.begin(), sorted_boxes.end(), [](const std::pair<int, float>& left,
                const std::pair<int, float>& right) {
            return left.second > right.second;
        });

        std::vector<size_t> indices;
        for (size_t i = 0; i < sorted_boxes.size(); ++i) {
            bool keep = true;
            auto current_idx = sorted_boxes[i].first;
            for (auto& kept : indices) {
                if (iou(boxes[kept], boxes[current_idx]) > threshold) {
                    keep = false;
                    break;
                }
            }

            if (keep) {
                indices.push_back(current_idx);
            }

        }

        return indices;
    }
} // NMSUtils
