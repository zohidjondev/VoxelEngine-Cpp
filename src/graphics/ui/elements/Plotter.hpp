#pragma once

#include "UINode.hpp"
#include "typedefs.hpp"

#include <memory>

class Assets;
class DrawContext;

namespace gui {
    class Plotter : public gui::UINode {
        std::unique_ptr<int[]> points;
        float multiplier;
        int index = 0;
        int dmwidth;
        int dmheight;
        int labelsInterval;
    public:
        Plotter(uint width, uint height, float multiplier, int labelsInterval) 
        : gui::UINode(glm::vec2(width, height)), 
            multiplier(multiplier),
            dmwidth(width-50),
            dmheight(height),
            labelsInterval(labelsInterval)
        {
            points = std::make_unique<int[]>(dmwidth);
        }

        void act(float delta) override;
        void draw(const DrawContext& pctx, const Assets& assets) override;
    };
}
