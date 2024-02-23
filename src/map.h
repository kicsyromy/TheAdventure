#pragma once
#include "renderer.h"

#include <vector>

class Map
{
public:
    enum class TileType
    {
        Grass1 = 0,
        Grass2,
        Grass3,
        Grass4,
    };

public:
    explicit Map(Renderer &renderer, std::size_t width, std::size_t height);

    TileType pos(std::size_t i, std::size_t j) const;
    void     render();

private:
    Renderer &m_renderer;

    std::size_t           m_width;
    std::size_t           m_height;
    std::vector<TileType> m_tiles;

    std::vector<std::int32_t> m_images;
};
