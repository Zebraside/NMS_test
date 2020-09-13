#include <algorithm>
#include <vector>
#include "nms.h"

namespace Utils {
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

    std::vector<BBox> nms_naive(const std::vector<BBox> &bboxes, float threshold) {
        auto boxes = bboxes;
        std::sort(boxes.begin(), boxes.end(), [](const BBox &left, const BBox &right) {
            return left.prob > right.prob;
        });

        auto it = boxes.begin();
        while (it != boxes.end() && boxes.size() > 1) {
            boxes.erase(std::remove_if(it + 1, boxes.end(), [&it, &threshold](const BBox &box) {
                return Utils::iou(it->box, box.box) >= threshold;
            }), boxes.end());

            ++it;
        }

        return boxes;
    }

    std::vector<int> getIntersections(const std::vector<BBox> &bboxes, const Rect &rect) {
        std::vector<int> intersections(bboxes.size());
        for (size_t i = 0; i < bboxes.size(); ++i) {
            intersections[i] = intersect_area(bboxes[i].box, rect);
        }

        return intersections;
    }

    std::vector<int> getArea(const std::vector<BBox> &bboxes) {
        std::vector<int> area(bboxes.size());
        for (size_t i = 0; i < bboxes.size(); ++i) {
            area[i] = rect_area(bboxes[i].box);
        }

        return area;
    }

    std::vector<float> getIoU(const std::vector<BBox> &bboxes, size_t idx) {
        auto intersections = getIntersections(bboxes, bboxes[idx].box);
        auto area = getArea(bboxes);

        std::vector<float> iou(bboxes.size());
        for (size_t i = 0; i < bboxes.size(); ++i) {
            iou[i] = static_cast<float>(intersections[i]) / (area[idx] + area[i] - intersections[i]);
        }

        return iou;
    }

    std::vector<BBox> nms_fast(const std::vector<BBox> &bboxes, float threshold) {
        if (bboxes.size() < 2)
            return bboxes;

        auto boxes = bboxes;
        std::sort(boxes.begin(), boxes.end(), [](const BBox &left, const BBox &right) {
            return left.prob > right.prob;
        });

        size_t idx = 0;
        for (; idx < boxes.size() - 1; ++idx) {
            std::vector<bool> get(boxes.size(), true);
            auto iou = getIoU(boxes, idx);

            std::vector<BBox> tmp(boxes.begin(), boxes.begin() + idx + 1);
            for (size_t i = idx + 1; i < boxes.size(); ++i) {
                if (iou[i] < threshold)
                    tmp.push_back(boxes[i]);
            }

            boxes = tmp;
        }

        return boxes;
    }

    std::vector<BBox> nms_real_fast(const std::vector<BBox> &bboxes, float threshold) {
        std::vector<std::pair<int, float>> sorted_boxes;
        for (size_t i = 0; i < bboxes.size(); ++i) {
            sorted_boxes.push_back({i, bboxes[i].prob});
        }

        std::stable_sort(sorted_boxes.begin(), sorted_boxes.end(), [](const std::pair<int, float>& left,
                const std::pair<int, float>& right) {
            return left.second > right.second;
        });

        std::vector<BBox> result;
        for (size_t i = 0; i < sorted_boxes.size(); ++i) {
            bool keep = true;

            for (auto& kept : result) {
                if (iou(kept.box, bboxes[sorted_boxes[i].first].box) > threshold) {
                    keep = false;
                    break;
                }
            }

            if (keep) {
                result.push_back(bboxes[sorted_boxes[i].first]);
            }

        }

        return result;
    }
} // Utils
