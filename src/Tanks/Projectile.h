#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"
#include "core/gpu/mesh.h"

namespace m1 {
	class Projectile {
	public:

		Projectile(glm::vec3 position, glm::vec3 speed, Mesh* mesh, float radius, bool enemy) {
			this->position = position;
			this->speed = speed;
			this->mesh = mesh;
			this->radius = radius;
			this->enemy = enemy;
		}

		bool impactTank(Tank t) {
			glm::vec2 r = glm::vec2(t.bodySize.x / 2, t.bodySize.z / 2);
			return glm::length(this->position - t.position) < this->radius + glm::length(r);
		}

		glm::vec3 position;
		glm::vec3 speed;
		Mesh* mesh;

		float radius;
		bool enemy;

	};
}