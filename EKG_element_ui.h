/**
 * @author Rina
 * @since 23/03/2022 at 15:22pm
 **/
#pragma once
#include "EKG_includes.h"
#include "EKG_abstract_ui_element.h"

/**
 * Name: Frame
 * Type: Frame
 * Description: A master element to place others elements.
 * Features: Set draggable, resizable & add child (elements).
 **/
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
    void SyncSize();
    void OnPreEvent(SDL_Event Event);
    void OnEvent(SDL_Event Event);
    void OnPostEvent(SDL_Event Event);
    void OnUpdate(float DeltaTicks);
    void OnRender(float PartialTicks);
    /* End of override methods. */
};

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
    void SyncSize();
    void OnPreEvent(SDL_Event Event);
    void OnEvent(SDL_Event Event);
    void OnPostEvent(SDL_Event Event);
    void OnUpdate(float DeltaTicks);
    void OnRender(float PartialTicks);
    /* End of override methods. */
};

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
    bool Pressed, Clicked;

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
    /* End of configurable methods. */

    /* Start of setters & getters. */
    void SetOffsetText(float OffsetText);
    float GetOffsetText();

    void SetPressed(bool State);
    bool IsPressed();

    void SetClicked(bool State);
    bool IsClicked();

    void SetWidth(float Width);

    void SetScale(float TextScale);
    float GetScale();

    std::vector<EKG_Texture> GetList();

    std::string GetFocused();
    std::string GetClicked();
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