/**
 * @author Rina
 * @since 16/04/2022 at 20:16pm
 **/
#pragma once

#include "EKG_includes.h"
#include "EKG_abstract_ui_element.h"

#ifndef EKG_UI_ELEMENT_POPUP_H
#define EKG_UI_ELEMENT_POPUP_H

/**
 * Name: Popup
 * Type: Container
 * Description: List elements and make selectable
 * Features: Enable or disable elements
 **/
class EKG_Popup : public EKG_AbstractElement {
protected:
    /* Settings. */
    std::vector<EKG_Texture> List;
    std::string Selected, Focused, Activy;

    /* States. */
    bool Pressed, Clicked, Show;

    /* Metrics of popup & buttons. */
    float MaximumHeight, MaximumWidth, TextOffset, TextScale;

    /* Help to return hovered components. */
    EKG_Texture GetHoveredComponent(float FX, float FY);

    /* Sync menus hovering. */
    bool IsUpdate(float FX, float FY);
public:
    /* Start of configurable methods. */
    void Insert(const std::list<std::string> &List);
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
    void SetShow(bool State);

    bool IsShow();
    void SetWidth(float Width);

    void SetScale(float TextScale);

    float GetScale();

    std::vector<EKG_Texture> GetList();

    std::string GetFocused();
    std::string GetClicked();

    void GetPath(std::string &PreviousPath);
    /* End of setters & getters. */

    /* Start of action methods. */
    void Place(EKG_Popup* Element);
    void Place(float X, float Y, float BoundingX, float BoundingY);
    /* End of action methods. */

    /* Start of override methods. */
    std::string InfoClass();

    void Kill();
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