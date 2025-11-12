#include "SelectionMask.h"
#include <algorithm>

void SelectionMask::Clear() {
    if (hasMask) {
        UnloadImage(maskImage);
        hasMask = false;
    }
    rect = {0, 0, 0, 0};
    lassoPoints.clear();
    type = Selection_None;
}

// prostokątne zaznaczenie
void SelectionMask::CreateFromRect(const Rectangle& r, Vector2 canvasSize) {
    Clear();
    type = Selection_Rectangle;
    rect = r;

    maskImage = GenImageColor((int)canvasSize.x, (int)canvasSize.y, BLANK);

    int x0 = std::max(0, (int)rect.x);
    int y0 = std::max(0, (int)rect.y);
    int x1 = std::min((int)canvasSize.x, (int)(rect.x + rect.width));
    int y1 = std::min((int)canvasSize.y, (int)(rect.y + rect.height));

    for (int y = y0; y < y1; y++) {
        for (int x = x0; x < x1; x++) {
            ImageDrawPixel(&maskImage, x, y, WHITE);
        }
    }
    hasMask = true;
}

// lasso — dowolny wielokąt
void SelectionMask::CreateFromLasso(const std::vector<Vector2>& points, Vector2 canvasSize) {
    Clear();
    if (points.size() < 3) return;

    type = Selection_Lasso;
    lassoPoints = points;
    maskImage = GenImageColor((int)canvasSize.x, (int)canvasSize.y, BLANK);

    // znajdź bounding box
    float minX = canvasSize.x, maxX = 0, minY = canvasSize.y, maxY = 0;
    for (auto& p : points) {
        if (p.x < minX) minX = p.x;
        if (p.x > maxX) maxX = p.x;
        if (p.y < minY) minY = p.y;
        if (p.y > maxY) maxY = p.y;
    }

    // wypełnij wnętrze (algorytm odd-even)
    for (int y = (int)minY; y <= (int)maxY; y++) {
        for (int x = (int)minX; x <= (int)maxX; x++) {
            Vector2 p = {(float)x, (float)y};
            bool inside = false;

            for (size_t i = 0, j = points.size() - 1; i < points.size(); j = i++) {
                const Vector2& pi = points[i];
                const Vector2& pj = points[j];
                if (((pi.y > p.y) != (pj.y > p.y)) &&
                    (p.x < (pj.x - pi.x) * (p.y - pi.y) / (pj.y - pi.y) + pi.x)) {
                    inside = !inside;
                }
            }

            if (inside)
                ImageDrawPixel(&maskImage, x, y, WHITE);
        }
    }

    hasMask = true;
}

// testowanie pojedynczego punktu
bool SelectionMask::Contains(Vector2 point) const {
    if (!hasMask) return false;

    if (type == Selection_Rectangle) {
        return CheckCollisionPointRec(point, rect);
    }

    if (type == Selection_Lasso) {
        // sprawdzamy czy piksel w masce jest biały
        Color c = GetImageColor(maskImage, (int)point.x, (int)point.y);
        return c.a > 0;
    }

    return false;
}
