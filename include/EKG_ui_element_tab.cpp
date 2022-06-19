#include "ekg_ui_element_tab.h"
#include "ekg.h"

void ekg_ui_element_tab::set_offset_text(float offset) {
    this->text_offset = offset;
}

float ekg_ui_element_tab::get_offset_text() {
    return this->text_offset;
}

void ekg_ui_element_tab::set_scale(float scale) {
    if (this->text_scale != scale) {
        this->text_scale = scale;
        this->sync_size();
    }
}

float ekg_ui_element_tab::get_scale() {
    return this->text_scale;
}

std::vector<ekg_data> &ekg_ui_element_tab::get_list() {
    return this->list;
}

std::string ekg_ui_element_tab::info_class() {
    return "frame-tab";
}

void ekg_ui_element_tab::sync_size() {
    ekg_ui_element_frame::sync_size();

    this->min_width = 0.0F;

    for (ekg_data &components : this->list) {
        float string_name_width = EKG_CORE->font_renderer.GetStringWidth(components.name);

        components.data_width = (this->button_size * 2) + string_name_width;
        components.data_height = (this->text_scale * 2) + EKG_CORE->font_renderer.GetStringHeight(components.name);

        if (components.data_width > this->min_width) {
            this->min_width = components.data_width;
        }

        if (components.data_height > this->min_height) {
            this->min_height = components.data_height;
        }

        components.data_x = (components.data_width / 2.0F) - (string_name_width / 2.0F);
    }

    float div_min_width = this->min_width / 2.0f;
    float div_border = this->border_offset / 2.0F;

    // We do not want small buttons, after sync_stack_scaled_metrics the most largest component,
    // we set the components less than largest size divided by 2.
    for (ekg_data &Components : this->list) {
        if (Components.data_width < div_min_width) {
            Components.data_width = div_min_width;

            float StringNameWidth = EKG_CORE->font_renderer.GetStringWidth(Components.name);
            Components.data_x = (Components.data_width / 2.0F) - (StringNameWidth / 2.0F);
        }

        Components.data_height = this->min_height - div_border;
    }
}

void ekg_ui_element_tab::on_pre_event(SDL_Event event) {
    ekg_ui_element_frame::on_pre_event(event);
}

void ekg_ui_element_tab::on_event(SDL_Event event) {
    ekg_ui_element_frame::on_event(event);

    switch (event.type) {
        case SDL_FINGERUP: {
            this->activy = "";
            break;
        }

        case SDL_FINGERMOTION: {
            this->sync_layout();
            break;
        }

        case SDL_FINGERDOWN: {
            if (!this->hovered) {
                return;
            }

            float FX = event.tfinger.x;
            float FY = event.tfinger.y;

            ekg::scaled_finger_pos(FX, FY);
            float YDiff = this->dock_tab == ekg::dock::TOP ? this->get_y() : this->get_y() +
                                                                             this->get_height() - this->get_min_height();

            if (!(FX > this->get_x() && FX < this->get_x() + this->get_size() && FY > YDiff && FY < YDiff + this->min_height)) {
                return;
            }

            ekg_data Component = this->get_component_by_pos(FX, FY);
            this->open(Component);
            this->sync_layout();
            this->activy = Component.name;

            break;
        }
    }
}

void ekg_ui_element_tab::on_post_event(SDL_Event event) {
    ekg_ui_element_frame::on_post_event(event);
}

void ekg_ui_element_tab::on_update(const float &delta_ticks) {
    ekg_ui_element_frame::on_update(delta_ticks);
}

void ekg_ui_element_tab::on_render(const float &render_ticks) {
    this->smooth_activy.NextFactory = this->activy.empty() ? 0 : EKG_CORE->color_theme.FrameActivy[3];
    this->smooth_activy.Update(render_ticks);

    // Background.
    ekg_color Color(EKG_CORE->color_theme.ContainerBackground);
    ekg_draw_filled_rect(this->rect, Color);

    this->size = 0;
    float X = this->get_x(), Y = this->get_y();

    switch (this->dock_tab) {
        case ekg::dock::BOTTOM: {
            Y = this->get_y() + this->rect.w - this->min_height;
            break;
        }
    }

    for (ekg_data &Component : this->list) {
        if (Component.name == this->activy) {
            Color.Set(EKG_CORE->color_theme.ContainerActivy, this->smooth_activy.Factory);
            ekg_draw_filled_shape(X + this->size, Y, Component.data_width, Component.data_height,
                                  Color);
        }

        if (Component.name == this->focused) {
            float UniqueHeight = Component.data_height / 10;
            float UniqueY = Y + Component.data_height - UniqueHeight - (1);

            ekg_draw_outline_shape(X + this->size, UniqueY, Component.data_width, 1, 1.5f,
                                   EKG_CORE->color_theme.StringColor);
        }

        EKG_CORE->font_renderer.DrawString(Component.name, X + this->size + Component.data_x, Y + this->text_scale, Component.tag.empty() ? EKG_CORE->color_theme.StringFadeColor : EKG_CORE->color_theme.StringColor);
        this->size += Component.data_width;
    }
}

void ekg_ui_element_tab::set_button_size(float size_val) {
    this->button_size = size_val;
}

float ekg_ui_element_tab::get_button_size() {
    return this->button_size;
}

void ekg_ui_element_tab::remove(const std::string &pattern) {
    if (pattern.empty()) {
        return;
    }

    std::vector<ekg_data> NewList;
    bool Flag;

    for (const ekg_data &Components : this->list) {
        if (ekg_string_in(pattern, Components.name)) {
            Flag = this->focused == Components.name;
            continue;
        }

        NewList.push_back(Components);
    }

    bool ShouldSync = this->list.size() != NewList.size();
    this->list = NewList;

    if (Flag) {
        this->sort_close_component();
    }

    if (ShouldSync) {
        this->sync_size();
    }
}

void ekg_ui_element_tab::disable(const std::string &pattern) {
    if (pattern.empty()) {
        return;
    }

    bool ShouldSort;

    for (ekg_data &Components : this->list) {
        if (ekg_string_in(pattern, Components.name)) {
            Components.tag = "";
            ShouldSort = this->focused == Components.name;
        }
    }

    if (ShouldSort) {
        this->sort_close_component();
        this->sync_size();
    }
}

void ekg_ui_element_tab::enable(const std::string &pattern) {
    if (pattern.empty()) {
        return;
    }

    bool ShouldSync;

    for (ekg_data &Components : this->list) {
        if (ekg_string_in(pattern, Components.name)) {
            Components.tag = "1";
            ShouldSync = true;
        }
    }

    if (ShouldSync) {
        this->sync_size();
    }
}

void ekg_ui_element_tab::tab_side(unsigned int dock) {
    if (dock == ekg::dock::TOP || dock == ekg::dock::BOTTOM) {
        this->dock_tab = dock;
    }
}

ekg_data ekg_ui_element_tab::get_component_by_pos(float x, float y) {
    float X = this->get_x(), Y = this->get_y();

    switch (this->dock_tab) {
        case ekg::dock::BOTTOM: {
            Y = this->rect.y + this->rect.h - this->min_height;
            break;
        }
    }

    ekg_data Component;

    for (ekg_data &Components : this->list) {
        if (!Components.tag.empty() && x > X && x < X + Components.data_width && y > Y && y < Y + Components.data_height) {
            Component = Components;
            break;
        }

        X += Components.data_width;
    }

    return Component;
}

void ekg_ui_element_tab::place(ekg_ui_element_frame *frame) {
    if (frame == nullptr || this->children_stack.Contains(frame->get_id()) || frame->get_id() ==
                                                                              this->get_id()) {
        return;
    }

    ekg_data Component;

    Component.name = frame->get_tag();
    Component.tag = "0";
    Component.Id = frame->get_id();

    this->children_stack.Put(frame->get_id());
    this->list.push_back(Component);

    frame->set_master_id(this->get_id());
    frame->draggable(NULL);
    frame->resizable(NULL);
    this->sync_size();

    frame->set_scaled(this->get_x(), this->get_y(), this->get_width(), this->get_height());
    frame->place(0, this->dock_tab == ekg::dock::TOP ? this->min_height : 0);
    frame->set_height(this->get_height() - this->min_height);
    frame->sync_parents_metric();

    if (this->focused.empty()) {
        this->focused = frame->get_tag();
        ekg::task(ekg::task::REORDER, frame->get_id());
    }

    this->open(this->focused);
    this->sync_layout();

    ekg::task(ekg::task::REFRESH);
}

void ekg_ui_element_tab::place(float x, float y) {
    ekg_ui_element_frame::place(x, y);
}

float ekg_ui_element_tab::get_size() {
    return this->size;
}

void ekg_ui_element_tab::set_border_offset(float offset) {
    this->border_offset = offset < 0.0f ? 5.0f : offset;
}

float ekg_ui_element_tab::get_border_offset() {
    return this->border_offset;
}

void ekg_ui_element_tab::open(const std::string &name) {
    ekg_data Component;

    for (ekg_data &Components : this->list) {
        if (Components.name == name) {
            Component = Components;
            break;
        }
    }

    this->open(Component);
}

void ekg_ui_element_tab::open(ekg_data &component) {
    if (!component.name.empty()) {
        this->dragging = false;
        this->resizing = false;

        for (ekg_data &Components : this->list) {
            auto* Element = EKG_CORE->get_element_by_id(Components.Id);

            if (Element != nullptr && !Element->is_dead()) {
                Element->set_visibility_flag(
                        Element->get_tag() == component.name ? ekg::visibility::VISIBLE
                                                             : ekg::visibility::INVISIBLE);
            }
        }

        this->focused = component.name;
        ekg::task(ekg::task::BLOCKED);
    }
}

void ekg_ui_element_tab::sync_layout() {
    for (unsigned int IDs : this->children_stack.StackedIds) {
        auto* Element = (ekg_ui_element_frame*) EKG_CORE->get_element_by_id(IDs);

        if (Element != nullptr && !Element->is_dead() && Element->get_tag() == this->focused) {
            Element->set_size(this->get_width() - this->border_offset * 2,
                              this->get_height() - this->min_height - this->border_offset);

            switch (this->dock_tab) {
                case ekg::dock::TOP: {
                    Element->place(this->get_border_offset(), this->min_height);
                    break;
                }

                case ekg::dock::BOTTOM: {
                    Element->place(this->get_border_offset(), 0);
                    break;
                }
            }
        }
    }
}

void ekg_ui_element_tab::on_parent_killed(unsigned int element_id) {
    auto* Element = EKG_CORE->get_element_by_id(element_id);

    if (Element != nullptr) {
        this->disable(Element->get_tag());
        this->sort_close_component();
        this->open(this->focused);
    }
}

void ekg_ui_element_tab::sort_close_component() {
    if (this->list.empty()) {
        this->focused = "";
        return;
    }

    int CurrentIndexPos = 0;

    if (this->list.size() != 1) {
        int ConcurrentIndexPos = -1;
        CurrentIndexPos = -1;
        bool Phase = true;

        for (int I = 0; I < this->list.size(); I++) {
            Phase = this->list.at(I).name == this->focused;

            if (Phase) {
                ConcurrentIndexPos = I;
                break;
            }
        }

        if (Phase) {
            int LeftIndexFound = 0, RightIndexFound = 0;
            bool Direction;

            for (int I = 0; I < this->list.size(); I++) {
                if (!Direction) {
                    int Factor = (ConcurrentIndexPos - I);
                    LeftIndexFound = this->list.at(Factor == -1 ? 0 : Factor).tag.empty() ? I : LeftIndexFound;
                } else {
                    RightIndexFound = this->list.at(I).tag.empty() ? I : RightIndexFound;
                }

                if (LeftIndexFound && Direction) {
                    Direction = true;
                }
            }


            if (LeftIndexFound != -1) {
                CurrentIndexPos = LeftIndexFound;
            } else if (RightIndexFound != -1) {
                CurrentIndexPos = RightIndexFound;
            }
        }
    }

    if (CurrentIndexPos == -1) {
        this->focused = "";
        return;
    }

    ekg_data Component = this->list.at(CurrentIndexPos);
    this->focused = Component.tag.empty() ? "" : this->list.at(CurrentIndexPos).name;
}
