#include "ekg_ui_element_button.h"
#include "ekg.h"

void ekg_ui_element_button::set_box_texture(const ekg_data &texture) {
    this->texture_box = texture;
}

void ekg_ui_element_button::set_press_state(bool state) {
    this->pressed = state;
}

bool ekg_ui_element_button::is_pressed() {
    return this->pressed;
}

void ekg_ui_element_button::set_check_state(bool state) {
    this->checked = state;
}

bool ekg_ui_element_button::is_checked() {
    return this->checked;
}

bool ekg_ui_element_button::is_check_box() {
    return this->box;
}

void ekg_ui_element_button::on_pre_event(SDL_Event event) {
    ekg_abstract_element::on_pre_event(event);

    if (event.type == SDL_FINGERDOWN || event.type == SDL_FINGERMOTION) {
        float FX = event.tfinger.x;
        float FY = event.tfinger.y;

        ekg::scaled_finger_pos(FX, FY);
        this->hovered_box = this->detect_point_collide_box(FX, FY);
    }
}

void ekg_ui_element_button::on_event(SDL_Event event) {
    ekg_abstract_element::on_event(event);

    switch (event.type) {
        case SDL_FINGERDOWN: {
            if (this->hovered) {
                ekg::task(ekg::task::BLOCKED);
                this->pressed = true;
            }

            break;
        }

        case SDL_FINGERUP: {
            if (this->pressed) {
                float FX = event.tfinger.x;
                float FY = event.tfinger.y;

                ekg::scaled_finger_pos(FX, FY);

                this->hovered_box = this->detect_point_collide_box(FX, FY);
                this->hovered = this->collide_with_pos(FX, FY);

                if (this->hovered_box || this->hovered) {
                    ekg::task(ekg::task::BLOCKED);

                    this->checked = this->is_check_box() ? !this->checked : this->checked;
                    this->clicked = true;
                }

                this->pressed = false;
            }

            break;
        }
    }
}

void ekg_ui_element_button::on_post_event(SDL_Event event) {
    ekg_abstract_element::on_post_event(event);
    this->hovered_box = false;}

void ekg_ui_element_button::on_update(const float &delta_ticks) {
    ekg_abstract_element::on_update(delta_ticks);
}

void ekg_ui_element_button::on_render(const float &render_ticks) {
    ekg_abstract_element::on_render(render_ticks);

    // Reset stuff.
    this->clicked = false;

    // update it.
    this->smooth_pressed.NextFactory = this->pressed && !this->hovered_box ? (float) EKG_CORE->color_theme.WidgetPressed[3] : 0;
    this->smooth_box_pressed.NextFactory = this->hovered_box && this->pressed ? (float) EKG_CORE->color_theme.WidgetPressed[3] : 0;
    this->smooth_box_activy.NextFactory = this->checked ? (float) EKG_CORE->color_theme.WidgetActivy[3] : 0;

    // update animations.
    this->smooth_pressed.Update(render_ticks);
    this->smooth_box_pressed.Update(render_ticks);
    this->smooth_box_activy.Update(render_ticks);

    // enable scissor test and cut off the fragments.
    ekg_scissor(this->get_scissor_x(), this->get_scissor_y(), this->get_scissor_w(), this->get_scissor_h());

    // Background
    ekg_color color(EKG_CORE->color_theme.WidgetBackground);
    ekg_draw_filled_rect(this->rect, color);

    // border
    if (EKG_CORE->color_theme.IsOutlineButtonEnabled()) {
        ekg_draw_outline_rect(this->rect, 1.5f, EKG_CORE->color_theme.StringColor);
    }

    // pressed.
    if (this->smooth_pressed.Factory > 10) {
        color.Set(EKG_CORE->color_theme.WidgetPressed, (unsigned int) this->smooth_pressed.Factory);
        ekg_draw_filled_rect(this->rect, color);
    }

    if (this->box) {
        if (this->smooth_box_activy.Factory > 10) {
            color.Set(EKG_CORE->color_theme.WidgetActivy, (unsigned int) this->smooth_box_activy.Factory);
            ekg_draw_filled_shape(this->get_x() + this->box_rect[0], this->get_y() + this->box_rect[1], this->box_rect[2], this->box_rect[3], color);
        }

        // border of box.
        ekg_draw_outline_shape(this->get_x() + this->box_rect[0], this->get_y() + this->box_rect[1], this->box_rect[2], this->box_rect[3], 2.0F, EKG_CORE->color_theme.StringColor);

        // box
        if (this->smooth_box_pressed.Factory > 10) {
            color.Set(EKG_CORE->color_theme.WidgetPressed, (unsigned int) this->smooth_box_pressed.Factory);
            ekg_draw_filled_shape(this->get_x() + this->box_rect[0], this->get_y() + this->box_rect[1], this->box_rect[2], this->box_rect[3], color);
        }
    }

    // String.
    EKG_CORE->font_renderer.DrawString(this->tag, this->rect.x + this->align_offset_text + this->offset_text, this->rect.y + this->size, EKG_CORE->color_theme.StringColor);

    // End scissor.
    ekg_end_scissor();
}

ekg_data &ekg_ui_element_button::get_check_box_texture() {
    return this->texture_box;
}

void ekg_ui_element_button::set_offset_text(float offset) {
    this->offset_text = offset;
}

float ekg_ui_element_button::get_offset_text() {
    return this->offset_text;
}

void ekg_ui_element_button::set_offset_box(float offset) {
    this->offset_box = offset;
}

float ekg_ui_element_button::get_offset_box() {
    return this->offset_box;
}

float* ekg_ui_element_button::get_box_rect() {
    return this->box_rect;
}

bool ekg_ui_element_button::is_hovered_box() {
    return this->hovered_box;
}

void ekg_ui_element_button::set_hovered_box_state(bool state) {
    this->hovered_box = state;
}

void ekg_ui_element_button::set_box_rect(float x, float y, float w, float h) {
    this->box_rect[0] = x;
    this->box_rect[1] = y;
    this->box_rect[2] = w;
    this->box_rect[3] = h;
}

bool ekg_ui_element_button::detect_point_collide_box(float x, float y) {
    float bx = this->get_x() + this->box_rect[0];
    float by = this->get_y() + this->box_rect[1];
    float bw = this->box_rect[2];
    float bh = this->box_rect[3];

    return x > bx && y > by && x < bx + bw && y < by + bh;
}

void ekg_ui_element_button::set_size(float button_size) {
    if (this->size != button_size) {
        this->size = button_size;
        this->sync_size();
    }
}

float ekg_ui_element_button::get_size() {
    return this->size;
}

void ekg_ui_element_button::sync_size() {
    ekg_abstract_element::sync_size();

    this->text_width = EKG_CORE->font_renderer.GetStringWidth(this->tag);
    this->text_height = EKG_CORE->font_renderer.GetStringHeight(this->tag);

    this->rect.H = this->size + this->text_height + this->size;

    /* After sync_stack_scaled_metrics minimal sizes of rect. */
    /* We need to sync_stack_scaled_metrics align text and box (if the mode is on). */

    if (this->box) {
        // The square of box.
        float factor = !this->box_scaled ? this->text_height : (this->rect.H / 2) + ((this->rect.H / 4));

        this->box_rect[2] = factor;
        this->box_rect[3] = factor;

        switch (this->align_box_docking) {
            case ekg::dock::LEFT: {
                this->align_offset_box = factor / 4;
                break;
            }

            case ekg::dock::RIGHT: {
                this->align_offset_box = this->rect.W - factor - (factor / 4);
                break;
            }

            case ekg::dock::CENTER: {
                this->align_offset_box = (this->rect.W / 2) - (factor / 2);
                break;
            }
        }
    }

    switch (this->align_text_docking) {
        case ekg::dock::LEFT: {
            this->align_offset_text = 4.0f;
            break;
        }

        case ekg::dock::RIGHT: {
            this->align_offset_text = this->rect.W - this->text_width - 4.0f;
            break;
        }

        case ekg::dock::CENTER: {
            this->align_offset_text = (this->rect.W / 2) - (this->text_width / 2);
            break;
        }
    }

    // Now update the check box ret offset pos.
    if (this->box) {
        this->box_rect[0] = this->align_offset_box + this->offset_box;
        this->box_rect[1] = this->box_scaled ? ((this->rect.H - this->box_rect[2]) / 2) : this->size;

        float factor = this->box_rect[2];
        float x = this->box_rect[0];

        if (this->align_box_docking == ekg::dock::LEFT && x + factor >= this->align_offset_text) {
            this->offset_text = ((x + factor) - this->align_offset_text) + 2.0F;
        }

        if (this->align_box_docking == ekg::dock::RIGHT && this->offset_text + this->align_offset_text + this->text_width >= this->align_offset_box) {
            this->offset_text = x - this->text_width - this->align_offset_text - 2.0F;
        }
    }
}

float ekg_ui_element_button::get_text_width() {
    return this->text_width;
}

float ekg_ui_element_button::get_text_height() {
    return this->text_height;
}

void ekg_ui_element_button::SetWidth(float Width) {
    if (this->rect.W != Width) {
        this->rect.W = Width;
        this->sync_size();
    }
}

bool ekg_ui_element_button::is_clicked() {
    return this->clicked;
}

void ekg_ui_element_button::set_click_state(bool state) {
    this->clicked = state;
}

void ekg_ui_element_button::set_mode(const std::string& mode) {
    bool should_sync = this->box != true;

    if (mode == "check-box-scaled") {
        this->box = true;
        this->box_scaled = true;
        this->button_mode = mode;
    } else if (mode == "check-box") {
        should_sync = should_sync == false ? true : should_sync;

        this->box = true;
        this->box_scaled = false;
        this->button_mode = mode;
    } else if (mode == "normal") {
        if (this->box != false) {
            should_sync = true;
        }

        this->box = false;
        this->button_mode = mode;
    } else {
        should_sync = false;
        ekg_log(ekg_print(this->tag, this->id) +
                "set_mode does not exist: (check-box-scaled - check-box - normal) ???\"" + mode +
                "\"???");
    }

    if (should_sync) {
        this->sync_size();
    }
}

void ekg_ui_element_button::align_box(unsigned int dock) {
    bool flag = (dock == ekg::dock::LEFT || dock == ekg::dock::CENTER || dock == ekg::dock::RIGHT);

    if (this->align_box_docking != dock && flag) {
        this->align_box_docking = dock;
        this->offset_box = 0.0F;
        this->sync_size();
    } else {
        ekg_log(ekg_print(this->tag, this->id) +
                "Incorrect box align: only accept (LEFT - CENTER - RIGHT)");
    }
}

void ekg_ui_element_button::align_text(unsigned int dock) {
    bool flag = (dock == ekg::dock::LEFT || dock == ekg::dock::CENTER || dock == ekg::dock::RIGHT);

    if (this->align_text_docking != dock && flag) {
        this->align_text_docking = dock;
        this->offset_text = 0.0F;
        this->sync_size();
    } else {
        ekg_log(ekg_print(this->tag, this->id) +
                "Incorrect text align: only accept (LEFT - CENTER - RIGHT)");
    }
}

std::string ekg_ui_element_button::info_class() {
    ekg_abstract_element::info_class();
    return "button";
}

std::string ekg_ui_element_button::get_mode() {
    return this->button_mode;
}
