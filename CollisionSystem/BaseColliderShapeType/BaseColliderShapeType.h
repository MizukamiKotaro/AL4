#pragma once
#include <vector>
#include "Utils/Math/Vector2.h"
#include "Utils/Math/Vector3.h"
#include <memory>

class BaseColliderShapeType {
public:
    enum class ShapeType
    {
        UNKNOWN = 0,
        BOX2D,
        MAPCHIP2D,
        CIRCLE,

    };

    struct MAPCHIP2D
    {
        //std::vector<std::vector<std::unique_ptr<BaseColliderShapeType>>> shapes_;
        std::vector<std::vector<int>> mapChip_;

        std::vector<int> noRigitBody_;
        std::vector<int> noCollider_;
    };

    ShapeType GetShapeType() const;

public:

    ShapeType shapeType_;

    MAPCHIP2D mapChip2D_;

    std::vector<int> iInfo_;

    std::vector<float> fInfo_;

    std::vector<Vector2> v2Info_;

    std::vector<Vector3> v3Info_;


};