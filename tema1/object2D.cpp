#include "lab_m1/tema1/object2D.h"

#define PI 3.1415926535897932384626433832795

Mesh* object2D::CreateRectangle(const std::string& name, glm::vec3 leftBottomCorner, float length, float width, glm::vec3 color, bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(width, 0, 0), color),
        VertexFormat(corner + glm::vec3(width, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

Mesh* object2D::CreateSquare(const std::string& name, glm::vec3 center, float length, glm::vec3 color, bool fill)
{
    glm::vec3 corner = center - glm::vec3(length / 2, length / 2, 0);

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateCircle(const std::string& name, GLfloat radius, glm::vec3 center, glm::vec3 color, bool fill)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    GLfloat TWO_PI = 2.0f * PI;
    int num_of_triangles = 50;
   
    for (int i = 0; i <= num_of_triangles; i++) {
        GLfloat theta = i * TWO_PI / num_of_triangles;

        vertices.push_back(VertexFormat(center + glm::vec3(radius * cos(theta), 
                                                           radius * sin(theta), 
                                                           0), color));
        indices.push_back(i);
    }

    Mesh* circle = new Mesh(name);
    
    if (fill) {
        circle->SetDrawMode(GL_TRIANGLE_FAN);
    }
    else {
        circle->SetDrawMode(GL_LINE_LOOP);
    }

    circle->InitFromData(vertices, indices);

    return circle;
}

