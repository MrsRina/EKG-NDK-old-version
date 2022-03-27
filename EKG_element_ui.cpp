#include "EKG_element_ui.h"
#include "EKG.h"

void EKG_Frame::Draggable(unsigned int Area) {
    this->DraggableDockFlags = Area == NULL ? 0 : Area;

    if (Area == EKG::Dock::FULL) {
        this->SetOffsetDrag(0.0f);
    }
}

void EKG_Frame::Resizable(unsigned int Area) {
    this->ResizableDockFlags = Area == NULL ? 0 : Area;

    if (Area == EKG::Dock::FULL) {
        this->SetOffsetResize(0.0f);
    }
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

        if (this->Resizing != 0) {
            float X;
            float Y;
            float W;
            float H;

            if (this->Resizing == EKG::Dock::LEFT || this->Resizing == EKG::Dock::TOP) {
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
            if (this->DraggableDockFlags != 0 && !this->Dragging && this->Resizing == 0) {
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

            if (this->ResizableDockFlags != -1 && this->Resizing == 0 && !this->Dragging) {
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

    // Background.
    EKG_Color Color(EKG_CORE->ColorTheme.FrameBackground);

    if (EKG_CORE->ColorTheme.IsOutlineFrameEnabled()) {
        Color.A = this->ValueAlpha;
    }

    EKG_DrawFilledRect(this->Rect, Color);

    // Border.
    if (EKG_CORE->ColorTheme.IsOutlineFrameEnabled()) {
        EKG_DrawOutlineRect(this->Rect, 1.0f, this->Border);
    }

    // Pressed state.
    if (this->Dragging || this->Resizing != 0) {
        Color.Set(EKG_CORE->ColorTheme.FramePressed);
        EKG_DrawFilledRect(this->Rect, Color);
    }

    // Highlight.
    if (this->Hovered) {
        Color.Set(EKG_CORE->ColorTheme.FrameHighlight);
        EKG_DrawFilledRect(this->Rect, Color);
    }
}

void EKG_Frame::Alpha(unsigned int Alpha) {
    this->CustomAlpha = true;
    this->ValueAlpha = Alpha;
}

EKG_Color EKG_Frame::GetBorderColor() {
    return this->Border;
}

void EKG_Frame::SetBorderColor(unsigned int R, unsigned int G, unsigned int B, unsigned int A) {
    this->Border.Set(R, G, B, A);
}

void EKG_Frame::Place(EKG_AbstractElement* Element, float X, float Y) {
    if (Element->GetId() == this->GetId()) {
        EKG_Log(EKG_Print(this->Tag, this->Id) + "Place waring: You can not add this element" + EKG_Print(Element->GetTag(), Element->GetId()));
        return;
    }

    if (this->Children.Contains(Element->GetId())) {
        return;
    }

    this->Children.Put(Element->GetId());

    Element->SetMasterId(this->Id);
    Element->Place(X, Y);

    EKG_CORE->Sync(this->Rect.X, this->Rect.Y, this->Rect.W, this->Rect.H, this->Children);
}

void EKG_Frame::SetOffsetResize(float Offset) {
    this->ResizeOffset = Offset;
}

float EKG_Frame::GetOffsetResize() {
    return this->ResizeOffset;
}

void EKG_Frame::SetOffsetDrag(float Offset) {
    this->DragOffset = Offset;
}

float EKG_Frame::GetOffsetDrag() {
    return this->DragOffset;
}

void EKG_Button::CheckBox(bool State) {
    this->Box = State;
}

void EKG_Button::BoxTexture(const EKG_Texture &Texture) {
    this->TextureBox = Texture;
}

void EKG_Button::BorderColor(unsigned int R, unsigned int G, unsigned int B, unsigned A) {
    this->Border.Set(R, G, B, A);
}

void EKG_Button::SetPressed(bool State) {
    this->Pressed = State;
}

bool EKG_Button::IsPressed() {
    return this->Pressed;
}

void EKG_Button::SetChecked(bool State) {
    this->Checked = State;
}

bool EKG_Button::IsChecked() {
    return this->Checked;
}

bool EKG_Button::IsCheckBox() {
    return this->Box;
}

void EKG_Button::OnPreEvent(SDL_Event Event) {
    EKG_AbstractElement::OnPreEvent(Event);
}

void EKG_Button::OnEvent(SDL_Event Event) {
    EKG_AbstractElement::OnEvent(Event);
}

void EKG_Button::OnPostEvent(SDL_Event Event) {
    EKG_AbstractElement::OnPostEvent(Event);
}

void EKG_Button::OnUpdate(float DeltaTicks) {
    EKG_AbstractElement::OnUpdate(DeltaTicks);

    this->SmoothHighlight.NextFactory = this->Highlight;
}

void EKG_Button::OnRender(float PartialTicks) {
    EKG_AbstractElement::OnRender(PartialTicks);

    // Background
    EKG_Color Color(EKG_CORE->ColorTheme.WidgetBackground);
    EKG_DrawFilledRect(this->Rect, Color);

    // Border
    if (EKG_CORE->ColorTheme.IsOutlineButtonEnabled()) {
        Color.Set(this->Boder.R, this->Border.G, this->Border.B, this->Border.A);
        EKG_DrawOutlineRect(this->Rect, Color);
    }

    if (this->SmoothPressed.Factory > 10) {
        Color.Set(EKG_CORE->ColorTheme.WidgetPressed, (unsigned int) this->SmoothPressed.Factory);
    }

    if (this->Box) {
        Color.Set(EKG_CORE->ColorTheme.WidgetActivy);
        EKG_DrawSolidShape(this->GetX() + this->Box[0], this->GetY() + this->Box[1], this->Box[2], this->Box[3], Color);

        // Box
        if (this->SmoothBoxPressed.Factory > 10) {
            Color.Set(EKG_CORE->ColorTheme.WidgetPressed, (unsigned int) this->SmoothBoxPressed.Factory);
            EKG_DrawFilledShape(this->GetX() + this->Box[0], this->GetY() + this->Box[1], this->Box[2], this->Box[3], Color);
        }

        // Box highlight
        if (this->SmoothBoxHighlight.Factory > 10) {
            Color.Set(EKG->ColorTheme.WidgetHighlight, (unsigned int) this->SmoothBoxHighlight.Factory);
            EKG_DrawOutlineShape(this->GetX() + this->Box[0], this->GetY() + this->Box[1], this->Box[2], this->Box[3], Color);
        }
    }

    // Highlight
    if (this->SmoothHighlight.Factory > 10) {
        Color.Set(EKG->ColorTheme.WidgetHighlight, (unsigned int) this->SmoothBoxHighlight.Factory);
        EKG_DrawSolidRect(this->Rect, Color);
    }
}

EKG_Texture EKG_Button::GetBoxTexture() {
    return this->TextureBox;
}

void EKG_Button::SetOffsetText(float Offset) {
    this->OffsetText = Offset;
}

float EKG_Button::GetOffsetText() {
    return this->OffsetText;
}

void EKG_Button::SetOffsetBox(float Offset) {
    this->OffsetBox = Offset;
}

float EKG_Button::GetOffsetBox() {
    return this->OffsetBox;
}

int EKG_Button::GetBoxRect() {
    return this->BoxRect;
}

bool EKG_Button::IsFingerHoveredBox() {
    return this->FingerHoveredBox;
}

void EKG_Button::SetFingeHoveredBox(bool Over) {
    this->FingerHoveredBox = Over;
}

void EKG_Button::SetBoxRect(float X, float Y, float W, float H) {
    this->BoxRect[0] = X;
    this->BoxRect[1] = Y;
    this->BoxRect[2] = W;
    this->BoxRect[3] = H;
}

bool EKG_Button::DetectPointCollideBox(float X, float Y) {
    float BX = this->BoxRect[0];
    float BY = this->BoxRect[1];
    float BW = this->BoxRect[2];
    float BH = this->BoxRect[3];

    return X > BX && Y > BY && X < BX + BW && Y < BY + BH;
}

void EKG_Button::SetScale(float ButtonScale) {
    this->Scale = ButtonScale;
}

float EKG_Button::GetScale() {
    return this->Scale;
}

void EKG_Button::SyncSize() {
    EKG_AbstractElement::SyncSize();

    this->TextWidth = EKG_CORE->FontRenderer.GetStringWidth(this->Tag);
    this->TextHeight = EKG_CORE->FontRenderer.GetStringHeight(this->Tag);
}

float EKG_Button::GetTextWidth() {
    return this->TextWidth;
}

float EKG_Button::GetTextHeight() {
    return this->TextHeight;
}
