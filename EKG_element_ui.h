/**
 * @author Rina
 * @since 23/03/2022 at 15:22pm
 **/
#pragma once
#include "EKG_includes.h"
#include "EKG_abstract_ui_element.h"

class EKG_Frame : public EKG_AbstractElement {
protected:
    /* Flags used to save build config in frame. */
    unsigned int DraggableDockFlags, ResizableDockFlags;

    /* States. */
    bool Dragging, Pressed, FreeDragAndDrop, CustomAlpha;
    int Resizing;

    /* Drag and resizable references. */
    float DragX, DragY, DragW, DragH, PreviousX, PreviousY, PreviousW, PreviousH;

    /* Limits of metric and offsets. */
    float MinimumWidth = 50, MinimumHeight = 50, DragOffset = 0.70f, ResizeOffset = 0.70f;

    /* Design additional. */
    unsigned int ValueAlpha;
    EKG_Color Border = EKG_Color(255, 255, 255, 255);
public:
    /* Start of configurable methods. */
    void Draggable(unsigned int Area);
    void Resizable(unsigned int Area);
    void Alpha(unsigned int A);
    void Place(EKG_AbstractElement* Element, float X, float Y);
    /* End of configurable methods. */

    /* Start of setters and getters. */
    void SetDragging(bool State);
    bool IsDragging();

    void SetResizing(bool State);
    bool IsResizing();

    void SetPressed(bool State);
    bool IsPressed();

    void SetOffsetResize(float Offset);
    float GetOffsetResize();

    void SetOffsetDrag(float Offset);
    float GetOffsetDrag();

    void SetWidth(float Width);
    void SetHeight(float Height);

    void SetLimit(float MinWidth, float MinHeight);

    void SetBorderColor(unsigned int R, unsigned int G, unsigned B, unsigned int A);
    EKG_Color GetBorderColor();
    /* End of setters and getters. */

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

    /* Design additional. */
    EKG_Color Border = EKG_Color(255, 255, 255, 255);

    /* Start of helper methods to box (checkbox mode on). */
    void SetBoxRect(float X, float Y, float W, float H);
    bool DetectPointCollideBox(float X, float Y);
    /* End of helper methods. */
public:
    /* Start of configurable methods. */
    void BoxTexture(const EKG_Texture &Texture);
    void BorderColor(unsigned int R, unsigned int G, unsigned int B, unsigned A);
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
    void SyncSize();
    void OnPreEvent(SDL_Event Event);
    void OnEvent(SDL_Event Event);
    void OnPostEvent(SDL_Event Event);
    void OnUpdate(float DeltaTicks);
    void OnRender(float PartialTicks);
    /* End of override methods. */
};

class EKG_Slider : public EKG_AbstractElement {
protected:
    /* Configurables. */
    double Min, Max, Value;

     /* States. */
     bool Pressed,Drag, Dragging, LabelVisible;
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
    void SyncSize();
    void OnPreEvent(SDL_Event Event);
    void OnEvent(SDL_Event Event);
    void OnPostEvent(SDL_Event Event);
    void OnUpdate(float DeltaTicks);
    void OnRender(float PartialTicks);
    /* End of override methods. */
};