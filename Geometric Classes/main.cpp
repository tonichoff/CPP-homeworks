#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "geometry.h"
#include <vector>

using namespace std;

TEST_CASE("Contain objects of different classes in one vector") {
  vector<Base*> vec(3);
  Point a, b;
  a.x = 0.0; a.y = 2.0;
  b.x = 4.0; b.y = 6.0;
  vec[0] = new Line(a, b);

  vector<Point> points(3);
  points[0].x = 5.0;  points[0].y = 3.0;
  points[1].x = -6.0; points[1].y = 8.0;
  points[2].x = 11.0; points[2].y = -7.0;
  vec[1] = new BrokenLine(points);

  Point c;
  c.x = 4.0; c.y = -9.0;
  vec[2] = new Circle(c, 8.0);

  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      vector<Point> result = vec[i]->intersect(*vec[j]);
}

TEST_CASE("Intersect line with line", "[]") {
  SECTION("Simple intersection") {
    Point a, b, c, d;
    a.x = 0.0; a.y = 0.0; b.x = 4.0; b.y = 4.0;
    c.x = 2.0; c.y = 0.0; d.x = 2.0; d.y = 4.0;

    Line line1(a, b);
    Line line2(c, d);

    Point expected_point(2.0, 2.0);
    vector<Point> result = line1.intersect(line2);

    REQUIRE(result[0].x == expected_point.x);
    REQUIRE(result[0].y == expected_point.y);
  }

  SECTION("No intersection") {
    Point a, b, c, d;
    a.x = 0.0; a.y = 0.0; b.x = 4.0; b.y = 4.0;
    c.x = 2.0; c.y = 3.0; d.x = 2.0; d.y = 4.0;

    Line line1(a, b);
    Line line2(c, d);

    vector<Point> result = line1.intersect(line2);

    REQUIRE(result.empty() == true);
  }

  SECTION("Same point") {
    Point a, b, c, d;
    a.x = 0.0; a.y = 0.0; b.x = 4.0; b.y = 4.0;
    c.x = 0.0; c.y = 0.0; d.x = 0.0; d.y = 2.0;

    Line line1(a, b);
    Line line2(c, d);

    Point expected_point(0.0, 0.0);
    vector<Point> result = line1.intersect(line2);

    REQUIRE(result[0].x == expected_point.x);
    REQUIRE(result[0].y == expected_point.y);
  }

  SECTION("Same lines") {
    Point a, b, c, d;
    a.x = 0.0; a.y = 0.0; b.x = 4.0; b.y = 4.0;
    c.x = 0.0; c.y = 0.0; d.x = 4.0; d.y = 4.0;

    Line line1(a, b);
    Line line2(c, d);

    vector<Point> result = line1.intersect(line2);

    REQUIRE(result.empty() == true);
  }

  SECTION("Very close not intersecting lines") {
    Point a, b, c, d;
    a.x = 0.0; a.y = 0.0; b.x = 4.0; b.y = 4.0;
    c.x = 2.0; c.y = 0.0; d.x = 2.0; d.y = 1.9999999999999;

    Line line1(a, b);
    Line line2(c, d);

    vector<Point> result = line1.intersect(line2);

    REQUIRE(result.empty() == true);
  }

}

TEST_CASE("Get line from broken line with wrong index", "[]") {
  vector<Point> points(3);
  points[0].x = 6.0; points[0].y = 4.0;
  points[1].x = 0.0; points[1].y = 1.0;
  points[2].x = 5.0; points[2].y = 1.0;
  BrokenLine broken_line(points);
  
  try {
    Line line = broken_line.getLine(4);
  }
  catch (length_error exp) {
    string mes = exp.what();
    REQUIRE(mes == "Index error");
  }
}

TEST_CASE("Intersect line with broken line", "[]") {
  vector<Point> points(4);
  points[0].x = 0.0; points[0].y = 0.0; points[1].x = 4.0; points[1].y = 4.0;
  points[2].x = 2.0; points[2].y = 0.0; points[3].x = 2.0; points[3].y = 4.0;
  BrokenLine broken_line(points);

  Point a, b;
  a.x = 0.0; a.y = 3.0; b.x = 4.0; b.y = 3.0;
  Line line(a, b);

  vector<Point> expected_points(3);
  expected_points[0].x = 3.0; expected_points[0].y = 3.0;
  expected_points[1].x = 3.5; expected_points[1].y = 3.0;
  expected_points[2].x = 2.0; expected_points[2].y = 3.0;
  SECTION("line.intersect") {
    vector<Point> result = line.intersect(broken_line);
    REQUIRE(result.size() == expected_points.size());
    for (int i = 0; i < result.size(); i++) {
      REQUIRE(result[i].x == expected_points[i].x);
      REQUIRE(result[i].y == expected_points[i].y);
    }
    SECTION("broken_line.intersect") {
      vector<Point> result = broken_line.intersect(line);
      REQUIRE(result.size() == expected_points.size());
      for (int i = 0; i < result.size(); i++) {
        REQUIRE(result[i].x == expected_points[i].x);
        REQUIRE(result[i].y == expected_points[i].y);
      }
    }
  }
}

TEST_CASE("Intersect broken line with broken line") {
  vector<Point> points(3);
  points[0].x = 6.0; points[0].y = 4.0;
  points[1].x = 0.0; points[1].y = 1.0;
  points[2].x = 5.0; points[2].y = 1.0;
  BrokenLine first_bline(points);

  points[0].x = 1.0; points[0].y = 3.0;
  points[1].x = 4.0; points[1].y = 0.0;
  points[2].x = 4.0; points[2].y = 4.0;
  BrokenLine second_bline(points);

  vector<Point> expected_points(4);
  expected_points[0].x = 2.0; expected_points[0].y = 2.0;
  expected_points[1].x = 4.0; expected_points[1].y = 3.0;
  expected_points[2].x = 3.0; expected_points[2].y = 1.0;
  expected_points[3].x = 4.0; expected_points[3].y = 1.0;

  vector<Point> result = first_bline.intersect(second_bline);
  REQUIRE(result.size() == expected_points.size());
  for (int i = 0; i < result.size(); i++) {
    REQUIRE(result[i].x == expected_points[i].x);
    REQUIRE(result[i].y == expected_points[i].y);
  }
}

TEST_CASE("Intersect line with circle") {
  SECTION("Simple intersection") {
    Point a; a.x = 2.0; a.y = 2.0;
    Circle circle(a, 2);

    Point b; b.x = 1.0; b.y = 5.0;
    Point c; c.x = 5.0; c.y = 1.0;
    Line line(b, c);

    vector<Point> expected_points(2);
    expected_points[0].x = 4.0;
    expected_points[0].y = 2.0;
    expected_points[1].x = 2.0;
    expected_points[1].y = 4.0;

    vector<Point> result = line.intersect(circle);
    REQUIRE(result.size() == 2);
    for (int i = 0; i < result.size(); i++) {
      REQUIRE(expected_points[i].x == result[i].x);
      REQUIRE(expected_points[i].y == result[i].y);
    }
  }

  SECTION("No intersection") {
    Point a; a.x = 2.0; a.y = 2.0;
    Circle circle(a, 2);

    Point b; b.x = 5.0; b.y = 5.0;
    Point c; c.x = 5.0; c.y = 1.0;
    Line line(b, c);

    vector<Point> result = circle.intersect(line);
    REQUIRE(result.size() == 0);
  }

  SECTION("In one point") {
    Point a; a.x = 2.0; a.y = 2.0;
    Circle circle(a, 2);

    Point b; b.x = 4.0; b.y = 5.0;
    Point c; c.x = 4.0; c.y = 1.0;
    Line line(b, c);

    Point expected_point;
    expected_point.x = 4.0;
    expected_point.y = 2.0;

    vector<Point> result = line.intersect(circle);
    REQUIRE(result.size() == 1);
    REQUIRE(expected_point.x == result[0].x);
    REQUIRE(expected_point.y == result[0].y);
  }
}

TEST_CASE("Intersect broken line with circle") {
  vector<Point> points(4);
  points[0].x = 2.0;  points[0].y = 5.0;
  points[1].x = 2.0;  points[1].y = -1.0;
  points[2].x = 4.0;  points[2].y = -1.0;
  points[3].x = 4.0;  points[3].y = 5.0;
  BrokenLine broken_line(points);

  Point a; a.x = 2.0; a.y = 2.0;
  Circle circle(a, 2.0);

  vector<Point> expected_points(3);
  expected_points[0].x = 2.0;  expected_points[0].y = 0.0;
  expected_points[1].x = 2.0; expected_points[1].y = 2.0;
  expected_points[2].x = 4.0;  expected_points[2].y = 2.0;

  vector<Point> result = circle.intersect(broken_line);
  REQUIRE(result.size() == 3);
  for (int i = 0; i < result.size(); i++) {
    REQUIRE(expected_points[i].x == result[i].x);
    REQUIRE(expected_points[i].y == result[i].y);
  }
}

TEST_CASE("Intersect circle with circle", "[]") {
  SECTION("Intersect in two points") {
    Point first_centre;
    first_centre.x = 0.0;
    first_centre.y = 0.0;
    Circle first_circle(first_centre, 2.0);

    Point second_centre;
    second_centre.x = 2.0;
    second_centre.y = 2.0;
    Circle second_circle(second_centre, 2.0);

    vector<Point> expected_points(2);
    expected_points[0].x = 0.0;
    expected_points[0].y = 2.0;
    expected_points[1].x = 2.0;
    expected_points[1].y = 0.0;
  
    vector<Point> result = first_circle.intersect(second_circle);
    REQUIRE(result.size() == expected_points.size());
    for (int i = 0; i < result.size(); i++) {
      REQUIRE(result[i].x == expected_points[i].x);
      REQUIRE(result[i].y == expected_points[i].y);
    }
  }

  SECTION("Intersect in one points") {
    Point first_centre;
    first_centre.x = 0.0;
    first_centre.y = 0.0;
    Circle first_circle(first_centre, 3.0);

    Point second_centre;
    second_centre.x = 0.0;
    second_centre.y = 6.0;
    Circle second_circle(second_centre, 3.0);

    vector<Point> expected_points(1);
    expected_points[0].x = 0.0;
    expected_points[0].y = 3.0;

    vector<Point> result = first_circle.intersect(second_circle);
    REQUIRE(result.size() == expected_points.size());
    for (int i = 0; i < result.size(); i++) {
      REQUIRE(result[i].x == expected_points[i].x);
      REQUIRE(result[i].y == expected_points[i].y);
    }
  }

  SECTION("No intersection") {
    Point first_centre;
    first_centre.x = 0.0;
    first_centre.y = 0.0;
    Circle first_circle(first_centre, 3.0);

    Point second_centre;
    second_centre.x = 5.0;
    second_centre.y = 0.0;
    Circle second_circle(second_centre, 1.0);

    vector<Point> result = first_circle.intersect(second_circle);
    REQUIRE(result.size() == 0);
  }

  SECTION("Nested circles") {
    Point first_centre;
    first_centre.x = 3.0;
    first_centre.y = 3.0;
    Circle first_circle(first_centre, 3.0);

    Point second_centre;
    second_centre.x = 3.0;
    second_centre.y = 3.0;
    Circle second_circle(second_centre, 1.0);

    vector<Point> result = first_circle.intersect(second_circle);
    REQUIRE(result.size() == 0);
  }
}