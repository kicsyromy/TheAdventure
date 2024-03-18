#pragma once

#include "renderer.h"

#include <vector>

class Map
{
public:
    enum class TileType
    {
        None   = 0,
        Grass1 = 1,
        Grass2,
        Grass3,
        Grass4,
    };

    struct Viewport
    {
        float        x;
        float        y;
        std::int32_t w;
        std::int32_t h;
    };

public:
    explicit Map(Renderer &renderer);

    TileType                pos(std::size_t i, std::size_t j) const;
    std::pair<float, float> update(float x_px, float y_px, float speed, float attenuation);
    void                    render();

    float width() const;
    float height() const;

private:
    Renderer &m_renderer;

    std::size_t           m_width;
    std::size_t           m_height;
    std::vector<TileType> m_tiles;

    std::int32_t m_map_image;

    Viewport m_viewport;
};
