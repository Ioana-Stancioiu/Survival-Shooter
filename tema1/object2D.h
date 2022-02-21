#pragma once

#include <string>
#include <vector>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"
#include "core/engine.h"
#include "utils/gl_utils.h"


namespace object2D
{
    Mesh* CreateRectangle(const std::string& name, glm::vec3 leftBottomCorner, float length, float width, glm::vec3 color, bool fill = false);

    Mesh* CreateSquare(const std::string& name, glm::vec3 center, float length, glm::vec3 color, bool fill = false);

    Mesh* CreateCircle(const std::string& name, GLfloat radius, glm::vec3 center, glm::vec3 color, bool fill = true);
}
