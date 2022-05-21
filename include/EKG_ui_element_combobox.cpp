/**
 * @author Rina
 * @since 28/04/2022 at 16:27
 **/

#include "ekg_ui_element_combobox.h"
#include "ekg.h"

void ekg_ui_element_combobox::add(std::string &string) {
    this->popup_element_list.push_back(string);
}

void ekg_ui_element_combobox::remove(std::string &string) {
    std::vector<std::string> new_list;

    for (const std::string &strings : this->popup_element_list) {
        if (ekg_string_in(string, strings)) {
            continue;
        }

        new_list.push_back(strings);
    }

    this->popup_element_list = new_list;
}

void ekg_ui_element_combobox::align_text(unsigned int dock) {
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

void ekg_ui_element_combobox::set_width(float width) {
    if (this->rect.W != width) {
        this->rect.W = width;
        this->sync_size();
    }
}

void ekg_ui_element_combobox::set_list(const std::vector<std::string> &list) {
    this->popup_element_list = list;
}

void ekg_ui_element_combobox::set_offset_text(float offset) {
    this->offset_text = offset;
}

float ekg_ui_element_combobox::get_offset_text() {
    return this->offset_text;
}

void ekg_ui_element_combobox::set_scale(float scale) {
    this->text_scale = scale;
}

float ekg_ui_element_combobox::get_scale() {
    return this->text_scale;
}

void ekg_ui_element_combobox::set_press_state(bool state) {
    this->pressed = state;
}

bool ekg_ui_element_combobox::is_pressed() {
    return this->pressed;
}

void ekg_ui_element_combobox::set_current(const std::string &string) {
    bool should_sync = this->popup_element_list.empty();

    if (should_sync && this->current != " ") {
        this->current = " ";
        this->sync_size();
    }

    if (!should_sync) {
        should_sync = false;

        for (const std::string &strings : this->popup_element_list) {
            if (strings == string) {
                should_sync = true;
                break;
            }
        }

        if (!should_sync && this->current != this->popup_element_list.at(0)) {
            this->current = this->popup_element_list.at(0);
            this->sync_size();
        }
    }

    if (should_sync && this->current != string) {
        this->current = string;
        this->sync_size();
    }
}

std::string ekg_ui_element_combobox::get_current() {
    return this->current;
}

std::string ekg_ui_element_combobox::info_class() {
    return "combobox";
}

void ekg_ui_element_combobox::sync_size() {
    ekg_abstract_element::sync_size();

    this->text_width = EKG_CORE->font_renderer.GetStringWidth(this->current);
    this->text_height = EKG_CORE->font_renderer.GetStringHeight(this->current);

    this->rect.H = this->text_height + (this->text_scale * 2);

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
}

void ekg_ui_element_combobox::on_pre_event(SDL_Event event) {
    ekg_abstract_element::on_pre_event(event);
}

void ekg_ui_element_combobox::on_event(SDL_Event event) {
    ekg_abstract_element::on_event(event);

    switch (event.type) {
        default: {
            if (!this->activy) {
                return;
            }

            ekg_event custom_event = ekg::event::read(event);

            if (custom_event.Type == ekg::event::POPUP && ekg_string_in(custom_event.popup.callback, this->get_tag() + "-combobox")) {
                for (const std::string &strings : this->popup_element_list) {
                    if (ekg_string_in(custom_event.popup.callback, strings)) {
                        this->set_current(strings);
                        break;
                    }
                }

                this->activy = false;
            }

            break;
        }

        case SDL_FINGERDOWN: {
            bool should_set_pressed = this->hovered;

            if (this->hovered && !this->children_stack.StackedIds.empty()) {
                auto* Element = (ekg_ui_element_popup*) EKG_CORE->get_element_by_id(this->children_stack.StackedIds.at(0));

                if (Element != nullptr) {
                    Element->set_visibility_flag(Element->get_visibility_flag() == ekg::visibility::VISIBLE ? ekg::visibility::INVISIBLE : ekg::visibility::VISIBLE);
                    should_set_pressed = Element->get_visibility_flag() == ekg::visibility::VISIBLE;
                }
            }

            this->pressed = should_set_pressed;

            if (this->pressed && this->hovered && ekg::current_focused_type() == "combobox" && this->children_stack.StackedIds.empty()) {
                auto popup = ekg::Popup(this->get_tag() + "-combobox", ekg::NOPOS, ekg::NOPOS, this->popup_element_list);

                popup->SetWidth(this->get_width());
                popup->place(this->get_x(), this->get_y() + this->get_height());
                popup->set_visibility_flag(ekg::visibility::VISIBLE);
                popup->SetScale(this->text_scale);
                popup->set_hover_state(true);
                popup->set_master_id(this->get_id());

                this->children_stack.Put(popup->get_id());
                ekg::task(ekg::task::BLOCKED | ekg::task::REFOCUS);
            }

            break;
        }
    }
}

void ekg_ui_element_combobox::on_post_event(SDL_Event event) {
    ekg_abstract_element::on_post_event(event);
}

void ekg_ui_element_combobox::on_update(const float &delta_ticks) {
    ekg_abstract_element::on_update(delta_ticks);
}

void ekg_ui_element_combobox::on_render(const float &render_ticks) {
    ekg_abstract_element::on_render(render_ticks);

    // update animations.
    this->smooth_pressed.Update(render_ticks);
    this->smooth_pressed.NextFactory = this->pressed ? (float) EKG_CORE->color_theme.WidgetPressed[3] : 0;

    // Enable scissor test and cut off the fragments.
    ekg_scissor(this->get_scissor_x(), this->get_scissor_y(), this->get_scissor_w(),
                this->get_scissor_h());

    // Background
    EKG_Color color(EKG_CORE->color_theme.WidgetBackground);
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

    // String.
    EKG_CORE->font_renderer.DrawString(this->current, this->rect.X + this->align_offset_text + this->offset_text, this->rect.Y + this->text_scale, EKG_CORE->color_theme.StringColor);

    // End scissor.
    ekg_end_scissor();
}

void ekg_ui_element_combobox::on_parent_killed(unsigned int element_id) {
    this->activy = true;
}
