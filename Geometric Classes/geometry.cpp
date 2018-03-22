#include "geometry.h"
#include <iostream>
#include <cmath>
#include <exception>

#define EPS 0.0000000001

using std::vector;
using std::length_error;

Point Line::getFirstPoint() {
  return point_a_;
}

Point Line::getSecondPoint() {
  return point_b_;
}

vector<Point> Line::intersect(Base& base_obj) {
  return base_obj.intersect(*this);
}

vector<Point> Line::intersect(Line& line) {
  //http://algolist.ru/maths/geom/intersect/lineline2d.php
  vector<Point> result;
  double x1, y1, x2, y2, x3, y3, x4, y4;
  x1 = this->point_a_.x; y1 = this->point_a_.y;
  x2 = this->point_b_.x; y2 = this->point_b_.y;
  x3 = line.point_a_.x;  y3 = line.point_a_.y;
  x4 = line.point_b_.x;  y4 = line.point_b_.y;
  
  double d = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
  double u_a = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / d;
  double u_b = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / d;
  
  if (u_a >= 0 && u_a <= 1 && u_b >= 0 && u_b <= 1) {
    Point point;
    point.x = x1 + u_a * (x2 - x1);
    point.y = y1 + u_a * (y2 - y1);
    result.push_back(point);
  }
  
  return result;
}

vector<Point> Line::intersect(BrokenLine& broken_line) {
  vector<Point> result;
  for (int i = 0; i < broken_line.numPoints() - 1; i++) {
    Line line = broken_line.getLine(i);
    vector<Point> points = this->intersect(line);
    for (int j = 0; j < points.size(); j++)
      result.push_back(points[j]);
  }
  return result;
}

vector<Point> Line::intersect(Circle& circle) {
  //http://e-maxx.ru/algo/circle_line_intersection
  vector<Point> result;
  Point a = this->getFirstPoint();
  Point b = this->getSecondPoint();
  Point o = circle.getCentre();
  double r = circle.getRadius();

  a.x -= o.x; a.y -= o.y;
  b.x -= o.x; b.y -= o.y;

  double A = a.y - b.y;
  double B = b.x - a.x;
  double C = a.x * b.y - b.x * a.y;

  Point q;
  q.x = -(A * C) / (A * A + B * B);
  q.y = -(B * C) / (A * A + B * B);

  if (C * C <= r * r * (A * A + B * B) + EPS) {
    if (abs(C * C - r * r * (A * A + B * B)) < EPS) {
      q.x += o.x;
      q.y += o.y;
      result.push_back(q);
    }
    else {
      double d = r * r - C * C / (A * A + B * B);
      double mult = sqrt(d / (A * A + B * B));
      Point v, w;
      v.x = q.x + B * mult + o.x;
      w.x = q.x - B * mult + o.x;
      v.y = q.y - A * mult + o.y;
      w.y = q.y + B * mult + o.y;
      result.push_back(v);
      result.push_back(w);
    }
  }
  return result;
}

int BrokenLine::numPoints() {
  return points_.size();
}

Line BrokenLine::getLine(int index) {
  if (index >= points_.size() || index < 0)
    throw length_error("Index error");
  return Line(points_[index], points_[index + 1]);
}

vector<Point> BrokenLine::intersect(Base& base_obj) {
  return base_obj.intersect(*this);
}

vector<Point> BrokenLine::intersect(Line& line) {
  return line.intersect(*this);
}

vector<Point> BrokenLine::intersect(BrokenLine& polyline) {
  vector<Point> result;
  for (int i = 0; i < this->numPoints() - 1; i++) {
    Line line = this->getLine(i);
    vector<Point> points = line.intersect(polyline);
    for (int j = 0; j < points.size(); j++)
      result.push_back(points[j]);
  }
  return result;
}

vector<Point> BrokenLine::intersect(Circle& circle) {
  vector<Point> result;
  for (int i = 0; i < this->numPoints() - 1; i++) {
    Line line = this->getLine(i);
    vector<Point> points = line.intersect(circle);
    for (int j = 0; j < points.size(); j++)
      result.push_back(points[j]);
  }
  return result;
}

Point Circle::getCentre() {
  return centre_;
}

double Circle::getRadius() {
  return radius_;
}

vector<Point> Circle::intersect(Base& base_obj) {
  return base_obj.intersect(*this);
}

vector<Point> Circle::intersect(Line& line) {
  return line.intersect(*this);
}

vector<Point> Circle::intersect(BrokenLine& polyline) {
  return polyline.intersect(*this);
}

vector<Point> Circle::intersect(Circle& circle) {
  //http://www.litunovskiy.com/gamedev/intersection_of_two_circles/
  vector<Point> result;

  double d = sqrt(pow(circle.centre_.x - this->centre_.x, 2) + pow(circle.centre_.y - this->centre_.y, 2));
  bool nesting = abs(circle.radius_ - this->radius_) > d;
  bool no_intersection = d > circle.radius_ + this->radius_;
  if (!(nesting || no_intersection)) {
    double b =(pow(this->radius_, 2) - pow(circle.radius_, 2) + pow(d, 2)) / (2 * d);
    double a = d - b;
    Point p0;
    p0.x = circle.centre_.x + a / d * (this->centre_.x - circle.centre_.x);
    p0.y = circle.centre_.y + a / d * (this->centre_.y - circle.centre_.y);
    if (d == circle.radius_ + this->radius_)
      result.push_back(p0);
    else {
      double h = sqrt(pow(circle.radius_, 2) - pow(a, 2));
      Point p3, p4;
      p3.x = p0.x + (this->centre_.y - circle.centre_.y) * h / d;
      p3.y = p0.y - (this->centre_.x - circle.centre_.x) * h / d;
      p4.x = p0.x - (this->centre_.y - circle.centre_.y) * h / d;
      p4.y = p0.y + (this->centre_.x - circle.centre_.x) * h / d;
      result.push_back(p3);
      result.push_back(p4);
    }
    return result;
  }

  return result;
}