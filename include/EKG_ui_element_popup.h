/**
 * @author Rina
 * @since 16/04/2022 at 20:16pm
 **/
#pragma once

#include "ekg_includes.h"
#include "ekg_abstract_ui_element.h"

#ifndef EKG_UI_ELEMENT_POPUP_H
#define EKG_UI_ELEMENT_POPUP_H

/**
 * Name: popup
 * Type: Container
 * Description: List elements and make selectable
 * Features: Enable or disable elements
 **/
class ekg_ui_element_popup : public ekg_abstract_element {
protected:
    /* Settings. */
    std::vector<EKG_Data> List;
    std::string Selected, Focused, Activy;

    /* States. */
    bool Pressed, Clicked, PopupMaster, Show;

    /* Metrics of popup & buttons. */
    float MaximumHeight, MaximumWidth, TextOffset, TextScale;

    /* Animations. */
    float LastTicks;
    EKG_Smooth SmoothPressed;

    /* Help to return hovered components. */
    EKG_Data GetHoveredComponent(float FX, float FY);

    /* sync_stack_scaled_metrics menus hovering. */
    bool IsUpdate(float FX, float FY);
public:
    /* Start of configurable methods. */
    void Insert(const std::vector<std::string> &List);
    void Delete(const std::string &Pattern);
    void Disable(const std::string &Pattern);
    void Enable(const std::string &Pattern);
    /* End of configurable methods. */

    /* Start of setters & getters. */
    void SetOffsetText(float OffsetText);
    float GetOffsetText();

    void SetPressed(bool State);
    bool IsPressed();

    void SetClicked(bool State);
    bool IsClicked();

    void SetWidth(float Width);
    void set_master_id(unsigned int Id);

    void SetScale(float TextScale);
    float GetScale();

    std::vector<EKG_Data> &GetList();

    std::string &GetFocused();
    std::string &GetClicked();

    void GetPath(std::string &PreviousPath);

    float GetMaximumWidth();
    float GetMaximumHeight();
    /* End of setters & getters. */

    /* Start of action methods. */
    void Place(ekg_ui_element_popup* Element);
    void Place(float X, float Y, float BoundingX);
    /* End of action methods. */

    /* Start of override methods. */
    std::string info_class();
    void set_visibility_flag(unsigned int VisibilityFlag);

    void kill();
    void place(float X, float Y);
    void sync_size();
    void on_pre_event(SDL_Event Event);
    void on_event(SDL_Event Event);
    void on_post_event(SDL_Event Event);
    void on_update(const float &DeltaTicks);
    void on_render(const float &PartialTicks);
    /* End of override methods. */
};

#endif