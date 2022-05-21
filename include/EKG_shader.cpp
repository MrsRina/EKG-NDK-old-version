#include "ekg_shader.h"
#include "ekg_util.h"
#include "ekg.h"

void ekg_shader_manager::Init() {
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

int ekg_shader_manager::CompileStatus(unsigned int &ShaderId, const std::string &Source, const std::string &Which) {
    if (Which == "vertex" || Which == "fragment") {
        ShaderId = glCreateShader(Which == "vertex" ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);

        GLint Result = GL_FALSE;
        const char* SourceCStr = Source.c_str();

        glShaderSource(ShaderId, 1, &SourceCStr, NULL);
        glCompileShader(ShaderId);

        // Verify if the shader is compiled without problems.
        glGetShaderiv(ShaderId, GL_COMPILE_STATUS, &Result);

        return Result;
    }

    ekg_log("Not an acceptable parameter: " + Source);
    return -2;
}

unsigned int ekg_shader_manager::LoadShader(const std::string &Name, const std::string &SourceVertexShader, const std::string &SourceFragmentShader) {
    unsigned int Shader = glCreateProgram();
    unsigned int ShaderVertex, ShaderFragment;

    int PhaseVertexCompileResult = this->CompileStatus(ShaderVertex, SourceVertexShader, "vertex");
    int PhaseFragmentCompileResult = this->CompileStatus(ShaderFragment, SourceFragmentShader, "fragment");

    if (PhaseVertexCompileResult == 0 || PhaseFragmentCompileResult == 0) {
        char* Log = new char[1024];

        if (PhaseVertexCompileResult == 0) {
            glGetShaderInfoLog(ShaderVertex, 1024, NULL, Log);
            ekg_log("Occurred an error while compiled vertex shader: \n" + std::string(Log));
        }

        if (PhaseFragmentCompileResult == 0) {
            glGetShaderInfoLog(ShaderFragment, 1024, NULL, Log);
            ekg_log("Occurred an error while compiled fragment shader: \n" + std::string(Log));
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

    GLint Result = GL_FALSE;
    GLint InfoLogLength;

    glGetProgramiv(Shader, GL_LINK_STATUS, &Result);
    glGetProgramiv(Shader, GL_INFO_LOG_LENGTH, &InfoLogLength);

    glDeleteShader(ShaderVertex);
    glDeleteShader(ShaderFragment);

    if (Result == 1) {
        this->ShaderList[Name] = Shader;
    }

    ekg_log(Name + " program compile status: " + (Result == 1 ? "compiled." : "not compiled."));
    return Shader;
}

unsigned int ekg_shader_manager::GetTessellatorShader() {
    return this->TessellatorShaderId;
}

void ekg_shader_manager::Quit() {
    // Nothing here.
}

unsigned int ekg_shader_manager::FindShader(const std::string &Shader) {
    unsigned int Id = this->ShaderList[Shader];
    return Id;
}
