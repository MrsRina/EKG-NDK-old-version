#include "EKG_util.h"
#include "EKG.h"

bool EKG_Stack::Contains(unsigned int Id) {
    for (unsigned int Ids : this->StackedIds) {
        if (Ids == Id) {
            return true;
        }
    }

    return false;
}

bool EKG_Stack::Rem(unsigned int Id) {
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

bool EKG_Stack::Put(unsigned int Id) {
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

void EKG_DrawFilledShape(float X, float Y, float W, float H, const EKG_Color &Color) {
    EKG_TESSELLATOR->NewDraw(GL_TRIANGLES, 6);
    EKG_TESSELLATOR->SetRectColor(Color);
    EKG_StoreShape(X, Y, W, H);
    EKG_TESSELLATOR->Draw();
}

void EKG_DrawOutlineShape(float X, float Y, float W, float H, float LineThickness, const EKG_Color &Color) {
    EKG_TESSELLATOR->NewDraw(GL_TRIANGLE_STRIP, 6);
    EKG_TESSELLATOR->SetRectColor(Color);

    // First triangle.
    EKG_TESSELLATOR->Vertex(X, Y, 0);
    EKG_TESSELLATOR->Vertex(X, Y + H, 0);
    EKG_TESSELLATOR->Vertex(X + W, Y + H, 0);

    // Second triangle.
    EKG_TESSELLATOR->Vertex(X + W, Y + H, 0);
    EKG_TESSELLATOR->Vertex(X + W, Y, 0);
    EKG_TESSELLATOR->Vertex(X, Y, 0);

    // Draw.
    EKG_TESSELLATOR->Draw();
}

void EKG_DrawFilledRect(const EKG_Rect &Rect, const EKG_Color &Color) {
    EKG_DrawFilledShape(Rect.X, Rect.Y, Rect.W, Rect.H, Color);
}

void EKG_DrawOutlineRect(const EKG_Rect &Rect, float LineThickness, const EKG_Color &Color) {
    EKG_DrawOutlineShape(Rect.X, Rect.Y, Rect.W, Rect.H, LineThickness, Color);
}

void EKG_StoreShape(float X, float Y, float W, float H) {
    // First triangle.
    EKG_TESSELLATOR->Vertex(X, Y, 0);
    EKG_TESSELLATOR->Vertex(X, Y + H, 0);
    EKG_TESSELLATOR->Vertex(X + W, Y + H, 0);

    // Second triangle.
    EKG_TESSELLATOR->Vertex(X + W, Y + H, 0);
    EKG_TESSELLATOR->Vertex(X, Y, 0);
    EKG_TESSELLATOR->Vertex(X + W, Y, 0);
}

void EKG_DrawTextureRect(const EKG_Rect &Rect, float TextureX, float TextureY, float TextureW, float TextureH, const EKG_Texture &Texture, const EKG_Color &Color) {
    EKG_DrawTextureShape(Rect.X, Rect.Y, Rect.W, Rect.H, TextureX, TextureY, TextureW, TextureH, Texture, Color);
}

void EKG_DrawTextureShape(float X, float Y, float W, float H, float TextureX, float TextureY, float TextureW, float TextureH, const EKG_Texture &Texture, const EKG_Color &Color) {
    EKG_TESSELLATOR->NewDraw(GL_TRIANGLES, 6);
    EKG_TESSELLATOR->BindTexture(Texture);
    EKG_TESSELLATOR->SetRectColor(Color);
    EKG_StoreTextureCoordinates(TextureX, TextureY, TextureW, TextureH);
    EKG_StoreShape(X, Y, W, H);
    EKG_TESSELLATOR->Draw();
}

void EKG_StoreTextureCoordinates(float TextureX, float TextureY, float TextureW, float TextureH) {
    float U = 1.0F + (TextureW != 1 ? ((TextureW - (float) EKG_TESSELLATOR->GetTextureWidth()) / (float) EKG_TESSELLATOR->GetTextureWidth()) : 0.0F);
    float V = 1.0F + (TextureH != 1 ? ((TextureH - (float) EKG_TESSELLATOR->GetTextureHeight()) / (float) EKG_TESSELLATOR->GetTextureHeight()) : 0.0F);

    EKG_TESSELLATOR->TextCoord2f(0, 0);
    EKG_TESSELLATOR->TextCoord2f(0, V);
    EKG_TESSELLATOR->TextCoord2f(U, V);

    EKG_TESSELLATOR->TextCoord2f(U, V);
    EKG_TESSELLATOR->TextCoord2f(0, 0);
    EKG_TESSELLATOR->TextCoord2f(U, 0);
}

std::string EKG_Print(const std::string& Tag, unsigned int Id) {
    return " [" + Tag + + "-" + std::to_string(Id) + "] ";
}

float EKG_LinearInterpolation(float A, float B, float T) {
    return A + (B - A) * T;
}

float EKG_Color::GetRedf() {
    return (float) this->R / 255.0F;
}

float EKG_Color::GetGreenf() {
    return (float) this->G / 255.0F;
}

float EKG_Color::GetBluef() {
    return (float) this->B / 255.0F;
}

float EKG_Color::GetAlphaf() {
    return (float) this->A / 255.0F;
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

EKG_Color::EKG_Color(unsigned int Color[4]) {
    this->R = Color[0];
    this->G = Color[1];
    this->B = Color[2];
    this->A = Color[3];
}

EKG_Color::EKG_Color(unsigned int Color[3], unsigned int Alpha) {
    this->R = Color[0];
    this->G = Color[1];
    this->B = Color[2];
    this->A = Alpha;
}

void EKG_Color::Set(unsigned int Color[4]) {
    this->R = Color[0];
    this->G = Color[1];
    this->B = Color[2];
    this->A = Color[3];
}

void EKG_Color::Set(unsigned int Color[3], unsigned int Alpha) {
    this->R = Color[0];
    this->G = Color[1];
    this->B = Color[2];
    this->A = Alpha;
}

bool EKG_Rect::CollideWithPoint(float PointX, float PointY) {
    return PointX > this->X && PointY > this->Y && PointX < this->X + this->W && PointY < this->Y + this->H;
}

bool EKG_Rect::CollideWithRect(float RectX, float RectY, float RectW, float RectH) {
    return true; //CollideWithPoint(RectX, RectY) ||
           //CollideWithPoint(RectX + RectW, RectY) ||
           //CollideWithPoint(RectX + RectW, RectY + RectH) ||
           //CollideWithPoint(RectX, RectY + RectH);
}

void EKG_ColorTheme::Frame(unsigned int Flag, unsigned int RedValue, unsigned int GreenValue, unsigned int BlueValue, unsigned int AlphaValue) {
    switch (Flag) {
        case BACKGROUND: {
            this->FrameBackground[0] = RedValue;
            this->FrameBackground[1] = GreenValue;
            this->FrameBackground[2] = BlueValue;
            this->FrameBackground[3] = AlphaValue;
            break;
        }

        case HIGHLIGHT: {
            this->FrameHighlight[0] = RedValue;
            this->FrameHighlight[1] = GreenValue;
            this->FrameHighlight[2] = BlueValue;
            this->FrameHighlight[3] = AlphaValue;
            break;
        }

        case FOCUSED: {
            this->FrameFocused[0] = RedValue;
            this->FrameFocused[1] = GreenValue;
            this->FrameFocused[2] = BlueValue;
            this->FrameFocused[3] = AlphaValue;
            break;
        }

        case PRESSED: {
            this->FramePressed[0] = RedValue;
            this->FramePressed[1] = GreenValue;
            this->FramePressed[2] = BlueValue;
            this->FramePressed[3] = AlphaValue;
            break;
        }

        case ACTIVY: {
            this->FrameActivy[0] = RedValue;
            this->FrameActivy[1] = GreenValue;
            this->FrameActivy[2] = BlueValue;
            this->FrameActivy[3] = AlphaValue;
            break;
        }
    }
}

void EKG_ColorTheme::Container(unsigned int Flag, unsigned int RedValue, unsigned int GreenValue, unsigned int BlueValue, unsigned int AlphaValue) {
    switch (Flag) {
        case BACKGROUND: {
            this->ContainerBackground[0] = RedValue;
            this->ContainerBackground[1] = GreenValue;
            this->ContainerBackground[2] = BlueValue;
            this->ContainerBackground[3] = AlphaValue;
            break;
        }

        case HIGHLIGHT: {
            this->ContainerHighlight[0] = RedValue;
            this->ContainerHighlight[1] = GreenValue;
            this->ContainerHighlight[2] = BlueValue;
            this->ContainerHighlight[3] = AlphaValue;
            break;
        }

        case FOCUSED: {
            this->ContainerFocused[0] = RedValue;
            this->ContainerFocused[1] = GreenValue;
            this->ContainerFocused[2] = BlueValue;
            this->ContainerFocused[3] = AlphaValue;
            break;
        }

        case PRESSED: {
            this->ContainerPressed[0] = RedValue;
            this->ContainerPressed[1] = GreenValue;
            this->ContainerPressed[2] = BlueValue;
            this->ContainerPressed[3] = AlphaValue;
            break;
        }

        case ACTIVY: {
            this->ContainerActivy[0] = RedValue;
            this->ContainerActivy[1] = GreenValue;
            this->ContainerActivy[2] = BlueValue;
            this->ContainerActivy[3] = AlphaValue;
            break;
        }
    }
}

void EKG_ColorTheme::Widget(unsigned int Flag, unsigned int RedValue, unsigned int GreenValue, unsigned int BlueValue, unsigned int AlphaValue)  {
    switch (Flag) {
        case BACKGROUND: {
            this->WidgetBackground[0] = RedValue;
            this->WidgetBackground[1] = GreenValue;
            this->WidgetBackground[2] = BlueValue;
            this->WidgetBackground[3] = AlphaValue;
            break;
        }

        case HIGHLIGHT: {
            this->WidgetHighlight[0] = RedValue;
            this->WidgetHighlight[1] = GreenValue;
            this->WidgetHighlight[2] = BlueValue;
            this->WidgetHighlight[3] = AlphaValue;
            break;
        }

        case FOCUSED: {
            this->WidgetFocused[0] = RedValue;
            this->WidgetFocused[1] = GreenValue;
            this->WidgetFocused[2] = BlueValue;
            this->WidgetFocused[3] = AlphaValue;
            break;
        }

        case PRESSED: {
            this->WidgetPressed[0] = RedValue;
            this->WidgetPressed[1] = GreenValue;
            this->WidgetPressed[2] = BlueValue;
            this->WidgetPressed[3] = AlphaValue;
            break;
        }

        case ACTIVY: {
            this->WidgetActivy[0] = RedValue;
            this->WidgetActivy[1] = GreenValue;
            this->WidgetActivy[2] = BlueValue;
            this->WidgetActivy[3] = AlphaValue;
            break;
        }
    }
}

void EKG_ColorTheme::Init() {
    Frame(BACKGROUND, 255, 255, 255, 50);
    Frame(HIGHLIGHT, 0, 0, 0, 0);
    Frame(PRESSED, 255, 255, 255, 50);
    Frame(FOCUSED, 0, 0, 0, 0);
    Frame(ACTIVY, 0, 0, 0, 0);

    Container(BACKGROUND, 255, 0, 0, 50);
    Container(HIGHLIGHT, 0, 0, 0, 0);
    Container(PRESSED, 0, 0, 0, 0);
    Container(FOCUSED, 0, 0, 0, 0);
    Container(ACTIVY, 0, 0, 0, 0);

    Widget(BACKGROUND, 0, 0, 0, 100);
    Widget(HIGHLIGHT, 255, 0, 255, 50);
    Widget(PRESSED, 255, 0, 255, 100);
    Widget(FOCUSED, 0, 0, 0, 0);
    Widget(ACTIVY, 255, 255, 255, 50);
}

bool EKG_ColorTheme::IsOutlineFrameEnabled() {
    return this->OutlineFrame;
}

bool EKG_ColorTheme::IsOutlineButtonEnabled() {
    return this->OutlineFrame;
}

void EKG_ColorTheme::Quit() {
    // Nothing.
}

void EKG_Smooth::Update(float PartialTicks) {
    this->Factory = EKG_LinearInterpolation(this->Factory, this->NextFactory, PartialTicks);
}