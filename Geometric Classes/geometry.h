#pragma once
#include <vector>

class Base;
class Line;
class BrokenLine;
class Circle;

struct Point {
  double x;
  double y;

  Point() {}
  Point(double x, double y) : x(x), y(y) {}
};

class Base {
public:
  virtual std::vector<Point> intersect(Base&) = 0;
  virtual std::vector<Point> intersect(Line&) = 0;
  virtual std::vector<Point> intersect(BrokenLine&) = 0;
  virtual std::vector<Point> intersect(Circle&) = 0;
};

class Line : public Base {
public:
  Line(Point a, Point b) : point_a_(a), point_b_(b) {};
  Point getFirstPoint();
  Point getSecondPoint();
  virtual std::vector<Point> intersect(Base&) override;
  virtual std::vector<Point> intersect(Line&) override;
  virtual std::vector<Point> intersect(BrokenLine&) override;
  virtual std::vector<Point> intersect(Circle&) override;
private:
  Point point_a_;
  Point point_b_;
};

class BrokenLine : public Base {
public:
  BrokenLine(std::vector<Point> points) : points_(points) {};
  int numPoints();
  Line getLine(int);
  virtual std::vector<Point> intersect(Base&) override;
  virtual std::vector<Point> intersect(Line&) override;
  virtual std::vector<Point> intersect(BrokenLine&) override;
  virtual std::vector<Point> intersect(Circle&) override;
private:
  std::vector<Point> points_;
};

class Circle : public Base {
public:
  Circle(Point centre, double radius) : centre_(centre), radius_(radius) {};
  Point getCentre();
  double getRadius();
  virtual std::vector<Point> intersect(Base&) override;
  virtual std::vector<Point> intersect(Line&) override;
  virtual std::vector<Point> intersect(BrokenLine&) override;
  virtual std::vector<Point> intersect(Circle&) override;
private:
  Point centre_;
  double radius_;
};