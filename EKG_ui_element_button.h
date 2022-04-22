/**
 * @author Rina
 * @since 23/03/2022 at 15:22pm
 **/
#pragma once

#include "EKG_includes.h"
#include "EKG_abstract_ui_element.h"

#ifndef EKG_UI_ELEMENT_BUTTON_H
#define EKG_UI_ELEMENT_BUTTON_H

/**
 * Name: Button
 * Type: Widget
 * Description: A normal button state & check.
 * Features: Check box or normal click.
 **/
class EKG_Button : public EKG_AbstractElement {
protected:
    /* States. */
    bool Pressed, Clicked, Checked, Box, BoxScaled, HoveredBox;

    /* Checkbox texture, metrics & scale. */
    EKG_Texture TextureBox;
    float BoxRect[4], AlignOffsetText, AlignOffsetBox, OffsetText, OffsetBox, TextWidth, TextHeight, Scale;
    unsigned int AlignTextDocking, AlignBoxDocking;

    /* Animations. */
    EKG_Smooth SmoothBoxPressed;
    EKG_Smooth SmoothPressed;
    EKG_Smooth SmoothBoxActivy;

    /* Start of helper methods to box (checkbox mode on). */
    void SetBoxRect(float X, float Y, float W, float H);
    bool DetectPointCollideBox(float X, float Y);
    /* End of helper methods. */
public:
    /* Start of configurable methods. */
    void BoxTexture(const EKG_Texture &Texture);
    void AlignBox(unsigned int Dock);
    void AlignText(unsigned int Dock);
    void Mode(std::string Mode);
    /* end of configurable methods. */

    /* Start of setters & getters. */
    void SetPressed(bool State);
    bool IsPressed();

    void SetClicked(bool IsClicked);
    bool IsClicked();

    void SetChecked(bool State);
    bool IsChecked();

    bool IsCheckBox();
    EKG_Texture GetBoxTexture();

    void SetOffsetText(float Offset);
    float GetOffsetText();

    void SetOffsetBox(float Offset);
    float GetOffsetBox();

    float* GetBoxRect();

    void SetHoveredBox(bool Over);
    bool IsHoveredBox();

    void SetScale(float ButtonScale);
    float GetScale();

    float GetTextWidth();
    float GetTextHeight();

    void SetWidth(float Width);
    /* End of setters & getters. */

    /* Start of override methods. */
    std::string InfoClass();
    void SyncSize();
    void OnPreEvent(SDL_Event Event);
    void OnEvent(SDL_Event Event);
    void OnPostEvent(SDL_Event Event);
    void OnUpdate(float DeltaTicks);
    void OnRender(float PartialTicks);
    /* End of override methods. */
};

#endif