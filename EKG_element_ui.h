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
    bool Pressed, Checked, CheckBox;

    /* Checkbox texture. */
    EKG_Texture BoxTexture;

    /* Design additional. */
    EKG_Color Border;
};