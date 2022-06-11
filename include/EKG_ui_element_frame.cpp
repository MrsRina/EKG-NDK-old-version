#include "ekg_ui_element_frame.h"
#include "ekg.h"

void ekg_ui_element_frame::draggable(unsigned int area) {
    this->flags_dock_draggable = area == NULL ? 0 : area;

    if (area == ekg::dock::FULL) {
        this->set_drag_offset(0.0f);
    }
}

void ekg_ui_element_frame::resizable(unsigned int area) {
    this->flags_dock_resizable = area == NULL ? 0 : area;

    if (area == ekg::dock::FULL) {
        this->set_resize_offset(0.0f);
    }
}

void ekg_ui_element_frame::set_drag_state(bool state) {
    this->dragging = state;
}

bool ekg_ui_element_frame::is_dragging() {
    return this->dragging;
}

void ekg_ui_element_frame::set_resize_state(bool state) {
    this->resizing = state;
}

bool ekg_ui_element_frame::is_resizing() {
    return this->resizing;
}

void ekg_ui_element_frame::set_press_state(bool state) {
    this->pressed = state;
}

bool ekg_ui_element_frame::is_pressed() {
    return this->pressed;
}

void ekg_ui_element_frame::on_pre_event(SDL_Event event) {
    ekg_abstract_element::on_pre_event(event);
}

void ekg_ui_element_frame::on_event(SDL_Event event) {
    ekg_abstract_element::on_event(event);

    switch (event.type) {
        case SDL_FINGERMOTION: {
            float fx = event.tfinger.x;
            float fy = event.tfinger.y;

            ekg::scaled_finger_pos(fx, fy);
            bool moving_event;

            if (this->dragging) {
                float x = fx - this->drag_x;
                float y = fy - this->drag_y;
                float w = this->rect.W;
                float h = this->rect.H;

                if (this->get_master_id() != 0) {
                    if (!this->free_drag_and_drop) {
                        if (x < 0) {
                            x = 0;
                        }

                        if (y < 0) {
                            y = 0;
                        }

                        if (x + w > this->scaled_width) {
                            x = this->scaled_width - w;
                        }

                        if (y + h > this->scale_height) {
                            y = this->scale_height - h;
                        }
                    }

                    this->sync_x = x;
                    this->sync_y = y;

                    this->sync_pos();
                } else {
                    this->rect.x = fx - this->drag_x;
                    this->rect.y = fy - this->drag_y;
                }

                moving_event = true;
                ekg::task(ekg::task::BLOCKED);
            }

            if (this->resizing != 0) {
                float x;
                float y;
                float w;
                float h;

                if (this->resizing == ekg::dock::LEFT || this->resizing == ekg::dock::TOP) {
                    float drag_next_x = fx - this->drag_w;
                    float drag_next_y = fy - this->drag_h;

                    float predict_w = this->previous_w + (this->previous_x - drag_next_x);
                    float predict_h = this->previous_h + (this->previous_y - drag_next_y);

                    x = predict_w < (float) this->min_width ? this->previous_x + this->previous_w - (float) this->min_width : drag_next_x;
                    y = predict_h < (float) this->min_height ? this->previous_y + this->previous_h - (float) this->min_height : drag_next_y;

                    if (this->get_master_id() == 0) {
                        this->rect.x = x;
                        this->rect.y = y;
                    } else {
                        x -= this->scaled_x;
                        y -= this->scaled_y;

                        if (x < 0) {
                            x = 0;
                        }

                        if (y < 0) {
                            y = 0;
                        }

                        this->sync_x = x;
                        this->sync_y = y;

                        this->sync_pos();
                    }

                    // Diff data_width & data_height. Not prediction anymore, recycled.
                    predict_w = this->previous_w + (this->previous_x - this->rect.x);
                    predict_h = this->previous_h + (this->previous_y - this->rect.y);

                    w = predict_w < (float) this->min_width ? (float) this->min_width : predict_w;
                    h = predict_h < (float) this->min_height ? (float) this->min_height : predict_h;

                    this->rect.W = w;
                    this->rect.H = h;
                } else {
                    x = this->rect.x;
                    y = this->rect.y;

                    w = fx - this->drag_w;
                    h = fy - this->drag_h;

                    if (this->get_master_id() != 0) {
                        if (x + w > this->scaled_x + this->scaled_width) {
                            w = this->scaled_x + this->scaled_width - x;
                        }

                        if (y + h > this->scaled_y + this->scale_height) {
                            h = this->scaled_y + this->scale_height - y;
                        }

                        this->sync_pos();
                    }

                    this->rect.W = w < (float) this->min_width ? (float) this->min_width : w;
                    this->rect.H = h < (float) this->min_height ? (float) this->min_height : h;
                }

                moving_event = true;
                ekg::task(ekg::task::BLOCKED);
            }

            if (moving_event && this->is_master()) {
                this->sync_parents_metric();
            }

            break;
        }

        case SDL_FINGERDOWN: {
            float fx = event.tfinger.x;
            float fy = event.tfinger.y;

            ekg::scaled_finger_pos(fx, fy);

            if (this->hovered) {
                if (this->flags_dock_draggable != 0 && !this->dragging && this->resizing == 0) {
                    int colliding_dock = ekg::point_collide_dock(this->flags_dock_draggable, fx, fy, 0,
                                                                 this->drag_offset_normal, this->rect);

                    if (colliding_dock != -1) {
                        if (this->get_master_id() == 0) {
                            this->drag_x = fx - this->rect.x;
                            this->drag_y = fy - this->rect.y;
                        } else {
                            this->drag_x = fx - (this->rect.x - this->scaled_x);
                            this->drag_y = fy - (this->rect.y - this->scaled_y);
                        }

                        // Say true for dragging to the element.
                        this->dragging = true;
                        ekg::task(ekg::task::BLOCKED);
                    }
                }

                if (this->flags_dock_resizable != -1 && this->resizing == 0 && !this->dragging) {
                    int colliding_dock = ekg::point_collide_dock(this->flags_dock_resizable, fx, fy, 0,
                                                                 this->resize_offset_normal, this->rect);

                    if (colliding_dock != -1) {
                        if (colliding_dock == ekg::dock::LEFT || colliding_dock == ekg::dock::TOP) {
                            // Its works like a drag.
                            this->drag_w = fx - (this->rect.x);
                            this->drag_h = fy - (this->rect.y);

                            // Save to use after.
                            this->previous_x = this->rect.x;
                            this->previous_y = this->rect.y;
                            this->previous_w = this->rect.W;
                            this->previous_h = this->rect.H;
                        } else {
                            this->drag_w = fx - this->rect.W;
                            this->drag_h = fy - this->rect.H;
                        }

                        // Say true for resizing to the element.
                        this->resizing = colliding_dock;
                        ekg::task(ekg::task::BLOCKED);
                    }
                }
            }

            break;
        }

        case SDL_FINGERUP: {
            this->hovered = false;
            this->dragging = false;
            this->resizing = 0;

            break;
        }
    }
}

void ekg_ui_element_frame::on_post_event(SDL_Event event) {
    ekg_abstract_element::on_post_event(event);
}

void ekg_ui_element_frame::on_update(const float &delta_ticks) {
    ekg_abstract_element::on_update(delta_ticks);
}

void ekg_ui_element_frame::on_render(const float &render_ticks) {
    ekg_abstract_element::on_render(render_ticks);

    // enable scissor test and cut off the fragments.
    ekg_scissor(this->get_scissor_x(), this->get_scissor_y(), this->get_scissor_w(),
                this->get_scissor_h());

    // Background.
    ekg_color color(EKG_CORE->color_theme.FrameBackground);

    if (this->alternative_alpha) {
        color.A = this->alpha;
    }

    ekg_draw_filled_rect(this->rect, color);

    // border.
    if (EKG_CORE->color_theme.IsOutlineFrameEnabled()) {
        ekg_draw_outline_rect(this->rect, 2.0f, this->border);
    }

    // pressed state.
    if (this->dragging || this->resizing != 0) {
        color.Set(EKG_CORE->color_theme.FramePressed);
        ekg_draw_filled_rect(this->rect, color);
    }

    // End scissor.
    ekg_end_scissor();
}

void ekg_ui_element_frame::custom_alpha(unsigned int alpha255) {
    this->alternative_alpha = true;
    this->alpha = alpha255;
}

ekg_color &ekg_ui_element_frame::get_border_color() {
    return this->border;
}

void ekg_ui_element_frame::set_border_color(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
    this->border.Set(r, g, b, a);
}

void ekg_ui_element_frame::place(ekg_abstract_element* element, float x, float y) {
    if (element->get_id() == this->get_id()) {
        ekg_log(ekg_print(this->tag, this->id) + "place waring: place loop (master) id" + ekg_print(
                element->get_tag(), element->get_id()));
        return;
    }

    if (this->children_stack.Contains(element->get_id())) {
        return;
    }

    this->children_stack.Put(element->get_id());

    element->set_master_id(this->id);
    element->place(x, y);

    this->sync_parents_metric();
}

void ekg_ui_element_frame::set_resize_offset(float offset) {
    this->resize_offset_normal = offset;
}

float ekg_ui_element_frame::get_resize_offset() {
    return this->resize_offset_normal;
}

void ekg_ui_element_frame::set_drag_offset(float offset) {
    this->drag_offset_normal = offset;
}

float ekg_ui_element_frame::get_drag_offset() {
    return this->drag_offset_normal;
}

void ekg_ui_element_frame::set_width(float width) {
    this->rect.W = width < this->min_width ? this->min_width : width;
}

void ekg_ui_element_frame::set_height(float height) {
    this->rect.H = height < this->min_height ? this->min_height : height;
}

void ekg_ui_element_frame::place(float X, float Y) {
    ekg_abstract_element::place(X, Y);
}

void ekg_ui_element_frame::sync_size() {
    ekg_abstract_element::sync_size();

    this->rect.W = this->rect.W < this->min_width ? this->min_width : this->rect.W;
    this->rect.H = this->rect.H < this->min_height ? this->min_height : this->rect.H;
}

void ekg_ui_element_frame::SetLimit(float minimum_width, float minimum_height) {
    float diff = minimum_width < 10 ? 10 : minimum_width;

    bool should_sync_phase1 = this->min_width != diff;
    bool should_sync_phase2;

    this->min_width = diff;

    diff = minimum_height < 10 ? 10 : minimum_height;
    should_sync_phase2 = this->min_height != diff;

    this->min_height = diff;

    if (should_sync_phase1 || should_sync_phase2) {
        this->sync_size();
    }
}

std::string ekg_ui_element_frame::info_class() {
    ekg_abstract_element::info_class();
    return "frame";
}

float ekg_ui_element_frame::get_min_width() {
    return this->min_width;
}

float ekg_ui_element_frame::get_min_height() {
    return this->min_height;
}

void ekg_ui_element_frame::set_size(float width, float height) {
    if (this->rect.W != width || this->rect.H != height) {
        this->rect.W = width;
        this->rect.H = height;
        this->sync_size();
    }
}

void ekg_ui_element_frame::sync_parents_metric() {
    this->sync_size();
    EKG_CORE->sync_stack_scaled_metrics(this->rect.x, this->rect.y, this->rect.W, this->rect.H,
                                        this->children_stack);
}
