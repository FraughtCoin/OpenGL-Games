#pragma once

#include "TD/Shapes.h"

using namespace shapes;

namespace m1 {
    class Tile {
    public:
        Tile(Rectangle rect, int row, int column, bool hasGrass, float cooldown) {
            this->rect = rect;
            this->row = row;
            this->column = column;
            this->hasGrass = hasGrass;
            this->cooldown = cooldown;
            reset = cooldown;
        }
        Tile() {

        }

        Rectangle getRectangle() {
            return rect;
        }

        Tower getTower() {
            return tower;
        }

        bool doesHaveTower() {
            return hasTower;
        }

        bool doesHaveGrass() {
            return hasGrass;
        }

        void addTower(Tower tower) {
            this->tower = tower;
            this->hasTower = true;
        }

        void setHasTower(bool value) {
            this->hasTower = value;
        }

        void setHasGrass(bool value) {
            this->hasGrass = value;
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

        void decreaseCooldown(float deltaTimeSeconds) {
            cooldown -= deltaTimeSeconds;
        }

        void resetCooldown() {
            cooldown = reset;
        }

    private:
        Rectangle rect;
        int row, column;
        bool hasTower = false;
        bool hasGrass;
        Tower tower;
        float cooldown;
        float reset;
    };


    class Enemy {
    public:
        Enemy(Hexagon outer, Hexagon inner, int row, int hp, int speed) {
            this->outer = outer;
            this->inner = inner;
            this->row = row;
            this->hp = hp;
            this->speed = speed;
        }

        Enemy() {

        }

        Hexagon getOuterHexagon() {
            return outer;
        }

        Hexagon getInnerHexagon() {
            return inner;
        }

        int getRow() {
            return row;
        }

        int getHp() {
            return hp;
        }

        int getSpeed() {
            return speed;
        }

        bool hasShield() {
            return shield;
        }

        void giveShield() {
            shield = true;
        }

        void loseHp() {
            if (shield && shieldHp > 0) {
                shieldHp--;
                if (shieldHp == 0) {
                    shield = false;
                }
            } else {
                hp--;
            }
        }

        void move(float distance) {
            outer.updateLeftCenterX(distance);
            inner.updateLeftCenterX(distance);
        }

        void shrink(float factor) {
            outer.decreaseSize(factor);
            inner.decreaseSize(factor);
        }

    private:
        Hexagon outer, inner;
        int row;
        int hp;
        int speed;
        bool shield = false;
        int shieldHp = 5;
        
        
    };

    class Explosion {
    public:
        Explosion(Star bigStar, Star smallStar, float timeLeft) {
            this->bigStar = bigStar;
            this->smallStar = smallStar;
            this->timeLeft = timeLeft;
        }

        Explosion() {

        }

        Star getBigStar() {
            return bigStar;
        }

        Star getSmallStar() {
            return smallStar;
        }
        
        float getTimeLeft() {
            return timeLeft;
        }

        void updateTimeLeft(float deltaTimeSeconds) {
            timeLeft -= deltaTimeSeconds;
            if (timeLeft < 0) {
                timeLeft = 0;
            }
        }
    private:
        Star bigStar;
        Star smallStar;
        float timeLeft;

    };

}