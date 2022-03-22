/**
 * @author Rina
 * @since 21/03/2022 at 21:05PM
 */
#pragma once
#include "EKG_includes.h"

/**
 * Shader manager to add shaders into EKG context or use in external context.
 */
class EKG_ShaderManager {
protected:
    /* Check shader if is compiled. */
    int CompileStatus(unsigned int &ShaderId, const std::string& Source, std::string Which);

    /* Map wth all shaders loaded. */
    std::map<std::string, unsigned int> ShaderList;
    
    /* Main shader already in code used to EKG context. */
    unsigned int TessellatorShaderId;
public:
    /* Start of main methods. */
    void Init();
    void Quit();
    /* End of main methods. */

    /* Start of main connected methods. */
    unsigned int GetTessellatorShader();
    unsigned int FindShader(const std::string &Shader);
    unsigned int LoadShader(const std::string &Name, const std::string &SourceVertexShader, const std::string &SourceFragmentShader);
    /* End of main connected methods. */
};