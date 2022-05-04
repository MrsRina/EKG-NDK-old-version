#include "EKG_util.h"
#include "EKG.h"

static float MaskQuadMaterialColor[6*4], MaskQuadMaterialTexture[6*2], MaskQuadVertex[6*3];

void EKG_Scissor(int X, int Y, int W, int H) {
    W++;
    H++;

    int FactorW = X + W;
    int FactorH = Y + H;

    glEnable(GL_SCISSOR_TEST);
    glScissor(X, (int) EKG::DeviceScreenHeight - FactorH, FactorW - X, FactorH - Y);
}

void EKG_EndScissor() {
    glDisable(GL_SCISSOR_TEST);
}

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

void EKG_Log(const std::string& Log) {
    std::string FullLog = "[EKG] " + Log;

    // Send log using SDL function.
    SDL_Log("%s", FullLog.c_str());
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

    float R = (float) Color.R / 255.0F;
    float G = (float) Color.G / 255.0F;
    float B = (float) Color.B / 255.0F;
    float A = (float) Color.A / 255.0F;

    MaskQuadMaterialColor[0] = R;
    MaskQuadMaterialColor[1] = G;
    MaskQuadMaterialColor[2] = B;
    MaskQuadMaterialColor[3] = A;

    MaskQuadMaterialColor[4] = R;
    MaskQuadMaterialColor[5] = G;
    MaskQuadMaterialColor[6] = B;
    MaskQuadMaterialColor[7] = A;

    MaskQuadMaterialColor[8] = R;
    MaskQuadMaterialColor[9] = G;
    MaskQuadMaterialColor[10] = B;
    MaskQuadMaterialColor[11] = A;

    MaskQuadMaterialColor[12] = R;
    MaskQuadMaterialColor[13] = G;
    MaskQuadMaterialColor[14] = B;
    MaskQuadMaterialColor[15] = A;

    MaskQuadMaterialColor[16] = R;
    MaskQuadMaterialColor[17] = G;
    MaskQuadMaterialColor[18] = B;
    MaskQuadMaterialColor[19] = A;

    MaskQuadMaterialColor[20] = R;
    MaskQuadMaterialColor[21] = G;
    MaskQuadMaterialColor[22] = B;
    MaskQuadMaterialColor[23] = A;

    u_int8_t I = 0;

    MaskQuadVertex[0]= X;
    MaskQuadVertex[1]= Y;
    MaskQuadVertex[2]= 0;
    MaskQuadVertex[3]= X;
    MaskQuadVertex[4]= Y + H;
    MaskQuadVertex[5]= 0;
    MaskQuadVertex[6]= X + W;
    MaskQuadVertex[7]= Y + H;
    MaskQuadVertex[8]= 0;
    MaskQuadVertex[9]= X + W;
    MaskQuadVertex[10]= Y + H;
    MaskQuadVertex[11]= 0;
    MaskQuadVertex[12]= X;
    MaskQuadVertex[13]= Y;
    MaskQuadVertex[14]= 0;
    MaskQuadVertex[15]= X + W;
    MaskQuadVertex[16]= Y;
    MaskQuadVertex[17]= 0;

    EKG_TESSELLATOR->Draw(18, 24, MaskQuadVertex, MaskQuadMaterialColor);
}

void EKG_DrawOutlineShape(float X, float Y, float W, float H, float LineThickness, const EKG_Color &Color) {
    EKG_TESSELLATOR->NewDraw(GL_LINE_STRIP, 6);
    EKG_TESSELLATOR->SetTextureColor(Color);

    MaskQuadVertex[0] = X;
    MaskQuadVertex[1] = Y;
    MaskQuadVertex[2] = 0;
    MaskQuadVertex[3] = X;
    MaskQuadVertex[4] = Y + H;
    MaskQuadVertex[5] = 0;
    MaskQuadVertex[6] = X + W;
    MaskQuadVertex[7] = Y + H;
    MaskQuadVertex[8] = 0;
    MaskQuadVertex[9] = X + W;
    MaskQuadVertex[10] = Y + H;
    MaskQuadVertex[11] = 0;
    MaskQuadVertex[12] = X + W;
    MaskQuadVertex[13] = Y;
    MaskQuadVertex[14] = 0;
    MaskQuadVertex[15] = X;
    MaskQuadVertex[16] = Y;
    MaskQuadVertex[17] = 0;

    float R = (float) Color.R / 255.0F;
    float G = (float) Color.G / 255.0F;
    float B = (float) Color.B / 255.0F;
    float A = (float) Color.A / 255.0F;

    MaskQuadMaterialColor[0] = R;
    MaskQuadMaterialColor[1] = G;
    MaskQuadMaterialColor[2] = B;
    MaskQuadMaterialColor[3] = A;
    MaskQuadMaterialColor[4] = R;
    MaskQuadMaterialColor[5] = G;
    MaskQuadMaterialColor[6] = B;
    MaskQuadMaterialColor[7] = A;
    MaskQuadMaterialColor[8] = R;
    MaskQuadMaterialColor[9] = G;
    MaskQuadMaterialColor[10] = B;
    MaskQuadMaterialColor[11] = A;
    MaskQuadMaterialColor[12] = R;
    MaskQuadMaterialColor[13] = G;
    MaskQuadMaterialColor[14] = B;
    MaskQuadMaterialColor[15] = A;
    MaskQuadMaterialColor[16] = R;
    MaskQuadMaterialColor[17] = G;
    MaskQuadMaterialColor[18] = B;
    MaskQuadMaterialColor[19] = A;
    MaskQuadMaterialColor[20] = R;
    MaskQuadMaterialColor[21] = G;
    MaskQuadMaterialColor[22] = B;
    MaskQuadMaterialColor[23] = A;

    // Draw.
    glLineWidth(LineThickness);
    EKG_TESSELLATOR->Draw(18, 24, MaskQuadVertex, MaskQuadMaterialColor);
}

void EKG_DrawFilledRect(const EKG_Rect &Rect, const EKG_Color &Color) {
    EKG_DrawFilledShape(Rect.X, Rect.Y, Rect.W, Rect.H, Color);
}

void EKG_DrawOutlineRect(const EKG_Rect &Rect, float LineThickness, const EKG_Color &Color) {
    EKG_DrawOutlineShape(Rect.X, Rect.Y, Rect.W, Rect.H, LineThickness, Color);
}

void EKG_StoreShape(float X, float Y, float W, float H) {

}

void EKG_DrawTextureRect(const EKG_Rect &Rect, float TextureX, float TextureY, float TextureW, float TextureH, const EKG_Texture &Texture, const EKG_Color &Color) {
    EKG_DrawTextureShape(Rect.X, Rect.Y, Rect.W, Rect.H, TextureX, TextureY, TextureW, TextureH, Texture, Color);
}

void EKG_DrawTextureShape(float X, float Y, float W, float H, float TextureX, float TextureY, float TextureW, float TextureH, const EKG_Texture &Texture, const EKG_Color &Color) {
    EKG_TESSELLATOR->NewDraw(GL_TRIANGLES, 6);
    EKG_TESSELLATOR->BindTexture(Texture);
    EKG_TESSELLATOR->SetTextureColor(Color);

    unsigned int I = 0;

    MaskQuadVertex[I]   = X;
    MaskQuadVertex[I++] = Y;
    MaskQuadVertex[I++] = 0;
    MaskQuadVertex[I++] = X;
    MaskQuadVertex[I++] = Y;
    MaskQuadVertex[I++] = H;
    MaskQuadVertex[I++] = 0;
    MaskQuadVertex[I++] = X + W;
    MaskQuadVertex[I++] = Y + H;
    MaskQuadVertex[I++] = 0;
    MaskQuadVertex[I++] = X + W;
    MaskQuadVertex[I++] = Y + H;
    MaskQuadVertex[I++] = 0;
    MaskQuadVertex[I++] = X;
    MaskQuadVertex[I++] = Y;
    MaskQuadVertex[I++] = 0;
    MaskQuadVertex[I++] = X + W;
    MaskQuadVertex[I++] = Y;
    MaskQuadVertex[I++] = 0;

    float U = 1.0F + (TextureW != 1 ? ((TextureW - (float) EKG_TESSELLATOR->GetTextureWidth()) / (float) EKG_TESSELLATOR->GetTextureWidth()) : 0.0F);
    float V = 1.0F + (TextureH != 1 ? ((TextureH - (float) EKG_TESSELLATOR->GetTextureHeight()) / (float) EKG_TESSELLATOR->GetTextureHeight()) : 0.0F);

    MaskQuadMaterialTexture[0] = 0;
    MaskQuadMaterialTexture[1] = 0;
    MaskQuadMaterialTexture[2] = 0;
    MaskQuadMaterialTexture[3] = V;
    MaskQuadMaterialTexture[4] = U;
    MaskQuadMaterialTexture[5] = V;
    MaskQuadMaterialTexture[6] = U;
    MaskQuadMaterialTexture[7] = V;
    MaskQuadMaterialTexture[8] = 0;
    MaskQuadMaterialTexture[9] = 0;
    MaskQuadMaterialTexture[10] = U;
    MaskQuadMaterialTexture[11] = 0;

    EKG_TESSELLATOR->Draw(18, 12, MaskQuadVertex, MaskQuadMaterialTexture);
}

std::string EKG_Print(const std::string& Tag, unsigned int Id) {
    return " [" + Tag + + "-" + std::to_string(Id) + "] ";
}

float EKG_LinearInterpolation(float A, float B, float T) {
    return A + (B - A) * T;
}

float EKG_Min(float Min, float Value) {
    return Value < Min ? Min : Value;
}

float EKG_Max(float Value, float Max) {
    return Value > Max ? Max : Value;
}

float EKG_Clampf(float V, float Min, float Max) {
    return V < Min ? Min : (V > Max ? Max : V);
}

bool EKG_StringContains(const std::string &StringOne, const std::string &StringTwo) {
    return StringOne.find(StringTwo) != std::string::npos;
}

float EKG_AnimationSmooth(float Duration, float Time) {
    float Factor = Time / Duration;
    return EKG_Clampf(6 * pow(Factor, 5) - (15 * pow(Factor, 4)) + (10 * pow(Factor, 3)), 0.0F, 1.0f);
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
    return this->X < RectX + RectW &&
           this->X + this->W > RectX &&
           this->Y < RectY + RectH &&
           this->Y + this->H > RectY;
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
    Frame(BACKGROUND, 61, 61, 60, 255);
    Frame(HIGHLIGHT, 0, 0, 0, 0);
    Frame(PRESSED, 255, 255, 255, 50);
    Frame(FOCUSED, 0, 0, 0, 0);
    Frame(ACTIVY, 0, 0, 0, 0);

    Container(BACKGROUND, 61, 61, 60, 255);
    Container(HIGHLIGHT, 0, 0, 0, 0);
    Container(PRESSED, 255, 255, 255, 50);
    Container(FOCUSED, 0, 0, 0, 0);
    Container(ACTIVY, 0, 0, 0, 0);

    Widget(BACKGROUND, 0, 0, 0, 100);
    Widget(HIGHLIGHT, 255, 0, 255, 50);
    Widget(PRESSED, 255, 0, 255, 100);
    Widget(FOCUSED, 0, 0, 0, 0);
    Widget(ACTIVY, 255, 0, 255, 50);

    this->OutlineButton = true;
    this->OutlineFrame = true;
    this->OutlineCombobox = true;

    this->StringColor.Set(255, 255, 255);
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

bool EKG_ColorTheme::IsOutlineSliderEnabled() {
    return this->OutlineSlider;
}

bool EKG_ColorTheme::isOutlineComboboxEnabled() {
    return this->OutlineCombobox;
}

void EKG_Smooth::Update(float PartialTicks) {
    this->Factory = EKG_LinearInterpolation(this->Factory, this->NextFactory, PartialTicks);
}

void EKG_Timing::Start() {
    this->PreviousTick = SDL_GetTicks();
    this->IsUsing = true;
}

bool EKG_Timing::EndIf(float MS) {
    const bool Flag = (float) SDL_GetTicks() - this->PreviousTick > MS;

    this->PreviousTick = (float) SDL_GetTicks();
    this->Stop();

    return Flag;
}

void EKG_Timing::Stop() {
    this->IsUsing = false;
}
