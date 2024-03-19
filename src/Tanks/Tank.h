#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"
#include "core/gpu/mesh.h"

using namespace std;

namespace m1 {
	class Tank {
	public:
		Tank(glm::vec3 direction, glm::vec3 position, Mesh* body, glm::vec3 bodySize, Mesh* turret, Mesh* cannon, Mesh* tracks) {
			this->direction = direction;
			this->position = position;
			this->body = body;
			this->bodySize = bodySize;
			this->turret = turret;
			this->cannon = cannon;
			this->tracks = tracks;
			this->verticalAngle = 0;
			this->horizontalAngle = 0;
			this->cooldown = 1;
			this->hp = 3;
			this->nextAction = 0;
		}

		Tank() {

		}

		bool collisionTank(Tank t) {
			glm::vec2 p1 = glm::vec2(this->position.x, this->position.z);
			glm::vec2 p2 = glm::vec2(t.position.x, t.position.z);

			glm::vec2 r1 = glm::vec2(this->bodySize.x / 2, this->bodySize.z / 2);
			glm::vec2 r2 = glm::vec2(t.bodySize.x / 2, t.bodySize.z / 2);

			return glm::length(p1 - p2) < glm::length(r1) + glm::length(r2);
		}
	
		void moveForward(float deltaTime) {
			this->position += glm::normalize(this->direction) * deltaTime * 3.f;
		}

		void rotateTank(float deltaTime) {
			glm::vec3 newDir = this->direction;
			newDir.x = this->direction.x * cos(deltaTime * this->rotationDirection) - this->direction.z * sin(deltaTime * this->rotationDirection);
			newDir.z = this->direction.x * sin(deltaTime * this->rotationDirection) + this->direction.z * cos(deltaTime * this->rotationDirection);
			this->direction = newDir;
		}

		bool alignWithEnemyAndTestFire(float deltaTime, Tank t) {
			glm::vec3 strive = glm::normalize(this->position - t.position);
			glm::vec3 actual = glm::normalize(glm::vec3(sin(this->horizontalAngle), 0, cos(this->horizontalAngle)));
			float angle = atan2(actual.z, actual.x) - atan2(strive.z, strive.x) + 3.141592f;
			if (angle > 3.141592f) {
				angle -= 2 * 3.141592f;
			}
			angle *= -1;
			
			if (abs(angle) > 0.01f) {
				if (angle > 0) {
					this->horizontalAngle -= deltaTime * 1.5f;
				} else {
					this->horizontalAngle += deltaTime * 1.5f;
				}
				return false;
			}
			return true;
		}

		void huntTank(float deltaTime, Tank t) {
			if (glm::length(this->position - t.position) >= 3.f) {
				glm::vec3 strive = glm::normalize(this->position - t.position);
				glm::vec3 actual = glm::normalize(this->direction);
				float angle = atan2(actual.z, actual.x) - atan2(strive.z, strive.x) + 3.141592f;
				if (angle > 3.141592f) {
					angle -= 2 * 3.141592f;
				}
				angle *= -1;

				if (abs(angle) > 0.01f) {
					if (angle < 0) {
						deltaTime *= -1;
					}
					glm::vec3 newDir = this->direction;
					newDir.x = this->direction.x * cos(deltaTime) - this->direction.z * sin(deltaTime);
					newDir.z = this->direction.x * sin(deltaTime) + this->direction.z * cos(deltaTime);
					this->direction = newDir;
					
				}
				
				moveForward(abs(deltaTime));

			}
		}

		glm::vec3 direction;

		glm::vec3 position;
		
		Mesh* body;
		glm::vec3 bodySize;
		
		Mesh* turret;
		Mesh* cannon;
		Mesh* tracks;

		float verticalAngle, horizontalAngle;
		float cooldown;
		int hp;

		float nextAction;
		int action;
		float rotationDirection;
	};
}