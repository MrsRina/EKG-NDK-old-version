#include "ekg_core.h"
#include "ekg.h"

void ekg_core::remove_element_by_id(unsigned int element_id) {
    this->buffer_render.fill(0);
    this->buffer_size = 0;
    this->buffer_copy.clear();

    for (ekg_abstract_element* &Element : this->buffer_update) {
        if (Element->get_id() == element_id) {
            Element->kill();

            delete Element;
            continue;
        }

        this->buffer_copy.push_back(Element);

        if (Element->get_visibility_flag() == ekg::visibility::VISIBLE) {
            this->buffer_render[this->buffer_size++] = Element;
        }
    }

    this->buffer_update = this->buffer_copy;
    this->buffer_copy.clear();
}

void ekg_core::add_element_to_queue(ekg_abstract_element* element) {
    this->buffer_new.push_back(element);
    this->should_swap_buffers = true;
}

void ekg_core::on_event(SDL_Event event) {
    if (event.type != SDL_FINGERDOWN && event.type != SDL_FINGERUP && event.type != SDL_FINGERMOTION && event.type != ekg::event::POPUP) {
        return;
    }

    if (event.type == SDL_FINGERDOWN) {
        this->task_blocked = false;
    }

    this->focused_element_id = 0;

    // Get focusing element id.
    for (ekg_abstract_element* &Element : this->buffer_update) {
        if (Element == nullptr || Element->is_dead()) {
            continue;
        }

        // update flags.
        Element->on_pre_event(event);

        // The next superior element is set.
        if (Element->is_hovered() && Element->get_visibility_flag() == ekg::visibility::VISIBLE) {
            this->focused_element_id = Element->get_id();
        }

        // Reset flags.
        Element->on_post_event(event);
    }

    this->buffer_render.fill(0);
    this->buffer_size = 0;

    // Call all events.
    for (ekg_abstract_element* &Element : this->buffer_update) {
        if (Element == nullptr || Element->is_dead()) {
            continue;
        }

        if (Element->get_id() == this->focused_element_id) {
            Element->on_pre_event(event);

            // Communicate for ekg environment the current focused element.
            this->focused_element_tag = Element->get_tag();
            this->focused_element_info_class = Element->info_class();
        }

        Element->on_event(event);

        if (Element->get_visibility_flag() == ekg::visibility::VISIBLE) {
            this->buffer_render[this->buffer_size++] = Element;
        }

        this->sync_scissor(Element);
    }

    if (this->should_swap_buffers) {
        this->swap_stack();
    }

    if (event.type == SDL_FINGERDOWN || event.type == SDL_FINGERUP) {
        if (this->task_refocus) {
            for (ekg_abstract_element* &Element : this->buffer_update) {
                if (Element == nullptr || Element->is_dead()) {
                    continue;
                }

                if (Element->is_hovered() &&
                        Element->get_visibility_flag() == ekg::visibility::VISIBLE) {
                    this->focused_element_id = Element->get_id();

                    // Communicate for ekg environment the current focused element.
                    this->focused_element_tag = Element->get_tag();
                    this->focused_element_info_class = Element->info_class();
                }
            }

            this->task_refocus = false;
        }

        if (this->focused_element_id == 0) {
            this->focused_element_tag = "NULL";
            this->focused_element_info_class = "NULL";
        } else {
            this->reorder_stack(this->focused_element_id);
        }
    }
}

void ekg_core::on_update(const float &delta_ticks) {
    if (this->should_swap_buffers) {
        this->swap_stack();
    }

    if (this->task_free) {
        this->free_stack();
        this->task_free = false;
    }

    if (this->task_refresh) {
        this->refresh_stack();
        this->task_refresh = false;
    }

    if (this->task_reorder) {
        this->reorder_stack(this->id_from_task);
        this->id_from_task = 0;
        this->task_reorder = false;
    }
}

void ekg_core::OnRender(const float &render_ticks) {
    for (ekg_abstract_element* &Element : this->buffer_render) {
        if (Element == nullptr) {
            continue;
        }

        Element->on_render(render_ticks);
    }
}

void ekg_core::init() {
    this->shader_manager.Init();
    this->font_renderer.Init();
    this->color_theme.Init();

    this->timing = new ekg_timing();
}

void ekg_core::quit() {
    for (ekg_abstract_element* &Element : this->buffer_update) {
        Element->kill();
        delete Element;
    }

    this->buffer_update.clear();
    ekg_log("Killed all alive elements.");

    this->shader_manager.Quit();
    this->font_renderer.Quit();
    this->color_theme.Quit();

    ekg_log("Shutdown complete.");
}

void ekg_core::reorder_stack(unsigned int element_id) {
    // Reset if is 0.
    if (element_id == 0) {
        return;
    }

    ekg_stack Current;
    ekg_stack Focused;

    for (ekg_abstract_element* &Element : this->buffer_update) {
        if (Element == nullptr) {
            continue;
        }

        // Ignore if contains at focused.
        if (Focused.Contains(Element->get_id())) {
            continue;
        }

        // Verify child in this element.
        ekg_stack Stack;
        Element->push_children_ids_to_stack(Stack);

        // If contains element id focused we set the stack value in focused.
        if (Stack.Contains(element_id)) {
            Focused = Stack;
        } else {
            // Else, we add in current stack.
            Current.Put(Element->get_id());
        }
    }

    this->buffer_render.fill(0);
    this->buffer_size = 0;
    this->buffer_copy.clear();

    // Put current.
    for (unsigned int IDs : Current.StackedIds) {
        auto* Element = (ekg_abstract_element*) this->get_element_by_id(IDs);

        if (Element == nullptr) {
            continue;
        }

        this->buffer_copy.push_back(Element);

        if (Element->get_visibility_flag() == ekg::visibility::VISIBLE) {
            this->buffer_render[this->buffer_size++] = Element;
        }
    }

    // Put the focused ids at top of list.
    for (unsigned int IDs : Focused.StackedIds) {
        auto* Element = (ekg_abstract_element*) this->get_element_by_id(IDs);

        if (Element == nullptr) {
            continue;
        }

        this->buffer_copy.push_back(Element);

        if (Element->get_visibility_flag() == ekg::visibility::VISIBLE) {
            this->buffer_render[this->buffer_size++] = Element;
        }
    }

    this->buffer_update = this->buffer_copy;
    this->buffer_copy.clear();
}

ekg_abstract_element* ekg_core::get_element_by_id(unsigned int Id) {
    for (ekg_abstract_element* &Element : this->buffer_new) {
        if (Element->get_id() == Id) {
            return Element;
        }
    }

    for (ekg_abstract_element* &Element : this->buffer_update) {
        if (Element->get_id() == Id) {
            return Element;
        }
    }

    return NULL;
}

unsigned int ekg_core::next_id() {
    this->previous_id_used++;
    return this->previous_id_used;
}

void ekg_core::sync_stack_scaled_metrics(float x, float y, float w, float h, const ekg_stack &stack) {
    for (unsigned int IDs : stack.StackedIds) {
        auto* Element = this->get_element_by_id(IDs);

        if (Element == nullptr) {
            continue;
        }

        Element->set_scaled(x, y, w, h);
        Element->sync_pos();

        auto PreviousVisibility = (ekg::visibility) Element->get_visibility_flag();
        bool Visible = Element->get_rect().CollideWithRect(x, y, w, h);

        if (PreviousVisibility == ekg::visibility::VISIBLE && !Visible) {
            Element->set_visibility_flag(ekg::visibility::LOW_PRIORITY);
        } else if (PreviousVisibility == ekg::LOW_PRIORITY && Visible) {
            Element->set_visibility_flag(ekg::visibility::VISIBLE);
        }

        if (Element->is_master()) {
            ekg_core::sync_stack_scaled_metrics(Element->get_x(), Element->get_y(),
                                                Element->get_width(),
                                                Element->get_height(), Element->GetChildren());
        }
    }
}

void ekg_core::sync_scissor(ekg_abstract_element* element) {
    element->set_scissor(element->get_x(), element->get_y(), element->get_width(),
                         element->get_height());

    if (!element->is_master()) {
        return;
    }

    int X = element->get_scissor_x();
    int Y = element->get_scissor_y();
    int W = element->get_scissor_w();
    int H = element->get_scissor_h();

    // if (x < (int) element->get_X()) {
    //     element->set_scissor((int) element->get_X(), element->get_scissor_y(), element->get_scissor_w(), element->get_scissor_h());
    // }

    // if (y < (int) element->get_y()) {
    //     element->set_scissor(element->get_scissor_x(), (int) element->get_y(), element->get_scissor_w(), element->get_scissor_h());
    // }

    // if (W < (int) element->get_width()) {
    //     element->set_scissor(element->get_scissor_x(), element->get_scissor_y(), (int) element->get_width(), element->get_scissor_h());
    // }

    // if (H < (int) element->get_height()) {
    //     element->set_scissor(element->get_scissor_x(), element->get_scissor_y(), element->get_scissor_w(), (int) element->get_height());
    //}

    for (unsigned int Ids : element->GetChildren().StackedIds) {
        auto* Elements = (ekg_abstract_element*) this->get_element_by_id(Ids);

        // if (x < (int) Elements->get_X()) {
        //     Elements->set_scissor((int) Elements->get_X(), Elements->get_scissor_y(), Elements->get_scissor_w(), Elements->get_scissor_h());
        // }
//
        // if (y < (int) element->get_y()) {
        //     Elements->set_scissor(Elements->get_scissor_x(), (int) Elements->get_y(), Elements->get_scissor_w(), Elements->get_scissor_h());
        // }
//
        // if (W < (int) Elements->get_width()) {
        //     Elements->set_scissor(Elements->get_scissor_x(), Elements->get_scissor_y(), (int) Elements->get_width(), Elements->get_scissor_h());
        // }
//
        // if (H < (int) Elements->get_height()) {
        //     Elements->set_scissor(Elements->get_scissor_x(), Elements->get_scissor_y(), Elements->get_scissor_w(), (int) Elements->get_height());
        // }
    }
}

int ekg_core::get_size_of_update_elements() {
    return this->buffer_update.size();
}

int ekg_core::get_size_of_render_elements() {
    return this->buffer_size;
}

unsigned int ekg_core::get_focused_element_id() {
    return this->focused_element_id;
}

std::string ekg_core::get_focused_element_tag() {
    return this->focused_element_tag;
}

std::string ekg_core::get_focused_element_info_class() {
    return this->focused_element_info_class;
}

void ekg_core::refresh_stack() {
    this->buffer_render.fill(0);
    this->buffer_size = 0;

    for (ekg_abstract_element* &Element : this->buffer_update) {
        if (Element == nullptr || Element->is_dead()) {
            continue;
        }

        if (Element->get_visibility_flag() == ekg::visibility::VISIBLE) {
            this->buffer_render[this->buffer_size++] = Element;
        }
    }
}

void ekg_core::free_stack() {
    this->buffer_render.fill(0);
    this->buffer_size = 0;
    this->buffer_copy.clear();

    for (int I = 0; I < this->buffer_update.size(); I++) {
        auto* Element = this->buffer_update.at(I);

        if (Element == nullptr) {
            continue;
        }

        if (Element->is_dead()) {
            delete Element;
            continue;
        }

        this->buffer_copy.push_back(Element);

        if (Element->get_visibility_flag() == ekg::visibility::VISIBLE) {
            this->buffer_render[this->buffer_size++] = Element;
        }
    }

    this->buffer_update = this->buffer_copy;
    this->buffer_copy.clear();
}


void ekg_core::swap_stack() {
    this->buffer_render.fill(0);
    this->buffer_size = 0;
    this->buffer_copy.clear();

    for (ekg_abstract_element* &Element : this->buffer_update) {
        if (Element == nullptr) {
            continue;
        }

        this->buffer_copy.push_back(Element);

        if (Element->get_visibility_flag() == ekg::visibility::VISIBLE) {
            this->buffer_render[this->buffer_size++] = Element;
        }
    }

    for (ekg_abstract_element* &Element : this->buffer_new) {
        if (Element == nullptr) {
            continue;
        }

        this->buffer_copy.push_back(Element);

        if (Element->get_visibility_flag() == ekg::visibility::VISIBLE) {
            this->buffer_render[this->buffer_size++] = Element;
        }
    }

    this->buffer_update = this->buffer_copy;
    this->buffer_new.clear();
    this->buffer_copy.clear();
    this->should_swap_buffers = false;
}

void ekg_core::add_task_to_queue(unsigned int task, unsigned int id) {
    if (EKG_FlagContains(task, ekg::task::REFRESH)) {
        this->task_refresh = true;
    }

    if (EKG_FlagContains(task, ekg::task::REORDER)) {
        this->task_reorder = true;
        this->id_from_task = id;
    }

    if (EKG_FlagContains(task, ekg::task::BLOCKED)) {
        this->task_blocked = true;
    }

    if (EKG_FlagContains(task, ekg::task::REFOCUS)) {
        this->task_refocus = true;
    }

    if (EKG_FlagContains(task, ekg::task::FREE)) {
        this->task_free = true;
    }
}

bool ekg_core::verify_task(unsigned int task) {
    switch (task) {
        case ekg::task::REORDER: {
            return this->task_reorder;
        }

        case ekg::task::REFOCUS: {
            return this->task_refocus;
        }

        case ekg::task::BLOCKED: {
            return this->task_blocked;
        }

        case ekg::task::REFRESH: {
            return this->task_refresh;
        }

        case ekg::task::FREE: {
            return this->task_free;
        }
    }

    return false;
}