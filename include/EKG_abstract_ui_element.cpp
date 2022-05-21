#include "ekg_abstract_ui_element.h"
#include "ekg.h"

void ekg_abstract_element::set_tag(const std::string &new_tag) {
    if (this->tag != new_tag) {
        this->tag = new_tag;
        this->sync_size();
    }
}

std::string ekg_abstract_element::get_tag() {
    return this->tag;
}

void ekg_abstract_element::set_id(unsigned int element_id) {
    this->id = element_id;
}

unsigned int ekg_abstract_element::get_id() {
    return this->id;
}

void ekg_abstract_element::set_master_id(unsigned int element_id) {
    this->master_id = element_id;
}

unsigned int ekg_abstract_element::get_master_id() {
    return this->master_id;
}

void ekg_abstract_element::set_children_stack(const ekg_stack &the_stack) {
    this->children_stack = the_stack;
}

ekg_stack ekg_abstract_element::GetChildren() {
    return this->children_stack;
}

void ekg_abstract_element::set_rect(const ekg_rect &rectangle) {
    this->rect = rectangle;
}

ekg_rect ekg_abstract_element::get_rect() {
    return this->rect;
}

float ekg_abstract_element::get_x() {
    return this->rect.X;
}

float ekg_abstract_element::get_y() {
    return this->rect.Y;
}

float ekg_abstract_element::get_width() {
    return this->rect.W;
}

float ekg_abstract_element::get_height() {
    return this->rect.H;
}

void ekg_abstract_element::set_scaled(float x, float y, float width, float height) {
    this->scaled_x = x;
    this->scaled_y = y;
    this->scaled_width = width;
    this->scale_height = height;
}

void ekg_abstract_element::set_hover_state(bool state) {
    this->hovered = state;
}

bool ekg_abstract_element::is_hovered() {
    return this->hovered;
}

void ekg_abstract_element::set_visibility_flag(unsigned int flag) {
    if (this->visible != flag) {
        this->visible = flag;

        if (this->visible != ekg::visibility::VISIBLE_ONCE && this->visible != ekg::visibility::INVISIBLE_ONCE && !this->children_stack.StackedIds.empty()) {
            for (unsigned int Ids : this->children_stack.StackedIds) {
                auto* Element = EKG_CORE->get_element_by_id(Ids);

                if (Element != NULL) {
                    Element->set_visibility_flag(flag);
                }
            }
        }

        this->visible = this->visible == ekg::visibility::VISIBLE_ONCE ? ekg::visibility::VISIBLE : (this->visible == ekg::visibility::INVISIBLE_ONCE ? ekg::visibility::INVISIBLE : this->visible);
    }
}

unsigned int ekg_abstract_element::get_visibility_flag() {
    return this->visible;
}

void ekg_abstract_element::set_disable_state(bool state) {
    this->disabled = state;
}

bool ekg_abstract_element::is_disabled() {
    return this->disabled;
}

void ekg_abstract_element::set_dead_state(bool state) {
    this->dead = state;
}

bool ekg_abstract_element::is_dead() {
    return this->dead;
}

void ekg_abstract_element::kill() {
    this->dead = true;
    this->visible = ekg::visibility::EXISTED;

    if (this->master_id != 0) {
        auto* Element = EKG_CORE->get_element_by_id(this->master_id);

        if (Element != nullptr) {
            Element->remove(this->get_id());
        }
    }

    // Set all children master ID to 0.
    for (unsigned int IDs : this->children_stack.StackedIds) {
        auto* Element = EKG_CORE->get_element_by_id(IDs);

        if (Element != nullptr) {
            Element->set_master_id(0);
            Element->on_master_killed(this->get_id());
            Element->kill();
        }
    }

    if (this->info_class() == "frame-tab") {
        ekg_log("oi");
    }

    ekg::task(ekg::task::FREE);
}

void ekg_abstract_element::on_parent_killed(unsigned int element_id) {

}

void ekg_abstract_element::on_master_killed(unsigned int element_id) {

}

void ekg_abstract_element::on_pre_event(SDL_Event Event) {
    if (Event.type == SDL_FINGERDOWN || Event.type == SDL_FINGERMOTION || Event.type == SDL_FINGERUP) {
        float FX = Event.tfinger.x;
        float FY = Event.tfinger.y;

        ekg::scaled_finger_pos(FX, FY);
        this->hovered = this->collide_with_pos(FX, FY);
    }
}

void ekg_abstract_element::on_event(SDL_Event Event) {

}

void ekg_abstract_element::on_post_event(SDL_Event Event) {
    this->hovered = false;
}

void ekg_abstract_element::on_update(const float &DeltaTicks) {

}

void ekg_abstract_element::on_render(const float &PartialTicks) {

}

void ekg_abstract_element::scissor_sync() {

}

void ekg_abstract_element::push_children_ids_to_stack(ekg_stack &stack) {
    if (stack.Contains(this->id)) {
        return;
    }

    stack.Put(this->id);

    for (unsigned int Ids : this->children_stack.StackedIds) {
        auto* Element = (ekg_abstract_element*) EKG_CORE->get_element_by_id(Ids);

        if (Element == NULL) {
            continue;
        }

        Element->push_children_ids_to_stack(stack);
    }
}

bool ekg_abstract_element::is_master() {
    return !this->children_stack.StackedIds.empty();
}

void ekg_abstract_element::set_scissor(int x, int y, int w, int h) {
    this->scissor_x = x;
    this->scissor_y = y;
    this->scissor_w = w;
    this->scissor_h = h;
}

int ekg_abstract_element::get_scissor_x() {
    return this->scissor_x;
}

int ekg_abstract_element::get_scissor_y() {
    return this->scissor_y;
}

int ekg_abstract_element::get_scissor_w() {
    return this->scissor_w;
}

int ekg_abstract_element::get_scissor_h() {
    return this->scissor_h;
}

bool ekg_abstract_element::collide_with_pos(float x, float y) {
    bool Flag = this->rect.CollideWithPoint(x, y);

    if (Flag && this->get_master_id() != 0) {
        auto* Element = EKG_CORE->get_element_by_id(this->get_master_id());

        if (Element != NULL && !Element->collide_with_pos(x, y)) {
            return false;
        }
    }

    return Flag;
}

void ekg_abstract_element::place(float x, float y) {
    if (this->get_master_id() == 0) {
        this->rect.X = x;
        this->rect.Y = y;
    } else {
        this->sync_x = x;
        this->sync_y = y;

        this->rect.X = this->scaled_x + this->sync_x;
        this->rect.Y = this->scaled_y + this->sync_y;
    }
}

void ekg_abstract_element::sync_pos() {
    this->place(this->sync_x, this->sync_y);
}

void ekg_abstract_element::sync_size() {

}

ekg_abstract_element::ekg_abstract_element() {

}

ekg_abstract_element::~ekg_abstract_element() {

}

std::string ekg_abstract_element::info_class() {
    return "abstract";
}

void ekg_abstract_element::remove(unsigned int element_id) {
    if (this->children_stack.Contains(element_id)) {
        this->children_stack.Rem(element_id);
        this->on_parent_killed(element_id);
    }
}