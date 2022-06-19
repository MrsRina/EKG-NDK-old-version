#include "ekg_ui_element_popup.h"
#include "ekg.h"

void ekg_ui_element_popup::insert(const std::vector<std::string> &new_list) {
    this->list.clear();

    for (const std::string& string : new_list) {
        if (string.empty()) {
            continue;
        }

        ekg_data component;

        component.name = string;
        component.tag = "1";

        this->list.push_back(component);
    }
}

void ekg_ui_element_popup::remove(const std::string &pattern) {
    std::vector<ekg_data> new_list;

    for (const ekg_data &components : this->list) {
        if (ekg_string_in(pattern, components.name)) {
            continue;
        }

        new_list.push_back(components);
    }

    bool should_sync = this->list.size() != new_list.size();
    this->list = new_list;

    if (should_sync) {
        this->sync_size();
    }
}

void ekg_ui_element_popup::disable(const std::string &pattern) {
    if (pattern.empty()) {
        return;
    }

    bool should_sync;

    for (ekg_data &components : this->list) {
        if (ekg_string_in(pattern, components.name)) {
            components.tag = "";
            should_sync = true;
        }
    }

    if (should_sync) {
        this->sync_size();
    }
}

void ekg_ui_element_popup::enable(const std::string &pattern) {
    if (pattern.empty()) {
        return;
    }

    bool should_sync;

    for (ekg_data &components : this->list) {
        if (ekg_string_in(pattern, components.name)) {
            components.tag = "1";
            should_sync = true;
        }
    }

    if (should_sync) {
        this->sync_size();
    }
}

void ekg_ui_element_popup::set_offset_text(float offset_text) {
    this->text_offset = offset_text;
}

float ekg_ui_element_popup::get_offset_text() {
    return this->text_offset;
}

void ekg_ui_element_popup::set_press_state(bool state) {
    this->pressed = state;
}

bool ekg_ui_element_popup::is_pressed() {
    return this->pressed;
}

void ekg_ui_element_popup::set_click_state(bool state) {
    this->clicked = state;
}

bool ekg_ui_element_popup::is_clicked() {
    return this->clicked;
}

void ekg_ui_element_popup::set_width(float width) {
    if (this->rect.w != width) {
        this->rect.w = width;
        this->sync_size();
    }
}

std::vector<ekg_data> &ekg_ui_element_popup::get_list() {
    return this->list;
}

std::string &ekg_ui_element_popup::get_focused() {
    return this->focused;
}

std::string &ekg_ui_element_popup::get_clicked() {
    return this->selected;
}

void ekg_ui_element_popup::sync_size() {
    ekg_abstract_element::sync_size();
    this->max_height = 0.0F;

    for (ekg_data &components : this->list) {
        // It cost too many ticks (hardware) to get string height.
        components.data_height = EKG_CORE->font_renderer.GetStringHeight(components.name) + (this->text_scale * 2);
        this->max_height += components.data_height;
    }
}

void ekg_ui_element_popup::on_pre_event(SDL_Event event) {
    // There is no problem in finger pos out of master bound.
    if (event.type == SDL_FINGERDOWN || event.type == SDL_FINGERMOTION || event.type == SDL_FINGERUP) {
        float fx = event.tfinger.x;
        float fy = event.tfinger.y;

        ekg::scaled_finger_pos(fx, fy);
        this->hovered = this->rect.collide_with_point(fx, fy);
    }
}

void ekg_ui_element_popup::on_event(SDL_Event event) {
    ekg_abstract_element::on_event(event);

    switch (event.type) {
        case SDL_FINGERUP: {
            float fx = event.tfinger.x;
            float fy = event.tfinger.y;

            ekg::scaled_finger_pos(fx, fy);
            ekg_data component = this->get_component_by_pos(fx, fy);

            this->selected = "";

            if (this->pressed && !this->focused.empty() && this->focused == component.name) {
                this->clicked = true;
                this->selected = this->focused;
            }

            bool flag = ekg::current_focused_type() != "popup";

            if (!this->is_master_popup && this->get_master_id() != 0 && flag) {
                auto* element = EKG_CORE->get_element_by_id(this->get_master_id());
                flag = element != NULL && !element->collide_with_pos(fx, fy);
            }

            if (flag || (!this->focused.empty() && !this->hovered) || (!this->selected.empty() && component.Id == 0) || (EKG_CORE->get_element_by_id(component.Id) == NULL && component.Id != 0)) {
                this->kill();
            }

            if (!this->focused.empty() && (component.name == this->focused || ekg::current_focused_type() == "popup") && component.Id == 0) {
                std::string path = this->focused;
                this->get_path(path);

                // We use smart pointers instead directly pointers.
                std::shared_ptr<void> pointer(new std::string(path));

                // Send popup event using ekg events processor.
                ekg::event::dispatch(ekg::event::POPUP, static_cast<void *>(pointer.get()), nullptr);
            }

            this->pressed = false;
            this->focused = "";

            break;
        }

        case SDL_FINGERDOWN: {
            float fx = event.tfinger.x;
            float fy = event.tfinger.y;

            ekg::scaled_finger_pos(fx, fy);

            if (this->hovered) {
                float y = this->get_y();
                float x, w, h;

                ekg_data component;
                component.name = "";

                for (const ekg_data &components : this->list) {
                    x = this->get_x();

                    w = x + this->get_width();
                    h = y + components.data_height;

                    if (components.tag == "1" && fx > x && fx < w && fy > y && fy < h) {
                        component = components;
                        break;
                    }

                    y += components.data_height;
                }

                this->focused = component.name;

                if (!this->focused.empty()) {
                    this->pressed = true;

                    if (component.Id != 0) {
                        auto element_out = (ekg_ui_element_popup*) EKG_CORE->get_element_by_id(component.Id);

                        if (element_out != NULL) {
                            element_out->set_visibility_flag(ekg::visibility::VISIBLE);
                            element_out->place(this->get_x() + this->get_width(), y, this->get_x());

                            this->activy = component.name;

                            for (const ekg_data &components : this->list) {
                                auto element_in = EKG_CORE->get_element_by_id(components.Id);

                                if (element_in != NULL && element_out->get_id() != components.Id) {
                                    element_in->set_visibility_flag(ekg::visibility::INVISIBLE);
                                }
                            }
                        }
                    }
                }
            }

            bool flag = ekg::current_focused_type() != "popup";

            if (!this->is_master_popup && this->get_master_id() != 0 && flag) {
                auto* element = EKG_CORE->get_element_by_id(this->get_master_id());
                flag = element != NULL && !element->collide_with_pos(fx, fy);
            }

            if (flag) {
                this->kill();
                this->focused = "";
                this->pressed = false;
            }

            break;
        }
    }
}

void ekg_ui_element_popup::on_post_event(SDL_Event event) {
    ekg_abstract_element::on_post_event(event);
}

void ekg_ui_element_popup::on_update(const float &delta_ticks) {
    ekg_abstract_element::on_update(delta_ticks);
}

void ekg_ui_element_popup::on_render(const float &render_ticks) {
    ekg_abstract_element::on_render(render_ticks);

    // render only if is open.
    if (this->show) {
        this->rect.h = EKG_AnimationSmooth(100, SDL_GetTicks() - this->last_tick) * this->max_height;
        this->smooth_pressed.NextFactory = !this->focused.empty() ? EKG_CORE->color_theme.WidgetActivy[3] : 0;
        this->smooth_pressed.Update(render_ticks);

        // Background container.
        ekg_color color(EKG_CORE->color_theme.ContainerBackground);
        ekg_draw_filled_rect(this->rect, color);

        // Now reset color to activy in theme.
        color.Set(EKG_CORE->color_theme.WidgetActivy, smooth_pressed.Factory);

        // We need calc a runtime height to render property the positions.
        float full_height = this->get_y();

        ekg_scissor((int) this->rect.x, (int) this->rect.y, (int) this->rect.w, (int) this->rect.h);

        // Iterate all component data.
        for (const ekg_data &components : this->list) {
            // Background when is focused.
            if (components.name == this->focused) {
                ekg_draw_filled_shape(this->get_x(), full_height, this->rect.w, components.data_height, color);
            }

            // Draw name of component.
            EKG_CORE->font_renderer.DrawString(components.name, this->get_x() + 2.0F + this->text_offset, full_height + this->text_scale, components.tag != "1" ? EKG_CORE->color_theme.StringFadeColor : EKG_CORE->color_theme.StringColor);

            // update the height to the next element be rendered property.
            full_height += components.data_height;
        }

        ekg_end_scissor();

        // Outline (YOU CAN NOT DISABLED IT SORRY).
        ekg_draw_outline_rect(this->rect, 1.5f, EKG_CORE->color_theme.StringColor);
    }
}

ekg_data ekg_ui_element_popup::get_component_by_pos(float x, float y) {
    float cy = this->get_y();
    float cx, cw, ch;

    for (const ekg_data &components : this->list) {
        cx = this->get_x();

        cw = cx + this->get_width();
        ch = cy + components.data_height;

        if (components.tag == "1" && x > cx && x < cw && y > cy && y < ch) {
            return components;
        }

        cy += components.data_height;
    }

    ekg_data Component;
    Component.name = "";

    return Component;
}

void ekg_ui_element_popup::set_scale(float scale) {
    if (this->text_scale != scale) {
        this->text_scale = scale;
        this->sync_size();
    }
}

float ekg_ui_element_popup::get_scale() {
    return this->text_scale;
}

void ekg_ui_element_popup::place(ekg_ui_element_popup* element) {
    if (element == NULL || this->children_stack.Contains(element->get_id()) || element->get_id() == this->get_id()) {
        return;
    }

    bool contains;

    for (ekg_data &components : this->list) {
        if (components.name == element->get_tag()) {
            components.Id = element->get_id();
            contains = true;
        }
    }

    if (contains) {
        this->children_stack.Put(element->get_id());

        element->set_master_id(this->get_id());
        element->set_visibility_flag(ekg::visibility::INVISIBLE);
    }
}

void ekg_ui_element_popup::place(float x, float y) {
    float factored_x = x < 0 ? 0 : x;
    float factored_y = y < 0 ? 0 : y;

    if (factored_x + this->get_width() > ekg::screen_width) {
        factored_x = ekg::screen_width - this->get_width();
    }

    if (factored_y + this->max_height > ekg::screen_height) {
        factored_y = ekg::screen_height - this->max_height;
    }

    ekg_abstract_element::place(factored_x, factored_y);
}

std::string ekg_ui_element_popup::info_class() {
    ekg_abstract_element::info_class();
    return "popup";
}

void ekg_ui_element_popup::place(float x, float y, float bounding_y) {
    float factored_x = x < 0 ? 0 : x;
    float factored_y = y < 0 ? 0 : y;

    if (factored_x + this->get_width() > ekg::screen_width) {
        factored_x = bounding_y - this->get_width();
    }

    if (factored_y + this->max_height > ekg::screen_height) {
        factored_y = ekg::screen_height - this->max_height;
    }

    this->rect.x = factored_x;
    this->rect.y = factored_y;
}

void ekg_ui_element_popup::kill() {
    if (this->dead) {
        return;
    }

    ekg_abstract_element::kill();

    for (unsigned int IDs : this->children_stack.StackedIds) {
        auto* element = EKG_CORE->get_element_by_id(IDs);

        if (element != NULL) {
            element->kill();
        }
    }

    this->children_stack.StackedIds.clear();

    if (this->get_master_id() != 0 && this->is_master_popup) {
        auto* element = EKG_CORE->get_element_by_id(this->get_master_id());

        if (element != NULL) {
            element->kill();
        }
    }
}

void ekg_ui_element_popup::get_path(std::string &previous_path) {
    previous_path = this->get_tag() + "|" + previous_path;

    if (this->get_master_id() != 0) {
        auto element = (ekg_ui_element_popup*) ekg::find(this->get_master_id());

        if (element != NULL) {
            element->get_path(previous_path);
        }
    }
}

void ekg_ui_element_popup::set_master_id(unsigned int id) {
    ekg_abstract_element::set_master_id(id);

    // We can use this as flag to prevent visual glitch.
    this->is_master_popup = true;

    if (this->master_id != 0) {
        auto* element = EKG_CORE->get_element_by_id(id);
        this->is_master_popup = element != NULL && element->info_class() == "popup";
    }
}

float ekg_ui_element_popup::get_max_width() {
    return this->max_width;
}

float ekg_ui_element_popup::get_max_height() {
    return this->max_height;
}

void ekg_ui_element_popup::set_visibility_flag(unsigned int flag) {
    if (flag == ekg::visibility::INVISIBLE) {
        this->rect.h = 0;
    }

    bool flag_visible_true = (flag == ekg::visibility::VISIBLE);

    if (this->show != flag_visible_true) {
        this->last_tick = SDL_GetTicks();
        ekg_abstract_element::set_visibility_flag(ekg::visibility::INVISIBLE);
        this->show = flag_visible_true;
    }

    this->visible = flag;
}
