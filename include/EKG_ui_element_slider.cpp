#include "ekg_ui_element_slider.h"
#include "ekg.h"

void ekg_ui_element_slider::sync_size() {
    std::string StringValue = std::to_string(this->GetValue());
    this->Format = StringValue.substr(0, StringValue.find('.') + 3);

    float NameHeight = EKG_CORE->font_renderer.GetStringHeight(this->tag);

    this->LabelWidth = EKG_CORE->font_renderer.GetStringWidth(this->Format);
    this->LabelHeight = EKG_CORE->font_renderer.GetStringHeight(this->Format);

    this->BarRect[0] = 0;
    this->BarRect[1] = 0;

    /*
     * 0 = Horizontal.
     * 1 = Vertical.
     */
    switch (this->BarOrientation) {
        case 0: {
            this->rect.W = this->Size;
            this->rect.H = this->Scale + NameHeight + this->Scale;

            this->BarRect[2] = ((float) this->rect.W) * ((float) this->Value - (float) this->Min) / ((float) this->Max - (float) this->Min);
            this->BarRect[3] = this->rect.H;
            break;
        }

        case 1: {
            this->rect.W = this->Scale + NameHeight + this->Scale;
            this->rect.H = this->Size;

            this->BarRect[2] = this->rect.W;
            this->BarRect[3] = ((float) this->rect.H) * ((float) this->Value - (float) this->Min) / ((float) this->Max - (float) this->Min);
            break;
        }
    }

    switch (this->LabelAlignDocking) {
        case ekg::dock::LEFT: {
            this->LabelAlignX = 2.0F;
            this->LabelAlignY = this->Scale;
            break;
        }

        case ekg::dock::CENTER: {
            if (this->BarOrientation == 0) {
                this->LabelAlignX = (this->get_width() / 2.0F) - (this->LabelWidth / 2.0F);
                this->LabelAlignY = this->Scale;
            } else {
                this->LabelAlignX = (this->get_width() / 2.0F) - (this->LabelWidth / 2.0F);
                this->LabelAlignY = (this->get_height() / 2.0F) - (this->LabelHeight / 2.0F);
            }
            break;
        }

        case ekg::dock::RIGHT: {
            this->LabelAlignX = this->get_width() - this->LabelWidth - 2.0F;
            this->LabelAlignY = this->Scale;
            break;
        }

        case ekg::dock::TOP: {
            this->LabelAlignX = (this->get_width() / 2.0F) - (this->LabelWidth / 2.0F);
            this->LabelAlignY = 2.0F;
            break;
        }

        case ekg::dock::BOTTOM: {
            this->LabelAlignX = (this->get_width() / 2.0F) - (this->LabelWidth / 2.0F);
            this->LabelAlignY = this->get_height() - this->LabelHeight - 2.0F;
            break;
        }
    }
}

void ekg_ui_element_slider::on_pre_event(SDL_Event Event) {
    ekg_abstract_element::on_pre_event(Event);
}

void ekg_ui_element_slider::on_event(SDL_Event Event) {
    ekg_abstract_element::on_event(Event);

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

                ekg::scaled_finger_pos(FX, FY);

                if (!this->Dragging && this->hovered && Event.type == SDL_FINGERDOWN) {
                    ekg::task(ekg::task::BLOCKED);

                    this->Pressed = true;
                    this->Dragging = true;
                }

                if (this->Dragging && this->Pressed) {
                    ekg::task(ekg::task::BLOCKED);

                    float Diff = (this->BarOrientation == 0 ? this->rect.W : this->rect.H);
                    float FingerPosFactored = this->BarOrientation == 0 ? FX : FY;

                    // Set bar progress.
                    this->SyncBar(std::min(Diff, (float) std::max(0.0F, FingerPosFactored - (this->BarOrientation == 0 ? this->rect.X : this->rect.Y))));
                }
            }

            break;
        }
    }
}

void ekg_ui_element_slider::on_post_event(SDL_Event Event) {
    ekg_abstract_element::on_post_event(Event);
}

void ekg_ui_element_slider::on_update(const float &DeltaTicks) {
    ekg_abstract_element::on_update(DeltaTicks);
}

void ekg_ui_element_slider::on_render(const float &PartialTicks) {
    ekg_abstract_element::on_render(PartialTicks);

    // Enable scissor test and cut off the fragments.
    ekg_scissor(this->get_scissor_x(), this->get_scissor_y(), this->get_scissor_w(),
                this->get_scissor_h());

    // Background.
    EKG_Color Color(EKG_CORE->color_theme.WidgetBackground);
    ekg_draw_filled_rect(this->rect, Color);

    // Bar.
    Color.Set(EKG_CORE->color_theme.WidgetActivy);
    ekg_draw_filled_shape(this->get_x() + this->BarRect[0], this->get_y() + this->BarRect[1],
                          this->BarRect[2], this->BarRect[3], Color);

    if (EKG_CORE->color_theme.IsOutlineSliderEnabled()) {
        ekg_draw_outline_rect(this->rect, 1.5f, EKG_CORE->color_theme.StringColor);
    }

    // Value.
    if (this->LabelVisible) {
        EKG_CORE->font_renderer.DrawString(this->Format, this->get_x() + this->LabelAlignX,
                                          this->get_y() + this->LabelAlignY, EKG_CORE->color_theme.StringColor);
    }

    // End scissor.
    ekg_end_scissor();
}

void ekg_ui_element_slider::Orientation(const std::string& Orientation) {
    unsigned int Flag = Orientation == "Horizontal" ? 0 : 1;

    if (this->BarOrientation != Flag) {
        this->BarOrientation = Flag;
        this->sync_size();
    }
}

void ekg_ui_element_slider::SetSize(float BarSize) {
    if (this->Size != BarSize) {
        this->Size = BarSize;
        this->sync_size();
    }
}

float ekg_ui_element_slider::GetSize() {
    return this->Size;
}

void ekg_ui_element_slider::SetMax(double Maximum) {
    if (this->Max != Maximum) {
        this->Max = Maximum;
        this->sync_size();
    }
}

double ekg_ui_element_slider::GetMax() {
    return this->Max;
}

void ekg_ui_element_slider::SetMin(double Minimum) {
    if (this->Min != Minimum) {
        this->Min = Minimum;
        this->sync_size();
    }
}

double ekg_ui_element_slider::GetMin() {
    return this->Min;
}

void ekg_ui_element_slider::SetValue(double Val) {
    double ValueClampf = Val < this->Min ? this->Min : (Val > this->Max ? this->Max : Val);

    if (this->Value != ValueClampf) {
        this->Value = ValueClampf;
        this->sync_size();
    }
}

double ekg_ui_element_slider::GetValue() {
    return this->Value;
}

void ekg_ui_element_slider::SetScale(float Amount) {
    if (this->Scale != Amount) {
        this->Scale = Amount;
        this->sync_size();
    }
}

float ekg_ui_element_slider::GetScale() {
    return this->Scale;
}

void ekg_ui_element_slider::SyncBar(float PositionFactory) {
    if (PositionFactory == 0) {
        this->SetValue(this->GetMin());
        return;
    }

    float BarSizeFactor = this->BarOrientation == 0 ? this->rect.W : this->rect.H;

    // In this case we set the new value.
    this->SetValue(((double) PositionFactory / (double) BarSizeFactor) * (this->Max - this->Min) + this->Min);
}

void ekg_ui_element_slider::Draggable(bool State) {
    this->Drag = State;
}

void ekg_ui_element_slider::LabelAlign(unsigned int Docking) {
    bool Flag = (Docking == ekg::dock::LEFT || Docking == ekg::dock::RIGHT || Docking == ekg::dock::CENTER || Docking == ekg::dock::TOP || Docking == ekg::dock::BOTTOM);
    bool ShouldSync = this->LabelAlignDocking != Docking;

    if (ShouldSync && Flag) {
        this->LabelAlignDocking = Docking;
        this->sync_size();
    } else if (!Flag) {
        ekg_log(ekg_print(this->get_tag(), this->get_id()) +
                " Incorrect label align: for horizontal docking (LEFT - CENTER - RIGHT), for vertical docking (TOP - CENTER - BOTTOM)");

        ShouldSync = this->LabelAlignDocking != ekg::dock::CENTER;
        this->LabelAlignDocking = ekg::dock::CENTER;
    }
}

void ekg_ui_element_slider::LabelVisibility(bool LabelState) {
    this->LabelVisible = LabelState;
}

void ekg_ui_element_slider::SetOffsetLabel(float Offset) {
    this->OffsetLabel = Offset;
}

float ekg_ui_element_slider::GetOffsetLabel() {
    return this->OffsetLabel;
}

std::string ekg_ui_element_slider::info_class() {
    ekg_abstract_element::info_class();
    return "slider";
}
