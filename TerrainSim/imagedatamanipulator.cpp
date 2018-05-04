#include "imagedatamanipulator.h"



/// Z Curve Utilities

// z curve encode
void ImageDataManipulator::ZC(uint64_t x, uint64_t y, uint64_t *d)
{
    x = std::min(std::max((uint64_t) 0, x), uint64_t(this->width - 1));
    y = std::min(std::max((uint64_t) 0, y), (uint64_t)(this->height - 1));
    x = (x | (x << 16)) & 0x0000FFFF0000FFFF;
    x = (x | (x << 8)) & 0x00FF00FF00FF00FF;
    x = (x | (x << 4)) & 0x0F0F0F0F0F0F0F0F;
    x = (x | (x << 2)) & 0x3333333333333333;
    x = (x | (x << 1)) & 0x5555555555555555;

    y = (y | (y << 16)) & 0x0000FFFF0000FFFF;
    y = (y | (y << 8)) & 0x00FF00FF00FF00FF;
    y = (y | (y << 4)) & 0x0F0F0F0F0F0F0F0F;
    y = (y | (y << 2)) & 0x3333333333333333;
    y = (y | (y << 1)) & 0x5555555555555555;

    *d = x | (y << 1);
}

// extract even bits
uint64_t m_1(uint64_t x)
{
    x = x & 0x5555555555555555;
    x = (x | (x >> 1)) & 0x3333333333333333;
    x = (x | (x >> 2)) & 0x0F0F0F0F0F0F0F0F;
    x = (x | (x >> 4)) & 0x00FF00FF00FF00FF;
    x = (x | (x >> 8)) & 0x0000FFFF0000FFFF;
    x = (x | (x >> 16)) & 0xFFFFFFFFFFFFFFFF;
    return x;
}

// z curve decode / inverse
void ImageDataManipulator::IZC(uint64_t d, uint64_t *x, uint64_t *y)
{
    d = std::min(std::max((uint64_t)0, d), (uint64_t) (this->size - 1));
    *x = m_1(d);
    *y = m_1(d >> 1);
}

/// Sampling

float ImageDataManipulator::bilinearDensity(std::vector<foliageCell> &map, vec2 pos)
{
    float fw = (float) width;
    float fx = fw * pos.x;
    float fy = fw * pos.y;

    int x0 = (int) floor(fx);
    int y0 = (int) floor(fy);
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    // coordinates in z curve space
    uint64_t i00 = 0;
    uint64_t i01 = 0;
    uint64_t i10 = 0;
    uint64_t i11 = 0;
    ZC(x0, y0, &i00);
    ZC(x1, y0, &i01);
    ZC(x0, y1, &i10);
    ZC(x1, y1, &i11);

    float d0 = lerp(map[i00].density, map[i01].density, fx - x0);
    float d1 = lerp(map[i10].density, map[i11].density, fx - x0);
    float d = lerp(d0, d1, fy - y0);
    d0 = lerp(map[i00].grassDensity, map[i01].grassDensity, fx - x0);
    d1 = lerp(map[i10].grassDensity, map[i11].grassDensity, fx - x0);
    return lerp(d, lerp(d0, d1, fy - y0), 0.25f);
}

float ImageDataManipulator::bilinear(std::vector<float> &map, vec2 pos)
{
    float fw = (float) width;
    float fx = fw * pos.x;
    float fy = fw * pos.y;

    int x0 = (int) floor(fx);
    int y0 = (int) floor(fy);
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    // coordinates in z curve space
    uint64_t i00 = 0;
    uint64_t i01 = 0;
    uint64_t i10 = 0;
    uint64_t i11 = 0;
    ZC(x0, y0, &i00);
    ZC(x1, y0, &i01);
    ZC(x0, y1, &i10);
    ZC(x1, y1, &i11);

    float h0 = lerp(map[i00], map[i01], fx - x0);
    float h1 = lerp(map[i10], map[i11], fx - x0);
    return lerp(h0, h1, fy - y0);
}

vec2 ImageDataManipulator::bilinear(std::vector<vec2> &map, vec2 pos)
{
    float fw = (float) width;
    float fx = fw * pos.x;
    float fy = fw * pos.y;

    int x0 = (int) floor(fx);
    int y0 = (int) floor(fy);
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    // coordinates in z curve space
    uint64_t i00 = 0;
    uint64_t i01 = 0;
    uint64_t i10 = 0;
    uint64_t i11 = 0;
    ZC(x0, y0, &i00);
    ZC(x1, y0, &i01);
    ZC(x0, y1, &i10);
    ZC(x1, y1, &i11);

    vec2 g0 = lerp(map[i00], map[i01], fx - x0);
    vec2 g1 = lerp(map[i10], map[i11], fx - x0);
    vec2 grad = lerp(g0, g1, fy - y0);
    return normalize(grad);
}

vec3 ImageDataManipulator::bilinear(std::vector<vec3> &map, vec2 pos)
{
    float fw = (float) width;
    float fx = fw * pos.x;
    float fy = fw * pos.y;

    int x0 = (int) floor(fx);
    int y0 = (int) floor(fy);
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    // coordinates in z curve space
    uint64_t i00 = 0;
    uint64_t i01 = 0;
    uint64_t i10 = 0;
    uint64_t i11 = 0;
    ZC(x0, y0, &i00);
    ZC(x1, y0, &i01);
    ZC(x0, y1, &i10);
    ZC(x1, y1, &i11);

    vec3 n0 = lerp(map[i00], map[i01], fx - x0);
    vec3 n1 = lerp(map[i10], map[i11], fx - x0);
    vec3 nor = lerp(n0, n1, fy - y0);
    return normalize(nor);
}

float ImageDataManipulator::bilinearAdd(std::vector<float> &map, vec2 pos, float amt)
{
    float fw = (float) width;
    float fx = fw * pos.x;
    float fy = fw * pos.y;

    int x0 = (int) floor(fx);
    int y0 = (int) floor(fy);
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    // coordinates in z curve space
    uint64_t i00 = 0;
    uint64_t i01 = 0;
    uint64_t i10 = 0;
    uint64_t i11 = 0;
    ZC(x0, y0, &i00);
    ZC(x1, y0, &i01);
    ZC(x0, y1, &i10);
    ZC(x1, y1, &i11);

    float tx = fx - x0;
    float ty = fy - y0;

    float a00 = amt * (1.0f - tx) * (1.0f - ty);
    float a01 = amt * (tx) * (1.0f - ty);
    float a10 = amt * (1.0f - tx) * (ty);
    float a11 = amt * (tx) * (ty);

    float h = map[i00];
    h = saturate(h + a00);
    map[i00] = h;
    h = map[i01];
    h = saturate(h + a01);
    map[i01] = h;
    h = map[i10];
    h = saturate(h + a10);
    map[i10] = h;
    h = map[i11];
    h = saturate(h + a11);
    map[i11] = h;

    return amt;
}

// check if sun is occluded
bool ImageDataManipulator::rayMarch(vec2 uv, vec3 dir)
{
    float hIsect = 0.5f * bilinear(heightData, uv) + 0.001f;
    float fw = (float) width;
    for (int i = 0; i < width / 6; i++) {
        vec2 uv2 = {uv.x + i * dir.x / fw, uv.y + i * dir.y / fw};
        hIsect += 1.0f * dir.z / fw; // simple assumption that height variance is 1/4 width max
        if (hIsect >= 1.0f) return false;
        if (uv2.x < 0.001f || uv2.x > 0.999f ||
                uv2.y < 0.001f || uv2.y > 0.999f) return ((float) rand() / RAND_MAX > 0.5f); // flip for edge artefacts
        float h = 0.5f * bilinear(heightData, uv2);
        if (h > hIsect) {
            return true;
        }
    }
    return false;
}

std::tuple<vec3, vec2> ImageDataManipulator::sobelNormal(int x, int y, float strength)
{
    const float skx[9] = {1.0f, 0.0f, -1.0f, 2.0f, 0.0f, -2.0f, 1.0f, 0.0f, -1.0f};
    const float sky[9] = {1.0f, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, -2.0f, -1.0f};
    float gx = 0.0f;
    float gy = 0.0f;

    int k = 0;
    for (int oy = -1; oy <= 1; ++oy) {
        for (int ox = -1; ox <= 1; ++ox) {
            uint64_t idx = 0;
            ZC((x + ox), (y + oy), &idx);
            float h = heightData[idx];
            gx += skx[k] * h;
            gy += sky[k] * h;
            k += 1;
        }
    }
    float gz = 1.0f / strength;
    vec3 nor = {gx, gy, gz};
    vec2 grad = {gx, gy};
    return std::tuple<vec3, vec2>(normalize(nor), normalize(grad));
}

float ImageDataManipulator::normalizeGrayscale(QRgb input)
{
    unsigned int test = (unsigned int) input;
    float r = (float) ((test >> 16) & 0xFF) / 255.0f;
    float g = (float) ((test >> 8) & 0xFF) / 255.0f;
    float b = (float) (test & 0xFF) / 255.0f;
    return 0.2126f * r + 0.7152f * g + 0.0722f * b;
}

QRgb ImageDataManipulator::grayscaleToPixel(float input)
{
    unsigned int g = (unsigned int) (255.0 * std::max(0.0f, std::min(1.0f, input)));
    return (255 << 24) + (g << 16) + (g << 8) + g;
}

QRgb ImageDataManipulator::floatsToPixel(vec3 n)
{
    unsigned int r = (unsigned int) (255.0f * std::max(0.0f, std::min(1.0f, n.x)));
    unsigned int g = (unsigned int) (255.0f * std::max(0.0f, std::min(1.0f, n.y)));
    unsigned int b = (unsigned int) (255.0f * std::max(0.0f, std::min(1.0f, n.z)));
    return (255 << 24) + (r << 16) + (g << 8) + b;
}

QRgb ImageDataManipulator::normalToPixel(vec3 n)
{
    unsigned int r = (unsigned int) (255.0f * std::max(0.0f, std::min(1.0f, 0.5f + 0.5f * n.x)));
    unsigned int g = (unsigned int) (255.0f * std::max(0.0f, std::min(1.0f, 0.5f + 0.5f * n.y)));
    unsigned int b = (unsigned int) (255.0f * std::max(0.0f, std::min(1.0f, 0.5f + 0.5f * n.z)));
    return (255 << 24) + (r << 16) + (g << 8) + b;
}

QRgb ImageDataManipulator::gradientToPixel(vec2 g)
{
    unsigned int r = (unsigned int) (255.0f * std::max(0.0f, std::min(1.0f, 0.5f + 0.5f * g.x)));
    unsigned int gr = (unsigned int) (255.0f * std::max(0.0f, std::min(1.0f, 0.5f + 0.5f * g.y)));
    unsigned int b = 0;
    return (255 << 24) + (r << 16) + (gr << 8) + b;
}

QRgb ImageDataManipulator::foliageIndexToPixel(int x, int y)
{
    uint64_t idx = 0;
    ZC((uint64_t)x, (uint64_t)y, &idx);
    float h = heightData[idx];
    foliageCell f = foliageData[idx];

    // color the foliage by density, painted & grown
    h = 0.5f * h + 0.5f;
    // 0.133, 0.545, 0.133
    vec3 green = {0.133f, 0.545f, 0.133f};
    vec3 brown = {0.714f, 0.486f, 0.259f};
    vec3 blend = lerp(brown, green, std::max(0.0f, f.grassDensity / (0.01f +f.grassDensity + f.density)));
    vec3 color = {h, h, h};
    vec3 color2 = {h * blend.x, h * blend.y, h * blend.z};
    color = lerp(color, color2, std::min(1.0f, std::max(0.0f, std::max(f.grassDensity, f.density))));
    unsigned int r = (unsigned int) (255.0f * std::max(0.0f, std::min(1.0f, color.x)));
    unsigned int g = (unsigned int) (255.0f * std::max(0.0f, std::min(1.0f, color.y)));
    unsigned int b = (unsigned int) (255.0f * std::max(0.0f, std::min(1.0f, color.z)));
    return (255 << 24) + (r << 16) + (g << 8) + b;
}


ImageDataManipulator::ImageDataManipulator()
{
    usable = false;
}

ImageDataManipulator::ImageDataManipulator(QImage& image)
{
    qDebug()<<"Parsing image data\n";
    this->width = image.width();
    this->height = image.height();
    this->size = this->width * this->height;
    this->heightData = std::vector<float>(this->size, 0.0f);
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            float px = this->normalizeGrayscale(image.pixel(j, i));
            //image.setPixelColor(j, i, QColor(this->grayscaleToPixel(px)));
            uint64_t idx = 0;
            ZC((uint64_t)j, (uint64_t)i, &idx);
            this->heightData[idx] = px;
        }
    }

    this->normalData = std::vector<vec3>(this->size, {0.f, 0.f, 0.f});
    this->gradientData = std::vector<vec2>(this->size, {0.f, 0.f});
    this->foliageData = std::vector<foliageCell>(this->size, foliageCell());
    this->soilData = std::vector<terrainCell>(this->size, terrainCell());

    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            uint64_t idx = 0;
            ZC((uint64_t)j, (uint64_t)i, &idx);
            std::tuple<vec3, vec2> info = sobelNormal(j, i, 4.0f);
            this->normalData[idx] = std::get<0>(info);
            this->gradientData[idx] = std::get<1>(info);
        }
    }
//#define MARCH
    image = image.convertToFormat(QImage::Format_RGB32);
#ifndef MARCH
    for (int i = 0; i < this->heightData.size(); i++) {
        uint64_t x = 0;
        uint64_t y = 0;
        IZC((uint64_t)i, &x, &y);
        image.setPixelColor(x, y, QColor(this->grayscaleToPixel(this->heightData[i])));
    }
#else
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            vec2 uv = {(float) j / width, (float) i / width};

            int numHits = 0;
            int numRays = 20;
            float elev = 0.35f * 3.14159f;
            float se = std::sin(elev);
            float ce = std::cos(elev);
            float rf, gf, bf = 0.0f;
            for (int ray = 0; ray < numRays; ray++) {
                vec3 dir = {std::cos((float) ray / (float) numRays * 3.141f), 0.0f, 0.05f + std::sin((float) ray / (float) numRays * 3.141f)};
                //qDebug() << std::cos((float) ray / (float) numRays * 3.141f);
                vec3 dirn = normalize(dir);
                dirn = {dirn.x, -ce * dirn.z, se * dirn.z};
                //qDebug()<<dir.x<< ", " << dir.y << ", " << dir.z;
                bool hit = rayMarch(uv, dirn);
                if (hit) numHits++;
            }
            float expo = 1.f - (float)(numHits) / (numRays);
            unsigned int g = (unsigned int) (255.0f * std::max(0.0f, std::min(1.0f, expo)));
            image.setPixelColor(j, i, (255 << 24) + (g << 16) + (g << 8) + g);
        }
    }
#endif

    this->imgRefHeight = &image;

    recalculateSoilParameters();

    qDebug()<<"Done\n";


    //image.save("somefile.png", "PNG");
}

void ImageDataManipulator::recalculateSoilParameters()
{
    float elev = 0.40f * 3.14159f;
    float se = std::sin(elev);
    float ce = std::cos(elev);

    qDebug()<<"Raymarching to find sunlight exposure";

    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            // raymarch to get average sun exposure
            vec2 uv = {(float) j / width, (float) i / width};

            int numHits = 0;
            int numRays = 18;

            for (int ray = 0; ray < numRays; ray++) {
                vec3 dir = {std::cos((float) ray / (float) numRays * 3.141f), 0.0f, 0.05f + std::sin((float) ray / (float) numRays * 3.141f)};
                vec3 dirn = normalize(dir);
                dirn = {dirn.x, -ce * dirn.z, se * dirn.z};
                bool hit = rayMarch(uv, dirn);
                if (hit) numHits++;
            }

            float exposure = 1.f - (float)(numHits + 1) / (numRays + 2);
            uint64_t idx = 0;
            ZC(j, i, &idx);
            soilData[idx].sunlight = exposure;

            // just add some random variation to the moisture
            float m = soilData[idx].moisture;
            m = saturate(m + (((float) rand()) / RAND_MAX - 0.5f) * 0.05f);
            soilData[idx].moisture = m;
        }
    }
}

void ImageDataManipulator::densityRecalculate(foliageCell &fc) {
    int c = fc.count;
    float h = fc.heightSum;
    // 0 to 1 scale per tree.
    // max amount of tree is therefore maxplants * 1.0
    //
    float dN = h / (float) maxPlantsPerCell;
    if (dN > 1.0f) {
        c -= 1;
        h -= 1.0;
        dN = h / (float) maxPlantsPerCell;
    }

    fc.count = c;
    fc.heightSum = h;
    fc.density = dN;
}


void ImageDataManipulator::brushGrassGrow(int x, int y, int r, float densityAddAmt) {
    float kr = std::max((float) r - 1.0, 1.0);
    for (int i = 1 - r; i < r; i++) {
        for (int j = 1 - r; j < r; j++) {
            int px = pxClamp(x + j);
            int py = pxClamp(y + i);
            uint64_t idx = 0;
            ZC((uint64_t) px,(uint64_t)  py, &idx);
            foliageCell fc = foliageData[idx];
            float dist = std::sqrt((float)(j * j + i * i));
            dist = saturate(1.0 - dist / kr);

            // say a tree has a minimum height of 0.0, max height of 1.0
            fc.grassDensity = saturate(fc.grassDensity + dist * densityAddAmt);

            foliageData[idx] = fc;
        }
    }
}


void ImageDataManipulator::brushFoliageGrow(int x, int y, int r, float heightPerTree) {
    float kr = std::max((float) r - 1.0, 1.0);
    for (int i = 1 - r; i < r; i++) {
        for (int j = 1 - r; j < r; j++) {
            int px = pxClamp(x + j);
            int py = pxClamp(y + i);
            uint64_t idx = 0;
            ZC((uint64_t) px,(uint64_t)  py, &idx);
            foliageCell fc = foliageData[idx];
            float dist = std::sqrt((float)(j * j + i * i));
            dist = saturate(1.0 - dist / kr);

            // will change height per tree
            float curDensity = fc.density;
            float tgtDensity = saturate(curDensity + dist * heightPerTree);
            if (tgtDensity <= 0.01f) {
                fc.density = 0.f;
                fc.count = 0.f;
                fc.heightSum = 0.f;
            } else if (tgtDensity > 0.999f) {
                fc.density = 1.f;
                fc.count = maxPlantsPerCell;
                fc.heightSum = (float) maxPlantsPerCell;
            } else {
                int c = fc.count;
                float dptMax = 1.f / (float) maxPlantsPerCell;
                float dptMin = 0.1f / (float) maxPlantsPerCell;
                if (c) {
                    // determine if we need to add trees
                    // need a better heuristic later...
                    float dpt = tgtDensity / c;
                    if (dpt > dptMax) {
                        c = (int) std::ceil(tgtDensity / dptMax);
                    } else if (dpt < dptMin) {
                        c--; // out of time here
                    }

                } else {
                    c = (int) std::ceil(tgtDensity / dptMax);
                }
                fc.density = tgtDensity;
                fc.count = c;
                fc.heightSum = maxPlantsPerCell * tgtDensity;
            }
            // say a tree has a minimum height of 0.0, max height of 1.0
            //fc.heightSum += dist * heightPerTree * fc.count;
            //densityRecalculate(fc);

            foliageData[idx] = fc;

        }
    }
}

void ImageDataManipulator::brushAdd(int x, int y, int r, float amt)
{
    float kr = std::max((float) r - 1.0, 1.0);
    for (int i = 1 - r; i < r; i++) {
        for (int j = 1 - r; j < r; j++) {
            int px = pxClamp(x + j);
            int py = pxClamp(y + i);
            uint64_t idx = 0;
            ZC((uint64_t) px,(uint64_t)  py, &idx);
            float h = heightData[idx];

            // cone falloff
            float dist = std::sqrt((float)(j * j + i * i));

            dist = saturate(1.0 - dist / kr);
            // smooth falloff
            dist = smoothstep(dist);
            h = saturate(h + dist * amt);
            heightData[idx] = h;
        }
    }
}

float ImageDataManipulator::calculateCurveDataForPoint(int x, int y)
{
    uint64_t h_idx = 0;
    uint64_t hv1_idx = 0;
    uint64_t hv2_idx = 0;
    uint64_t hh1_idx = 0;
    uint64_t hh2_idx = 0;
    uint64_t px = (uint64_t) x;
    uint64_t py = (uint64_t) y;
    uint64_t d = 3;

    ZC(px, py, &h_idx);
    ZC(px, py - d, &hv1_idx);
    ZC(px, py + d, &hv2_idx);
    ZC(px - d, py, &hh1_idx);
    ZC(px + d, py, &hh2_idx);

    float h = heightData[h_idx];
    float hv1 = heightData[hv1_idx];
    float hv2 = heightData[hv2_idx];
    float hh1 = heightData[hh1_idx];
    float hh2 = heightData[hh2_idx];
    float vcurviness = 0;
    float hcurviness = 0;

    if(hv1 < hv2)
    {
         vcurviness = (h - hv2) - (hv1 - h);
    } else {
         vcurviness = (h - hv1) - (hv2 - h);
    }

    if(hh1 < hh2)
    {
        hcurviness = (h - hh2) - (hh1 - h);
    } else {
        hcurviness = (h - hh1) - (hh2 - h);
    }

    float curveData = (vcurviness + hcurviness)/(2.0);
    return curveData;

}

void ImageDataManipulator::brushPolish(int x, int y, int r, float amt)
{
    float kr = std::max((float) r - 1.0, 1.0);
    uint64_t centerIdx = 0;
    ZC((uint64_t) x, (uint64_t) y, &centerIdx);
    float centerVal = heightData[centerIdx];
    vec3 centerNor = normalData[centerIdx];


    for (int i = 1 - r; i < r; i++) {
        for (int j = 1 - r; j < r; j++) {
            int px = pxClamp(x + j);
            int py = pxClamp(y + i);
            uint64_t idx = 0;
            ZC((uint64_t) px,(uint64_t)  py, &idx);
            float h = heightData[idx];

            // cone falloff
            float dist = std::sqrt((float)(j * j + i * i));

            dist = saturate(1.0 - dist / kr);
            // smooth falloff
            dist = smoothstep(dist);

            //calculate curvature on the spot:
            float c = calculateCurveDataForPoint(px, py);
            c = 1.0 - clamp(2.0 * std::abs(c), 0.0, 1.0);
            float x0 = (float) px / width;
            float y0 = (float) py / height;
            float z0 = centerVal * centerNor.z / heightIntensity;
            float z1 = (centerNor.x * x0 - centerNor.y * y0 + z0) / centerNor.z * heightIntensity;
            h = mix(h, clamp(z1, 0.0, 1.0), c * dist * amt);
            heightData[idx] = h;

        }
    }
}

void ImageDataManipulator::brushFlatten(int x, int y, int r, float amt)
{
    float kr = std::max((float) r - 1.0, 1.0);
    for (int i = 1 - r; i < r; i++) {
        for (int j = 1 - r; j < r; j++) {
            int px = pxClamp(x + j);
            int py = pxClamp(y + i);
            uint64_t idx = 0;
            ZC((uint64_t) px,(uint64_t)  py, &idx);
            float h = heightData[idx];

            // cone falloff
            float dist = std::sqrt((float)(j * j + i * i));

            dist = saturate(1.0 - dist / kr);
            // smooth falloff
            dist = smoothstep(dist);

           uint64_t centerIdx = 0;
           ZC((uint64_t) x, (uint64_t) y, &centerIdx);
           float centerVal = heightData[centerIdx];
           h = mix(h, centerVal, dist * amt);
           heightData[idx] = h;
        }
    }
}

void ImageDataManipulator::refreshRegion(int x, int y, int r)
{
    for (int i = 1 - r; i < r; i++) {
        for (int j = 1 - r; j < r; j++) {
            int px = pxClamp(x + j);
            int py = pxClamp(y + i);
            uint64_t idx = 0;
            ZC((uint64_t) px, (uint64_t)  py, &idx);
            float pixel = heightData[idx];
            imgRefHeight->setPixelColor(px, py, QColor(this->grayscaleToPixel(pixel)));

            // foliage
            imgRefFoliage->setPixelColor(px, py, foliageIndexToPixel(px, py));
        }
    }
}

void ImageDataManipulator::bilinearRepair(vec2 p)
{
    float fw = (float) width;
    float fx = fw * p.x;
    float fy = fw * p.y;

    int x0 = (int) floor(fx);
    int y0 = (int) floor(fy);
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    // coordinates in z curve space
    uint64_t i00 = 0;
    uint64_t i01 = 0;
    uint64_t i10 = 0;
    uint64_t i11 = 0;
    ZC(x0, y0, &i00);
    ZC(x1, y0, &i01);
    ZC(x0, y1, &i10);
    ZC(x1, y1, &i11);

    std::tuple<vec3, vec2> info = sobelNormal(x0, y0, normalStr);
    normalData[i00] = std::get<0>(info);
    gradientData[i00] = std::get<1>(info);
    info = sobelNormal(x1, y0, normalStr);
    normalData[i01] = std::get<0>(info);
    gradientData[i01] = std::get<1>(info);
    info = sobelNormal(x0, y1, normalStr);
    normalData[i10] = std::get<0>(info);
    gradientData[i10] = std::get<1>(info);
    info = sobelNormal(x1, y1, normalStr);
    normalData[i11] = std::get<0>(info);
    gradientData[i11] = std::get<1>(info);
}

void ImageDataManipulator::radiusRepair(vec2 p, int r)
{
    float fw = (float) width;
    float fx = fw * p.x;
    float fy = fw * p.y;

    int x0 = (int) floor(fx);
    int y0 = (int) floor(fy);
    int oy = y0 - r;
    int oy1 = y0 + r;
    int ox = x0 - r;
    int ox1 = x0 + r;
    for (int i = oy; i < oy1; i++) {
        for (int j = ox; j < ox1; j++) {
            uint64_t idx = 0;
            ZC(j, i, &idx);
            std::tuple<vec3, vec2> info = sobelNormal(j, i, normalStr);
            normalData[idx] = std::get<0>(info);
            gradientData[idx] = std::get<1>(info);
        }
    }
}

void ImageDataManipulator::refreshFoliageImage() {
    if (imgRefFoliage == NULL) return;
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            imgRefFoliage->setPixelColor(j, i, foliageIndexToPixel(j, i));
        }
    }
}

void ImageDataManipulator::setFoliageRef(QImage &img)
{
    imgRefFoliage = &img;
    refreshFoliageImage();
}

// TODO: multithreading support
void ImageDataManipulator::phDoErosion(int dropletCount)
{
    dropletOptions params = dropletOptions();
    params.kInertia = 0.03f;
    //params.kAccumulate = 0.75f;
    params.unitDist = 1.0f / (float) width;
    float kGain = 4.0f;

    int kr = 3;
    int kl = 2 * (kr - 1) + 1;
    std::vector<float> kernel = std::vector<float>(kl * kl, 0.0f);
    float dt = params.unitDist;
    float sum = 0.0f;
    for (int ky = 0; ky < kl; ky++) {
        for (int kx = 0; kx < kl; kx++) {
            float dx = (float) kx - kr + 1.0f;
            float dy = (float) ky - kr + 1.0f;
            float d = sqrt(dx * dx + dy * dy);
            d = std::max(0.f, kr - d);
            d *= d;
            kernel[ky * kl + kx] = d;
            sum += d;
        }
    }

    for (int ky = 0; ky < kl; ky++) {
        for (int kx = 0; kx < kl; kx++) {
            kernel[ky * kl + kx] /= sum;
        }
    }

    for (int drop = 0; drop < dropletCount; ++drop) {
        if (drop % 1000 == 0) qDebug() << "erosion event: " << drop << "\n";
        float rx = ((float) rand() / (RAND_MAX));
        float ry = ((float) rand() / (RAND_MAX));
        //qDebug() << rx << ", " << ry << "\n";
        vec2 p = {rx, ry};
        Droplet droplet = Droplet(rx, ry, params);
        vec2 dir = bilinear(gradientData, p);
        droplet.currentHeight = bilinear(heightData, p);
        droplet.changeDirection(dir);

        for (int st = 0; st < 100 && !droplet.isDead(); ++st) {
            bilinearRepair(p);

            droplet.move();
            p = {droplet.x, droplet.y};
            // update step is nasty
            float h0 = droplet.currentHeight;
            vec2 p1 = {droplet.dir.x * droplet.params.unitDist + droplet.x, droplet.dir.y * droplet.params.unitDist + droplet.y};
            float h1 = bilinear(heightData, p1);
            float dens = bilinearDensity(foliageData, p1);
            float hdif = h1 - h0;
            // plant interference w/ erosion:
            // shrink the slope if downhill, grow if uphill
            hdif *= (hdif > 0.0f ? 1.f - dens : 1.f + dens);

            float c = std::max(0.03f, -hdif) * std::max(droplet.speed, 0.01f) * droplet.water * kGain;

            if (hdif > 0.0f) {
                // deposit everything needed to fill pit
                if (droplet.sediment < hdif) {
                    // cannot continue, trapped
                    droplet.dead = true;
                    for (int ky = 0; ky < kl; ky++) {
                        for (int kx = 0; kx < kl; kx++) {
                            vec2 pk = {p.x + dt * (kx - kr + 1), p.y + dt * (ky - kr + 1)};
                            float kw = kernel[ky * kl + kx];
                            bilinearAdd(heightData, pk, kw * droplet.sediment);
                        }
                    }
                } else {

                    for (int ky = 0; ky < kl; ky++) {
                        for (int kx = 0; kx < kl; kx++) {
                            vec2 pk = {p.x + dt * (kx - kr + 1), p.y + dt * (ky - kr + 1)};
                            float kw = kernel[ky * kl + kx];
                            bilinearAdd(heightData, pk, kw * hdif);
                        }
                    }
                    droplet.sediment -= hdif;
                }
            } else {
                if (droplet.sediment < c) {
                    // under capacity, pick up some percent
                    float amt = -hdif * droplet.params.kAccumulate;
                    if (amt + droplet.sediment > c) amt = c - droplet.sediment;

                    for (int ky = 0; ky < kl; ky++) {
                        for (int kx = 0; kx < kl; kx++) {
                            vec2 pk = {p.x + dt * (kx - kr + 1), p.y + dt * (ky - kr + 1)};
                            float kw = kernel[ky * kl + kx];
                            bilinearAdd(heightData, pk, kw * -amt);
                        }
                    }
                    droplet.sediment += amt;
                } else {
                    // over capacity, drop percent of excess
                    float amt = (droplet.sediment - c) * droplet.params.kDeposit;

                    for (int ky = 0; ky < kl; ky++) {
                        for (int kx = 0; kx < kl; kx++) {
                            vec2 pk = {p.x + dt * (kx - kr + 1), p.y + dt * (ky - kr + 1)};
                            float kw = kernel[ky * kl + kx];
                            bilinearAdd(heightData, pk, kw * amt);
                        }
                    }
                    droplet.sediment -= amt;
                }
            }

            droplet.currentHeight = h1;
            droplet.speed = sqrt(droplet.speed * droplet.speed + std::max(0.01f * droplet.params.kGrav, -hdif * droplet.params.kGrav));
            droplet.water *= (1.0f - droplet.params.kEvap);

            droplet.dead = droplet.checkIfDead();
            if (droplet.isDead()) {
                if (p.x >0.f && p.x < 1.f && p.y > 0.f && p.y < 1.f)
                    for (int ky = 0; ky < kl; ky++) {
                        for (int kx = 0; kx < kl; kx++) {
                            vec2 pk = {p.x + dt * (kx - kr + 1), p.y + dt * (ky - kr + 1)};
                            float kw = kernel[ky * kl + kx];
                            bilinearAdd(heightData, pk, kw * droplet.sediment);
                        }
                    }
            }

            dir = bilinear(gradientData, p);
            droplet.changeDirection(dir);
            bilinearRepair(p);

            // repair the info of everything droplet touched
            //if (--backup < 0) break;
        }
    }

    qDebug() << " refreshing image\n";
    // refresh the image
    for (int i = 0; i < this->heightData.size(); i++) {
        uint64_t x = 0;
        uint64_t y = 0;
        IZC((uint64_t)i, &x, &y);
        imgRefHeight->setPixelColor(x, y, QColor(this->grayscaleToPixel(this->heightData[i])));
    }
}

void ImageDataManipulator::ecosystemEvent(int x, int y)
{
    uint64_t idx = 0;
    ZC(x, y, &idx);
    // get all info at cell
    foliageCell fc = foliageData[idx];
    terrainCell tc = soilData[idx];
    float h = heightData[idx];
    float temp = heightToTemp.get(h);
    float count = fc.count;
    float density = fc.density;
    float penaltyDensity = std::exp(-density);
    float penaltyResources = std::exp(-density - fc.grassDensity);
    // determine -1 to 1 measure of plant viability here
    // density blocks some sunlight
    float viability = plant1.growth(tc.moisture * penaltyResources, temp, tc.sunlight * penaltyDensity);
    // grass has harder time growing under dense foliage
    float viabilityGrass = grass.growth(tc.moisture * penaltyResources, temp, tc.sunlight * penaltyDensity);

    // fast acos approx
    float z = normalData[idx].z;
    z = 0.63661977f * ((-0.6981317007977f * z * z - 0.872664625997f) * z + 1.57079632679f);
    //const float PI_2 = M_PI * 0.5f;
    //float vbSlope = (PI_2 - z);
    float vbSlope1 = remap(1.f - z, 0.60f, 0.8f, -1.f, 1.f);
    float vbSlope2 = remap(1.f - z, 0.40f, 0.65f, -1.f, 1.f);

    viability = std::min(viability, vbSlope1);
    viabilityGrass = std::min(viabilityGrass, vbSlope2);

    // determine new number of plants
    int newPlants = (int) std::floor(std::max(0.0f, 2.5f * viability));
    float npNoise = ((float) rand() / (RAND_MAX));
    newPlants = (int) std::round(std::max(0.f, viability) * npNoise * (maxPlantsPerCell - count));
    //float avgAge = fc.ageSum / (float)std::max(1, fc.count);
    int dyingPlants = (int) std::round(std::max(0.0f, -viability * (1.f - npNoise) * count));
    // introduce some noise to dying plants, they catch a flu or something
    dyingPlants += (int) (0.5f * std::max(0.f, ((float) rand() / (RAND_MAX) - penaltyDensity) * (count - dyingPlants))); // density based

    int newCount = count + newPlants - dyingPlants;

    if (newCount > maxPlantsPerCell) {

        dyingPlants += (newCount - maxPlantsPerCell);
        newCount = maxPlantsPerCell;

    }
    if (newCount <= 0) {

        fc.count = 0;
        fc.density = 0.0f;
        fc.ageSum = 0.0f;
        fc.heightSum = 0.0f;

    } else if (count == 0) {

        fc.heightSum = 0.1f * newCount;
        fc.density = fc.heightSum / (float) maxPlantsPerCell;
        fc.ageSum = 0.0f;
        fc.count = newCount;

    } else {

        float avgHeight = fc.heightSum / (float) (count);
        // add some penalty for dying plants
        float avgHNew = (fc.heightSum - avgHeight * dyingPlants) / (float) (count - dyingPlants);
        float hNew = avgHNew * (count - dyingPlants);
        if (viability >= 0.0f) {
            // grow existing plants
            avgHNew += 0.05f * std::max(0.0f, viability);
            hNew = avgHNew * (count - dyingPlants) + 0.1f * newPlants;
        }
        float dNew = hNew / (float) maxPlantsPerCell;

        // force a plant to die and penalize the rest, prevent convergence
        if (dNew > 1.0f) {
            newCount = maxPlantsPerCell - 1;
            hNew = newCount * 0.95;
            dNew = hNew / maxPlantsPerCell;
        }
        fc.density = dNew;
        fc.count = newCount;
        fc.heightSum = hNew;

    }
    // if grass viability is low but above zero some will still die
    fc.grassDensity = saturate(fc.grassDensity + 0.05f * (viabilityGrass - 0.2f));
    foliageData[idx] = fc;
}

void ImageDataManipulator::phDoEcosystem(int eventCount)
{
    for (int evt = 0; evt < eventCount; ++evt) {
        if (evt % 1000 == 0) qDebug() << "ecosystem event: " << evt << "\n";
        float rx = ((float) rand() / (RAND_MAX));
        float ry = ((float) rand() / (RAND_MAX));
        int px = std::floor(rx * width);
        int py = std::floor(ry * width);
        ecosystemEvent(px, py);
    }

    refreshFoliageImage();
}

void ImageDataManipulator::phDoFires(int eventCount)
{
    for (int evt = 0; evt < eventCount; ++evt) {
        if (evt % 1000 == 0) qDebug() << "fire event: " << evt << "\n";
        float rx = ((float) rand() / (RAND_MAX));
        float ry = ((float) rand() / (RAND_MAX));
        int px = std::floor(rx * width);
        int py = std::floor(ry * width);

        uint64_t idx = 0;
        ZC(px, py, &idx);

        float currHeight = heightData[idx];

        int maxSteps = 20;
        float strength = 0.25f * ((float) rand() / (RAND_MAX));
        float decay = 0.15f;
        int chIdx = 2;
        for (int step = 0; step < maxSteps; ++step) {

            // move to a new cell
            ZC(px, py, &idx);

            float h = heightData[idx];
            if (std::abs(h - currHeight) > 0.1f) break; // big height discontinuity
            currHeight = h;
            foliageCell fc = foliageData[idx];


            // if the amount is below a threshold there isnt enough fuel
            if (fc.density + fc.grassDensity < 0.1f) break;
            // determine how much of the foliage must die
            float fuel = (fc.density + fc.grassDensity) * 0.1f;

            // kill pct grass
            fc.grassDensity = fc.grassDensity * (1.0 - strength);

            // kill pct plants
            float avgHeight = fc.heightSum / (std::max(1.f, (float) fc.count));
            int newCount = std::round(fc.count * (1.0 - strength));
            if (newCount <= 0) {
                fc.count = 0;
                fc.heightSum = 0.f;
                fc.density = 0.f;
            } else {
                fc.count = newCount;
                fc.heightSum = avgHeight * newCount;
                fc.density = fc.heightSum / (float) maxPlantsPerCell;
            }


            float rc = ((float) rand() / (RAND_MAX));
            int shift = std::floor(rc * 3.f) + 1;
            chIdx = (chIdx - 2 + shift); // never return in the same dir
            chIdx = chIdx > 0 ? chIdx % 4 : chIdx + 4;
            switch(chIdx) {
                case 0:
                    px -= 1;
                    break;
                case 1:
                    py -= 1;
                    break;
                case 2:
                    px += 1;
                    break;
                case 3:
                    py += 1;
                    break;
             }

            foliageData[idx] = fc;
            strength *= (1.f - decay + fuel);
            if (px <= 0 || px >= width) break;
            if (py <= 0 || py >= width) break;
            if (strength < 0.01f) break;
        }
    }
    refreshFoliageImage();
}

QImage ImageDataManipulator::exportFoliageImage()
{
    QImage img = QImage(width, height, QImage::Format_RGB888);
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            uint64_t idx = 0;
            ZC((uint64_t) j,(uint64_t) i, &idx);
            foliageCell fc = foliageData[idx];
            terrainCell tc = soilData[idx];
            uint32_t color = 0; //255 << 24; // alpha
            color += fc.count << 16; // number of trees as red

            //qDebug()<<fc.count;
            // average 0 to 1 height as green
            color += std::min((int) (255.0f * fc.heightSum / (float) fc.count), 255) << 8;

            // blue is grass
            color += (int) std::round(255.0f * fc.grassDensity);
            img.setPixelColor(j, i, color);
        }
    }

    return img; // pass by value!!!
}

void ImageDataManipulator::simRefresh()
{
    recalculateSoilParameters();
}

/// Droplet Stuff
///

Droplet::Droplet() {
    x = 0.5f;
    y = 0.5f;
    params = dropletOptions();
}

Droplet::Droplet(float x, float y)
{
    this->x = x;
    this->y = y;
    params = dropletOptions();
}

Droplet::Droplet(float x, float y, dropletOptions op)
{
    this->x = x;
    this->y = y;
    params = op;
}

void Droplet::move()
{
    params.steps -= 1;
    x += dir.x * params.unitDist;
    y += dir.y * params.unitDist;
}

void Droplet::changeDirection(vec2 dirN)
{
    dir = normalize(lerp(dirN, dir, params.kInertia));
}

bool Droplet::checkIfDead()
{
    if (dead) return true;
    if (params.steps <= 0) return true;
    if (x <= 0.f || y <= 0.f || x >= 1.f || y >= 1.f) return true;
    if (water < 0.001f) return true;
    return false;
}


