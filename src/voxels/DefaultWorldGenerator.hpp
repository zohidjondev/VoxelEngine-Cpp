#pragma once

#include "typedefs.hpp"
#include "WorldGenerator.hpp"

struct voxel;
class Content;

class DefaultWorldGenerator : WorldGenerator {
public:
    DefaultWorldGenerator(const Content* content) : WorldGenerator(content) {
    }

    void generate(voxel* voxels, int x, int z, int seed);
};
