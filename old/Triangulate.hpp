#ifndef EE_DETAIL_HPP
#define EE_DETAIL_HPP

#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <vector>

namespace ee {

bool generateVertices(const std::vector<sf::Vector2f>& points,
                      const std::vector<float>& radii,
                      const std::vector<std::size_t>& smoothness,
                      std::vector<sf::Vector2f>& vertices);

bool roundCorner(sf::Vector2f& A,
                 sf::Vector2f& B,
                 sf::Vector2f& C,
                 float r,
                 std::vector<sf::Vector2f>& corner);

bool snip(const std::vector<sf::Vector2f>& vertices,
          int u,
          int v,
          int w,
          int n,
          std::vector<int>& V);

bool triangulate(const std::vector<sf::Vector2f>& vertices,
                 std::vector<std::size_t>& indices_out);

} // namespace ee

#endif  // EE_DETAIL_HPP
