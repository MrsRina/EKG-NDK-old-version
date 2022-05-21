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

void ekg_ui_element_frame::on_pre_event(SDL_Event Event) {
    ekg_abstract_element::on_pre_event(Event);
}

void ekg_ui_element_frame::on_event(SDL_Event Event) {
    ekg_abstract_element::on_event(Event);

    switch (Event.type) {
        case SDL_FINGERMOTION: {
            float FX = Event.tfinger.x;
            float FY = Event.tfinger.y;

            ekg::scaled_finger_pos(FX, FY);
            bool MovingEvent;

            if (this->dragging) {
                float X = FX - this->drag_x;
                float Y = FY - this->drag_y;
                float W = this->rect.W;
                float H = this->rect.H;

                if (this->get_master_id() != 0) {
                    if (!this->free_drag_and_drop) {
                        if (X < 0) {
                            X = 0;
                        }

                        if (Y < 0) {
                            Y = 0;
                        }

                        if (X + W > this->scaled_width) {
                            X = this->scaled_width - W;
                        }

                        if (Y + H > this->scale_height) {
                            Y = this->scale_height - H;
                        }
                    }

                    this->sync_x = X;
                    this->sync_y = Y;

                    this->sync_pos();
                } else {
                    this->rect.X = FX - this->drag_x;
                    this->rect.Y = FY - this->drag_y;
                }

                MovingEvent = true;
                ekg::task(ekg::task::BLOCKED);
            }

            if (this->resizing != 0) {
                float X;
                float Y;
                float W;
                float H;

                if (this->resizing == ekg::dock::LEFT || this->resizing == ekg::dock::TOP) {
                    float DragUpdateX = FX - this->drag_w;
                    float DragUpdateY = FY - this->drag_h;

                    float PredictionW = this->previous_w + (this->previous_x - DragUpdateX);
                    float PredictionH = this->previous_h + (this->previous_y - DragUpdateY);

                    X = PredictionW < (float) this->min_width ? this->previous_x + this->previous_w - (float) this->min_width : DragUpdateX;
                    Y = PredictionH < (float) this->min_height ? this->previous_y + this->previous_h - (float) this->min_height : DragUpdateY;

                    if (this->get_master_id() == 0) {
                        this->rect.X = X;
                        this->rect.Y = Y;
                    } else {
                        X -= this->scaled_x;
                        Y -= this->scaled_y;

                        if (X < 0) {
                            X = 0;
                        }

                        if (Y < 0) {
                            Y = 0;
                        }

                        this->sync_x = X;
                        this->sync_y = Y;

                        this->sync_pos();
                    }

                    // Diff DataWidth & DataHeight. Not prediction anymore, recycled.
                    PredictionW = this->previous_w + (this->previous_x - this->rect.X);
                    PredictionH = this->previous_h + (this->previous_y - this->rect.Y);

                    W = PredictionW < (float) this->min_width ? (float) this->min_width : PredictionW;
                    H = PredictionH < (float) this->min_height ? (float) this->min_height : PredictionH;

                    this->rect.W = W;
                    this->rect.H = H;
                } else {
                    X = this->rect.X;
                    Y = this->rect.Y;

                    W = FX - this->drag_w;
                    H = FY - this->drag_h;

                    if (this->get_master_id() != 0) {
                        if (X + W > this->scaled_x + this->scaled_width) {
                            W = this->scaled_x + this->scaled_width - X;
                        }

                        if (Y + H > this->scaled_y + this->scale_height) {
                            H = this->scaled_y + this->scale_height - Y;
                        }

                        this->sync_pos();
                    }

                    this->rect.W = W < (float) this->min_width ? (float) this->min_width : W;
                    this->rect.H = H < (float) this->min_height ? (float) this->min_height : H;
                }

                MovingEvent = true;
                ekg::task(ekg::task::BLOCKED);
            }

            if (MovingEvent && this->is_master()) {
                this->sync_parents_metric();
            }

            break;
        }

        case SDL_FINGERDOWN: {
            float FX = Event.tfinger.x;
            float FY = Event.tfinger.y;

            ekg::scaled_finger_pos(FX, FY);

            if (this->hovered) {
                if (this->flags_dock_draggable != 0 && !this->dragging && this->resizing == 0) {
                    int CollidingDock = ekg::point_collide_dock(this->flags_dock_draggable, FX, FY, 0,
                                                                this->drag_offset_normal, this->rect);

                    if (CollidingDock != -1) {
                        if (this->get_master_id() == 0) {
                            this->drag_x = FX - this->rect.X;
                            this->drag_y = FY - this->rect.Y;
                        } else {
                            this->drag_x = FX - (this->rect.X - this->scaled_x);
                            this->drag_y = FY - (this->rect.Y - this->scaled_y);
                        }

                        // Say true for dragging to the element.
                        this->dragging = true;
                        ekg::task(ekg::task::BLOCKED);
                    }
                }

                if (this->flags_dock_resizable != -1 && this->resizing == 0 && !this->dragging) {
                    int CollidingDock = ekg::point_collide_dock(this->flags_dock_resizable, FX, FY, 0,
                                                                this->resize_offset_normal, this->rect);

                    if (CollidingDock != -1) {
                        if (CollidingDock == ekg::dock::LEFT || CollidingDock == ekg::dock::TOP) {
                            // Its works like a drag.
                            this->drag_w = FX - (this->rect.X);
                            this->drag_h = FY - (this->rect.Y);

                            // Save to use after.
                            this->previous_x = this->rect.X;
                            this->previous_y = this->rect.Y;
                            this->previous_w = this->rect.W;
                            this->previous_h = this->rect.H;
                        } else {
                            this->drag_w = FX - this->rect.W;
                            this->drag_h = FY - this->rect.H;
                        }

                        // Say true for resizing to the element.
                        this->resizing = CollidingDock;
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

void ekg_ui_element_frame::on_post_event(SDL_Event Event) {
    ekg_abstract_element::on_post_event(Event);
}

void ekg_ui_element_frame::on_update(const float &DeltaTicks) {
    ekg_abstract_element::on_update(DeltaTicks);
}

void ekg_ui_element_frame::on_render(const float &PartialTicks) {
    ekg_abstract_element::on_render(PartialTicks);

    // Enable scissor test and cut off the fragments.
    ekg_scissor(this->get_scissor_x(), this->get_scissor_y(), this->get_scissor_w(),
                this->get_scissor_h());

    // Background.
    EKG_Color Color(EKG_CORE->color_theme.FrameBackground);

    if (this->alternative_alpha) {
        Color.A = this->alpha;
    }

    ekg_draw_filled_rect(this->rect, Color);

    // border.
    if (EKG_CORE->color_theme.IsOutlineFrameEnabled()) {
        ekg_draw_outline_rect(this->rect, 2.0f, this->border);
    }

    // pressed state.
    if (this->dragging || this->resizing != 0) {
        Color.Set(EKG_CORE->color_theme.FramePressed);
        ekg_draw_filled_rect(this->rect, Color);
    }

    // End scissor.
    ekg_end_scissor();
}

void ekg_ui_element_frame::custom_alpha(unsigned int alpha255) {
    this->alternative_alpha = true;
    this->alpha = alpha255;
}

EKG_Color &ekg_ui_element_frame::get_border_color() {
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
    float Diff = minimum_width < 10 ? 10 : minimum_width;

    bool ShouldSyncPhase1 = this->min_width != Diff;
    bool ShouldSyncPhase2;

    this->min_width = Diff;

    Diff = minimum_height < 10 ? 10 : minimum_height;
    ShouldSyncPhase2 = this->min_height != Diff;

    this->min_height = Diff;

    if (ShouldSyncPhase1 || ShouldSyncPhase2) {
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
    EKG_CORE->sync_stack_scaled_metrics(this->rect.X, this->rect.Y, this->rect.W, this->rect.H,
                                        this->children_stack);
}
