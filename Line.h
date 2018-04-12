#pragma once

#include <glm/vec3.hpp>
#include <experimental/optional>

struct Line {
    const glm::vec3 start{};
    const glm::vec3 end{};
    const glm::vec3 direction{};
    const float magnitude;

    Line(const glm::vec3 &start, const glm::vec3 &end)
            : start(start),
              end(end),
              direction(end - start),
              magnitude(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z) {};
    static float perp(vec3 u, vec3 v){
        return u.x * v.y - u.y * v.x;
    }
public:
    static std::experimental::optional<vec3> intersectLines(Line a, Line b) {
        vec3 u = a.direction;
        vec3 v = b.direction;
        vec3 w = a.start - b.start;
        float d = perp(u, v);

        float sI = perp(v, w) / d;
        if (sI < 0 || sI > 1)
            return {};

        float tI = perp(u, w) / d;
        if (tI < 0 || tI > 1)
            return {};

        return a.start + sI * u;
    }

    virtual ~Line() = default;
};