/**
 * @author Rina
 * @since 21/03/2022 at 21:05PM
 */
#pragma once
#include "ekg_includes.h"

/**
 * Shader manager to add shaders into ekg context or use in external context.
 */
class ekg_shader_manager {
protected:
    /* Check shader if is compiled. */
    int CompileStatus(unsigned int &ShaderId, const std::string& Source, const std::string& Which);

    /* Map wth all shaders loaded. */
    std::map<std::string, unsigned int> ShaderList;
    
    /* Main shader already in code used to ekg context. */
    unsigned int TessellatorShaderId;
public:
    /* Start of main methods. */
    void Init();
    void Quit();
    /* End of main methods. */

    /* Start of main connected methods. */
    unsigned int GetTessellatorShader();
    unsigned int find_shader(const std::string &Shader);
    unsigned int LoadShader(const std::string &Name, const std::string &SourceVertexShader, const std::string &SourceFragmentShader);
    /* End of main connected methods. */
};