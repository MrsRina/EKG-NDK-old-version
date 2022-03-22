/**
 * @author Rina
 * @since 21/03/2022 at 21:05PM
 */
#pragma once
#include "EKG_includes.h"
#include "EKG_abstract_ui_element.h"
#include "EKG_shader.h"

/**
 * The core of EKG, where everything is processed.
 */
class EKG_Core {
private:
    /* Buffers element used in context gui. */
    std::vector<EKG_AbstractElement*> BufferUpdate, BufferRender;

    /* ALL very important managers/controllers
     * and util objects (classes) used in EKG. */
    EKG_ShaderManager ShaderManager;
public:
    EKG_Core() {}

    /* Start of main getters (only). */
    EKG_ShaderManager GetShaderManager();
    /* End of main getters. */

    /* Start of important methods. */
    void Init();
    void Quit();
    void AddElement(EKG_AbstractElement* Element);
    void RemoveElement(EKG_AbstractElement* Element);
    /* End of important methods. */

    /* Start of concurrent methods. */
    void OnEvent(SDL_Event Event);
    void OnUpdate(float DeltaTicks);
    void OnRender(float PartialTicks);
    /* End of concurrent methods. */
};