/**
 * @author Rina
 * @since 03/05/2022 at 16:47
 **/
#pragma once

#include "ekg_includes.h"
#include "ekg_ui_element_frame.h"

#ifndef EKG_UI_ELEMENT_TAB_H
#define EKG_UI_ELEMENT_TAB_H

/**
 * Name: Tab
 * Type: Container
 * Description: A container with frames.
 * Features: Lists elements and set current.
 **/
class ekg_ui_element_tab : public ekg_ui_element_frame {
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
    EKG_Smooth SmoothActivy;

    EKG_Data GetComponentHovered(float FX, float FY);
    void SyncLayout();
    void SortCloseComponent();
public:
    /* Start of configurable methods. */
    void Delete(const std::string &Pattern);
    void Disable(const std::string &Pattern);
    void Enable(const std::string &Pattern);
    void TabSide(unsigned int Dock);
    void Place(ekg_ui_element_frame* Frame);
    void Open(const std::string &Name);
    void Open(EKG_Data &Component);
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
    std::string info_class();
    void place(float X, float Y);
    void sync_size();
    void on_parent_killed(unsigned int ChildElementId);
    void on_pre_event(SDL_Event Event);
    void on_event(SDL_Event Event);
    void on_post_event(SDL_Event Event);
    void on_update(const float &DeltaTicks);
    void on_render(const float &PartialTicks);
    /* End of override methods. */
};

#endif