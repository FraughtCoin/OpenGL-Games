#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"
#include <iostream>


namespace camera
{
    class Camera
    {
    public:
        Camera()
        {
            position = glm::vec3(0, 2, 5);
            forward = glm::vec3(0, 0, -1);
            up = glm::vec3(0, 1, 0);
            right = glm::vec3(1, 0, 0);
            distanceToTarget = 2;
        }

        Camera(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
        {
            Set(position, center, up);
        }

        ~Camera()
        { }

        void Set(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
        {
            this->position = position;
            forward = glm::normalize(center - position);
            right = glm::cross(forward, up);
            this->up = glm::cross(right, forward);
            this->distanceToTarget = glm::distance(position, center);
        }

        void MoveForward(float distance)
        {
            glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
            position += dir * distance;
        }

        void TranslateForward(float distance)
        {
            position += glm::normalize(forward) * distance;
        }

        void TranslateUpward(float distance)
        {
            position += glm::normalize(up) * distance;
        }

        void TranslateRight(float distance)
        {
            position += glm::normalize(right) * distance;
        }

        void RotateFirstPerson_OX(float angle)
        {
            glm::vec4 aux = glm::rotate(glm::mat4(1.f), angle, right) * glm::vec4(forward, 0);
            forward = glm::normalize(glm::vec3(aux));
            up = glm::cross(right, forward);
        }

        void RotateFirstPerson_OY(float angle)
        {
            glm::vec4 aux = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 0);
            forward = glm::normalize(glm::vec3(aux));

            aux = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 0);
            right = glm::normalize(glm::vec3(aux));

            up = glm::cross(right, forward);

        }

        void RotateFirstPerson_OZ(float angle)
        {
            glm::vec4 aux = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1);
            right = glm::normalize(glm::vec3(aux));

            aux = glm::rotate(glm::mat4(1.f), angle, forward) * glm::vec4(up, 0);
            forward = glm::normalize(glm::vec3(aux));

            up = glm::cross(right, forward);
        }

        void RotateThirdPerson_OX(float angle)
        {
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OX(angle);
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_OY(float angle)
        {
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OY(angle);
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_OZ(float angle)
        {
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OZ(angle);
            TranslateForward(-distanceToTarget);
        }

        glm::mat4 GetViewMatrix()
        {
            return glm::lookAt(position, position + forward, up);
        }

        glm::vec3 GetTargetPosition()
        {
            return position + forward * distanceToTarget;
        }

        Camera getConfiguration() {
            Camera c;
            c.distanceToTarget = this->distanceToTarget;
            c.position = this->position;
            c.forward = this->forward;
            c.right = this->right;
            c.up = this->up;

            return c;
        }

        void loadConfiguration(Camera c) {
            this->distanceToTarget = c.distanceToTarget;
            this->position = c.position;
            this->forward = c.forward;
            this->right = c.right;
            this->up = c.up;
        }

        void printInfo() {
            std::cout << "pozitie: " << position << "; forawrd: " << forward << "; right: " << right << "; up: " << up << "\n";
        }

    public:
        float distanceToTarget;
        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;
    };
}
