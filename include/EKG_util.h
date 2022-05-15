/**
 * @author Rina
 * @since 21/03/2022 at 21:05PM
 */
#pragma once
#include "EKG_includes.h"

/**
 * Smooth animation.
 * @param Progress
 * @param Duration
 * @param Time
 * @return
 */
float EKG_AnimationSmooth(float Duration, float Time);

/**
 * Compare strings and find equals char.
 * @param StringOne
 * @param StringTwo
 * @return
 */
bool EKG_StringContains(const std::string &StringOne, const std::string &StringTwo);

/**
 * Min da value.
 * @param Min
 * @return Value.
 */
float EKG_Min(float Min, float Value);

/**
 * Max da value.
 * @param Max
 * @return Value.
 */
float EKG_Max(float Value, float Max);

/**
 * Clampf.
 * @param V
 * @param Min
 * @param Max
 * @return
 */
float EKG_Clampf(float V, float Min, float Max);

/**
 * Apply ortho view 2D.
 */
void EKG_Ortho2D(float* Mat, float Left, float Right, float Bottom, float Top);

/**
 * Send log to console (remember before call this, invoke EKG::Init() to prevent errors using this function)
 * @param Log - The log.
 */
void EKG_Log(const std::string &Log);

/**
 * Convert to a pattern design.
 * @param Tag - The tag of some element.
 * @param Id - The id of some element.
 * @return [TAG-ID] pattern.
 */
std::string EKG_Print(const std::string &Tag, unsigned int Id);

/**
 * A lerp fuction.
 * @param A - The previous value.
 * @param B - The next value.
 * @param T - Parcial ticks.
 * @return
 */
float EKG_LinearInterpolation(float A, float B, float T);

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
    EKG_Color(uint8_t Red, uint8_t Green, uint8_t Blue, uint8_t Alpha);
    EKG_Color(uint8_t Red, uint8_t Green, uint8_t Blue);
    EKG_Color(uint8_t Color[4]);
    EKG_Color(uint8_t Color[3], uint8_t Alpha);

    void Set(uint8_t Red, uint8_t Green, uint8_t Blue, uint8_t Alpha);
    void Set(uint8_t Red, uint8_t Green, uint8_t Blue);
    void Set(uint8_t Color[4]);
    void Set(uint8_t Color[3], uint8_t Alpha);

    uint8_t R, G, B, A;

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
    bool OutlineSlider;
    bool OutlineCombobox;
public:
    static const uint8_t BACKGROUND = 0;
    static const uint8_t HIGHLIGHT  = 1;
    static const uint8_t FOCUSED    = 2;
    static const uint8_t PRESSED    = 3;
    static const uint8_t ACTIVY     = 5;

    uint8_t FrameBackground[4];
    uint8_t FrameHighlight[4];
    uint8_t FrameFocused[4];
    uint8_t FramePressed[4];
    uint8_t FrameActivy[4];

    uint8_t ContainerBackground[4];
    uint8_t ContainerHighlight[4];
    uint8_t ContainerFocused[4];
    uint8_t ContainerPressed[4];
    uint8_t ContainerActivy[4];

    uint8_t WidgetBackground[4];
    uint8_t WidgetHighlight[4];
    uint8_t WidgetFocused[4];
    uint8_t WidgetPressed[4];
    uint8_t WidgetActivy[4];

    EKG_Color StringColor = EKG_Color(255, 255, 255);
    EKG_Color StringFadeColor = EKG_Color(255, 255, 255, 150);

    void Init();
    void Quit();

    bool IsOutlineFrameEnabled();
    bool IsOutlineButtonEnabled();
    bool IsOutlineSliderEnabled();
    bool isOutlineComboboxEnabled();

    void Frame(uint8_t Flag, uint8_t RedValue, uint8_t GreenValue, uint8_t BlueValue, uint8_t AlphaValue);
    void Container(uint8_t Flag, uint8_t RedValue, uint8_t GreenValue, uint8_t BlueValue, uint8_t AlphaValue);
    void Widget(uint8_t Flag, uint8_t RedValue, uint8_t GreenValue, uint8_t BlueValue, uint8_t AlphaValue);
};

/**
 * Save texture id, position or store into memory.
 */
struct EKG_Data {
    std::string Name, Tag;

    float DataX = 0.0F, DataY = 0.0F, DataWidth = 0.0F, DataHeight = 0.0F;
    unsigned int Id = 0;
};

/**
 * Handler finger press timing.
 **/
typedef struct EKG_Timing {
    float PreviousTick;
    bool IsUsing;

    void Start();
    void Stop();
    bool EndIf(float MS);
} EKG_Timing;

/**
 * An linear interpolation helper, used in UI animations.
 */
struct EKG_Smooth {
    float Factory;
    float NextFactory;

    void Update(float PartialTicks);
};

/* Start of scissor functions. */
void EKG_Scissor(int X, int Y, int W, int H);
void EKG_EndScissor();
/* End of scissor functions. */

/* Start of draw functions. */
void EKG_DrawFilledShape(float X, float Y, float W, float H, const EKG_Color &Color);
void EKG_DrawOutlineShape(float X, float Y, float W, float H, float LineThickness, const EKG_Color &Color);
void EKG_DrawFilledRect(const EKG_Rect &Rect, const EKG_Color &Color);
void EKG_DrawOutlineRect(const EKG_Rect &Rect, float LineThickness, const EKG_Color &Color);
void EKG_StoreShape(float X, float Y, float W, float H);
void EKG_StoreTextureCoordinates(float TextureX, float TextureY, float TextureW, float TextureH);
void EKG_DrawTextureShape(float X, float Y, float W, float H, float TextureX, float TextureY, float TextureW, float TextureH, const EKG_Data &Texture, const EKG_Color &Color);
void EKG_DrawTextureRect(const EKG_Rect &Rect, float TextureX, float TextureY, float TextureW, float TextureH, const EKG_Data &Texture, const EKG_Color &Color);
/* End of draw functions. */