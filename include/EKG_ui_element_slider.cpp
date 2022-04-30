#include "EKG_ui_element_slider.h"
#include "EKG.h"

void EKG_Slider::SyncSize() {
    std::string StringValue = std::to_string(this->GetValue());
    this->Format = StringValue.substr(0, StringValue.find('.') + 3);

    float NameHeight = EKG_CORE->FontRenderer.GetStringHeight(this->Tag);

    this->LabelWidth = EKG_CORE->FontRenderer.GetStringWidth(this->Format);
    this->LabelHeight = EKG_CORE->FontRenderer.GetStringHeight(this->Format);

    this->BarRect[0] = 0;
    this->BarRect[1] = 0;

    /*
     * 0 = Horizontal.
     * 1 = Vertical.
     */
    switch (this->BarOrientation) {
        case 0: {
            this->Rect.W = this->Size;
            this->Rect.H = this->Scale + NameHeight + this->Scale;

            this->BarRect[2] = ((float) this->Rect.W) * ((float) this->Value - (float) this->Min) / ((float) this->Max - (float) this->Min);
            this->BarRect[3] = this->Rect.H;
            break;
        }

        case 1: {
            this->Rect.W = this->Scale + NameHeight + this->Scale;
            this->Rect.H = this->Size;

            this->BarRect[2] = this->Rect.W;
            this->BarRect[3] = ((float) this->Rect.H) * ((float) this->Value - (float) this->Min) / ((float) this->Max - (float) this->Min);
            break;
        }
    }

    switch (this->LabelAlignDocking) {
        case EKG::Dock::LEFT: {
            this->LabelAlignX = 2.0F;
            this->LabelAlignY = this->Scale;
            break;
        }

        case EKG::Dock::CENTER: {
            if (this->BarOrientation == 0) {
                this->LabelAlignX = (this->GetWidth() / 2.0F) - (this->LabelWidth / 2.0F);
                this->LabelAlignY = this->Scale;
            } else {
                this->LabelAlignX = (this->GetWidth() / 2.0F) - (this->LabelWidth / 2.0F);
                this->LabelAlignY = (this->GetHeight() / 2.0F) - (this->LabelHeight / 2.0F);
            }
            break;
        }

        case EKG::Dock::RIGHT: {
            this->LabelAlignX = this->GetWidth() - this->LabelWidth - 2.0F;
            this->LabelAlignY = this->Scale;
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
                    EKG_CORE->ActionHappen();

                    this->Pressed = true;
                    this->Dragging = true;
                }

                if (this->Dragging && this->Pressed) {
                    EKG_CORE->ActionHappen();

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

void EKG_Slider::OnUpdate(const float &DeltaTicks) {
    EKG_AbstractElement::OnUpdate(DeltaTicks);
}

void EKG_Slider::OnRender(const float &PartialTicks) {
    EKG_AbstractElement::OnRender(PartialTicks);

    // Enable scissor test and cut off the fragments.
    EKG_Scissor(this->GetScissorX(), this->GetScissorY(), this->GetScissorW(), this->GetScissorH());

    // Background.
    EKG_Color Color(EKG_CORE->ColorTheme.WidgetBackground);
    EKG_DrawFilledRect(this->Rect, Color);

    // Bar.
    Color.Set(EKG_CORE->ColorTheme.WidgetActivy);
    EKG_DrawFilledShape(this->GetX() + this->BarRect[0], this->GetY() + this->BarRect[1], this->BarRect[2], this->BarRect[3], Color);

    if (EKG_CORE->ColorTheme.IsOutlineSliderEnabled()) {
        EKG_DrawOutlineRect(this->Rect, 1.5f, EKG_CORE->ColorTheme.StringColor);
    }

    // Value.
    if (this->LabelVisible) {
        EKG_CORE->FontRenderer.DrawString(this->Format, this->GetX() + this->LabelAlignX, this->GetY() + this->LabelAlignY, EKG_CORE->ColorTheme.StringColor);
    }

    // End scissor.
    EKG_EndScissor();
}

void EKG_Slider::Orientation(const std::string& Orientation) {
    unsigned int Flag = Orientation == "Horizontal" ? 0 : 1;

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
    bool ShouldSync = this->LabelAlignDocking != Docking;

    if (ShouldSync && Flag) {
        this->LabelAlignDocking = Docking;
        this->SyncSize();
    } else if (!Flag) {
        EKG_Log(EKG_Print(this->GetTag(), this->GetId()) + " Incorrect label align: for horizontal docking (LEFT - CENTER - RIGHT), for vertical docking (TOP - CENTER - BOTTOM)");

        ShouldSync = this->LabelAlignDocking != EKG::Dock::CENTER;
        this->LabelAlignDocking = EKG::Dock::CENTER;
    }
}

void EKG_Slider::LabelVisibility(bool LabelState) {
    this->LabelVisible = LabelState;
}

void EKG_Slider::SetOffsetLabel(float Offset) {
    this->OffsetLabel = Offset;
}

float EKG_Slider::GetOffsetLabel() {
    return this->OffsetLabel;
}

std::string EKG_Slider::InfoClass() {
    EKG_AbstractElement::InfoClass();
    return "Slider";
}
