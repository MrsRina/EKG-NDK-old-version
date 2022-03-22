#include "EKG_shader.h"
#include "EKG_util.h"
#include "EKG.h"

void EKG_ShaderManager::Init() {
    std::string TessellatorVertexSource = "attribute vec4 VertexPosition;\n"
                                          "attribute vec4 VertexColor;\n"
                                          "\n"
                                          "uniform vec4 ColorVertex;\n"
                                          "uniform mat4 ProjMatrix;\n"
                                          "\n"
                                          "varying vec4 FragmentColor;\n"
                                          "\n"
                                          "void main() {\n"
                                          "    gl_Position = ProjMatrix * VertexPosition;\n"
                                          "    FragmentColor = VertexColor;\n"
                                          "}";

    std::string TessellatorFragmentSource = "precision mediump float;\n"
                                            "\n"
                                            "varying vec4 FragmentColor;\n"
                                            "\n"
                                            "uniform sampler2D TextureSampler;\n"
                                            "uniform bool ContainsTexture;\n"
                                            "uniform vec4 DirectVertexColor;\n"
                                            "\n"
                                            "void main() {\n"
                                            "    if (ContainsTexture) {\n"
                                            "        vec4 Sampler = texture2D(TextureSampler, vec2(FragmentColor.x, FragmentColor.y));\n"
                                            "\n"
                                            "        float R = Sampler.r - (DirectVertexColor.r - 1.0);\n"
                                            "        float G = Sampler.g - (DirectVertexColor.g - 1.0);\n"
                                            "        float B = Sampler.b - (DirectVertexColor.b - 1.0);\n"
                                            "        float A = Sampler.a - (1.0 - DirectVertexColor.a);\n"
                                            "\n"
                                            "        gl_FragColor = vec4(R, G, B, A);\n"
                                            "    } else {\n"
                                            "        gl_FragColor = FragmentColor;\n"
                                            "    }\n"
                                            "}";

    this->TessellatorShaderId = this->LoadShader("Tessellator", TessellatorVertexSource, TessellatorFragmentSource);

    TessellatorFragmentSource = "";
    TessellatorVertexSource = "";
}

int EKG_ShaderManager::CompileStatus(unsigned int &ShaderId, const std::string &Source, std::string Which) {
    GLint Result = GL_FALSE;

    if (Source == "vertex" || Source == "fragment") {
        ShaderId = glCreateShader(Which == "vertex" ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
        const char* SourceCStr = Source.c_str();

        glShaderSource(ShaderId, 1, &SourceCStr, NULL);
        glCompileShader(ShaderId);

        // Verify if the shader is compiled without problems.
        glGetShaderiv(ShaderId, GL_COMPILE_STATUS, &Result);

        return Result;
    }

    EKG_Log("Not an acceptable parameter: " + Source);
    return -2;
}

unsigned int EKG_ShaderManager::LoadShader(const std::string &Name, const std::string &SourceVertexShader, const std::string &SourceFragmentShader) {
    unsigned int Shader = glCreateProgram();
    unsigned int ShaderVertex, ShaderFragment;

    int PhaseVertexCompileResult = this->CompileStatus(ShaderVertex, SourceVertexShader, "vertex");
    int PhaseFragmentCompileResult = this->CompileStatus(ShaderFragment, SourceFragmentShader, "fragment");

    if (PhaseVertexCompileResult == -1 || PhaseFragmentCompileResult == -1) {
        char* Log = new char[1024];

        if (PhaseVertexCompileResult == -1) {
            glGetShaderInfoLog(ShaderVertex, 1024, NULL, Log);
            EKG_Log("Occurred an error while compiled vertex shader: \n" + std::string(Log));
        }

        if (PhaseFragmentCompileResult == -1) {
            glGetShaderInfoLog(ShaderFragment, 1024, NULL, Log);
            EKG_Log("Occurred an error while compiled fragment shader: \n" + std::string(Log));
        }

        delete[] Log;
        return 0;
    }

    if (PhaseVertexCompileResult == -2 || PhaseFragmentCompileResult == -2) {
        return 0;
    }

    glAttachShader(Shader, ShaderVertex);
    glAttachShader(Shader, ShaderFragment);
    glLinkProgram(Shader);

    glDeleteShader(ShaderVertex);
    glDeleteShader(ShaderFragment);

    this->ShaderList[Name] = Shader;
    return Shader;
}

unsigned int EKG_ShaderManager::GetTessellatorShader() {
    return this->TessellatorShaderId;
}

void EKG_ShaderManager::Quit() {
    // Nothing here.
}

unsigned int EKG_ShaderManager::FindShader(const std::string &Shader) {
    unsigned int Id = this->ShaderList.at(Shader);

    return Id;
}
