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
    std::string Selected, Focused;

    /* States. */
    bool Pressed, Clicked, Show;

    /* Metrics of popup & buttons. */
    float MaximumHeight, MaximumWidth, TextOffset, TextScale;

    /* Help to returns hovered components. */
    std::string GetHoveredComponent(float FX, float FY);
public:
    /* Start of configurable methods. */
    void Insert(const std::string StringList[32]);
    void Delete(const std::string &Pattern);
    void Disable(const std::string &Pattern);
    void Enable(const std::string &Pattern);
    void Place(EKG_AbstractElement* Element, const std::string &Component);
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
    /* End of setters & getters. */

    /* Start of override methods. */
    void Place(float X, float Y);
    void SyncSize();
    void OnPreEvent(SDL_Event Event);
    void OnEvent(SDL_Event Event);
    void OnPostEvent(SDL_Event Event);
    void OnUpdate(float DeltaTicks);
    void OnRender(float PartialTicks);
    /* End of override methods. */
};

#endif