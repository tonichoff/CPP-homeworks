#include "actor.h"
#include <cstdlib>

Actor::Actor(Coords coords, int symbol, unsigned dmg, unsigned hp, unsigned mp) {
  coords_ = coords;
  symbol_ = symbol;
  damage_ = dmg;
  health_points_ = hp;
  mana_points_ = mp;
}

int Actor::getSymbol() const {
  return symbol_;
}

void Actor::setCoords(Coords coords) {
  coords_ = coords;
}

Coords Actor::getCoords() const {
  return coords_;
}

bool Actor::isDead() const {
  return dead_;
}

bool Actor::isWon() const {
  return won_;
}

void Actor::takeDamage(unsigned damage) {
  health_points_ -= damage;
  if (health_points_ <= 0)
    dead_ = true;
}

unsigned Actor::getDamage() const {
  return damage_;
}

Coords Wall::move() {
  return coords_;
}

void Wall::collide(Actor* actor) {
  actor->collide(this);
}

Knight::Knight(Coords coords, int symbol) : Actor(coords, symbol, 5, 15, 10) {}

Coords Knight::move() {
  return coords_;
}

void Knight::collide(Actor* actor) {
  actor->collide(this);
}

void Knight::collide(Zombie* zombie) {
  zombie->takeDamage(this->getDamage());
  this->takeDamage(zombie->getDamage());
}

void Knight::collide(Princess* princess) {
  won_ = true;
}

Zombie::Zombie(Coords coords, int symbol) : Actor(coords, symbol, 3, 12, 0) {}

Coords Zombie::move() {
  return getCoords();
}

void Zombie::collide(Actor* actor) {
  actor->collide(this);
}

void Zombie::collide(Knight* knight) {
  this->takeDamage(knight->getDamage());
}

Coords Princess::move() {
  return getCoords();
}

void Princess::collide(Actor* actor) {
  actor->collide(this);
}

void Princess::collide(Knight* knight) {
  knight->collide(this);
}

Princess::Princess(Coords coords, int symbol) : Actor(coords, symbol, 1, 10, 0) {}