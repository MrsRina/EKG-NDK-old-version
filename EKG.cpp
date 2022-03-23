#include "EKG.h"

EKG_Core* const EKG_CORE = new EKG_Core();
EKG_Tessellator* const EKG_TESSELLATOR = new EKG_Tessellator();

void EKG_StartUseShader(GLuint ShaderId) {
    glUseProgram(ShaderId);
}

void EKG_EndUseShader() {
    glUseProgram(0);
}

void EKG_ShaderProjMatrix(unsigned int ShaderId, float *ProjMatrix) {
    glUseProgram(ShaderId);
    glUniformMatrix4fv(glGetUniformLocation(ShaderId, "ProjMatrix"), 1, GL_FALSE, ProjMatrix);
    glUseProgram(0);
}

GLuint EKG_GetShaderAttribute(const std::string &ShaderName, unsigned int ShaderId, const std::string &AttributeName) {
    GLint Attribute = glGetAttribLocation(ShaderId, AttributeName.c_str());

    if (Attribute == -1) {
        EKG_Log("Unknown attribute in shader: " + ShaderName + " '" + AttributeName + "' (attribute)!");
        return 0;
    }

    return (GLuint) Attribute;
}

void EKG_SetShaderUniformBool(unsigned int ShaderId, const std::string &Name, bool Value) {
    GLint Flag = glGetUniformLocation(ShaderId, Name.c_str());

    if (Flag == -1) {
        EKG_Log("Shader waring: Uniform location does not exist '" + Name + "'.");
        return;
    }

    glUniform1i(Flag, (int) Value);
}

void EKG_SetShaderUniformInt(unsigned int ShaderId, const std::string &Name, int Value) {
    GLint Flag = glGetUniformLocation(ShaderId, Name.c_str());

    if (Flag == -1) {
        EKG_Log("Shader waring: Uniform location does not exist '" + Name + "'.");
        return;
    }

    glUniform1i(Flag, Value);
}

void EKG_SetShaderUniformFloat(unsigned int ShaderId, const std::string &Name, float Value) {
    GLint Flag = glGetUniformLocation(ShaderId, Name.c_str());

    if (Flag == -1) {
        EKG_Log("Shader waring: Uniform location does not exist '" + Name + "'.");
        return;
    }

    glUniform1f(Flag, Value);
}

void EKG_SetShaderUniformVec2(unsigned int ShaderId, const std::string &Name, const glm::vec2 &Value) {
    GLint Flag = glGetUniformLocation(ShaderId, Name.c_str());

    if (Flag == -1) {
        EKG_Log("Shader waring: Uniform location does not exist '" + Name + "'.");
        return;
    }

    glUniform2f(Flag, 1, Value[0]);
}

void EKG_SetShaderUniformVec2(unsigned int ShaderId, const std::string &Name, float X, float Y) {
    GLint Flag = glGetUniformLocation(ShaderId, Name.c_str());

    if (Flag == -1) {
        EKG_Log("Shader waring: Uniform location does not exist '" + Name + "'.");
        return;
    }

    glUniform2f(Flag, X, Y);
}

void EKG_SetShaderUniformVec3(unsigned int ShaderId, const std::string &Name, const glm::vec3 &Value) {
    GLint Flag = glGetUniformLocation(ShaderId, Name.c_str());

    if (Flag == -1) {
        EKG_Log("Shader waring: Uniform location does not exist '" + Name + "'.");
        return;
    }

    glUniform3fv(Flag, 1, &Value[0]);
}

void EKG_SetShaderUniformVec3(unsigned int ShaderId, const std::string &Name, float X, float Y, float Z) {
    GLint Flag = glGetUniformLocation(ShaderId, Name.c_str());

    if (Flag == -1) {
        EKG_Log("Shader waring: Uniform location does not exist '" + Name + "'.");
        return;
    }

    glUniform3f(Flag, X, Y, Z);
}

void EKG_SetShaderUniformVec4(unsigned int ShaderId, const std::string &Name, const glm::vec4 &Value) {
    GLint Flag = glGetUniformLocation(ShaderId, Name.c_str());

    if (Flag == -1) {
        EKG_Log("Shader waring: Uniform location does not exist '" + Name + "'.");
        return;
    }

    glUniform4fv(Flag, 1, &Value[0]);
}

void EKG_SetShaderUniformVec4(unsigned int ShaderId, const std::string &Name, float X, float Y, float Z, float W) {
    GLint Flag = glGetUniformLocation(ShaderId, Name.c_str());

    if (Flag == -1) {
        EKG_Log("Shader waring: Uniform location does not exist '" + Name + "'.");
        return;
    }

    glUniform4f(Flag, X, Y, Z, W);
}

void EKG_SetShaderUniformMat2(unsigned int ShaderId, const std::string &Name, const glm::mat2 &Mat) {
    GLint Flag = glGetUniformLocation(ShaderId, Name.c_str());

    if (Flag == -1) {
        EKG_Log("Shader waring: Uniform location does not exist '" + Name + "'.");
        return;
    }

    glUniformMatrix2fv(Flag, 1, GL_FALSE, &Mat[0][0]);
}

void EKG_SetShaderUniformMat3(unsigned int ShaderId, const std::string &Name, const glm::mat3 &Mat) {
    GLint Flag = glGetUniformLocation(ShaderId, Name.c_str());

    if (Flag == -1) {
        EKG_Log("Shader waring: Uniform location does not exist '" + Name + "'.");
        return;
    }

    glUniformMatrix3fv(Flag, 1, GL_FALSE, &Mat[0][0]);
}

void EKG_SetShaderUniformMat4(unsigned int ShaderId, const std::string &Name, const glm::mat4 &Mat) {
    GLint Flag = glGetUniformLocation(ShaderId, Name.c_str());

    if (Flag == -1) {
        EKG_Log("Shader waring: Uniform location does not exist '" + Name + "'.");
        return;
    }

    glUniformMatrix4fv(Flag, 1, GL_FALSE, &Mat[0][0]);
}

bool EKG::ContextOkay = false;

void EKG::Init() {
    EKG_CORE->Init();
    EKG_TESSELLATOR->Init();

    // Call it was initialized property (or just initialized).
    EKG_Log("Core initialized.");
}

void EKG::Quit() {
    EKG_CORE->Quit();
    EKG_TESSELLATOR->Quit();

    // Say it was quited successfully or just quit.
    EKG_Log("Core quited successfully.");
}

void EKG::OnEvent(SDL_Event Event) {

}

void EKG::OnUpdate(float DeltaTicks) {
    EKG_CORE->OnUpdate(DeltaTicks);
}

void EKG::OnRender(float PartialTicks) {
    EKG_CORE->OnRender(PartialTicks);
}

void EKG::InitFont(const std::string &Path, unsigned int InitialSize) {
    EKG_CORE->FontRenderer.SetFontPath(Path);
    EKG_CORE->FontRenderer.SetFontSize(InitialSize);
}

void EKG::PrepareContext() {
    float Viewport[4];
    glGetFloatv(GL_VIEWPORT, Viewport);

    float ProjectionMatrix4x4[16];
    EKG_Ortho2D(ProjectionMatrix4x4, 0.0F, Viewport[2], Viewport[3], 0.0F);

    // We set projection ortho.
    EKG_ShaderProjMatrix(EKG_CORE->ShaderManager.GetTessellatorShader(), ProjectionMatrix4x4);
}

EKG_FontRenderer EKG::GetFontRenderer() {
    return EKG_CORE->FontRenderer;
}

EKG_ShaderManager EKG::GetShaderManager() {
    return EKG_CORE->ShaderManager;
}