#ifndef IMAGEDATAMANIPULATOR_H
#define IMAGEDATAMANIPULATOR_H
#pragma once

#include <QImage>
#include <QRgb>
#include <QColor>
#include <QVector>
#include <QLabel>
#include <QMouseEvent>
#include <QDebug>
#include <iostream>
#include <math.h>
#include <tuple>

typedef struct vec3 {
    float x;
    float y;
    float z;
} vec3;

typedef struct vec2 {
    float x;
    float y;
} vec2;

inline vec3 cross(vec3 a, vec3 b)
{
    vec3 r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    return r;
}

inline float dot(vec3 a, vec3 b) { return a.x * b.x + a.y * b.y + b.z * b.z; }

inline float dot(vec2 a, vec2 b) { return a.x * b.x + a.y * b.y; }

inline float length(vec3 in) { return sqrt(dot(in, in)); }

inline float length(vec2 in) { return sqrt(dot(in, in)); }

inline float lerp(float a, float b, float t) { return t * b + (1.f - t) * a; }

inline vec3 lerp(vec3 a, vec3 b, float t)
{
    vec3 r;
    r.x = lerp(a.x, b.x, t);
    r.y = lerp(a.y, b.y, t);
    r.z = lerp(a.z, b.z, t);
    return r;
}

inline vec2 lerp(vec2 a, vec2 b, float t)
{
    vec2 r;
    r.x = lerp(a.x, b.x, t);
    r.y = lerp(a.y, b.y, t);
    return r;
}

inline vec2 rvec2() {
    float rx = ((float) rand() / (RAND_MAX));
    rx = 2.f * rx - 1.f;
    float ry = ((float) rand() / (RAND_MAX));
    ry = 2.f * ry - 1.f;
    vec2 r = {rx, ry};
    float l = length(r);
    r = {r.x / l, r.y / l};
    return r;
}

inline vec3 normalize(vec3 in) {
    float l = length(in);
    vec3 r = {0.f, 0.f, 0.f};
    if (l < 0.001f) return r;
    r = {in.x / l, in.y / l, in.z / l};
    return r;
}

inline vec2 normalize(vec2 in) {
    float l = length(in);
    vec2 r = {0.f, 0.f};
    if (l < 0.001f) return {0.f, 0.f};
    r = {in.x / l, in.y / l};
    return r;
}

inline float clamp(float min, float max, float t) {
    return std::min(max, std::max(min, t));
}

typedef struct plantParams {
    plantParams(float agem,
                float tmini, float tmaxi,
                float tmin, float tmax,
                float smini, float smaxi,
                float smin, float smax,
                float wmini, float wmaxi,
                float wmin, float wmax):
        ageMax(agem)
      , tempMinIdeal(tmini)
      , tempMaxIdeal(tmaxi)
      , tempMin(tmin)
      , tempMax(tmax)
      , sunMinIdeal(smini)
      , sunMaxIdeal(smaxi)
      , sunMin(smin)
      , sunMax(smax)
      , waterMinIdeal(wmini)
      , waterMaxIdeal(wmaxi)
      , waterMin(wmin)
      , waterMax(wmax) {}
    float ageMax;
    float tempMinIdeal;
    float tempMaxIdeal;
    float tempMin;
    float tempMax;
    float sunMinIdeal;
    float sunMaxIdeal;
    float sunMin;
    float sunMax;
    float waterMinIdeal;
    float waterMaxIdeal;
    float waterMin;
    float waterMax;
    float growth(float water, float temp, float sun) {
        float vw = clamp(-1.0f, 1.0f, (water - waterMin) / (waterMinIdeal - waterMin)) * clamp(-1.0f, 1.0f, (water - waterMaxIdeal) / (waterMax - waterMaxIdeal));
        float vt = clamp(-1.0f, 1.0f, (temp - tempMin) / (tempMinIdeal - tempMin)) * clamp(-1.0f, 1.0f, (temp - tempMaxIdeal) / (tempMax - tempMaxIdeal));
        float vs = clamp(-1.0f, 1.0f, (sun - sunMin) / (sunMinIdeal - sunMin)) * clamp(-1.0f, 1.0f, (sun - sunMaxIdeal) / (sunMax - sunMaxIdeal));

        return std::min(std::min(vw, vt), vs);
    }
} plantParams;

typedef struct heightTemp {
    heightTemp(float loElev, float hiElev): loElevT(loElev), hiElevT(hiElev) {}
    float hiElevT;
    float loElevT;
    float get(float h) { return lerp(loElevT, hiElevT, h); }
} heightTemp;

typedef struct terrainCell {
    terrainCell(): moisture(0.5f), sunlight(0.5f) {}
    float moisture; // 0.0 completely dry, 1.0 saturated
    float sunlight; // 0.0 always dark, 1.0 always lit during day
} terrainCell;

typedef struct foliageCell {
    foliageCell(): count(0), heightAvg(0.0f), ageSum(0.0f), density(0.0f) {}
    int count;
    float heightAvg;
    float ageSum;
    float density;
} foliageCell;

typedef struct dropletOptions {
    dropletOptions(): kInertia(0.f), kGrav(0.01f), kEvap(0.02f), kDeposit(0.1f), kAccumulate(0.1f), steps(100), unitDist(1.f / 256.f) {}
    float kInertia;
    float kGrav;
    float kEvap;
    float kDeposit;
    float kAccumulate;
    float unitDist;
    int steps;
} dropletOptions;


class Droplet
{
private:

public:
    Droplet();
    Droplet(float x, float y);
    Droplet(float x, float y, dropletOptions op);
    void move();
    void changeDirection(vec2 dirN);
    void update();
    bool isDead() { return dead; }
    bool checkIfDead();

    // better organized? eh
    float x;
    float y;
    vec2 dir = {0.0f, 0.0f};
    float speed = 0.f;
    float sediment = 0.0f;
    float water = 1.0f;
    bool dead = false;
    dropletOptions params;
    float currentHeight = 0.0f;
};


class ImageDataManipulator
{

private:
    bool usable = true;
    int width = -1;
    int height = -1;
    int size = -1; // w * h

    std::vector<float> heightData;
    std::vector<vec3> normalData;
    std::vector<vec2> gradientData;
    std::vector<foliageCell> foliageData;
    std::vector<terrainCell> soilData;

    QImage* imgRefHeight;
    QImage* imgRefFoliage;

    float normalizeGrayscale(QRgb input);
    std::tuple<vec3, vec2> sobelNormal(int x, int y, float strength);
    QRgb normalToPixel(vec3 n);
    QRgb gradientToPixel(vec2 g);
    QRgb floatsToPixel(vec3 n);
    QRgb grayscaleToPixel(float input);
    QRgb foliageIndexToPixel(int x, int y);

    void ZC(uint64_t x, uint64_t y, uint64_t *d);
    void IZC(uint64_t d, uint64_t *x, uint64_t *y);
    int pxClamp (int p) { return std::max(0, std::min(width - 1, p)); }
    float saturate(float f) {return std::max(0.0f, std::min(1.0f, f)); }
    float smoothstep(float f) { return f * f * (3.0f - 2.0f * f);}
    float mix(float a, float b, float t) {
        t = std::max(0.0f, std::min(1.0f, t));
        return t * b + (1.0 - t) * a;
    }

    void bilinearRepair(vec2 p);
    void radiusRepair(vec2 p, int r);
    void refreshFoliageImage();
    float normalStr = 4.0;

    bool rayMarch(vec2 uv, vec3 dir);

    void recalculateSoilParameters();
    // TODO: replace
    int maxPlantsPerCell = 10.0;
    plantParams plant1 = plantParams(20.0f, 15.0f, 25.0f, 10.0f, 30.0f, 0.3f, 0.9f, 0.1f, 1.0f, 0.3f, 0.8f, 0.2f, 0.9f);
    heightTemp heightToTemp = heightTemp(33.0f, 8.0f); // 30c at 0 elevation, 10c at 1 elevation
public:
    ImageDataManipulator();
    ImageDataManipulator(QImage& image);
    ~ImageDataManipulator() {}

    void setFoliageRef(QImage& img);

    void brushAdd(int x, int y, int r, float amt);
    void refreshRegion(int x, int y, int r);

    void brushPolish(int x, int y, int r, float amt);
    void brushFlatten(int x, int y, int r, float amt);

    float bilinear(std::vector<float> &map, vec2 pos);
    vec2 bilinear(std::vector<vec2> &map, vec2 pos);
    vec3 bilinear(std::vector<vec3> &map, vec2 pos);

    float bilinearAdd(std::vector<float> &map, vec2 pos, float amt);
    void ecosystemEvent(int x, int y);

    void phDoErosion(int dropletCount);
};


#endif // IMAGEDATAMANIPULATOR_H




