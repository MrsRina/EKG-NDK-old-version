/**
 * @author Rina
 * @since 21/03/2022 at 21:05PM
 */
#pragma once
#include "ekg_includes.h"

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
bool ekg_string_in(const std::string &StringOne, const std::string &StringTwo);

/**
 * min da value.
 * @param Min
 * @return value.
 */
float EKG_Min(float Min, float Value);

/**
 * max da value.
 * @param Max
 * @return value.
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
 * Send log to console (remember before call this, invoke ekg::init() to prevent errors using this function)
 * @param Log - The log.
 */
void ekg_log(const std::string &Log);

/**
 * Convert to a pattern design.
 * @param Tag - The tag of some element.
 * @param Id - The id of some element.
 * @return [TAG-ID] pattern.
 */
std::string ekg_print(const std::string &Tag, unsigned int Id);

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
 * rect AABB.
 */
struct ekg_rect {
    float x, y, W, H;

    bool CollideWithPoint(float PointX, float PointY);
    bool CollideWithRect(float RectX, float RectY, float RectW, float RectH);
};

/**
 * Put ids in a stack.
 */
struct ekg_stack {
    std::vector<unsigned int> StackedIds;

    bool Contains(unsigned int Id);
    bool Rem(unsigned int Id);
    bool Put(unsigned int Id);
};

/**
 * Color util.
 */
struct ekg_color {
    ekg_color(uint8_t Red, uint8_t Green, uint8_t Blue, uint8_t Alpha);
    ekg_color(uint8_t Red, uint8_t Green, uint8_t Blue);
    ekg_color(uint8_t Color[4]);
    ekg_color(uint8_t Color[3], uint8_t Alpha);

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
class ekg_color_theme {
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

    ekg_color StringColor = ekg_color(255, 255, 255);
    ekg_color StringFadeColor = ekg_color(255, 255, 255, 150);

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
struct ekg_data {
    std::string name, tag;

    float data_x = 0.0F, DataY = 0.0F, data_width = 0.0F, data_height = 0.0F;
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
} ekg_timing;

/**
 * An linear interpolation helper, used in UI animations.
 */
struct EKG_Smooth {
    float Factory;
    float NextFactory;

    void Update(float PartialTicks);
};

/* Start of scissor functions. */
void ekg_scissor(int X, int Y, int W, int H);
void ekg_end_scissor();
/* End of scissor functions. */

/* Start of draw functions. */
void ekg_draw_filled_shape(float X, float Y, float W, float H, const ekg_color &Color);
void ekg_draw_outline_shape(float X, float Y, float W, float H, float LineThickness, const ekg_color &Color);
void ekg_draw_filled_rect(const ekg_rect &Rect, const ekg_color &Color);
void ekg_draw_outline_rect(const ekg_rect &Rect, float LineThickness, const ekg_color &Color);
void EKG_StoreShape(float X, float Y, float W, float H);
void EKG_StoreTextureCoordinates(float TextureX, float TextureY, float TextureW, float TextureH);
void EKG_DrawTextureShape(float X, float Y, float W, float H, float TextureX, float TextureY, float TextureW, float TextureH, const ekg_data &Texture, const ekg_color &Color);
void EKG_DrawTextureRect(const ekg_rect &Rect, float TextureX, float TextureY, float TextureW, float TextureH, const ekg_data &Texture, const ekg_color &Color);
/* End of draw functions. */