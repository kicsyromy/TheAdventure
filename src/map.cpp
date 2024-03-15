#include "map.h"

#include "resources.h"

#include <array>
#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

static constexpr std::int32_t TILE_SRC_SIZE = 16;
static constexpr std::int32_t TILE_DST_SIZE = 16;

Map::Map(Renderer &renderer)
  : m_renderer{ renderer }
  , m_viewport({ 0, 0, renderer.width() / TILE_SRC_SIZE, renderer.height() / TILE_SRC_SIZE })
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

    std::string line;
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
                    m_viewport.w   = std::min(static_cast<int>(m_width), m_viewport.w);
                }

                if (str == "heigh")
                {
                    auto height_val = std::string_view{ line.c_str() + i + 8 };
                    m_height        = std::stoi(std::string{ height_val });
                    m_viewport.h    = std::min(static_cast<int>(m_height), m_viewport.h);
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

    auto *token = std::strtok(tile_data.data(), ",");
    while (token != nullptr)
    {
        const auto tok = std::string_view{ token };
        if (tok == "1")
        {
            m_tiles.push_back(TileType::Grass1);
        }
        else if (tok == "2")
        {
            m_tiles.push_back(TileType::Grass2);
        }
        else if (tok == "3")
        {
            m_tiles.push_back(TileType::Grass3);
        }
        else if (tok == "4")
        {
            m_tiles.push_back(TileType::Grass4);
        }
        else
        {
            m_tiles.push_back(TileType::None);
        }

        token = std::strtok(nullptr, ",");
    }
}

Map::TileType Map::pos(std::size_t i, std::size_t j) const
{
    return m_tiles[i * m_width + j];
}

std::pair<float, float> Map::update(float hero_x, float hero_y)
{
    std::cerr << "X: " << hero_x << " Y: " << hero_y << '\n';
    static constexpr int SCROLL_THRESHOLD = 100;

    const auto window_width  = m_renderer.width();
    const auto window_height = m_renderer.height();

    float offset_x = 0.F;
    float offset_y = 0.F;

    if ((hero_x > window_width - SCROLL_THRESHOLD) && (m_viewport.x + m_viewport.w < m_width))
    {
        m_viewport.x += 1;
        offset_x = static_cast<float>(-TILE_SRC_SIZE);
    }

    if ((hero_x < SCROLL_THRESHOLD) && (m_viewport.x > 0))
    {
        m_viewport.x -= 1;
        offset_x = static_cast<float>(TILE_SRC_SIZE);
    }

    if ((hero_y > window_height - SCROLL_THRESHOLD) && (m_viewport.y + m_viewport.h < m_height))
    {
        m_viewport.y += 1;
        offset_y = static_cast<float>(-TILE_SRC_SIZE);
    }

    if ((hero_y < SCROLL_THRESHOLD) && (m_viewport.y > 0))
    {
        m_viewport.y -= 1;
        offset_y = static_cast<float>(TILE_SRC_SIZE);
    }

    return { offset_x, offset_y };
}

void Map::render()
{
    for (std::size_t i = m_viewport.y; i < m_viewport.h + m_viewport.y; ++i)
    {
        for (std::size_t j = m_viewport.x; j < m_viewport.w + m_viewport.x; ++j)
        {
            const auto tile_type = pos(i, j);
            m_renderer.draw_image(m_images[static_cast<std::size_t>(tile_type) - 1],
                                  0,
                                  0,
                                  (j - m_viewport.x) * TILE_DST_SIZE,
                                  (i - m_viewport.y) * TILE_DST_SIZE,
                                  false,
                                  false,
                                  TILE_SRC_SIZE,
                                  TILE_SRC_SIZE,
                                  TILE_DST_SIZE,
                                  TILE_DST_SIZE);
        }
    }
}

float Map::width() const
{
    return static_cast<float>(m_width * TILE_SRC_SIZE);
}

float Map::height() const
{
    return static_cast<float>(m_height * TILE_SRC_SIZE);
}
