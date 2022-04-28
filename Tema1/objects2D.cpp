#include "objects2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* objects2D::CreateSquare(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3, 0, 2 };

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* objects2D::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    float width,
    glm::vec3 color,
    bool fill) 
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, width, 0), color),
        VertexFormat(corner + glm::vec3(0, width, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3, };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

Mesh* objects2D::CreateCircle(std::string name, glm::vec3 center, float radius, glm::vec3 color, bool fill) {

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    glm::vec3 point;

    vertices.push_back(VertexFormat(center, color));
    indices.push_back(0);

    int i = 1;
    for (int degrees = 0; degrees <= 360; degrees += 1) {
        point = glm::vec3(center[0] + radius * cos(RADIANS(degrees)), center[1] + radius * sin(RADIANS(degrees)), 0);
        
        vertices.push_back(VertexFormat(point, color));
        indices.push_back(i);

        i++;
    }

    Mesh* circle = new Mesh(name);

    circle->SetDrawMode(GL_TRIANGLE_FAN);

    circle->InitFromData(vertices, indices);
    return circle;
}
