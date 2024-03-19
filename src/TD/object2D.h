#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{
    Mesh* CreateSquareMesh(const std::string& name) {
        glm::vec3 color = glm::vec3(0, 0, 0);

        std::vector<VertexFormat> vertices = {
            VertexFormat(glm::vec3(-0.5f, -0.5f, 0), color),
            VertexFormat(glm::vec3(0.5f, -0.5f, 0), color),
            VertexFormat(glm::vec3(0.5f, 0.5f, 0), color),
            VertexFormat(glm::vec3(-0.5f, 0.5f, 0), color)
        };

        Mesh* square = new Mesh(name);

        std::vector<unsigned int> indices = {
            0, 1, 2,
            0, 2, 3
        };

        square->InitFromData(vertices, indices);
        return square;
    }

    Mesh* CreateEmptySquareMesh(const std::string& name) {
        glm::vec3 color = glm::vec3(0, 0, 0);

        std::vector<VertexFormat> vertices = {
            VertexFormat(glm::vec3(-0.5f, -0.5f, 0), color),
            VertexFormat(glm::vec3(0.5f, -0.5f, 0), color),
            VertexFormat(glm::vec3(0.5f, 0.5f, 0), color),
            VertexFormat(glm::vec3(-0.5f, 0.5f, 0), color)
        };

        Mesh* square = new Mesh(name);

        std::vector<unsigned int> indices = {
            0, 1, 2, 3
        };

        square->SetDrawMode(GL_LINE_LOOP);
        square->InitFromData(vertices, indices);
        return square;
    }

    Mesh* CreateStarMesh(const std::string& name) {
        glm::vec3 color = glm::vec3(0, 0, 0);

        std::vector<VertexFormat> vertices = {
            VertexFormat(glm::vec3(0, 1, 0), color), // punctul B 0
            VertexFormat(glm::vec3(-0.22422f, 0.309f, 0), color), // punctul G 1
            VertexFormat(glm::vec3(-0.95105f, 0.30901f, 0), color), // punctul C 2
            VertexFormat(glm::vec3(-0.58778f, -0.80901f, 0), color), // punctul D 3
            VertexFormat(glm::vec3(0.58778f, -0.80901f, 0), color), // punctul E 4
            VertexFormat(glm::vec3(0.95105f, 0.30901f, 0), color), // punctul F 5
            VertexFormat(glm::vec3(0.22422f, 0.309f, 0), color) // punctul H 6

        };

        Mesh* star = new Mesh(name);

        std::vector<unsigned int> indices = {
            0, 1, 6,
            2, 4, 6,
            3, 5, 1
        };

        star->InitFromData(vertices, indices);
        return star;
    }

    Mesh* CreateHexagonMesh(const std::string& name) {
        glm::vec3 color = glm::vec3(0, 0, 0);

        std::vector<VertexFormat> vertices = {
            VertexFormat(glm::vec3(0, 0, 0), color), // O 0
            VertexFormat(glm::vec3(1, 0, 0), color), // A 1
            VertexFormat(glm::vec3(0.5f, 0.86602f, 0), color), // B 2
            VertexFormat(glm::vec3(-0.5f, 0.86602f, 0), color), // C 3
            VertexFormat(glm::vec3(-1, 0, 0), color), // D 4
            VertexFormat(glm::vec3(-0.5f, -0.86602f, 0), color), // E 5
            VertexFormat(glm::vec3(0.5f, -0.86602f, 0), color) // F 6
        };

        Mesh* star = new Mesh(name);

        std::vector<unsigned int> indices = {
            0, 1, 2,
            0, 2, 3,
            0, 3, 4,
            0, 4, 5,
            0, 5, 6,
            0, 6, 1
        };

        star->InitFromData(vertices, indices);
        return star;
    }

    Mesh* CreateTowerMesh(const std::string& name) {
        glm::vec3 color = glm::vec3(0, 0, 0);

        std::vector<VertexFormat> vertices = {
            VertexFormat(glm::vec3(-0.2f, 0.5f, 0), color), // A 0
            VertexFormat(glm::vec3(-0.5f, 0, 0), color), // B 1 
            VertexFormat(glm::vec3(-0.2f, -0.5f, 0), color), // C 2
            VertexFormat(glm::vec3(0.1f, 0, 0), color), // D 3 
            VertexFormat(glm::vec3(0.3f, 0.2f, 0), color), // E 4
            VertexFormat(glm::vec3(0.3f, -0.2f, 0), color), // F 5 
            VertexFormat(glm::vec3(-0.2f, 0.2f, 0), color), // G 6
            VertexFormat(glm::vec3(-0.2f, -0.2f, 0), color) // H 7 
        };

        Mesh* tower = new Mesh(name);

        std::vector<unsigned int> indices = {
            0, 1, 2,
            2, 3, 0,
            4, 6, 7,
            7, 5, 4
        };

        tower->InitFromData(vertices, indices);
        return tower;
    }

    std::vector<Mesh*> CreateDigitsMeshes() {
        glm::vec3 color = glm::vec3(0, 0, 0);

        std::vector<VertexFormat> vertices = {
            VertexFormat(glm::vec3(-0.4f, 1, 0), color), // A 0
            VertexFormat(glm::vec3(-0.4f, 0.8f, 0), color), // B 1
            VertexFormat(glm::vec3(0.4f, 0.8f, 0), color), // C 2
            VertexFormat(glm::vec3(0.4f, 1, 0), color), // D 3
            VertexFormat(glm::vec3(0.4f, 0.1f, 0), color), // E 4
            VertexFormat(glm::vec3(0.6f, 0.1f, 0), color), // F 5
            VertexFormat(glm::vec3(0.6f, 0.8f, 0), color), // G 6
            VertexFormat(glm::vec3(0.4f, -0.1f, 0), color), // H 7
            VertexFormat(glm::vec3(0.4f, -0.8f, 0), color), // I 8
            VertexFormat(glm::vec3(0.6f, -0.8f, 0), color), // J 9
            VertexFormat(glm::vec3(0.6f, -0.1f, 0), color), // K 10
            VertexFormat(glm::vec3(-0.4f, -0.8f, 0), color), // L 11
            VertexFormat(glm::vec3(-0.4f, -1, 0), color), // M 12
            VertexFormat(glm::vec3(0.4f, -1, 0), color), // N 13
            VertexFormat(glm::vec3(-0.4f, -0.1f, 0), color), // O 14
            VertexFormat(glm::vec3(-0.6f, -0.1f, 0), color), // P 15
            VertexFormat(glm::vec3(-0.6f, -0.8f, 0), color), // Q 16
            VertexFormat(glm::vec3(-0.6f, 0.8f, 0), color), // R 17
            VertexFormat(glm::vec3(-0.6f, 0.1f, 0), color), // S 18
            VertexFormat(glm::vec3(-0.4f, 0.1f, 0), color), // T 19
        };

        std::vector<Mesh*> digits;

        Mesh* zero = new Mesh("0");
        std::vector<unsigned int> indices = {
            1, 2, 3,
            1, 3, 0,
            17, 18, 1,
            18, 19, 1,
            7, 8, 10,
            8, 9, 10,
            8, 11, 12,
            12, 13, 8,
            15, 16, 14,
            16, 11, 14,
            2, 4, 6,
            4, 5, 6
        };
        zero->InitFromData(vertices, indices);
        digits.push_back(zero);


        Mesh* one = new Mesh("1");
        indices = {
            2, 4, 6,
            4, 5, 6,
            7, 8, 10,
            8, 9, 10
        };
        one->InitFromData(vertices, indices);
        digits.push_back(one);

        
        Mesh* two = new Mesh("2");
        indices = {
            1, 2, 3,
            1, 3, 0,
            2, 4, 6,
            4, 5, 6,
            19, 14, 4,
            14, 7, 4,
            15, 16, 14,
            16, 11, 14,
            8, 11, 12,
            12, 13, 8
        };
        two->InitFromData(vertices, indices);
        digits.push_back(two);


        Mesh* three = new Mesh("3");
        indices = {
            1, 2, 3,
            1, 3, 0,
            2, 4, 6,
            4, 5, 6,
            19, 14, 4,
            14, 7, 4,
            7, 8, 10,
            8, 9, 10,
            8, 11, 12,
            12, 13, 8
        };
        three->InitFromData(vertices, indices);
        digits.push_back(three);


        Mesh* four = new Mesh("4");
        indices = {
            17, 18, 1,
            18, 19, 1,
            19, 14, 4,
            14, 7, 4,
            2, 4, 6,
            4, 5, 6,
            7, 8, 10,
            8, 9, 10
        };
        four->InitFromData(vertices, indices);
        digits.push_back(four);

        
        Mesh* five = new Mesh("5");
        indices = {
            1, 2, 3,
            1, 3, 0,
            17, 18, 1,
            18, 19, 1,
            19, 14, 4,
            14, 7, 4,
            7, 8, 10,
            8, 9, 10,
            8, 11, 12,
            12, 13, 8
        };
        five->InitFromData(vertices, indices);
        digits.push_back(five);


        Mesh* six = new Mesh("6");
        indices = {
            1, 2, 3,
            1, 3, 0,
            17, 18, 1,
            18, 19, 1,
            19, 14, 4,
            14, 7, 4,
            7, 8, 10,
            8, 9, 10,
            8, 11, 12,
            12, 13, 8,
            15, 16, 14,
            16, 11, 14
        };
        six->InitFromData(vertices, indices);
        digits.push_back(six);


        Mesh* seven = new Mesh("7");
        indices = {
            1, 2, 3,
            1, 3, 0,
            2, 4, 6,
            4, 5, 6,
            7, 8, 10,
            8, 9, 10
        };
        seven->InitFromData(vertices, indices);
        digits.push_back(seven);


        Mesh* eight = new Mesh("8");
        indices = {
            1, 2, 3,
            1, 3, 0,
            17, 18, 1,
            18, 19, 1,
            19, 14, 4,
            14, 7, 4,
            7, 8, 10,
            8, 9, 10,
            8, 11, 12,
            12, 13, 8,
            15, 16, 14,
            16, 11, 14,
            2, 4, 6,
            4, 5, 6
        };
        eight->InitFromData(vertices, indices);
        digits.push_back(eight);


        Mesh* nine = new Mesh("9");
        indices = {
            1, 2, 3,
            1, 3, 0,
            17, 18, 1,
            18, 19, 1,
            19, 14, 4,
            14, 7, 4,
            7, 8, 10,
            8, 9, 10,
            8, 11, 12,
            12, 13, 8,
            2, 4, 6,
            4, 5, 6
        };
        nine->InitFromData(vertices, indices);
        digits.push_back(nine);

        return digits;
    }

    std::vector<Mesh*> CreateHeartMeshes() {
        glm::vec3 color = glm::vec3(0, 0, 0);
        std::vector<Mesh*> hearts;

        std::vector<VertexFormat> vertices = {
            VertexFormat(glm::vec3(0, 0, 0), color),
            VertexFormat(glm::vec3(0, 0.5f, 0), color),
            VertexFormat(glm::vec3(-0.5f, 1, 0), color),
            VertexFormat(glm::vec3(-1, 0.75f, 0), color),
            VertexFormat(glm::vec3(-1, 0.25f, 0), color),
            VertexFormat(glm::vec3(0, -1, 0), color),
            VertexFormat(glm::vec3(1, 0.25f, 0), color),
            VertexFormat(glm::vec3(1, 0.75f, 0), color),
            VertexFormat(glm::vec3(0.5f, 1, 0), color), // full heart - 8
            VertexFormat(glm::vec3(0.55f, 0.45f, 0), color),
            VertexFormat(glm::vec3(0.75f, 0, 0), color),
            VertexFormat(glm::vec3(-0.4f, -0.35f, 0), color)
            
        };

        Mesh* fullHeart = new Mesh("fullHeart");

        std::vector<unsigned int> indices = {
            0, 1, 2,
            0, 2, 3,
            0, 3, 4,
            0, 4, 5,
            0, 5, 6,
            0, 6, 7,
            0, 7, 8,
            0, 8, 1
        };
        fullHeart->InitFromData(vertices, indices);
        hearts.push_back(fullHeart);


        Mesh* twoLivesHeart = new Mesh("2LivesHeart");
        indices = {
            0, 1, 2,
            0, 2, 3,
            0, 3, 4,
            0, 4, 5,
            0, 5, 10,
            0, 10, 9,
            0, 9, 1
        };
        twoLivesHeart->InitFromData(vertices, indices);
        hearts.push_back(twoLivesHeart);


        Mesh* oneLifeHeart = new Mesh("1LifeHeart");
        indices = {
            0, 11, 5,
            0, 5, 10,
            0, 10, 9,
            0, 9, 1
        };
        oneLifeHeart->InitFromData(vertices, indices);
        hearts.push_back(oneLifeHeart);

        return hearts;
    }

    Mesh* CreateClaymoreMesh() {
        glm::vec3 black = glm::vec3(0, 0, 0);
        glm::vec3 camo = glm::vec3(0.5f, 0.5f, 0.25f);
        glm::vec3 red = glm::vec3(1, 0, 0);

        std::vector<VertexFormat> vertices = {
            VertexFormat(glm::vec3(-0.8f, -0.8f, 0), camo),
            VertexFormat(glm::vec3(-0.75f, -0.8f, 0), camo), //left base
            VertexFormat(glm::vec3(-0.4f, -0.45f, 0), camo),
            VertexFormat(glm::vec3(-0.4f, -0.4f, 0), camo),  // center base
            VertexFormat(glm::vec3(-0.05f, -0.8f, 0), camo),
            VertexFormat(glm::vec3(0, -0.8f, 0), camo), // right base  all for first set of legs

            VertexFormat(glm::vec3(-0.2f, -0.4f, 0), black), // 6
            VertexFormat(glm::vec3(-0.25f, -0.4f, 0), black),
            VertexFormat(glm::vec3(-0.25f, -0.35f, 0), black),
            VertexFormat(glm::vec3(-0.2f, -0.35f, 0), black),

            VertexFormat(glm::vec3(-0.6f, -0.4f, 0), black), // 10
            VertexFormat(glm::vec3(-0.65f, -0.4f, 0), black),
            VertexFormat(glm::vec3(-0.65f, -0.35f, 0), black),
            VertexFormat(glm::vec3(-0.6f, -0.35f, 0), black),

            VertexFormat(glm::vec3(-0.6f, 0.2f, 0), black), // 14
            VertexFormat(glm::vec3(-0.65f, 0.2f, 0), black),
            VertexFormat(glm::vec3(-0.65f, 0.25f, 0), black),
            VertexFormat(glm::vec3(-0.6f, 0.25f, 0), black),

            VertexFormat(glm::vec3(-0.2f, 0.2f, 0), black), // 18
            VertexFormat(glm::vec3(-0.25f, 0.2f, 0), black),
            VertexFormat(glm::vec3(-0.25f, 0.25f, 0), black),
            VertexFormat(glm::vec3(-0.2f, 0.25f, 0), black),

            VertexFormat(glm::vec3(0, 0.8f, 0), black), // 22
            VertexFormat(glm::vec3(-0.05, 0.8f, 0), black),
            VertexFormat(glm::vec3(-0.05, 0.85f, 0), black),
            VertexFormat(glm::vec3(0, 0.85f, 0), black),

            VertexFormat(glm::vec3(0.4f, 0.8f, 0), black), // 26
            VertexFormat(glm::vec3(0.35f, 0.8f, 0), black),
            VertexFormat(glm::vec3(0.35f, 0.85f, 0), black),
            VertexFormat(glm::vec3(0.4f, 0.85f, 0), black),

            VertexFormat(glm::vec3(0.4f, 0.2f, 0), black), // 30
            VertexFormat(glm::vec3(0.35f, 0.2f, 0), black),
            VertexFormat(glm::vec3(0.35f, 0.25f, 0), black),
            VertexFormat(glm::vec3(0.4f, 0.25f, 0), black),

            VertexFormat(glm::vec3(0.6f, -0.2f, 0), camo),
            VertexFormat(glm::vec3(0.6f, -0.15f, 0), camo),
            VertexFormat(glm::vec3(0.2f, 0.15f, 0), camo),
            VertexFormat(glm::vec3(0.2f, 0.2f, 0), camo),


            VertexFormat(glm::vec3(-0.25f, -0.35f, 0), camo), // 38
            VertexFormat(glm::vec3(-0.6f, -0.35f, 0), camo),
            VertexFormat(glm::vec3(-0.6f, 0.2f, 0), camo),
            VertexFormat(glm::vec3(-0.25f, 0.2f, 0), camo),

            VertexFormat(glm::vec3(-0.6f, 0.25f, 0), camo), // 42
            VertexFormat(glm::vec3(0, 0.8f, 0), camo),
            VertexFormat(glm::vec3(0.35f, 0.8f, 0), camo),
            VertexFormat(glm::vec3(-0.25f, 0.25f, 0), camo),

            VertexFormat(glm::vec3(-0.2f, -0.35f, 0), camo), // 46
            VertexFormat(glm::vec3(0.35f, 0.25f, 0), camo),
            VertexFormat(glm::vec3(0.4f, 0.8f, 0), camo),
            VertexFormat(glm::vec3(-0.2f, 0.2f, 0), camo),

            VertexFormat(glm::vec3(0.06f, 0.45f, 0), red),
            VertexFormat(glm::vec3(0.06f, 0.5f, 0), red),
            VertexFormat(glm::vec3(0.68f, 0.7f, 0), red),
            VertexFormat(glm::vec3(0.68f, 0.65f, 0), red),
            VertexFormat(glm::vec3(0.68f, 0.25f, 0), red),
            VertexFormat(glm::vec3(0.68f, 0.20f, 0), red)

            
        };

        Mesh* claymore = new Mesh("claymore");

        std::vector<unsigned int> indices = {
            52, 50, 51,
            52, 51, 53,
            50, 51, 55,
            50, 55, 54,

            0, 1, 2,
            1, 2, 3, // left leg
            2, 4, 5,
            4, 5, 3,

            12, 11, 6,
            6, 9, 12,
            16, 11, 17,
            11, 10, 17,
            20, 16, 19,
            16, 15, 19,
            20, 7, 21,
            7, 6, 21,

            25, 24, 15,
            15, 14, 25,
            24, 23, 29,
            23, 26, 29,
            29, 28, 20,
            20, 21, 29,

            28, 31, 30,
            28, 30, 29,
            32, 8, 7,
            7, 33, 32,

            36, 34, 35,
            34, 35, 37,

            40, 39, 38,
            38, 41, 40,

            43, 42, 44,
            44, 42, 45,

            46, 47, 48,
            46, 48, 49

            
        };

        claymore->InitFromData(vertices, indices);
        return claymore;
    }

    Mesh* CreateWaterMesh() {
        glm::vec3 blue = glm::vec3(0, 0.4f, 1);

        std::vector<VertexFormat> vertices = {
            VertexFormat(glm::vec3(0, 0, 0), blue), //0
            VertexFormat(glm::vec3(0, -0.25f, 0) - glm::vec3(0, 0.2f, 0), blue), // A
            VertexFormat(glm::vec3(0.1275f, -0.2175f, 0) - glm::vec3(0, 0.2f, 0), blue),
            VertexFormat(glm::vec3(0.25f, -0.125f, 0) - glm::vec3(0, 0.2f, 0), blue),
            VertexFormat(glm::vec3(0.3f, 0, 0) - glm::vec3(0, 0.2f, 0), blue),
            VertexFormat(glm::vec3(0.305f, 0.13f, 0) - glm::vec3(0, 0.2f, 0), blue),
            VertexFormat(glm::vec3(0.2525f, 0.305f, 0) - glm::vec3(0, 0.2f, 0), blue),
            VertexFormat(glm::vec3(0.155f, 0.4675f, 0) - glm::vec3(0, 0.2f, 0), blue),
            VertexFormat(glm::vec3(0.0675f, 0.6025f, 0) - glm::vec3(0, 0.2f, 0), blue),
            VertexFormat(glm::vec3(0, 0.75f, 0) - glm::vec3(0, 0.2f, 0), blue), // I
            VertexFormat(glm::vec3(-0.0675f, 0.6025f, 0) - glm::vec3(0, 0.2f, 0), blue),
            VertexFormat(glm::vec3(-0.155f, 0.4675f, 0) - glm::vec3(0, 0.2f, 0), blue),
            VertexFormat(glm::vec3(-0.2525f, 0.305f, 0) - glm::vec3(0, 0.2f, 0), blue),
            VertexFormat(glm::vec3(-0.305f, 0.13f, 0) - glm::vec3(0, 0.2f, 0), blue),
            VertexFormat(glm::vec3(-0.3f, 0, 0) - glm::vec3(0, 0.2f, 0), blue),
            VertexFormat(glm::vec3(-0.25f, -0.125f, 0) - glm::vec3(0, 0.2f, 0), blue),
            VertexFormat(glm::vec3(-0.1275f, -0.2175f, 0) - glm::vec3(0, 0.2f, 0), blue)
        };

        Mesh* water = new Mesh("water");

        std::vector<unsigned int> indices = {
            0, 1, 2,
            0, 2, 3,
            0, 3, 4,
            0, 4, 5,
            0, 5, 6,
            0, 6, 7,
            0, 7, 8,
            0, 8, 9,
            0, 9, 10,
            0, 10, 11,
            0, 11, 12,
            0, 12, 13,
            0, 13, 14,
            0, 14, 15,
            0, 15, 16,
            0, 16, 1
        };

        water->InitFromData(vertices, indices);
        return water;
    }

    Mesh* CreateTreeMesh() {
        glm::vec3 green = glm::vec3(0, 0.2f, 0);
        glm::vec3 brown = glm::vec3(0.4f, 0.2f, 0);

        std::vector<VertexFormat> vertices = {
            VertexFormat(glm::vec3(-0.2f, -0.8f, 0), brown),
            VertexFormat(glm::vec3(0.2f, -0.8f, 0), brown),
            VertexFormat(glm::vec3(0.2f, -0.4f, 0), brown),
            VertexFormat(glm::vec3(-0.2f, -0.4f, 0), brown),
            VertexFormat(glm::vec3(-0.6f, -0.4f, 0), green),
            VertexFormat(glm::vec3(0.6f, -0.4f, 0), green),
            VertexFormat(glm::vec3(0, 0.2f, 0), green),
            VertexFormat(glm::vec3(-0.4f, 0.2f, 0), green),
            VertexFormat(glm::vec3(0.4f, 0.2f, 0), green),
            VertexFormat(glm::vec3(0, 0.7f, 0), green)
        };

        Mesh* tree = new Mesh("tree");

        std::vector<unsigned int> indices = {
            0, 1, 2,
            0, 2, 3,
            4, 5, 6,
            7, 8, 9
        };

        tree->InitFromData(vertices, indices);
        return tree;
    }

    Mesh* CreateBombMesh() {
        glm::vec3 red = glm::vec3(1, 0, 0);
        glm::vec3 white = glm::vec3(1, 1, 1);
        glm::vec3 black = glm::vec3(0, 0, 0);

        std::vector<VertexFormat> vertices = {
            VertexFormat(glm::vec3(-1, 1, 0), red),
            VertexFormat(glm::vec3(1, 1, 0), red),
            VertexFormat(glm::vec3(1, 0.4f, 0), red),
            VertexFormat(glm::vec3(-1, 0.4f, 0), red),

            VertexFormat(glm::vec3(-1, -0.4f, 0), red),
            VertexFormat(glm::vec3(1, -0.4f, 0), red),
            VertexFormat(glm::vec3(1, -1, 0), red),
            VertexFormat(glm::vec3(-1, -1, 0), red),

            VertexFormat(glm::vec3(-1, 0.4f, 0), white),
            VertexFormat(glm::vec3(1, 0.4, 0), white),
            VertexFormat(glm::vec3(1, -0.4f, 0), white),
            VertexFormat(glm::vec3(-1, -0.4f, 0), white),

            VertexFormat(glm::vec3(-1, 0.4f, 0), black),
            VertexFormat(glm::vec3(-0.4f, 0.4f, 0), black),
            VertexFormat(glm::vec3(-0.4f, 0.2f, 0), black),
            VertexFormat(glm::vec3(-1, 0.2f, 0), black),

            VertexFormat(glm::vec3(-0.8f, 0.2f, 0), black),
            VertexFormat(glm::vec3(-0.6f, 0.2f, 0), black),
            VertexFormat(glm::vec3(-0.6f, -0.6f, 0), black),
            VertexFormat(glm::vec3(-0.8f, -0.6f, 0), black),

            VertexFormat(glm::vec3(-0.2f, 0.4f, 0), black),
            VertexFormat(glm::vec3(-0.2f, -0.4f, 0), black),
            VertexFormat(glm::vec3(-0.4f, -0.4f, 0), black),

            VertexFormat(glm::vec3(-0.2f, 0.2f, 0), black),
            VertexFormat(glm::vec3(0, 0.2f, 0), black),
            VertexFormat(glm::vec3(0, 0, 0), black),
            VertexFormat(glm::vec3(-0.2f, 0, 0), black),

            VertexFormat(glm::vec3(0.2f, 0, 0), black),
            VertexFormat(glm::vec3(0.2f, -0.2f, 0), black),
            VertexFormat(glm::vec3(0, -0.2f, 0), black),

            VertexFormat(glm::vec3(0.2f, 0.4f, 0), black),
            VertexFormat(glm::vec3(0.4f, 0.4f, 0), black),
            VertexFormat(glm::vec3(0.4f, -0.4f, 0), black),
            VertexFormat(glm::vec3(0.2f, -0.4f, 0), black),

            VertexFormat(glm::vec3(1, 0.4f, 0), black),
            VertexFormat(glm::vec3(1, 0.2f, 0), black),
            VertexFormat(glm::vec3(0.4f, 0.2f, 0), black),

            VertexFormat(glm::vec3(0.6f, 0.4f, 0), black),
            VertexFormat(glm::vec3(0.8f, 0.4f, 0), black),
            VertexFormat(glm::vec3(0.8f, -0.4f, 0), black),
            VertexFormat(glm::vec3(0.6f, -0.4f, 0), black),

        };

        Mesh* bomb = new Mesh("bomb");

        std::vector<unsigned int> indices = {
            0, 3, 1,
            3, 2, 1,
            4, 7, 5,
            7, 6, 5,

            12, 15, 13,
            15, 14, 13,
            16, 19, 17,
            19, 18, 17,

            13, 22, 20,
            22, 21, 20,
            23, 26, 24,
            26, 25, 24,
            25, 29, 27,
            29, 28, 27,
            30, 33, 31,
            33, 32, 31,

            31, 36, 34,
            36, 35, 34,
            37, 40, 38,
            40, 39, 38,

            8, 11, 9,
            11, 10, 9
            
        };
        bomb->InitFromData(vertices, indices);
        return bomb;
    }
}
