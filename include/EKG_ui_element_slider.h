/**
 * @author Rina
 * @since 16/04/2022 at 20:16pm
 **/
#pragma once

#include "EKG_includes.h"
#include "EKG_abstract_ui_element.h"

#ifndef EKG_UI_ELEMENT_SLIDER_H
#define EKG_UI_ELEMENT_SLIDER_H

/**
 * Name: Slider
 * Type: Widget
 * Description: Control a bar with value min & max.
 * Features: Set draggable or make a progress bar,
 * two align (Horizontal - Vertical)
 **/
class EKG_Slider : public EKG_AbstractElement {
protected:
    /* For render value. */
    std::string Format;

    /* Settings. */
    double Min, Max, Value;

    /* States. */
    bool Pressed, Drag, Dragging, LabelVisible;
    unsigned int BarOrientation;

    /* Bar metrics. */
    float BarRect[4], Size, Scale;

    /* Label metrics & docking of the label. */
    float LabelAlignX, LabelAlignY, LabelWidth, LabelHeight, OffsetLabel, AlignLabel;
    unsigned int LabelAlignDocking;

    /* Update slider progress with a pos factory. */
    void SyncBar(float PositionFactory);
public:
    /* Start of configurable methods. */
    void Orientation(const std::string& Orientation);
    void Draggable(bool State);
    void LabelAlign(unsigned int Docking);
    void LabelVisibility(bool LabelState);
    /* End of configurable methods. */

    /* Start of setters & getters. */
    void SetOffsetLabel(float Offset);
    float GetOffsetLabel();

    void SetScale(float Amount);
    float GetScale();

    void SetSize(float BarSize);
    float GetSize();

    void SetMax(double Maximum);
    double GetMax();

    void SetMin(double Minimum);
    double GetMin();

    void SetValue(double Val);
    double GetValue();
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