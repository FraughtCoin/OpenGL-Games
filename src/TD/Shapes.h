#pragma once

#include "components/simple_scene.h"
namespace shapes {
	
	class Rectangle {
	public:
		Rectangle(float width, float height, float centerX, float centerY, Mesh* mesh, glm::vec3 color) {
			this->width = width;
			this->height = height;
			this->centerX = centerX;
			this->centerY = centerY;
			this->mesh = mesh;
			this->color = color;
		};

		Rectangle() {

		};

		float getWidth() {
			return width;
		}

		float getHeight() {
			return height;
		}

		float getCenterX() {
			return centerX;
		}

		float getCenterY() {
			return centerY;
		}

		Mesh* getMesh() {
			return mesh;
		}

		glm::vec3 getColor() {
			return color;
		}

		bool checkHitbox(int mouseX, int mouseY) {
			return mouseX > centerX - width / 2 && mouseX < centerX + width / 2 && mouseY > centerY - height / 2 && mouseY < centerY + height / 2;
		}
		
	private:
		float width, height;
		float centerX, centerY;
		Mesh* mesh;
		glm::vec3 color;
	};

	class Star {
	public:
		Star(int type, float centerX, float centerY, float size, Mesh* mesh, glm::vec3 color) {
			this->type = type;
			if (type == 0) {
				timeAlive = 8;
			}
			this->centerX = centerX;
			this->centerY = centerY;
			this->size = size;
			this->mesh = mesh;
			this->color = color;
		}

		Star() {

		}

		int getType() {
			return type;
		}

		float getCenterX() {
			return centerX;
		}

		float getCenterY() {
			return centerY;
		}

		float getSize() {
			return size;
		}

		Mesh* getMesh() {
			return mesh;
		}

		glm::vec3 getColor() {
			return color;
		}

		float getTimeAlive() {
			return timeAlive;
		}

		int getRow() {
			return row;
		}

		float getSpin() {
			return spin;
		}

		void setRow(int row) {
			this->row = row;
		}

		void updateTimeAlive(float deltaTimeSeconds) {
			timeAlive -= deltaTimeSeconds;
		}

		bool checkHitbox(int mouseX, int mouseY) {
			float distance = sqrt(pow(mouseX - centerX, 2) + pow(mouseY - centerY, 2));
			return distance < size;
		}

		void updateRightCenterX(float distance) {
			centerX += distance;
		}

		void updateSpin(float deltaTimeSeconds) {
			if (spin > 2 * 3.1415f) {
				spin = 0;
			} else {
				spin += deltaTimeSeconds;
			}
		}


	private:
		int type;
		float timeAlive = 7;
		float centerX, centerY;
		float size;
		float spin = 0;
		Mesh* mesh;
		glm::vec3 color;
		int row;
	};

	class Tower {
	public:
		Tower(float centerX, float centerY, float size, int cost, Mesh* mesh, glm::vec3 color, int type) {
			this->centerX = centerX;
			this->centerY = centerY;
			this->size = size;
			this->cost = cost;
			this->mesh = mesh;
			this->color = color;
			this->type = type;
			if (type == 0) {
				cooldown = 2;
			} else if (type == 2) {
				cooldown = 10;
			}
		}

		Tower() {

		}


		float getCenterX() {
			return centerX;
		}

		float getCenterY() {
			return centerY;
		}

		float getSize() {
			return size;
		}

		int getCost() {
			return cost;
		}

		Mesh* getMesh() {
			return mesh;
		}

		glm::vec3 getColor() {
			return color;
		}

		int getRow() {
			return row;
		}

		int getColumn() {
			return column;
		}

		float getCooldown() {
			return cooldown;
		}

		int getType() {
			return type;
		}

		bool canShoot() {
			return cooldown == 0;
		}

		void setCenterX(float centerX) {
			this->centerX = centerX;
		}

		void setCenterY(float centerY) {
			this->centerY = centerY;
		}

		void setRow(int row) {
			this->row = row;
		}

		void setColumn(int column) {
			this->column = column;
		}

		void setCooldown(float seconds) {
			cooldown = seconds;
		}

		void decreaseSize(float scale) {
			size -= scale;
			if (size < 0) {
				size = 0;
			}
		}

		void updateCooldown(float deltaTimeSeconds) {
			if (cooldown > 0) {
				cooldown -= deltaTimeSeconds;
			} else if (cooldown < 0) {
				cooldown = 0;
			}
		}

	private:
		float size;
		float centerX, centerY;
		float cooldown = 2;
		int cost;
		int row, column;
		Mesh* mesh;
		glm::vec3 color;
		int type;
	};


	class Hexagon {
	public:
		Hexagon(float size, float centerX, float centerY, float tilt, Mesh* mesh, glm::vec3 color) {
			this->size = size;
			this->centerX = centerX;
			this->centerY = centerY;
			this->tilt = tilt;
			this->mesh = mesh;
			this->color = color;
			
		}

		Hexagon() {

		}

		float getCenterX() {
			return centerX;
		}

		float getCenterY() {
			return centerY;
		}

		float getSize() {
			return size;
		}

		float getTilt() {
			return tilt;
		}

		Mesh* getMesh() {
			return mesh;
		}

		glm::vec3 getColor() {
			return color;
		}

		void decreaseSize(float scale) {
			size -= scale;
			if (size < 0) {
				size = 0;
			}
			
		}

		void updateLeftCenterX(float distance) {
			centerX -= distance;
		}

	private:
		float size;
		float centerX, centerY;
		float tilt;
		Mesh* mesh;
		glm::vec3 color;
	};
}