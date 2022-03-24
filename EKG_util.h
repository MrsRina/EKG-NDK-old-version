/**
 * @author Rina
 * @since 21/03/2022 at 21:05PM
 */
#pragma once
#include "EKG_includes.h"

/**
 * Apply ortho view 2D.
 */
void EKG_Ortho2D(float* Mat, float Left, float Right, float Bottom, float Top);

/**
 * Send log to console (remember before call this, invoke EKG::Init() to prevent errors using this function)
 * @param Log - The log.
 */
void EKG_Log(std::string Log);

/**
 * Vec with two dimensions.
 */
struct EKG_Vec2d {
    double X, Y;
};

/**
 * Rect AABB.
 */
struct EKG_Rect {
    float X, Y, W, H;

    bool CollideWithPoint(float PointX, float PointY);
    bool CollideWithRect(float RectX, float RectY, float RectW, float RectH);
};

/**
 * Put ids in a stack.
 */
struct EKG_Stack {
    std::vector<unsigned int> StackedIds;

    bool Contains(unsigned int Id);
    bool Rem(unsigned int Id);
    bool Put(unsigned int Id);
};

/**
 * Color util.
 */
struct EKG_Color {
    EKG_Color(unsigned int Red, unsigned int Green, unsigned int Blue, unsigned int Alpha);
    EKG_Color(unsigned int Red, unsigned int Green, unsigned int Blue);

    void Set(unsigned int Red, unsigned int Green, unsigned int Blue, unsigned int Alpha);
    void Set(unsigned int Red, unsigned int Green, unsigned int Blue);

    unsigned int R, G, B, A;

    float GetRedf();
    float GetGreenf();
    float GetBluef();
    float GetAlphaf();
};

/**
 * Save texture id, width & height of original size texture.
 */
struct EKG_Texture {
    float Width, Height;
    unsigned int Id;
};