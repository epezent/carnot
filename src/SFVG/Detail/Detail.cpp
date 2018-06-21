#include <SFVG/Detail/Detail.hpp>
#include <algorithm>

namespace sfvg {

float polygonArea(const std::vector<sf::Vector2f> vertices) {
    // float area = 0.0f;
    // for (int p = vertices.size() - 1, q = 0; q < vertices.size(); p = q++) {
    //     area += vertices[p].x * vertices[q].y - vertices[q].x *
    //     vertices[p].y;
    // }
    // return 0.5f * area;
    float area = 0.0f;
    std::size_t size   = vertices.size();
    for (std::size_t i = 1; i < size - 1; ++i)
        area += vertices[i].x * (vertices[i + 1].y - vertices[i - 1].y);
    area += vertices[size - 1].x * (vertices[0].y - vertices[size - 2].y);
    area += vertices[0].x * (vertices[1].y - vertices[size - 1].y);
    return area * 0.5f;
}

bool insideTriangle(const sf::Vector2f& A,
                    const sf::Vector2f& B,
                    const sf::Vector2f& C,
                    const sf::Vector2f& P) {
    // return ((((C.x - B.x) * (P.y - B.y) - (C.y - B.y) * (P.x - B.x)) >= 0.0f)
    // &&
    //         (((B.x - A.x) * (P.y - A.y) - (B.y - A.y) * (P.x - A.x)) >= 0.0f)
    //         &&
    //         (((A.x - C.x) * (P.y - C.y) - (A.y - C.y) * (P.x - C.x)) >=
    //         0.0f));
    float s = A.y * C.x - A.x * C.y + (C.y - A.y) * P.x + (A.x - C.x) * P.y;
    float t = A.x * B.y - A.y * B.x + (A.y - B.y) * P.x + (B.x - A.x) * P.y;

    if ((s < 0) != (t < 0))
        return false;

    float area = -B.y * C.x + A.y * (C.x - B.x) + A.x * (B.y - C.y) + B.x * C.y;
    if (area < 0.0) {
        s    = -s;
        t    = -t;
        area = -area;
    }
    return s > 0 && t > 0 && (s + t) <= area;
}

bool insideLine(const sf::Vector2f& L1,
                const sf::Vector2f& L2,
                const sf::Vector2f& P) {
    float crossproduct =
        (P.y - L1.y) * (L2.x - L1.x) - (P.x - L1.x) * (L2.y - L1.y);
    if (std::abs(crossproduct) > 0.1)
        return false;

    float dotproduct =
        (P.x - L1.x) * (L2.x - L1.x) + (P.y - L1.y) * (L2.y - L1.y);
    if (dotproduct < 0.0f)
        return false;

    float squarelength =
        (L2.x - L1.x) * (L2.x - L1.x) + (L2.y - L1.y) * (L2.y - L1.y);
    if (dotproduct > squarelength)
        return false;

    return true;
}

std::vector<float> linspace(float a, float b, std::size_t N) {
    std::vector<float> linspace_out(N);
    float delta     = (b - a) / (N - 1);
    linspace_out[0] = a;
    for (std::size_t i = 1; i < N - 1; i++)
        linspace_out[i] = linspace_out[i - 1] + delta;
    linspace_out[N - 1] = b;
    return linspace_out;
}

bool generateVertices(const std::vector<sf::Vector2f>& points,
                      const std::vector<float>& radii,
                      const std::vector<std::size_t>& smoothness,
                      std::vector<sf::Vector2f>& vertices) {
    std::size_t j = 0;
    for (std::size_t i = 0; i < points.size(); ++i) {
        if (radii[i] > 0.0f && smoothness[i] > 1) {
            sf::Vector2f a, b, c;
            if (i == 0) {
                a.x = points[points.size() - 1].x;
                a.y = points[points.size() - 1].y;
                b.x = points[i].x;
                b.y = points[i].y;
                c.x = points[i + 1].x;
                c.y = points[i + 1].y;
            } else if (i == points.size() - 1) {
                a.x = points[i - 1].x;
                a.y = points[i - 1].y;
                b.x = points[i].x;
                b.y = points[i].y;
                c.x = points[0].x;
                c.y = points[0].y;
            } else {
                a.x = points[i - 1].x;
                a.y = points[i - 1].y;
                b.x = points[i].x;
                b.y = points[i].y;
                c.x = points[i + 1].x;
                c.y = points[i + 1].y;
            }
            std::vector<sf::Vector2f> corner(smoothness[i]);
            if (roundCorner(a, b, c, radii[i], corner)) {
                for (std::size_t k = 0; k < corner.size(); ++k) {
                    vertices[j++] = corner[k];
                }
            } else
                return false;
        } else {
            vertices[j++] = points[i];
        }
    }
    return true;
}

bool roundCorner(sf::Vector2f& A,
                 sf::Vector2f& B,
                 sf::Vector2f& C,
                 float r,
                 std::vector<sf::Vector2f>& out) {
    // Find directional vectors of line segments
    sf::Vector2f V1 = B - A;
    sf::Vector2f V2 = B - C;

    // Check if corner radius is longer than vectors
    if (r >= magnitude(V1) || r >= magnitude(V2)) {
        return false;  // set corner equal to B
    }

    // Find unit vectors
    sf::Vector2f U1 = V1 / magnitude(V1);
    sf::Vector2f U2 = V2 / magnitude(V2);

    // Find normal vectors
    sf::Vector2f N1, N2;
    N1.x = -U1.y;
    N1.y = U1.x;
    N2.x = -U2.y;
    N2.y = U2.x;

    // Check/fix direction of normal vector
    if (dot(N1, -V2) < 0.0f)
        N1 = -N1;
    if (dot(N2, -V1) < 0.0f)
        N2 = -N2;

    // Find end-points of offset lines
    sf::Vector2f O11 = A + N1 * r;
    sf::Vector2f O10 = B + N1 * r;
    sf::Vector2f O22 = C + N2 * r;
    sf::Vector2f O20 = B + N2 * r;

    // Find intersection point of offset lines
    sf::Vector2f I = intersection(O11, O10, O22, O20);

    // Find tangent points
    sf::Vector2f T1 = I + -N1 * r;
    sf::Vector2f T2 = I + -N2 * r;

    // Check if tangent points are on line segments
    // TODO

    float angle1 = std::atan2(T1.y - I.y, T1.x - I.x);
    float angle2 = std::atan2(T2.y - I.y, T2.x - I.x);
    std::vector<float> angles;
    if (std::abs(angle1 - angle2) < PI)
        angles = linspace(angle1, angle2, out.size());
    else
        angles = linspace(wrapTo2Pi(angle1), wrapTo2Pi(angle2), out.size());

    for (std::size_t i = 0; i < out.size(); i++) {
        out[i].x = r * std::cos(angles[i]) + I.x;
        out[i].y = r * std::sin(angles[i]) + I.y;
    }

    return true;
}

bool snip(const std::vector<sf::Vector2f>& vertices,
          int u,
          int v,
          int w,
          int n,
          std::vector<int>& V) {
    int p;

    sf::Vector2f A(vertices[V[u]].x, vertices[V[u]].y);
    sf::Vector2f B(vertices[V[v]].x, vertices[V[v]].y);
    sf::Vector2f C(vertices[V[w]].x, vertices[V[w]].y);

    if (
#ifdef _MSC_VER
        FLT_EPSILON >
#else
        __FLT_EPSILON__ >
#endif
        (((B.x - A.x) * (C.y - A.y)) - ((B.y - A.y) * (C.x - A.x)))) {
        return 0;
    }
    for (p = 0; p < n; p++) {
        if ((p == u) || (p == v) || (p == w)) {
            continue;
        }
        sf::Vector2f P(vertices[V[p]].x, vertices[V[p]].y);
        if (insideTriangle(A, B, C, P)) {
            return false;
        }
    }
    return true;
}

bool triangulate(const std::vector<sf::Vector2f>& vertices,
                 std::vector<std::size_t>& indices_out) {
    std::size_t vertices_size = vertices.size();

    if (vertices_size < 3)
        return false;

    std::vector<int> V(vertices_size, 0);
    if (polygonArea(vertices) > 0) {
        for (std::size_t v = 0; v < vertices_size; v++)
            V[v] = v;
    } else {
        for (std::size_t v = 0; v < vertices_size; v++)
            V[v] = (vertices_size - 1) - v;
    }

    int nv    = vertices_size;
    int count = 2 * nv;
    int i     = 0;
    for (int m = 0, v = nv - 1; nv > 2;) {
        if ((count--) <= 0) {
            return false;
        }

        int u = v;
        if (nv <= u)
            u = 0;
        v = u + 1;
        if (nv <= v)
            v = 0;
        int w = v + 1;
        if (nv <= w)
            w = 0;

        if (snip(vertices, u, v, w, nv, V)) {
            int a, b, c, s, t;
            a = V[u];
            b = V[v];
            c = V[w];

            indices_out[i++] = a;
            indices_out[i++] = b;
            indices_out[i++] = c;
            m++;
            for (s = v, t = v + 1; t < nv; s++, t++)
                V[s] = V[t];
            nv--;
            count = 2 * nv;
        }
    }

    // std::reverse(indices_out.begin(), indices_out.end());
    return true;
}

} // namespace sfvg

