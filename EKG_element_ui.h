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
    bool Dragging, Pressed, FreeDragAndDrop;
    int Resizing;

    /* Drag and resizable references. */
    float DragX, DragY, DragW, DragH, PreviousX, PreviousY, PreviousW, PreviousH;

    /* Limits of metric and offsets. */
    float MinimumWidth, MinimumHeight, DragOffset, ResizeOffset;
public:
    /* Start of configurable methods. */
    void Draggable(unsigned int Area);
    void Resizable(unsigned int Area);
    /* End of configurable methods. */

    /* Start of setters and getters. */
    void SetDragging(bool State);
    bool IsDragging();

    void SetResizing(bool State);
    bool IsResizing();

    void SetPressed(bool State);
    bool IsPressed();
    /* End of setters and getters. */

    /* Start of override methods. */
    void OnPreEvent(SDL_Event Event);
    void OnEvent(SDL_Event Event);
    void OnPostEvent(SDL_Event Event);
    void OnUpdate(float DeltaTicks);
    void OnRender(float PartialTicks);
    /* End of override methods. */
};