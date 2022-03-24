#include "EKG_element_ui.h"
#include "EKG.h"

void EKG_Frame::Draggable(unsigned int Area) {
    this->DraggableDockFlags = Area == NULL ? EKG::Dock::UNKNOWN : Area;
}

void EKG_Frame::Resizable(unsigned int Area) {
    this->ResizableDockFlags = Area == NULL ? EKG::Dock::UNKNOWN : Area;
}

void EKG_Frame::SetDragging(bool State) {
    this->Dragging = State;
}

bool EKG_Frame::IsDragging() {
    return this->Dragging;
}

void EKG_Frame::SetResizing(bool State) {
    this->Resizing = State;
}

bool EKG_Frame::IsResizing() {
    return this->Resizing;
}

void EKG_Frame::SetPressed(bool State) {
    this->Pressed = State;
}

bool EKG_Frame::IsPressed() {
    return this->Pressed;
}

void EKG_Frame::OnPreEvent(SDL_Event Event) {
    EKG_AbstractElement::OnPreEvent(Event);
}

void EKG_Frame::OnEvent(SDL_Event Event) {
    EKG_AbstractElement::OnEvent(Event);

    if (Event.type == SDL_FINGERMOTION) {
        float FX = Event.tfinger.x;
        float FY = Event.tfinger.y;

        EKG::ScaledFingerPos(FX, FY);
        bool MovingEvent;

        if (this->Dragging) {
            float X = FX - this->DragX;
            float Y = FY - this->DragY;
            float W = this->Rect.W;
            float H = this->Rect.H;

            if (this->GetMasterId() != 0) {
                if (!this->FreeDragAndDrop) {
                    if (X < 0) {
                        X = 0;
                    }

                    if (Y < 0) {
                        Y = 0;
                    }

                    if (X + W > this->ScaledWidth) {
                        X = this->ScaledWidth - W;
                    }

                    if (Y + H > this->ScaledHeight) {
                        Y = this->ScaledHeight - H;
                    }
                }

                this->SyncX = X;
                this->SyncY = Y;

                this->SyncPos();
            } else {
                this->Rect.X = FX - this->DragX;
                this->Rect.Y = FY - this->DragY;
            }

            MovingEvent = true;
        }

        if (this->ResizableDockFlags != EKG::Dock::UNKNOWN) {
            float X;
            float Y;
            float W;
            float H;

            if (this->ResizableDockFlags == EKG::Dock::LEFT || this->ResizableDockFlags == EKG::Dock::TOP) {
                float DragUpdateX = FX - this->DragW;
                float DragUpdateY = FY - this->DragH;

                float PredictionW = this->PreviousW + (this->PreviousX - DragUpdateX);
                float PredictionH = this->PreviousH + (this->PreviousY - DragUpdateY);

                X = PredictionW < (float) this->MinimumWidth ? this->PreviousX + this->PreviousW - (float) this->MinimumWidth : DragUpdateX;
                Y = PredictionH < (float) this->MinimumHeight ? this->PreviousY + this->PreviousH - (float) this->MinimumHeight : DragUpdateY;

                if (this->GetMasterId() == 0) {
                    this->Rect.X = X;
                    this->Rect.Y = Y;
                } else {
                    X -= this->ScaledX;
                    Y -= this->ScaledY;

                    if (X < 0) {
                        X = 0;
                    }

                    if (Y < 0) {
                        Y = 0;
                    }

                    this->SyncX = X;
                    this->SyncY = Y;

                    this->SyncPos();
                }

                // Diff Width & Height. Not prediction anymore, recycled.
                PredictionW = this->PreviousW + (this->PreviousX - this->Rect.X);
                PredictionH = this->PreviousH + (this->PreviousY - this->Rect.Y);

                W = PredictionW < (float) this->MinimumWidth ? (float) this->MinimumWidth : PredictionW;
                H = PredictionH < (float) this->MinimumHeight ? (float) this->MinimumHeight : PredictionH;

                this->Rect.W = W;
                this->Rect.H = H;
            } else {
                X = this->Rect.X;
                Y = this->Rect.Y;

                W = FX - this->DragW;
                H = FY - this->DragH;

                if (this->GetMasterId() != 0) {
                    if (X + W > this->ScaledX + this->ScaledWidth) {
                        W = this->ScaledX + this->ScaledWidth - X;
                    }

                    if (Y + H > this->ScaledY + this->ScaledHeight) {
                        H = this->ScaledY + this->ScaledHeight - Y;
                    }

                    this->SyncPos();
                }

                this->Rect.W = W < (float) this->MinimumWidth ? (float) this->MinimumWidth : W;
                this->Rect.H = H < (float) this->MinimumHeight ? (float) this->MinimumHeight : H;
            }

            MovingEvent = true;
        }

        if (MovingEvent && this->IsMaster()) {
            this->SyncSize();
            EKG_CORE->Sync(this->Rect.X, this->Rect.Y, this->Rect.W, this->Rect.H, this->Children);
        }
    }

    if (Event.type == SDL_FINGERDOWN) {
        float FX = Event.tfinger.x;
        float FY = Event.tfinger.y;

        EKG::ScaledFingerPos(FX, FY);

        if (this->Hovered) {
            if (this->DraggableDockFlags != EKG::UNKNOWN && !this->Dragging && this->Resizing == 0) {
                int CollidingDock = EKG::PointCollideDock(this->DraggableDockFlags, FX, FY, 0, this->DragOffset, this->Rect);

                if (CollidingDock != -1) {
                    if (this->GetMasterId() == 0) {
                        this->DragX = FX - this->Rect.X;
                        this->DragY = FY - this->Rect.Y;
                    } else {
                        this->DragX = FX - (this->Rect.X - this->ScaledX);
                        this->DragY = FY - (this->Rect.Y - this->ScaledY);
                    }

                    // Say dragging to the element.
                    this->Dragging = true;
                }
            }

            if (this->ResizableDockFlags != EKG::Dock::UNKNOWN && this->Resizing == 0 && !this->Dragging) {
                int CollidingDock = EKG::PointCollideDock(this->ResizableDockFlags, FX, FY, 0, this->ResizeOffset, this->Rect);

                if (CollidingDock != -1) {
                    if (CollidingDock == EKG::Dock::LEFT || CollidingDock == EKG::Dock::TOP) {
                        // Its works like a drag.
                        this->DragW = FX - (this->Rect.X);
                        this->DragH = FY - (this->Rect.Y);

                        // Save to use after.
                        this->PreviousX = this->Rect.X;
                        this->PreviousY = this->Rect.Y;
                        this->PreviousW = this->Rect.W;
                        this->PreviousH = this->Rect.H;
                    } else {
                        this->DragW = FX - this->Rect.W;
                        this->DragH = FY - this->Rect.H;
                    }

                    // Say to resizing to the element.
                    this->Resizing = CollidingDock;
                }
            }
        }
    }

    if (Event.type == SDL_FINGERUP) {
        this->Hovered = false;
        this->Dragging = false;
        this->Resizing = 0;
    }
}

void EKG_Frame::OnPostEvent(SDL_Event Event) {
    EKG_AbstractElement::OnPostEvent(Event);
}

void EKG_Frame::OnUpdate(float DeltaTicks) {
    EKG_AbstractElement::OnUpdate(DeltaTicks);
}

void EKG_Frame::OnRender(float PartialTicks) {
    EKG_AbstractElement::OnRender(PartialTicks);

    EKG_Color Color(EKG_THEME->FrameBackground);

    // Background.
    EKES_DrawRect(this->Rect, Color);

    // Border.
    if (EKG_THEME->IsWireframeFrameEnabled()) {
        Color.Set(this->BorderColorTheme);
        EKES_DrawWireframeRect(this->Rect, Color);
    }

    // Pressed state.
    if (this->Dragging || this->Resizing != 0) {
        Color.Set(EKG_THEME->FramePressed);
        EKES_DrawRect(this->Rect, Color);
    }

    // Highlight.
    if (this->Hovered) {
        Color.Set(EKG_THEME->FrameHighlight);
        EKES_DrawRect(this->Rect, Color);
    }

}
