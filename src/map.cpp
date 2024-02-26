#include "map.h"

#include "resources.h"

#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

static constexpr std::int32_t TILE_SRC_SIZE = 16;
static constexpr std::int32_t TILE_DST_SIZE = 16 * 2;

Map::Map(Renderer &renderer)
  : m_renderer{ renderer }
{
    m_images.push_back(m_renderer.load_image(resource_grass_001, resource_grass_001_size));
    m_images.push_back(m_renderer.load_image(resource_grass_002, resource_grass_002_size));
    m_images.push_back(m_renderer.load_image(resource_grass_003, resource_grass_003_size));
    m_images.push_back(m_renderer.load_image(resource_grass_004, resource_grass_004_size));

    auto map_file = std::ifstream{ "dist/map01.tmx" };
    if (!map_file.is_open())
    {
        std::cerr << "Failed to open dist/map01.tmx" << '\n';
        std::terminate();
    }

    std::string line{ "hello world" };
    std::string tile_data;
    do
    {
        std::getline(map_file, line);

        auto line_start = std::string_view{ line.c_str(), 7 };
        if (line_start == " <layer")
        {
            for (std::size_t i = 0; i < line.size(); ++i)
            {
                auto str = std::string_view{ line.c_str() + i, 5 };
                if (str == "width")
                {
                    auto width_val = std::string_view{ line.c_str() + i + 7 };
                    m_width        = std::stoi(std::string{ width_val });
                }

                if (str == "heigh")
                {
                    auto height_val = std::string_view{ line.c_str() + i + 8 };
                    m_height        = std::stoi(std::string{ height_val });
                }
            }
        }

        if (line_start == "  <data")
        {
            for (int i = 0; i < m_height; ++i)
            {
                std::getline(map_file, line);
                tile_data += line;
            }

            break;
        }
    } while (!map_file.eof());

    std::cout << "width: " << m_width << " height: " << m_height << '\n'
              << "tile data: " << tile_data << '\n';

    m_tiles.resize(m_width * m_height, TileType::None);
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
                m_renderer.draw_image(m_images[0],
                                      0,
                                      0,
                                      j * TILE_DST_SIZE,
                                      i * TILE_DST_SIZE,
                                      false,
                                      false,
                                      TILE_SRC_SIZE,
                                      TILE_SRC_SIZE,
                                      TILE_DST_SIZE,
                                      TILE_DST_SIZE);
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
