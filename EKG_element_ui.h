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

    void SetBorderColor(unsigned int R, unsigned int G, unsigned B, unsigned int A);
    EKG_Color GetBorderColor();
    /* End of setters and getters. */

    /* Start of override methods. */
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
    bool Pressed, Checked, Box, FingerHoveredBox;

    /* Checkbox texture, metrics & scale. */
    EKG_Texture TextureBox;
    float[4] BoxRect;
    float OffsetText, OffsetBox, TextWidth, TextHeight, Scale;

    /* Animations. */
    EKG_Smooth SmoothHighlight;
    EKG_Smooth SmoothBoxHighlight;
    EKG_Smooth SmoothBoxPressed;
    EKG_Smooth SmoothPressed;

    /* Design additional. */
    EKG_Color Border;

    /* Start of helper methods to box (checkbox mode on). */
    void SetBoxRect(float X, float Y, float W, float H);
    bool DetectPointCollideBox(float X, float Y);
    /* End of helper methods. */
public:
    /* Start of configurable methods. */
    void CheckBox(bool State);
    void BoxTexture(const EKG_Texture &Texture);
    void BorderColor(unsigned int R, unsigned int G, unsigned int B, unsigned A);
    /* end of cconfigurable methods. */

    /* Start of setters & getters. */
    void SetPressed(bool State);
    bool IsPressed();

    void SetChecked(bool State);
    bool IsChecked();

    bool IsCheckBox();
    EKG_Texture GetBoxTexture();

    void SetOffsetText(float Offset);
    float GetOffsetText();

    void SetOffsetBox(float Offset);
    float GetOffsetBox();

    float[4] GetBoxRect();

    void SetFingeHoveredBox(bool Over);
    bool IsFingerHoveredBox();

    void SetScale(float ButtonScale);
    float GetScale();

    float GetTextWidth();
    float GetTextHeight();
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