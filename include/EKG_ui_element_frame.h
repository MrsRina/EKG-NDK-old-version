/**
 * @author Rina
 * @since 23/03/2022 at 15:22pm
 **/
#pragma once

#include "EKG_includes.h"
#include "EKG_abstract_ui_element.h"

#ifndef EKG_UI_ELEMENT_FRAME_H
#define EKG_UI_ELEMENT_FRAME_H

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
    std::string InfoClass();
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