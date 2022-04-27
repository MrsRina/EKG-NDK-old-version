/**
 * @author Rina
 * @since 21/03/2022 at 21:05PM
 */
#pragma once
#include "EKG_core.h"
#include "EKG_tessellator.h"

#include "EKG_ui_element_frame.h"
#include "EKG_ui_element_button.h"
#include "EKG_ui_element_slider.h"
#include "EKG_ui_element_popup.h"

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

/**
 * Popup event.
 **/
typedef struct {
    std::string Info;
} EKG_PopupEvent;

/**
 * Event structure.
 **/
struct EKG_Event {
    Sint32 Type;

    EKG_PopupEvent Popup;
};

// Global runtime variable.
extern EKG_Core* const EKG_CORE;
extern EKG_Tessellator* const EKG_TESSELLATOR;

/**
 * Main class to call and build GUIs.
 */
class EKG {
protected:
    static bool ContextOkay;
public:
    /* Metrics of device. */
    static float DeviceScreenWidth, DeviceScreenHeight;

    /* Flags used in environment. */
    static const int NOPOS = -256;

    /**
     * Coordinates used in EKG.
     **/
    struct Dock {
    public:
        static const unsigned int FULL    = 1 << 1;
        static const unsigned int TOP     = 1 << 2;
        static const unsigned int BOTTOM  = 1 << 3;
        static const unsigned int LEFT    = 1 << 4;
        static const unsigned int RIGHT   = 1 << 5;
        static const unsigned int CENTER  = 1 << 6;
    };

    /**
     * Get elements value released in popup.
     **/
    struct Event {
        static const Sint32  POPUP = 1;

        static void Dispatch(Sint32 Type, void* Data1, void* Data2);
        static EKG_Event Read(SDL_Event Event);
    };

    /* Start of UI helpers method. */
    static unsigned int PointCollideDock(unsigned int Flags, float PointX, float PointY, float MinOffset, float MaxOffset, const EKG_Rect &Rect);
    static EKG_Rect GetRectDock(unsigned int p_Dock, float InitialOffset, float SizeOffset, const EKG_Rect &Origin);
    static void ScaledFingerPos(float &X, float &Y);

    static std::string CurrentFocusedTag();
    static std::string CurrentFocusedType();
    static unsigned int CurrentFocusedId();

    static EKG_Timing* Timing();
    /* End of UI helpers method. */

    /* Start of static instance methods. */
    static EKG_ShaderManager GetShaderManager();
    static EKG_FontRenderer GetFontRenderer();
    static EKG_ColorTheme GetTheme();
    /* End of static instance methods. */

    /* Start of main EKG static methods (needed use). */
    static void InitFont(const std::string &Path, unsigned int InitialSize);
    static void Init();
    static void Quit();
    /* End of main EKG static methods. */

    /* Start of setup/concurrent static methods. */
    static EKG_Frame* Frame(const std::string &Name, float InitialPosX, float InitialPosY, float InitialSizeWidth, float InitialSizeHeight);
    static EKG_Button* Button(const std::string &Name, float InitialPosX, float InitialPosY, float InitialScale);
    static EKG_Slider* Slider(const std::string &Name, float Value, float Min, float Max, float InitialPosX, float InitialPosY, float InitialScale);
    static EKG_Popup* Popup(const std::string &Name, float InitialPosX, float InitialPosY, float InitialSizeWidth, const std::list<std::string> &List);

    static std::vector<unsigned int> Children(EKG_AbstractElement* Element);
    static EKG_AbstractElement* Find(unsigned int Id);

    static void Kill(EKG_AbstractElement* &Element);
    static void Context();
    static void PollEvent(SDL_Event Event);
    static void Update(const float &DeltaTicks);
    static void Render(const float &PartialTicks);
    /* End of setup/concurrent static methods. */
};