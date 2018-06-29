#ifndef SFVG_STARSHAPE_HPP
#define SFVG_STARSHAPE_HPP

#include <SFVG/Graphics/Shape.hpp>

namespace sfvg {

class StarShape : public Shape {
public:
    StarShape(std::size_t starPointCount, float starRadiusA, float starRadiusB);

    void setStarPointCount(std::size_t starPointCount);

    std::size_t getStarPointCount() const;

    void setStarRadiusA(float starRadiusA);

    float getStarRadiusA() const;

    void setStarRadiusB(float starRadiusB);

    float getStarRadiusB() const;

private:

    void updateStarShape();

private:
    std::size_t m_starPointCount;
    float m_starRadiusA;
    float m_starRadiusB;
};

} // namespace sfvg

#endif // SFVG_STARSHAPE_HPP
