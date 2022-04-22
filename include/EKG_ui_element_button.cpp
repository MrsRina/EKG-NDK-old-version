#include "EKG_ui_element_button.h"
#include "EKG.h"

void EKG_Button::BoxTexture(const EKG_Texture &Texture) {
    this->TextureBox = Texture;
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
                EKG_CORE->ActionHappen();
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
                    EKG_CORE->ActionHappen();

                    this->Checked = this->IsCheckBox() ? !this->Checked : this->Checked;
                    this->Clicked = true;
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

    // Enable scissor test and cut off the fragments.
    EKG_Scissor(this->GetScissorX(), this->GetScissorY(), this->GetScissorW(), this->GetScissorH());

    // Background
    EKG_Color Color(EKG_CORE->ColorTheme.WidgetBackground);
    EKG_DrawFilledRect(this->Rect, Color);

    // Border
    if (EKG_CORE->ColorTheme.IsOutlineButtonEnabled()) {
        EKG_DrawOutlineRect(this->Rect, 1.5f, EKG_CORE->ColorTheme.StringColor);
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
        EKG_DrawOutlineShape(this->GetX() + this->BoxRect[0], this->GetY() + this->BoxRect[1], this->BoxRect[2], this->BoxRect[3], 2.0F, EKG_CORE->ColorTheme.StringColor);

        // Box
        if (this->SmoothBoxPressed.Factory > 10) {
            Color.Set(EKG_CORE->ColorTheme.WidgetPressed, (unsigned int) this->SmoothBoxPressed.Factory);
            EKG_DrawFilledShape(this->GetX() + this->BoxRect[0], this->GetY() + this->BoxRect[1], this->BoxRect[2], this->BoxRect[3], Color);
        }
    }

    // String.
    EKG_CORE->FontRenderer.DrawString(this->Tag, this->Rect.X + this->AlignOffsetText + this->OffsetText, this->Rect.Y + this->Scale, EKG_CORE->ColorTheme.StringColor);

    // End scissor.
    EKG_EndScissor();
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
    bool Flag = (Dock == EKG::Dock::LEFT || Dock == EKG::Dock::CENTER || Dock == EKG::Dock::RIGHT);

    if (this->AlignBoxDocking != Dock) {
        this->AlignBoxDocking = Dock;
        this->OffsetBox = 0.0F;
        this->SyncSize();
    } else {
        EKG_Log(EKG_Print(this->Tag, this->Id) + "Incorrect box align: only accept (LEFT - CENTER - RIGHT)");
    }
}

void EKG_Button::AlignText(unsigned int Dock) {
    bool Flag = (Dock == EKG::Dock::LEFT || Dock == EKG::Dock::CENTER || Dock == EKG::Dock::RIGHT);

    if (this->AlignTextDocking != Dock && Flag) {
        this->AlignTextDocking = Dock;
        this->OffsetText = 0.0F;
        this->SyncSize();
    } else {
        EKG_Log(EKG_Print(this->Tag, this->Id) + "Incorrect text align: only accept (LEFT - CENTER - RIGHT)");
    }
}

std::string EKG_Button::InfoClass() {
    EKG_AbstractElement::InfoClass();
    return "Button";
}
