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
    /* Settings. */
    std::vector<EKG_Texture> List;
    std::string Focused, Activy;

    /* States. */
    bool Pressed;

    /* Metrics of tab & buttons. */
    float MinimumWidth, MinimumHeight, Size, TextOffset, TextScale, ButtonSize;

    /* Configuration. */
    unsigned int DockTab;

    EKG_Texture GetComponentHovered(float FX, float FY);
public:
    /* Start of configurable methods. */
    void Insert(const std::vector<std::string> &ToAddList);
    void Delete(const std::string &Pattern);
    void Disable(const std::string &Pattern);
    void Enable(const std::string &Pattern);
    void TabSide(unsigned int Dock);
    /* End of configurable methods. */

    /* Start of setters & getters. */
    void SetOffsetText(float OffsetText);
    float GetOffsetText();

    void SetPressed(bool State);
    bool IsPressed();

    void SetWidth(float Width);
    void SetMasterId(unsigned int Id);

    void SetScale(float Scale);
    float GetScale() const;

    std::vector<EKG_Texture> &GetList();

    void SetButtonSize(float Size);
    float GetButtonSize();
    /* End of setters & getters. */

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