#include "ekg_ui_element_slider.h"
#include "ekg.h"

void ekg_ui_element_slider::sync_size() {
    std::string string_value = std::to_string(this->get_value());
    this->format = string_value.substr(0, string_value.find('.') + 3);

    float name_height = EKG_CORE->font_renderer.GetStringHeight(this->tag);

    this->text_width = EKG_CORE->font_renderer.GetStringWidth(this->format);
    this->text_height = EKG_CORE->font_renderer.GetStringHeight(this->format);

    this->bar_rect[0] = 0;
    this->bar_rect[1] = 0;

    /*
     * 0 = Horizontal.
     * 1 = Vertical.
     */
    switch (this->bar_orientation) {
        case 0: {
            this->rect.W = this->size;
            this->rect.H = this->scale + name_height + this->scale;

            this->bar_rect[2] = ((float) this->rect.W) * ((float) this->value - (float) this->min) / ((float) this->max - (float) this->min);
            this->bar_rect[3] = this->rect.H;
            break;
        }

        case 1: {
            this->rect.W = this->scale + name_height + this->scale;
            this->rect.H = this->size;

            this->bar_rect[2] = this->rect.W;
            this->bar_rect[3] = ((float) this->rect.H) * ((float) this->value - (float) this->min) / ((float) this->max - (float) this->min);
            break;
        }
    }

    switch (this->text_align_docking) {
        case ekg::dock::LEFT: {
            this->text_align_x = 2.0F;
            this->text_align_y = this->scale;
            break;
        }

        case ekg::dock::CENTER: {
            if (this->bar_orientation == 0) {
                this->text_align_x = (this->get_width() / 2.0F) - (this->text_width / 2.0F);
                this->text_align_y = this->scale;
            } else {
                this->text_align_x = (this->get_width() / 2.0F) - (this->text_width / 2.0F);
                this->text_align_y = (this->get_height() / 2.0F) - (this->text_height / 2.0F);
            }
            break;
        }

        case ekg::dock::RIGHT: {
            this->text_align_x = this->get_width() - this->text_width - 2.0F;
            this->text_align_y = this->scale;
            break;
        }

        case ekg::dock::TOP: {
            this->text_align_x = (this->get_width() / 2.0F) - (this->text_width / 2.0F);
            this->text_align_y = 2.0F;
            break;
        }

        case ekg::dock::BOTTOM: {
            this->text_align_x = (this->get_width() / 2.0F) - (this->text_width / 2.0F);
            this->text_align_y = this->get_height() - this->text_height - 2.0F;
            break;
        }
    }
}

void ekg_ui_element_slider::on_pre_event(SDL_Event event) {
    ekg_abstract_element::on_pre_event(event);
}

void ekg_ui_element_slider::on_event(SDL_Event event) {
    ekg_abstract_element::on_event(event);

    switch (event.type) {
        case SDL_FINGERUP: {
            this->pressed = false;
            this->dragging = false;
            break;
        }

        default: {
            if (this->drag && (event.type == SDL_FINGERDOWN || event.type == SDL_FINGERMOTION)) {
                float fx = event.tfinger.x;
                float fy = event.tfinger.y;

                ekg::scaled_finger_pos(fx, fy);

                if (!this->dragging && this->hovered && event.type == SDL_FINGERDOWN) {
                    ekg::task(ekg::task::BLOCKED);

                    this->pressed = true;
                    this->dragging = true;
                }

                if (this->dragging && this->pressed) {
                    ekg::task(ekg::task::BLOCKED);

                    float diff = (this->bar_orientation == 0 ? this->rect.W : this->rect.H);
                    float finger_pos_factored = this->bar_orientation == 0 ? fx : fy;

                    // Set bar progress.
                    this->sync_bar(std::min(diff, (float) std::max(0.0F, finger_pos_factored - (this->bar_orientation == 0 ? this->rect.x : this->rect.y))));
                }
            }

            break;
        }
    }
}

void ekg_ui_element_slider::on_post_event(SDL_Event event) {
    ekg_abstract_element::on_post_event(event);
}

void ekg_ui_element_slider::on_update(const float &delta_ticks) {
    ekg_abstract_element::on_update(delta_ticks);
}

void ekg_ui_element_slider::on_render(const float &render_ticks) {
    ekg_abstract_element::on_render(render_ticks);

    // enable scissor test and cut off the fragments.
    ekg_scissor(this->get_scissor_x(), this->get_scissor_y(), this->get_scissor_w(), this->get_scissor_h());

    // Background.
    ekg_color color(EKG_CORE->color_theme.WidgetBackground);
    ekg_draw_filled_rect(this->rect, color);

    // Bar.
    color.Set(EKG_CORE->color_theme.WidgetActivy);
    ekg_draw_filled_shape(this->get_x() + this->bar_rect[0], this->get_y() + this->bar_rect[1], this->bar_rect[2], this->bar_rect[3], color);

    if (EKG_CORE->color_theme.IsOutlineSliderEnabled()) {
        ekg_draw_outline_rect(this->rect, 1.5f, EKG_CORE->color_theme.StringColor);
    }

    // value.
    if (this->text_visible) {
        EKG_CORE->font_renderer.DrawString(this->format, this->get_x() + this->text_align_x, this->get_y() + this->text_align_y, EKG_CORE->color_theme.StringColor);
    }

    // End scissor.
    ekg_end_scissor();
}

void ekg_ui_element_slider::orientation(const std::string& side) {
    unsigned int flag = side == "Horizontal" ? 0 : 1;

    if (this->bar_orientation != flag) {
        this->bar_orientation = flag;
        this->sync_size();
    }
}

void ekg_ui_element_slider::set_size(float bar_side) {
    if (this->size != bar_side) {
        this->size = bar_side;
        this->sync_size();
    }
}

float ekg_ui_element_slider::get_size() {
    return this->size;
}

void ekg_ui_element_slider::set_max(double maximum) {
    if (this->max != maximum) {
        this->max = maximum;
        this->sync_size();
    }
}

double ekg_ui_element_slider::get_max() {
    return this->max;
}

void ekg_ui_element_slider::set_min(double minimum) {
    if (this->min != minimum) {
        this->min = minimum;
        this->sync_size();
    }
}

double ekg_ui_element_slider::get_min() {
    return this->min;
}

void ekg_ui_element_slider::set_value(double val) {
    double value_clampf = val < this->min ? this->min : (val > this->max ? this->max : val);

    if (this->value != value_clampf) {
        this->value = value_clampf;
        this->sync_size();
    }
}

double ekg_ui_element_slider::get_value() {
    return this->value;
}

void ekg_ui_element_slider::set_scale(float amount) {
    if (this->scale != amount) {
        this->scale = amount;
        this->sync_size();
    }
}

float ekg_ui_element_slider::GetScale() {
    return this->scale;
}

void ekg_ui_element_slider::sync_bar(float pos_factor) {
    if (pos_factor == 0) {
        this->set_value(this->get_min());
        return;
    }

    float bar_size_factory = this->bar_orientation == 0 ? this->rect.W : this->rect.H;

    // In this case we set the new value.
    this->set_value(
            ((double) pos_factor / (double) bar_size_factory) * (this->max - this->min) + this->min);
}

void ekg_ui_element_slider::draggable(bool state) {
    this->drag = state;
}

void ekg_ui_element_slider::text_align(unsigned int docking) {
    bool flag = (docking == ekg::dock::LEFT || docking == ekg::dock::RIGHT || docking == ekg::dock::CENTER || docking == ekg::dock::TOP || docking == ekg::dock::BOTTOM);
    bool should_sync = this->text_align_docking != docking;

    if (should_sync && flag) {
        this->text_align_docking = docking;
        this->sync_size();
    } else if (!flag) {
        ekg_log(ekg_print(this->get_tag(), this->get_id()) +
                " Incorrect label align: for horizontal docking (LEFT - CENTER - RIGHT), for vertical docking (TOP - CENTER - BOTTOM)");

        should_sync = this->text_align_docking != ekg::dock::CENTER;
        this->text_align_docking = ekg::dock::CENTER;
    }
}

void ekg_ui_element_slider::text_visibility(bool visible) {
    this->text_visible = visible;
}

void ekg_ui_element_slider::set_offset_text(float offset) {
    this->offset_text = offset;
}

float ekg_ui_element_slider::get_offset_text() {
    return this->offset_text;
}

std::string ekg_ui_element_slider::info_class() {
    ekg_abstract_element::info_class();
    return "slider";
}
