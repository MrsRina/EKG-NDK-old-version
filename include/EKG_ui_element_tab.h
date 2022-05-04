/**
 * @author Rina
 * @since 03/05/2022 at 16:47
 **/
#pragma once

#include "EKG_includes.h"
#include "EKG_abstract_ui_element.h"

#ifndef EKG_UI_ELEMENT_TAB_H
#define EKG_UI_ELEMENT_TAB_H

/**
 * Name: Tab
 * Type: Container
 * Description: A container with frames.
 * Features: Lists elements and set current.
 **/
class EKG_Tab : public EKG_AbstractElement {
protected:

public:
    /* Start of override methods. */
    std::string InfoClass();
    void SyncSize();
    void OnPreEvent(SDL_Event Event);
    void OnEvent(SDL_Event Event);
    void OnPostEvent(SDL_Event Event);
    void OnUpdate(const float &DeltaTicks);
    void OnRender(const float &PartialTicks);
    /* End of override methods. */
};

#endif