#include "EKG_util.h"

bool EKG_Stack::Contains(unsigned int Id) {
    for (unsigned int Ids : this->StackedIds) {
        if (Ids == Id) {
            return true;
        }
    }

    return false;
}

bool EKG_Stack::Remove(unsigned int Id) {
    std::vector<unsigned int> NewStackedIds;
    bool Flag;

    for (unsigned int Ids : this->StackedIds) {
        if (Ids == Id) {
            Flag = true;
        } else {
            NewStackedIds.push_back(Ids);
        }
    }

    this->StackedIds = NewStackedIds;
    return Flag;
}

bool EKG_Stack::Add(unsigned int Id) {
    this->StackedIds.push_back(Id);
    return true;
}

void EKG_Log(std::string Log) {
    std::string FullaLog = "[EKG] " + Log;

    // Send log using SDL function.
    SDL_Log("%s", FullaLog.c_str());
}

void EKG_Ortho2D(float* Mat, float Left, float Right, float Bottom, float Top) {
    const float zNear = -1.0f;
    const float zFar = 1.0f;
    const float inv_z = 1.0f / (zFar - zNear);
    const float inv_y = 1.0f / (Top - Bottom);
    const float inv_x = 1.0f / (Right - Left);

    //first column
    *Mat++ = (2.0f * inv_x);
    *Mat++ = (0.0f);
    *Mat++ = (0.0f);
    *Mat++ = (0.0f);

    //second
    *Mat++ = (0.0f);
    *Mat++ = (2.0 * inv_y);
    *Mat++ = (0.0f);
    *Mat++ = (0.0f);

    //third
    *Mat++ = (0.0f);
    *Mat++ = (0.0f);
    *Mat++ = (-2.0f * inv_z);
    *Mat++ = (0.0f);

    //fourth
    *Mat++ = (-(Right + Left) * inv_x);
    *Mat++ = (-(Top + Bottom) * inv_y);
    *Mat++ = (-(zFar + zNear) * inv_z);
    *Mat++ = (1.0f);
}

float EKG_Color::GetRedf() {
    return 1.0F / (float) this->R;
}

float EKG_Color::GetGreenf() {
    return 1.0F / (float) this->G;
}

float EKG_Color::GetBluef() {
    return 1.0F / (float) this->B;
}

float EKG_Color::GetAlphaf() {
    return 1.0F / (float) this->A;
}

EKG_Color::EKG_Color(unsigned int Red, unsigned int Green, unsigned int Blue, unsigned int Alpha) {
    this->R = Red;
    this->G = Green;
    this->B = Blue;
    this->A = Alpha;
}

EKG_Color::EKG_Color(unsigned int Red, unsigned int Green, unsigned int Blue) {
    this->R = Red;
    this->G = Green;
    this->B = Blue;
    this->A = 255;
}

void EKG_Color::Set(unsigned int Red, unsigned int Green, unsigned int Blue, unsigned int Alpha) {
    this->R = Red;
    this->G = Green;
    this->B = Blue;
    this->A = Alpha;
}

void EKG_Color::Set(unsigned int Red, unsigned int Green, unsigned int Blue) {
    this->R = Red;
    this->G = Green;
    this->B = Blue;
}
