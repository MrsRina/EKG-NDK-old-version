/**
 * @author Rina
 * @since 21/03/2022 at 21:05PM
 */
#pragma once
#include "EKG_core.h"
#include "EKG_tessellator.h"

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
    static void PrepareContext();
    static void OnEvent(SDL_Event Event);
    static void OnUpdate(float DeltaTicks);
    static void OnRender(float PartialTicks);
    /* End of setup/concurrent static methods. */
};