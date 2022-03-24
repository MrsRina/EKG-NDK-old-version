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
 * Convert to a pattern design.
 * @param Tag - The tag of some element.
 * @param Id - The id of some element.
 * @return [TAG-ID] pattern.
 */
std::string EKG_Print(const std::string& Tag, unsigned int Id);

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
    EKG_Color(unsigned int Color[4]);
    EKG_Color(unsigned int Color[3], unsigned int Alpha);

    void Set(unsigned int Red, unsigned int Green, unsigned int Blue, unsigned int Alpha);
    void Set(unsigned int Red, unsigned int Green, unsigned int Blue);
    void Set(unsigned int Color[4]);
    void Set(unsigned int Color[3], unsigned int Alpha);

    unsigned int R, G, B, A;

    float GetRedf();
    float GetGreenf();
    float GetBluef();
    float GetAlphaf();
};

/**
 * Theme colors for widget or config.
 */
class EKG_ColorTheme {
private:
    bool OutlineFrame;
    bool OutlineButton;
public:
    static const unsigned int BACKGROUND = 0;
    static const unsigned int HIGHLIGHT  = 1;
    static const unsigned int FOCUSED    = 2;
    static const unsigned int PRESSED    = 3;
    static const unsigned int ACTIVY     = 5;

    unsigned int FrameBackground[4];
    unsigned int FrameHighlight[4];
    unsigned int FrameFocused[4];
    unsigned int FramePressed[4];
    unsigned int FrameActivy[4];

    unsigned int ContainerBackground[4];
    unsigned int ContainerHighlight[4];
    unsigned int ContainerFocused[4];
    unsigned int ContainerPressed[4];
    unsigned int ContainerActivy[4];

    unsigned int WidgetBackground[4];
    unsigned int WidgetHighlight[4];
    unsigned int WidgetFocused[4];
    unsigned int WidgetPressed[4];
    unsigned int WidgetActivy[4];

    void Init();
    void Quit();

    bool IsOutlineFrameEnabled();
    bool IsOutlineButtonEnabled();

    void Frame(unsigned int Flag, unsigned int RedValue, unsigned int GreenValue, unsigned int BlueValue, unsigned int AlphaValue);
    void Container(unsigned int Flag, unsigned int RedValue, unsigned int GreenValue, unsigned int BlueValue, unsigned int AlphaValue);
    void Widget(unsigned int Flag, unsigned int RedValue, unsigned int GreenValue, unsigned int BlueValue, unsigned int AlphaValue);
};

/**
 * Save texture id, width & height of original size texture.
 */
struct EKG_Texture {
    float Width, Height;
    unsigned int Id;
};

/* Start of draw functions. */
void EKG_DrawFilledShape(float X, float Y, float W, float H, const EKG_Color &Color);
void EKG_DrawOutlineShape(float X, float Y, float W, float H, float LineThickness, const EKG_Color &Color);
void EKG_DrawFilledRect(const EKG_Rect &Rect, const EKG_Color &Color);
void EKG_DrawOutlineRect(const EKG_Rect &Rect, float LineThickness, const EKG_Color &Color);
void EKG_StoreShape(float X, float Y, float W, float H);
void EKG_StoreTextureCoordinates(float TextureX, float TextureY, float TextureW, float TextureH);
void EKG_DrawTextureShape(float X, float Y, float W, float H, float TextureX, float TextureY, float TextureW, float TextureH, const EKG_Texture &Texture, const EKG_Color &Color);
void EKG_DrawTextureRect(const EKG_Rect &Rect, float TextureX, float TextureY, float TextureW, float TextureH, const EKG_Texture &Texture, const EKG_Color &Color);
/* End of draw functions. */