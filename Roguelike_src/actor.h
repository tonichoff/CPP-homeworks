#pragma once

class Actor;
class Wall;
class Knight;
class Zombie;
class Princess;

struct Coords {
  Coords() {}
  Coords(int x, int y) : x_(x), y_(y) {}


  bool operator<(const Coords& rhs) const {
    if (x_ < rhs.x_)
      return true;
    else if (x_ == rhs.x_ && y_ < rhs.y_)
      return true;
    else
      return false;
  }
  int x_;
  int y_;
};

class Actor {
public:
  Actor(Coords, int, unsigned, unsigned, unsigned);
  virtual int getSymbol() const;
  virtual void setCoords(Coords);
  virtual Coords getCoords() const;
  virtual bool isDead() const;
  virtual bool isWon() const;
  virtual void takeDamage(unsigned);
  virtual unsigned getDamage() const;
  virtual Coords move() = 0;
  virtual void collide(Actor*) = 0;
  virtual void collide(Wall*) = 0;
  virtual void collide(Knight*) = 0;
  virtual void collide(Zombie*) = 0;
  virtual void collide(Princess*) = 0;
protected:
  unsigned damage_;
  int health_points_;
  int mana_points_;
  int symbol_;
  Coords coords_;
  bool dead_ = false;
  bool won_ = false;
};

class Wall : public Actor {
public:
  Wall(Coords coords, int symbol = '#') : Actor(coords, symbol, 0, 1, 0) {}
  virtual ~Wall() {}
  virtual Coords move() override;
  virtual void collide(Actor*) override;
  virtual void collide(Wall*) override {}
  virtual void collide(Knight*) override {}
  virtual void collide(Zombie*) override {}
  virtual void collide(Princess*) override {}
};

class Knight : public Actor {
public:
  Knight(Coords, int);
  virtual ~Knight() {}
  virtual Coords move() override;
  virtual void collide(Actor*) override;
  virtual void collide(Wall*) override {}
  virtual void collide(Knight*) override {};
  virtual void collide(Zombie*) override;
  virtual void collide(Princess*) override;
};

class Zombie : public Actor {
public:
  Zombie(Coords, int);
  virtual ~Zombie() {}
  virtual Coords move() override;
  virtual void collide(Actor*) override;
  virtual void collide(Wall*) override {}
  virtual void collide(Knight*) override;
  virtual void collide(Zombie*) override {}
  virtual void collide(Princess*) override {}
};

class Princess : public Actor {
public:
  Princess(Coords, int);
  virtual ~Princess() {}
  virtual Coords move() override;
  virtual void collide(Actor*) override;
  virtual void collide(Wall*) override {}
  virtual void collide(Knight*) override;
  virtual void collide(Zombie*) override {}
  virtual void collide(Princess*) override {}
};