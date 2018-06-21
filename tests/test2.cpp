#include <vector>
#include <iostream>
#include <algorithm>

int main(int argc, char* argv[]) {

    // [a b][c d e f g][h i j]
    // [2  ][5        ][3    ]
    // [2  ][7        ][10   ]
    // [0  ][1        ][2    ]
    // [0 1][0 1 2 3 4][0 1 2]

    std::vector<char> v1{'a','b'};
    std::vector<char> v2{'c','d','e','f','g'};
    std::vector<char> v3{'h','i','j'};

    std::vector<std::vector<char>> polygon;
    polygon.push_back(v1);
    polygon.push_back(v2);
    polygon.push_back(v3);

    std::vector<std::size_t> sums(polygon.size() + 1);
    sums[0] = 0;
    for (std::size_t i = 1; i < polygon.size() + 1; ++i)
        sums[i] = sums[i-1] + polygon[i-1].size();

    // sums = [0 2 7 10]

    for (std::size_t i = 0; i < 10; ++i) {
        std::size_t index = 0;
        for (; index < sums.size(); ++index) {
            if (i < sums[index+1])
                break;
        }
        std::cout << index << " ";
        std::size_t subIndex = i - sums[index];
        //std::cout << index << " " << subIndex << std::endl;
        std::cout << i << " " << polygon[index][subIndex] << std::endl;
    }

    return 0;
}
