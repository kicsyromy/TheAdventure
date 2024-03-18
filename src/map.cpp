#include "map.h"

#include "resources.h"

#include <array>
#include <cstdint>
#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <vector>

static constexpr std::int32_t TILE_SIZE = 16;

Map::Map(Renderer &renderer)
  : m_renderer{ renderer }
  , m_viewport({ 0, 0, renderer.width(), renderer.height() })
{
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
                    m_width        = std::stoi(std::string{ width_val }) * TILE_SIZE;
                    m_viewport.w   = std::min(static_cast<int>(m_width), m_viewport.w);
                }

                if (str == "heigh")
                {
                    auto height_val = std::string_view{ line.c_str() + i + 8 };
                    m_height        = std::stoi(std::string{ height_val }) * TILE_SIZE;
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

    auto map_img_file = std::ifstream{ "dist/map01.png", std::ios::binary };
    if (!map_img_file.is_open())
    {
        std::cerr << "Failed to open dist/map01.png" << '\n';
        std::terminate();
    }

    auto map_img_data = std::vector<std::uint8_t>{ std::istreambuf_iterator<char>{ map_img_file },
                                                   std::istreambuf_iterator<char>{} };
    std::int32_t map_img_width;
    std::int32_t map_img_height;
    m_map_background = m_renderer.load_image(map_img_data.data(),
                                             map_img_data.size(),
                                             &map_img_width,
                                             &map_img_height);

    if (map_img_width != m_width || map_img_height != m_height)
    {
        std::cerr << ".tmx file and .png have inconsistent sizes!"
                  << "TMX: W: " << m_width << " H: " << m_height << " PNG: W: " << map_img_width
                  << " H: " << map_img_height << '\n';
        std::terminate();
    }
}

Map::TileType Map::pos(std::size_t i, std::size_t j) const
{
    return m_tiles[(i / TILE_SIZE) * (m_width / TILE_SIZE) + (j / TILE_SIZE)];
}

std::pair<float, float> Map::update(float hero_x, float hero_y, float speed, float attenuation)
{
    static constexpr int SCROLL_THRESHOLD = 100;

    const auto window_width  = m_renderer.width();
    const auto window_height = m_renderer.height();

    float offset_x = 0.F;
    float offset_y = 0.F;

    if ((hero_x > window_width - SCROLL_THRESHOLD) && (m_viewport.x + m_viewport.w < m_width))
    {
        m_viewport.x += speed * attenuation;
        offset_x = -speed * attenuation;
    }
    else if ((hero_x < SCROLL_THRESHOLD) && (m_viewport.x > 0))
    {
        m_viewport.x -= speed * attenuation;
        offset_x = speed * attenuation;
    }

    if ((hero_y > window_height - SCROLL_THRESHOLD) && (m_viewport.y + m_viewport.h < m_height))
    {
        m_viewport.y += speed * attenuation;
        offset_y = -speed * attenuation;
    }
    else if ((hero_y < SCROLL_THRESHOLD) && (m_viewport.y > 0))
    {
        m_viewport.y -= speed * attenuation;
        offset_y = speed * attenuation;
    }

    return { offset_x, offset_y };
}

void Map::render()
{
    m_renderer.draw_image(m_map_background,
                          static_cast<std::int32_t>(m_viewport.x),
                          static_cast<std::int32_t>(m_viewport.y),
                          0,
                          0,
                          false,
                          false,
                          m_viewport.w,
                          m_viewport.h,
                          m_renderer.width(),
                          m_renderer.height());
}

float Map::width() const
{
    return static_cast<float>(m_width);
}

float Map::height() const
{
    return static_cast<float>(m_height);
}
