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

    switch (Event.type) {
        case SDL_FINGERMOTION: {
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

            break;
        }

        case SDL_FINGERDOWN: {
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

                        // Say true for dragging to the element.
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

                        // Say true for resizing to the element.
                        this->Resizing = CollidingDock;
                    }
                }
            }

            break;
        }

        case SDL_FINGERUP: {
            this->Hovered = false;
            this->Dragging = false;
            this->Resizing = 0;

            break;
        }
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

    if (this->CustomAlpha) {
        Color.A = this->ValueAlpha;
    }

    EKG_DrawFilledRect(this->Rect, Color);

    // Border.
    if (EKG_CORE->ColorTheme.IsOutlineFrameEnabled()) {
        EKG_DrawOutlineRect(this->Rect, 2.0f, this->Border);
    }

    // Pressed state.
    if (this->Dragging || this->Resizing != 0) {
        Color.Set(EKG_CORE->ColorTheme.FramePressed);
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
        EKG_Log(EKG_Print(this->Tag, this->Id) + "Place waring: Place loop (master) id" + EKG_Print(Element->GetTag(), Element->GetId()));
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

void EKG_Frame::SetWidth(float Width) {
    this->Rect.W = Width < this->MinimumWidth ? this->MinimumWidth : Width;
}

void EKG_Frame::SetHeight(float Height) {
    this->Rect.H = Height < this->MinimumHeight ? this->MinimumHeight : Height;
}

void EKG_Frame::Place(float X, float Y) {
    EKG_AbstractElement::Place(X, Y);
}

void EKG_Frame::SyncSize() {
    EKG_AbstractElement::SyncSize();

    this->Rect.W = this->Rect.W < this->MinimumWidth ? this->MinimumWidth : this->Rect.W;
    this->Rect.H = this->Rect.H < this->MinimumHeight ? this->MinimumHeight : this->Rect.H;
}

void EKG_Frame::SetLimit(float MinWidth, float MinHeight) {
    float Diff = MinWidth < 10 ? 10 : MinWidth;

    bool ShouldSyncPhase1 = this->MinimumWidth != Diff;
    bool ShouldSyncPhase2;

    this->MinimumWidth = Diff;

    Diff = MinHeight < 10 ? 10 : MinHeight;
    ShouldSyncPhase2 = this->MinimumHeight != Diff;

    this->MinimumHeight = Diff;

    if (ShouldSyncPhase1 || ShouldSyncPhase2) {
        this->SyncSize();
    }
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

    if (Event.type == SDL_FINGERDOWN || Event.type == SDL_FINGERMOTION) {
        float FX = Event.tfinger.x;
        float FY = Event.tfinger.y;

        EKG::ScaledFingerPos(FX, FY);
        this->HoveredBox = this->DetectPointCollideBox(FX, FY);
    }
}

void EKG_Button::OnEvent(SDL_Event Event) {
    EKG_AbstractElement::OnEvent(Event);

    switch (Event.type) {
        case SDL_FINGERDOWN: {
            if (this->Hovered) {
                this->Pressed = true;
            }

            break;
        }

        case SDL_FINGERUP: {
            if (this->Pressed) {
                float FX = Event.tfinger.x;
                float FY = Event.tfinger.y;

                EKG::ScaledFingerPos(FX, FY);

                this->HoveredBox = this->DetectPointCollideBox(FX, FY);
                this->Hovered = this->IsFingerOver(FX, FY);

                if (this->HoveredBox || this->Hovered) {
                    this->Checked = this->IsCheckBox() ? !this->Checked : this->Checked;
                }

                this->Pressed = false;
            }

            break;
        }
    }
}

void EKG_Button::OnPostEvent(SDL_Event Event) {
    EKG_AbstractElement::OnPostEvent(Event);
    this->HoveredBox = false;
}

void EKG_Button::OnUpdate(float DeltaTicks) {
    EKG_AbstractElement::OnUpdate(DeltaTicks);

    this->SmoothPressed.NextFactory = this->Pressed && !this->HoveredBox? (float) EKG_CORE->ColorTheme.WidgetPressed[3] : 0;
    this->SmoothBoxPressed.NextFactory = this->HoveredBox && this->Pressed ? (float) EKG_CORE->ColorTheme.WidgetPressed[3] : 0;
    this->SmoothBoxActivy.NextFactory = this->Checked ? (float) EKG_CORE->ColorTheme.WidgetActivy[3] : 0;

    if (this->Clicked) {
        this->Clicked = false;
    }
}

void EKG_Button::OnRender(float PartialTicks) {
    EKG_AbstractElement::OnRender(PartialTicks);

    // Update animations.
    this->SmoothPressed.Update(PartialTicks);
    this->SmoothBoxPressed.Update(PartialTicks);
    this->SmoothBoxActivy.Update(PartialTicks);

    // Background
    EKG_Color Color(EKG_CORE->ColorTheme.WidgetBackground);
    EKG_DrawFilledRect(this->Rect, Color);

    // Border
    if (EKG_CORE->ColorTheme.IsOutlineButtonEnabled()) {
        Color.Set(this->Border.R, this->Border.G, this->Border.B, this->Border.A);
        EKG_DrawOutlineRect(this->Rect, 1.0f, Color);
    }

    // Pressed.
    if (this->SmoothPressed.Factory > 10) {
        Color.Set(EKG_CORE->ColorTheme.WidgetPressed, (unsigned int) this->SmoothPressed.Factory);
        EKG_DrawFilledRect(this->Rect, Color);
    }

    if (this->Box) {
        if (this->SmoothBoxActivy.Factory > 10) {
            Color.Set(EKG_CORE->ColorTheme.WidgetActivy, (unsigned int) this->SmoothBoxActivy.Factory);
            EKG_DrawFilledShape(this->GetX() + this->BoxRect[0], this->GetY() + this->BoxRect[1], this->BoxRect[2], this->BoxRect[3], Color);
        }

        // Border of box.
        Color.Set(this->Border.R, this->Border.G, this->Border.B, this->Border.A);
        EKG_DrawOutlineShape(this->GetX() + this->BoxRect[0], this->GetY() + this->BoxRect[1], this->BoxRect[2], this->BoxRect[3], 2.0F, Color);

        // Box
        if (this->SmoothBoxPressed.Factory > 10) {
            Color.Set(EKG_CORE->ColorTheme.WidgetPressed, (unsigned int) this->SmoothBoxPressed.Factory);
            EKG_DrawFilledShape(this->GetX() + this->BoxRect[0], this->GetY() + this->BoxRect[1], this->BoxRect[2], this->BoxRect[3], Color);
        }
    }

    // String.
    EKG_CORE->FontRenderer.DrawString(this->Tag, this->Rect.X + this->AlignOffsetText + this->OffsetText, this->Rect.Y + this->Scale, EKG_CORE->ColorTheme.StringColor);
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

float* EKG_Button::GetBoxRect() {
    return this->BoxRect;
}

bool EKG_Button::IsHoveredBox() {
    return this->HoveredBox;
}

void EKG_Button::SetHoveredBox(bool Over) {
    this->HoveredBox = Over;
}

void EKG_Button::SetBoxRect(float X, float Y, float W, float H) {
    this->BoxRect[0] = X;
    this->BoxRect[1] = Y;
    this->BoxRect[2] = W;
    this->BoxRect[3] = H;
}

bool EKG_Button::DetectPointCollideBox(float X, float Y) {
    float BX = this->GetX() + this->BoxRect[0];
    float BY = this->GetY() + this->BoxRect[1];
    float BW = this->BoxRect[2];
    float BH = this->BoxRect[3];

    return X > BX && Y > BY && X < BX + BW && Y < BY + BH;
}

void EKG_Button::SetScale(float ButtonScale) {
    if (this->Scale != ButtonScale) {
        this->Scale = ButtonScale;
        this->SyncSize();
    }
}

float EKG_Button::GetScale() {
    return this->Scale;
}

void EKG_Button::SyncSize() {
    EKG_AbstractElement::SyncSize();

    this->TextWidth = EKG_CORE->FontRenderer.GetStringWidth(this->Tag);
    this->TextHeight = EKG_CORE->FontRenderer.GetStringHeight(this->Tag);

    this->Rect.H = this->Scale + this->TextHeight + this->Scale;

    /* After sync minimal sizes of rect. */
    /* We need to sync align text and box (if mode is on). */

    if (this->Box) {
        // The square of box.
        float Square = !this->BoxScaled ? this->TextHeight : (this->Rect.H / 2) + ((this->Rect.H / 4));

        this->BoxRect[2] = Square;
        this->BoxRect[3] = Square;

        switch (this->AlignBoxDocking) {
            case EKG::Dock::LEFT: {
                this->AlignOffsetBox = Square / 4;
                break;
            }

            case EKG::Dock::RIGHT: {
                this->AlignOffsetBox = this->Rect.W - Square - (Square / 4);
                break;
            }

            case EKG::Dock::CENTER: {
                this->AlignOffsetBox = (this->Rect.W / 2) - (Square / 2);
                break;
            }
        }
    }

    switch (this->AlignTextDocking) {
        case EKG::Dock::LEFT: {
            this->AlignOffsetText = 0;
            break;
        }

        case EKG::Dock::RIGHT: {
            this->AlignOffsetText = this->Rect.W - this->TextWidth;
            break;
        }

        case EKG::Dock::CENTER: {
            this->AlignOffsetText = (this->Rect.W / 2) - (this->TextWidth / 2);
            break;
        }
    }

    // Now update the check box ret offset pos.
    if (this->Box) {
        this->BoxRect[0] = this->AlignOffsetBox + this->OffsetBox;
        this->BoxRect[1] = this->BoxScaled ? ((this->Rect.H - this->BoxRect[2]) / 2) : this->Scale;

        float Square = this->BoxRect[2];
        float X = this->BoxRect[0];

        if (this->AlignBoxDocking == EKG::Dock::LEFT && X + Square >= this->AlignOffsetText) {
            this->OffsetText = ((X + Square) - this->AlignOffsetText) + 2.0F;
        }

        if (this->AlignBoxDocking == EKG::Dock::RIGHT && this->OffsetText + this->AlignOffsetText + this->TextWidth >= this->AlignOffsetBox) {
            this->OffsetText = X - this->TextWidth - this->AlignOffsetText - 2.0F;
        }
    }
}

float EKG_Button::GetTextWidth() {
    return this->TextWidth;
}

float EKG_Button::GetTextHeight() {
    return this->TextHeight;
}

void EKG_Button::SetWidth(float Width) {
    if (this->Rect.W != Width) {
        this->Rect.W = Width;
        this->SyncSize();
    }
}

bool EKG_Button::IsClicked() {
    return this->Clicked;
}

void EKG_Button::SetClicked(bool IsClicked) {
    this->Clicked = IsClicked;
}

void EKG_Button::Mode(std::string Mode) {
    bool ShouldSync = this->Box != true;

    if (Mode == "CheckBoxScaled") {
        this->Box = true;
        this->BoxScaled = true;
    } else if (Mode == "CheckBox") {
        ShouldSync = ShouldSync == false ? true : ShouldSync;

        this->Box = true;
        this->BoxScaled = false;
    } else if (Mode == "Normal") {
        if (this->Box != false) {
             ShouldSync = true;
        }

        this->Box = false;
    } else {
        ShouldSync = false;
        EKG_Log(EKG_Print(this->Tag, this->Id) + "Mode does not exist: (CheckBoxScaled - CheckBox - Normal) ???\"" + Mode + "\"???");
    }

    if (ShouldSync) {
        this->SyncSize();
    }
}

void EKG_Button::AlignBox(unsigned int Dock) {
    if (Dock == EKG::Dock::LEFT || Dock == EKG::Dock::CENTER || Dock == EKG::Dock::RIGHT) {
        if (this->AlignBoxDocking != Dock) {
            this->AlignBoxDocking = Dock;
            this->OffsetBox = 0.0F;
            this->SyncSize();
        }
    } else {
        EKG_Log(EKG_Print(this->Tag, this->Id) + "Incorrect align box: only accept (LEFT - CENTER - RIGHT)");
    }
}

void EKG_Button::AlignText(unsigned int Dock) {
    if (Dock == EKG::Dock::LEFT || Dock == EKG::Dock::CENTER || Dock == EKG::Dock::RIGHT) {
        if (this->AlignTextDocking != Dock) {
            this->AlignTextDocking = Dock;
            this->OffsetText = 0.0F;
            this->SyncSize();
        }
    } else {
        EKG_Log(EKG_Print(this->Tag, this->Id) + "Incorrect align text: only accept (LEFT - CENTER - RIGHT)");
    }
}

void EKG_Slider::SyncSize() {
    this->LabelHeight = EKG_CORE->FontRenderer.GetStringHeight(std::to_string(this->GetValue()));
    this->LabelWidth = EKG_CORE->FontRenderer.GetStringWidth(std::to_string(this->GetValue()));

    this->BarRect[0] = 0;
    this->BarRect[1] = 0;

    /*
     * 0 = Horizontal.
     * 1 = Vertical.
     */
    switch (this->BarOrientation) {
        case 0: {
            this->Rect.W = this->Size;
            this->Rect.H = this->Scale + this->LabelHeight + this->Scale;

            this->BarRect[2] = ((float) this->Rect.W) * ((float) this->Value - (float) this->Min) / ((float) this->Max - (float) this->Min);
            this->BarRect[3] = this->Rect.H;
            break;
        }

        case 1: {
            this->Rect.W = this->Scale + this->LabelHeight + this->Scale;
            this->Rect.H = this->Size;

            this->BarRect[2] = this->Rect.W;
            this->BarRect[3] = ((float) this->Rect.H) * ((float) this->Value - (float) this->Min) / ((float) this->Max - (float) this->Min);
            break;
        }
    }

    switch (this->LabelAlignDocking) {
        case EKG::Dock::LEFT: {
            this->LabelAlignX = 2.0F;
            this->LabelAlignY = this->Size;
            break;
        }

        case EKG::Dock::CENTER: {
            if (this->BarOrientation == 0) {
                this->LabelAlignX = (this->GetWidth() / 2.0F) - (this->LabelWidth / 2.0F);
                this->LabelAlignY = this->Size;
            } else {
                this->LabelAlignX = (this->GetWidth() / 2.0F) - (this->LabelWidth / 2.0F);
                this->LabelAlignY = (this->GetHeight() / 2.0F) - (this->LabelHeight / 2.0F);
            }
            break;
        }

        case EKG::Dock::RIGHT: {
            this->LabelAlignX = this->GetWidth() - this->LabelWidth - 2.0F;
            this->LabelAlignY = this->Size;
            break;
        }

        case EKG::Dock::TOP: {
            this->LabelAlignX = (this->GetWidth() / 2.0F) - (this->LabelWidth / 2.0F);
            this->LabelAlignY = 2.0F;
            break;
        }

        case EKG::Dock::BOTTOM: {
            this->LabelAlignX = (this->GetWidth() / 2.0F) - (this->LabelWidth / 2.0F);
            this->LabelAlignY = this->GetHeight() - this->LabelHeight - 2.0F;
            break;
        }
    }
}

void EKG_Slider::OnPreEvent(SDL_Event Event) {
    EKG_AbstractElement::OnPreEvent(Event);
}

void EKG_Slider::OnEvent(SDL_Event Event) {
    EKG_AbstractElement::OnEvent(Event);

    switch (Event.type) {
        case SDL_FINGERUP: {
            this->Pressed = false;
            this->Dragging = false;
            break;
        }

        default: {
            if (this->Drag && (Event.type == SDL_FINGERDOWN || Event.type == SDL_FINGERMOTION)) {
                float FX = Event.tfinger.x;
                float FY = Event.tfinger.y;

                EKG::ScaledFingerPos(FX, FY);

                if (!this->Dragging && this->Hovered && Event.type == SDL_FINGERDOWN) {
                    this->Pressed = true;
                    this->Dragging = true;
                }

                if (this->Dragging && this->Pressed) {
                    float Diff = (this->BarOrientation == 0 ? this->Rect.W : this->Rect.H);
                    float FingerPosFactored = this->BarOrientation == 0 ? FX : FY;

                    // Set bar progress.
                    this->SyncBar(std::min(Diff, (float) std::max(0.0F, FingerPosFactored - (this->BarOrientation == 0 ? this->Rect.X : this->Rect.Y))));
                }
            }

            break;
        }
    }
}

void EKG_Slider::OnPostEvent(SDL_Event Event) {
    EKG_AbstractElement::OnPostEvent(Event);
}

void EKG_Slider::OnUpdate(float DeltaTicks) {
    EKG_AbstractElement::OnUpdate(DeltaTicks);
}

void EKG_Slider::OnRender(float PartialTicks) {
    EKG_AbstractElement::OnRender(PartialTicks);

    // Background.
    EKG_Color Color(EKG_CORE->ColorTheme.WidgetBackground);
    EKG_DrawFilledRect(this->Rect, Color);

    // Bar.
    Color.Set(EKG_CORE->ColorTheme.WidgetActivy);
    EKG_DrawFilledShape(this->GetX() + this->BarRect[0], this->GetY() + this->BarRect[1], this->BarRect[2], this->BarRect[3], Color);

    // Value.
    if (this->LabelVisible) {
        EKG_CORE->FontRenderer.DrawString(std::to_string(this->GetValue()), this->GetX() + this->LabelAlignX, this->GetY() + this->LabelAlignY, EKG_CORE->ColorTheme.StringColor);
    }
}

void EKG_Slider::Orientation(const std::string& Orientation) {
    int Flag = Orientation == "Horizontal" ? 0 : 1;

    if (this->BarOrientation != Flag) {
        this->BarOrientation = Flag;
        this->SyncSize();
    }
}

void EKG_Slider::SetSize(float BarSize) {
    if (this->Size != BarSize) {
        this->Size = BarSize;
        this->SyncSize();
    }
}

float EKG_Slider::GetSize() {
    return this->Size;
}

void EKG_Slider::SetMax(double Maximum) {
    if (this->Max != Maximum) {
        this->Max = Maximum;
        this->SyncSize();
    }
}

double EKG_Slider::GetMax() {
    return this->Max;
}

void EKG_Slider::SetMin(double Minimum) {
    if (this->Min != Minimum) {
        this->Min = Minimum;
        this->SyncSize();
    }
}

double EKG_Slider::GetMin() {
    return this->Min;
}

void EKG_Slider::SetValue(double Val) {
    double ValueClampf = Val < this->Min ? this->Min : (Val > this->Max ? this->Max : Val);

    if (this->Value != ValueClampf) {
        this->Value = ValueClampf;
        this->SyncSize();
    }
}

double EKG_Slider::GetValue() {
    return this->Value;
}

void EKG_Slider::SetScale(float Amount) {
    if (this->Scale != Amount) {
        this->Scale = Amount;
        this->SyncSize();
    }
}

float EKG_Slider::GetScale() {
    return this->Scale;
}

void EKG_Slider::SyncBar(float PositionFactory) {
    if (PositionFactory == 0) {
        this->SetValue(this->GetMin());
        return;
    }

    float BarSizeFactor = this->BarOrientation == 0 ? this->Rect.W : this->Rect.H;

    // In this case we set the new value.
    this->SetValue(((double) PositionFactory / (double) BarSizeFactor) * (this->Max - this->Min) + this->Min);
}

void EKG_Slider::Draggable(bool State) {
    this->Drag = State;
}

void EKG_Slider::LabelAlign(unsigned int Docking) {
    bool Flag = (Docking == EKG::Dock::LEFT || Docking == EKG::Dock::RIGHT || Docking == EKG::Dock::CENTER || Docking == EKG::Dock::TOP || Docking == EKG::Dock::BOTTOM);

    if (this->LabelAlignDocking != Docking && Flag) {
        this->LabelAlignDocking = Docking;
        this->SyncSize();
    } else if (!Flag) {
        EKG_Log(EKG_Print(this->GetTag(), this->GetId()) + " Incorrect align label: For horizontal docking (LEFT - CENTER - RIGHT), for vertical docking (TOP - CENTER - BOTTOM)");
        this->LabelAlignDocking = EKG::Dock::CENTER;
    }
}

void EKG_Slider::LabelVisibility(bool LabelState) {
    this->Visible = LabelState;
}
