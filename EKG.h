/**
 * @author Rina
 * @since 21/03/2022 at 21:05PM
 */
#pragma once
#include "EKG_core.h"
#include "EKG_tessellator.h"
#include "EKG_element_ui.h"

/* Start of shader util functions. */
void EKG_StartUseShader(GLuint ShaderId);
void EKG_EndUseShader();
void EKG_ShaderProjMatrix(unsigned int ShaderId, float *ProjMatrix);
GLuint EKG_GetShaderAttribute(const std::string &ShaderName, unsigned int ShaderId, const std::string &AttributeName);
void EKG_SetShaderUniformBool(unsigned int ShaderId, const std::string &Name, bool Value);
void EKG_SetShaderUniformInt(unsigned int ShaderId, const std::string &Name, int Value);
void EKG_SetShaderUniformFloat(unsigned int ShaderId, const std::string &Name, float Value);
void EKG_SetShaderUniformVec2(unsigned int ShaderId, const std::string &Name, const glm::vec2 &Value);
void EKG_SetShaderUniformVec2(unsigned int ShaderId, const std::string &Name, float X, float Y);
void EKG_SetShaderUniformVec3(unsigned int ShaderId, const std::string &Name, const glm::vec3 &Value);
void EKG_SetShaderUniformVec3(unsigned int ShaderId, const std::string &Name, float X, float Y, float Z);
void EKG_SetShaderUniformVec4(unsigned int ShaderId, const std::string &Name, const glm::vec4 &Value);
void EKG_SetShaderUniformVec4(unsigned int ShaderId, const std::string &Name, float X, float Y, float Z, float W);
void EKG_SetShaderUniformMat2(unsigned int ShaderId, const std::string &Name, const glm::mat2 &Mat);
void EKG_SetShaderUniformMat3(unsigned int ShaderId, const std::string &Name, const glm::mat3 &Mat);
void EKG_SetShaderUniformMat4(unsigned int ShaderId, const std::string &Name, const glm::mat4 &Mat);
/* End of shader util functions. */

/* Start of helper functions. */
bool EKG_FlagContains(unsigned int Flags, unsigned int Flag);
void EKG_FlagRemove(unsigned int &Flags, unsigned int Flag);
void EKG_FlagAdd(unsigned int &Flags, unsigned int Flag);
/* End of helper functions. */

// Global runtime variable.
extern EKG_Core* const EKG_CORE;
extern EKG_Tessellator* const EKG_TESSELLATOR;

/**
 * Theme colors for widget or config.
 */
class EKG_ColorTheme {
private:
    bool WireframeFrame;
    bool WireframeButton;
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

    void OnInit();

    bool IsWireframeFrameEnabled();
    bool IsWireframeButtonEnabled();

    void Frame(unsigned int Flag, unsigned int RedValue, unsigned int GreenValue, unsigned int BlueValue, unsigned int AlphaValue);
    void Container(unsigned int Flag, unsigned int RedValue, unsigned int GreenValue, unsigned int BlueValue, unsigned int AlphaValue);
    void Widget(unsigned int Flag, unsigned int RedValue, unsigned int GreenValue, unsigned int BlueValue, unsigned int AlphaValue);
};

/**
 * Main class to call and build GUIs.
 */
class EKG {
protected:
    static bool ContextOkay;
public:
    /* Metrics of device. */
    static float DeviceScreenWidth, DeviceScreenHeight;

    /**
     * Coordinates used in EKG.
     */
    enum Dock {
        UNKNOWN = 1 << 0,
        FULL    = 1 << 1,
        TOP     = 1 << 2,
        BOTTOM  = 1 << 3,
        LEFT    = 1 << 4,
        RIGHT   = 1 << 5,
        CENTER  = 1 << 6
    };

    /* Start of UI helpers method. */
    static Dock PointCollideDock(unsigned int Flags, float PointX, float PointY, float MinOffset, float MaxOffset, const EKG_Rect &Rect);
    static EKG_Rect GetRectDock(Dock p_Dock, float InitialOffset, float SizeOffset, const EKG_Rect &Origin);
    static void ScaledFingerPos(float &X, float &Y);
    /* End of UI helpers method. */

    /* Start of static instance methods. */
    static EKG_ShaderManager GetShaderManager();
    static EKG_FontRenderer GetFontRenderer();
    /* End of static instance methods. */

    /* Start of main EKG static methods (needed use). */
    static void InitFont(const std::string &Path, unsigned int InitialSize);
    static void Init();
    static void Quit();
    /* End of main EKG static methods. */

    /* Start of setup/concurrent static methods. */
    static EKG_Frame* Frame(const std::string &Name, float InitialPosX, float InitialPosY, float InitialSizeWidth, float InitialSizeHeight);

    static void Context();
    static void Event(SDL_Event Event);
    static void Update(float DeltaTicks);
    static void Render(float PartialTicks);
    /* End of setup/concurrent static methods. */
};