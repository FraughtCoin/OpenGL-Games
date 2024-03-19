#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace objects3D {
    Mesh* CreateCubeMesh() {
        glm::vec3 color = glm::vec3(0);

        std::vector<VertexFormat> vertices = {
            VertexFormat(glm::vec3(0.5f, -0.5f, 0.5f), color),
            VertexFormat(glm::vec3(-0.5f, -0.5f, 0.5f), color),
            VertexFormat(glm::vec3(-0.5f, -0.5f, -0.5f), color),
            VertexFormat(glm::vec3(0.5f, -0.5f, -0.5f), color),
            VertexFormat(glm::vec3(0.5f, 0.5f, 0.5f), color),
            VertexFormat(glm::vec3(-0.5f, 0.5f, 0.5f), color),
            VertexFormat(glm::vec3(-0.5f, 0.5f, -0.5f), color),
            VertexFormat(glm::vec3(0.5f, 0.5f, -0.5f), color)
        };

        Mesh* square = new Mesh("cube");

        std::vector<unsigned int> indices = {
            0, 1, 2,
            0, 2, 3,
            0, 1, 4,
            1, 4, 5,
            1, 2, 5,
            2, 5, 6,
            2, 3, 6,
            3, 6, 7,
            0, 4, 3,
            3, 4, 7,
            4, 5, 6,
            4, 6, 7
        };

        square->InitFromData(vertices, indices);
        return square;
    }

    Mesh* CreateCylinderMesh() {
        glm::vec3 color = glm::vec3(0);

        std::vector<VertexFormat> vertices = {};
        float angle = 0;
        int precision = 10;

        for (int i = 0; i < precision; i++) {
            vertices.push_back(VertexFormat(glm::vec3(cos(angle) * 0.5f, 0.5f, sin(angle) * 0.5f), color));
            vertices.push_back(VertexFormat(glm::vec3(cos(angle) * 0.5f, -0.5f, sin(angle) * 0.5f), color));
            angle += 2 * 3.1415f / precision;
        }
        vertices.push_back(VertexFormat(glm::vec3(0, 0.5f, 0), color));
        vertices.push_back(VertexFormat(glm::vec3(0, -0.5f, 0), color));

        std::vector<unsigned int> indices = {};
        for (int i = 0; i < 2 * precision; i++) {
            indices.push_back(i);
            indices.push_back((i + 1) % (2 * precision));
            indices.push_back((i + 2) % (2 * precision));
        }
        for (int i = 0; i < precision; i++) {
            indices.push_back(2 * i);
            indices.push_back(2 * precision);
            indices.push_back(2 * (i + 1) % (2 * precision));

            indices.push_back(2 * i + 1);
            indices.push_back(2 * precision + 1);
            indices.push_back((2 * (i + 1) + 1) % (2 * precision));
        }

        Mesh* cylinder = new Mesh("cylinder");
        cylinder->InitFromData(vertices, indices);
        return cylinder;
    }
}