/**
 * @author Rina
 * @since 03/05/2022 at 16:47
 **/
#pragma once

#include "EKG_includes.h"
#include "EKG_ui_element_frame.h"

#ifndef EKG_UI_ELEMENT_TAB_H
#define EKG_UI_ELEMENT_TAB_H

/**
 * Name: Tab
 * Type: Container
 * Description: A container with frames.
 * Features: Lists elements and set current.
 **/
class EKG_Tab : public EKG_Frame {
protected:
    /* Settings. */
    std::vector<EKG_Data> List;
    std::string Focused, Activy;

    /* States. */
    bool Pressed;

    /* Metrics of tab & buttons. */
    float MinimumWidth, MinimumHeight, BorderOffset, Size, TextOffset, TextScale, ButtonSize;

    /* Configuration. */
    unsigned int DockTab;

    EKG_Data GetComponentHovered(float FX, float FY);
public:
    /* Start of configurable methods. */
    void Delete(const std::string &Pattern);
    void Disable(const std::string &Pattern);
    void Enable(const std::string &Pattern);
    void TabSide(unsigned int Dock);
    void Place(EKG_Frame* Frame);
    /* End of configurable methods. */

    /* Start of setters & getters. */
    void SetOffsetText(float OffsetText);
    float GetOffsetText();

    void SetScale(float Scale);
    float GetScale();

    float GetSize();

    void SetBorderOffset(float Offset);
    float GetBorderOffset();

    std::vector<EKG_Data> &GetList();

    void SetButtonSize(float Size);
    float GetButtonSize();
    /* End of setters & getters. */

    /* Start of override methods. */
    std::string InfoClass();
    void Place(float X, float Y);
    void SyncSize();
    void OnPreEvent(SDL_Event Event);
    void OnEvent(SDL_Event Event);
    void OnPostEvent(SDL_Event Event);
    void OnUpdate(const float &DeltaTicks);
    void OnRender(const float &PartialTicks);
    /* End of override methods. */
};

#endif