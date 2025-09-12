#include "Kmplete/Math/math.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Math GetCenter 2D tests", "[math]")
{
    Kmplete::Rect2I rectInt({0, 0}, {10, 10});
    Kmplete::Point2I centerInt = rectInt.GetCenter();
    REQUIRE((centerInt.x == 5 && centerInt.y == 5));

    rectInt = Kmplete::Rect2I({-20, -20}, {40, 40});
    centerInt = rectInt.GetCenter();
    REQUIRE((centerInt.x == 0 && centerInt.y == 0));

    rectInt = Kmplete::Rect2I({0, 0}, {-10, -10});
    centerInt = rectInt.GetCenter();
    REQUIRE((centerInt.x == -5 && centerInt.y == -5));

    rectInt = Kmplete::Rect2I({-20, 100}, {40, -10});
    centerInt = rectInt.GetCenter();
    REQUIRE((centerInt.x == 0 && centerInt.y == 95));

    rectInt = Kmplete::Rect2I({11, 11}, {5, 5});
    centerInt = rectInt.GetCenter();
    REQUIRE((centerInt.x == 13/*.5*/ && centerInt.y == 13/*.5*/));

    rectInt = Kmplete::Rect2I({9, 9}, {0, 0});
    centerInt = rectInt.GetCenter();
    REQUIRE((centerInt.x == 9 && centerInt.y == 9));


    Kmplete::Rect2F rectFloat({0.0f, 0.0f}, {10.0f, 10.0f});
    Kmplete::Point2F centerFloat = rectFloat.GetCenter();
    REQUIRE((centerFloat.x == 5.0f && centerFloat.y == 5.0f));

    rectFloat = Kmplete::Rect2F({-20.0f, -20.0f}, {40.0f, 40.0f});
    centerFloat = rectFloat.GetCenter();
    REQUIRE((centerFloat.x == 0.0f && centerFloat.y == 0.0f));

    rectFloat = Kmplete::Rect2F({0.0f, 0.0f}, {-10.0f, -10.0f});
    centerFloat = rectFloat.GetCenter();
    REQUIRE((centerFloat.x == -5.0f && centerFloat.y == -5.0f));

    rectFloat = Kmplete::Rect2F({-20.0f, 100.0f}, {40.0f, -10.0f});
    centerFloat = rectFloat.GetCenter();
    REQUIRE((centerFloat.x == 0.0f && centerFloat.y == 95.0f));

    rectFloat = Kmplete::Rect2F({11.0f, 11.0f}, {5.0f, 5.0f});
    centerFloat = rectFloat.GetCenter();
    REQUIRE((centerFloat.x == 13.5f && centerFloat.y == 13.5f));

    rectFloat = Kmplete::Rect2F({9.0f, 9.0f}, {0.0f, 0.0f});
    centerFloat = rectFloat.GetCenter();
    REQUIRE((centerFloat.x == 9.0f && centerFloat.y == 9.0f));
}
//--------------------------------------------------------------------------

TEST_CASE("Math GetCenter 3D tests", "[math]")
{
    Kmplete::Rect3I rectInt({ 0, 0, 0 }, { 10, 10, 10 });
    Kmplete::Point3I centerInt = rectInt.GetCenter();
    REQUIRE((centerInt.x == 5 && centerInt.y == 5 && centerInt.z == 5));

    rectInt = Kmplete::Rect3I({ -20, -20, -20 }, { 40, 40, 40 });
    centerInt = rectInt.GetCenter();
    REQUIRE((centerInt.x == 0 && centerInt.y == 0 && centerInt.z == 0));

    rectInt = Kmplete::Rect3I({ 0, 0, 0 }, { -10, -10, -10 });
    centerInt = rectInt.GetCenter();
    REQUIRE((centerInt.x == -5 && centerInt.y == -5 && centerInt.z == -5));

    rectInt = Kmplete::Rect3I({ -20, 100, 200 }, { 40, -10, 40 });
    centerInt = rectInt.GetCenter();
    REQUIRE((centerInt.x == 0 && centerInt.y == 95 && centerInt.z == 220));

    rectInt = Kmplete::Rect3I({ 11, 11, 11 }, { 5, 5, 5 });
    centerInt = rectInt.GetCenter();
    REQUIRE((centerInt.x == 13/*.5*/ && centerInt.y == 13/*.5*/ && centerInt.z == 13/*.5*/));

    rectInt = Kmplete::Rect3I({ 9, 9, 9 }, { 0, 0, 0 });
    centerInt = rectInt.GetCenter();
    REQUIRE((centerInt.x == 9 && centerInt.y == 9 && centerInt.z == 9));


    Kmplete::Rect3F rectFloat({ 0.0f, 0.0f, 0.0f }, { 10.0f, 10.0f, 10.0f });
    Kmplete::Point3F centerFloat = rectFloat.GetCenter();
    REQUIRE((centerFloat.x == 5.0f && centerFloat.y == 5.0f && centerFloat.z == 5.0f));

    rectFloat = Kmplete::Rect3F({ -20.0f, -20.0f, -20.0f }, { 40.0f, 40.0f, 40.0f });
    centerFloat = rectFloat.GetCenter();
    REQUIRE((centerFloat.x == 0.0f && centerFloat.y == 0.0f, centerFloat.z == 0.0f));

    rectFloat = Kmplete::Rect3F({ 0.0f, 0.0f, 0.0f }, { -10.0f, -10.0f, -10.0f });
    centerFloat = rectFloat.GetCenter();
    REQUIRE((centerFloat.x == -5.0f && centerFloat.y == -5.0f && centerFloat.z == -5.0f));

    rectFloat = Kmplete::Rect3F({ -20.0f, 100.0f, 200.0f }, { 40.0f, -10.0f, 40.0f });
    centerFloat = rectFloat.GetCenter();
    REQUIRE((centerFloat.x == 0.0f && centerFloat.y == 95.0f, centerFloat.z == 220.0f));

    rectFloat = Kmplete::Rect3F({ 11.0f, 11.0f, 11.0f }, { 5.0f, 5.0f, 5.0f });
    centerFloat = rectFloat.GetCenter();
    REQUIRE((centerFloat.x == 13.5f && centerFloat.y == 13.5f && centerFloat.z == 13.5f));

    rectFloat = Kmplete::Rect3F({ 9.0f, 9.0f, 9.0f }, { 0.0f, 0.0f, 0.0f });
    centerFloat = rectFloat.GetCenter();
    REQUIRE((centerFloat.x == 9.0f && centerFloat.y == 9.0f && centerFloat.z == 9.0f));
}
//--------------------------------------------------------------------------

TEST_CASE("Math ContainsPoint 2D tests", "[math]")
{
    Kmplete::Rect2I rectInt({0, 0}, {10, 10});
    Kmplete::Point2I point(5, 5);
    REQUIRE(rectInt.ContainsPoint(point));

    point = Kmplete::Point2I(10, 10);
    REQUIRE(rectInt.ContainsPoint(point));

    point = Kmplete::Point2I(5, 11);
    REQUIRE_FALSE(rectInt.ContainsPoint(point));

    point = Kmplete::Point2I(-10, 5);
    REQUIRE_FALSE(rectInt.ContainsPoint(point));

    rectInt = Kmplete::Rect2I({10, 10}, {10, 10});
    point = Kmplete::Point2I(5, 5);
    REQUIRE_FALSE(rectInt.ContainsPoint(point));

    point = Kmplete::Point2I(20, 10);
    REQUIRE(rectInt.ContainsPoint(point));

    point = Kmplete::Point2I(100, 100);
    REQUIRE_FALSE(rectInt.ContainsPoint(point));

    point = Kmplete::Point2I(-15, -15);
    REQUIRE_FALSE(rectInt.ContainsPoint(point));

    rectInt = Kmplete::Rect2I({10, 10}, {-20, -20});
    point = Kmplete::Point2I(0, 0);
    REQUIRE(rectInt.ContainsPoint(point));

    point = Kmplete::Point2I(-10, 10);
    REQUIRE(rectInt.ContainsPoint(point));

    point = Kmplete::Point2I(-10, 11);
    REQUIRE_FALSE(rectInt.ContainsPoint(point));

    rectInt = Kmplete::Rect2I({0, 0}, {0, 0});
    point = Kmplete::Point2I(0, 0);
    REQUIRE(rectInt.ContainsPoint(point));

    point = Kmplete::Point2I(1, 0);
    REQUIRE_FALSE(rectInt.ContainsPoint(point));

    point = Kmplete::Point2I(0, -1);
    REQUIRE_FALSE(rectInt.ContainsPoint(point));


    Kmplete::Rect2F rectFloat({ 0.0f, 0.0f }, { 10.0f, 10.0f });
    Kmplete::Point2F pointF(5.0f, 5.0f);
    REQUIRE(rectFloat.ContainsPoint(pointF));

    pointF = Kmplete::Point2F(10.0f, 10.0f);
    REQUIRE(rectFloat.ContainsPoint(pointF));

    pointF = Kmplete::Point2F(5.0f, 11.0f);
    REQUIRE_FALSE(rectFloat.ContainsPoint(pointF));

    pointF = Kmplete::Point2F(-10.0f, 5.0f);
    REQUIRE_FALSE(rectFloat.ContainsPoint(pointF));

    rectFloat = Kmplete::Rect2F({ 10.0f, 10.0f }, { 10.0f, 10.0f });
    pointF = Kmplete::Point2F(5.0f, 5.0f);
    REQUIRE_FALSE(rectFloat.ContainsPoint(pointF));

    pointF = Kmplete::Point2F(20.0f, 10.0f);
    REQUIRE(rectFloat.ContainsPoint(pointF));

    pointF = Kmplete::Point2F(100.0f, 100.0f);
    REQUIRE_FALSE(rectFloat.ContainsPoint(pointF));

    pointF = Kmplete::Point2F(-15.0f, -15.0f);
    REQUIRE_FALSE(rectFloat.ContainsPoint(pointF));

    rectFloat = Kmplete::Rect2F({ 10.0f, 10.0f }, { -20.0f, -20.0f });
    pointF = Kmplete::Point2F(0.0f, 0.0f);
    REQUIRE(rectFloat.ContainsPoint(pointF));

    pointF = Kmplete::Point2F(-10.0f, 10.0f);
    REQUIRE(rectFloat.ContainsPoint(pointF));

    pointF = Kmplete::Point2F(-10.0f, 11.0f);
    REQUIRE_FALSE(rectFloat.ContainsPoint(pointF));

    rectFloat = Kmplete::Rect2F({ 0.0f, 0.0f }, { 0.0f, 0.0f });
    pointF = Kmplete::Point2F(0.0f, 0.0f);
    REQUIRE(rectFloat.ContainsPoint(pointF));

    pointF = Kmplete::Point2F(1.0f, 0.0f);
    REQUIRE_FALSE(rectFloat.ContainsPoint(pointF));

    pointF = Kmplete::Point2F(0.0f, -1.0f);
    REQUIRE_FALSE(rectFloat.ContainsPoint(pointF));
}
//--------------------------------------------------------------------------

TEST_CASE("Math ContainsPoint 3D tests", "[math]")
{
    Kmplete::Rect3I rectInt({ 0, 0, 0 }, { 10, 10, 10 });
    Kmplete::Point3I point(5, 5, 5);
    REQUIRE(rectInt.ContainsPoint(point));

    point = Kmplete::Point3I(10, 10, 10);
    REQUIRE(rectInt.ContainsPoint(point));

    point = Kmplete::Point3I(5, 11, 5);
    REQUIRE_FALSE(rectInt.ContainsPoint(point));

    point = Kmplete::Point3I(-10, 5, 1);
    REQUIRE_FALSE(rectInt.ContainsPoint(point));

    rectInt = Kmplete::Rect3I({ 10, 10, 10 }, { 10, 10, 10 });
    point = Kmplete::Point3I(5, 5, 15);
    REQUIRE_FALSE(rectInt.ContainsPoint(point));

    point = Kmplete::Point3I(20, 10, 20);
    REQUIRE(rectInt.ContainsPoint(point));

    point = Kmplete::Point3I(100, 100, 10);
    REQUIRE_FALSE(rectInt.ContainsPoint(point));

    point = Kmplete::Point3I(-15, -15, -15);
    REQUIRE_FALSE(rectInt.ContainsPoint(point));

    rectInt = Kmplete::Rect3I({ 10, 10, 10 }, { -20, -20, -20 });
    point = Kmplete::Point3I(0, 0, 0);
    REQUIRE(rectInt.ContainsPoint(point));

    point = Kmplete::Point3I(-10, 10, 5);
    REQUIRE(rectInt.ContainsPoint(point));

    point = Kmplete::Point3I(-10, 11, -10);
    REQUIRE_FALSE(rectInt.ContainsPoint(point));

    rectInt = Kmplete::Rect3I({ 0, 0, 4 }, { 0, 0, 4 });
    point = Kmplete::Point3I(0, 0, 4);
    REQUIRE(rectInt.ContainsPoint(point));

    point = Kmplete::Point3I(1, 0, 4);
    REQUIRE_FALSE(rectInt.ContainsPoint(point));

    point = Kmplete::Point3I(0, -1, 0);
    REQUIRE_FALSE(rectInt.ContainsPoint(point));


    Kmplete::Rect3F rectFloat({ 0.0f, 0.0f, 0.0f }, { 10.0f, 10.0f, 10.0f });
    Kmplete::Point3F pointF(5.0f, 5.0f, 5.0f);
    REQUIRE(rectFloat.ContainsPoint(pointF));

    pointF = Kmplete::Point3F(10.0f, 10.0f, 10.0f);
    REQUIRE(rectFloat.ContainsPoint(pointF));

    pointF = Kmplete::Point3F(5.0f, 6.0f, 11.0f);
    REQUIRE_FALSE(rectFloat.ContainsPoint(pointF));

    pointF = Kmplete::Point3F(-10.0f, 5.0f, 5.0f);
    REQUIRE_FALSE(rectFloat.ContainsPoint(pointF));

    rectFloat = Kmplete::Rect3F({ 10.0f, 10.0f, 10.0f }, { 10.0f, 10.0f, 1.0f });
    pointF = Kmplete::Point3F(5.0f, 5.0f, 5.0f);
    REQUIRE_FALSE(rectFloat.ContainsPoint(pointF));

    pointF = Kmplete::Point3F(20.0f, 10.0f, 10.0f);
    REQUIRE(rectFloat.ContainsPoint(pointF));

    pointF = Kmplete::Point3F(100.0f, 100.0f, 0.0f);
    REQUIRE_FALSE(rectFloat.ContainsPoint(pointF));

    pointF = Kmplete::Point3F(-15.0f, -15.0f, 0.0f);
    REQUIRE_FALSE(rectFloat.ContainsPoint(pointF));

    rectFloat = Kmplete::Rect3F({ 10.0f, 10.0f, 10.0f }, { -20.0f, -20.0f, -20.0f });
    pointF = Kmplete::Point3F(0.0f, 0.0f, 0.0f);
    REQUIRE(rectFloat.ContainsPoint(pointF));

    pointF = Kmplete::Point3F(-10.0f, 10.0f, -10.0f);
    REQUIRE(rectFloat.ContainsPoint(pointF));

    pointF = Kmplete::Point3F(-10.0f, 0.0f, 100.0f);
    REQUIRE_FALSE(rectFloat.ContainsPoint(pointF));

    rectFloat = Kmplete::Rect3F({ 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 5.0f });
    pointF = Kmplete::Point3F(0.0f, 0.0f, 5.0f);
    REQUIRE(rectFloat.ContainsPoint(pointF));

    pointF = Kmplete::Point3F(1.0f, 0.0f, 5.0f);
    REQUIRE_FALSE(rectFloat.ContainsPoint(pointF));

    pointF = Kmplete::Point3F(0.0f, -1.0f, 5.0f);
    REQUIRE_FALSE(rectFloat.ContainsPoint(pointF));
}
//--------------------------------------------------------------------------