/**
 * @author Diyng
 * @since 28/04/2022 at 16:28
 **/

#pragma once

#include "EKG_includes.h"
#include "EKG_abstract_ui_element.h"

#ifndef EKG_UI_ELEMENT_COMBOBOX_H
#define EKG_UI_ELEMENT_COMBOBOX_H

/**
 * Name: Combobox
 * Type: Widget
 * Description: A selection box with elements.
 * Features: Lists elements and set current.
 **/
class EKG_Combobox : public EKG_AbstractElement {
protected:
    /* Settings. */
    std::string Current = " ";
    std::vector<std::string> PopupElementList;

    /* States. */
    bool Pressed, Activy;

    /* Metrics of text. */
    float AlignOffsetText, OffsetText, TextScale, TextWidth, TextHeight;
    unsigned int AlignTextDocking;

    /* Animations. */
    EKG_Smooth SmoothBoxPressed;
    EKG_Smooth SmoothPressed;
    EKG_Smooth SmoothBoxActivy;
public:
    /* Start of configurable methods. */
    void Add(std::string &String);
    void Remove(std::string &Pattern);
    void AlignText(unsigned int Dock);
    /* End of configurable methods. */

    /* Start of setters & getters. */
    void SetList(const std::vector<std::string> &List);
    void SetWidth(float Width);

    void SetOffsetText(float Offset);
    float GetOffsetText() const;

    void SetScale(float Scale);
    float GetScale() const;

    void SetPressed(bool State);
    bool IsPressed() const;

    void SetCurrent(const std::string &String);
    std::string &GetCurrent();
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
