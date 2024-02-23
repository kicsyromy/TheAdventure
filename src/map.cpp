#include "map.h"

#include "resources.h"

Map::Map(Renderer &renderer, std::size_t width, std::size_t height)
  : m_renderer{ renderer }
  , m_width{ width }
  , m_height{ height }
{
    m_tiles.resize(m_width * m_height, TileType::Grass1);

    m_images.push_back(m_renderer.load_image(resource_grass_001, resource_grass_001_size));
}

Map::TileType Map::pos(std::size_t i, std::size_t j) const
{
    return m_tiles[i * m_width + j];
}

void Map::render()
{
    for (std::size_t i = 0; i < m_height; ++i)
    {
        for (std::size_t j = 0; j < m_width; ++j)
        {
            switch (pos(i, j))
            {
            case TileType::Grass1:
                m_renderer
                    .draw_image(m_images[0], 0, 0, j * 16, i * 16, false, false, 16, 16, 16, 16);
                break;
            case TileType::Grass2:
                break;
            case TileType::Grass3:
                break;
            case TileType::Grass4:
                break;
            }
        }
    }
}
