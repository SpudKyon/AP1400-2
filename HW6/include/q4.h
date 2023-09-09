#ifndef Q4_H
#define Q4_H

#include <vector>

namespace q4 {

    struct Vector2D {
        double x{};
        double y{};
    };

    struct Sensor {
        Vector2D pos;
        double accuracy;
    };

    inline Vector2D kalman_filter(const std::vector<Sensor>& sensors) {
        double acc = 0;
        double x = 0, y = 0;
        for (const auto &item: sensors) {
            acc += item.accuracy;
            x += item.pos.x * item.accuracy;
            y += item.pos.y * item.accuracy;
        }
        return Vector2D(x / acc, y / acc);
    }

}

#endif //Q4_H