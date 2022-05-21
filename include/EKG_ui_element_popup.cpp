#include "ekg_ui_element_popup.h"
#include "ekg.h"

void ekg_ui_element_popup::Insert(const std::vector<std::string> &ToAddList) {
    this->List.clear();

    for (const std::string& String : ToAddList) {
        if (String.empty()) {
            continue;
        }

        EKG_Data Component;

        Component.Name = String;
        Component.Tag = "1";

        this->List.push_back(Component);
    }
}

void ekg_ui_element_popup::Delete(const std::string &Pattern) {
    std::vector<EKG_Data> NewList;

    for (const EKG_Data &Components : this->List) {
        if (ekg_string_in(Pattern, Components.Name)) {
            continue;
        }

        NewList.push_back(Components);
    }

    bool ShouldSync = this->List.size() != NewList.size();
    this->List = NewList;

    if (ShouldSync) {
        this->sync_size();
    }
}

void ekg_ui_element_popup::Disable(const std::string &Pattern) {
    if (Pattern.empty()) {
        return;
    }

    bool ShouldSync;

    for (EKG_Data &Components : this->List) {
        if (ekg_string_in(Pattern, Components.Name)) {
            Components.Tag = "";
            ShouldSync = true;
        }
    }

    if (ShouldSync) {
        this->sync_size();
    }
}

void ekg_ui_element_popup::Enable(const std::string &Pattern) {
    if (Pattern.empty()) {
        return;
    }

    bool ShouldSync;

    for (EKG_Data &Components : this->List) {
        if (ekg_string_in(Pattern, Components.Name)) {
            Components.Tag = "1";
            ShouldSync = true;
        }
    }

    if (ShouldSync) {
        this->sync_size();
    }
}

void ekg_ui_element_popup::SetOffsetText(float OffsetText) {
    this->TextOffset = OffsetText;
}

float ekg_ui_element_popup::GetOffsetText() {
    return this->TextOffset;
}

void ekg_ui_element_popup::SetPressed(bool State) {
    this->Pressed = State;
}

bool ekg_ui_element_popup::IsPressed() {
    return this->Pressed;
}

void ekg_ui_element_popup::SetClicked(bool State) {
    this->Clicked = State;
}

bool ekg_ui_element_popup::IsClicked() {
    return this->Clicked;
}

void ekg_ui_element_popup::SetWidth(float Width) {
    if (this->rect.W != Width) {
        this->rect.W = Width;
        this->sync_size();
    }
}

std::vector<EKG_Data> &ekg_ui_element_popup::GetList() {
    return this->List;
}

std::string &ekg_ui_element_popup::GetFocused() {
    return this->Focused;
}

std::string &ekg_ui_element_popup::GetClicked() {
    return this->Selected;
}

void ekg_ui_element_popup::sync_size() {
    ekg_abstract_element::sync_size();
    this->MaximumHeight = 0.0F;

    for (EKG_Data &Components : this->List) {
        // It cost too many ticks (hardware) to get string height.
        Components.DataHeight = EKG_CORE->font_renderer.GetStringHeight(Components.Name) + (this->TextScale * 2);
        this->MaximumHeight += Components.DataHeight;
    }
}

void ekg_ui_element_popup::on_pre_event(SDL_Event Event) {
    // There is no problem in finger pos out of master bound.
    if (Event.type == SDL_FINGERDOWN || Event.type == SDL_FINGERMOTION || Event.type == SDL_FINGERUP) {
        float FX = Event.tfinger.x;
        float FY = Event.tfinger.y;

        ekg::scaled_finger_pos(FX, FY);
        this->hovered = this->rect.CollideWithPoint(FX, FY);
    }
}

void ekg_ui_element_popup::on_event(SDL_Event Event) {
    ekg_abstract_element::on_event(Event);

    switch (Event.type) {
        case SDL_FINGERUP: {
            float FX = Event.tfinger.x;
            float FY = Event.tfinger.y;

            ekg::scaled_finger_pos(FX, FY);
            EKG_Data Component = this->GetHoveredComponent(FX, FY);

            this->Selected = "";

            if (this->Pressed && !this->Focused.empty() && this->Focused == Component.Name) {
                this->Clicked = true;
                this->Selected = this->Focused;
            }

            bool FingerNotHoveredMasterOrCurrentFocusedIsNotPopup =
                    ekg::current_focused_type() != "popup";

            if (!this->PopupMaster && this->get_master_id() != 0 && FingerNotHoveredMasterOrCurrentFocusedIsNotPopup) {
                auto* Element = EKG_CORE->get_element_by_id(this->get_master_id());
                FingerNotHoveredMasterOrCurrentFocusedIsNotPopup = Element != NULL && !Element->collide_with_pos(
                        FX, FY);
            }

            if (FingerNotHoveredMasterOrCurrentFocusedIsNotPopup || (!this->Focused.empty() && !this->hovered) || (!this->Selected.empty() && Component.Id == 0) || (EKG_CORE->get_element_by_id(
                    Component.Id) == NULL && Component.Id != 0)) {
                this->kill();
            }

            if (!this->Focused.empty() && (Component.Name == this->Focused ||
                    ekg::current_focused_type() == "popup") && Component.Id == 0) {
                std::string Path = this->Focused;
                this->GetPath(Path);

                // We use smart pointers instead directly pointers.
                std::shared_ptr<void> P(new std::string(Path));

                // Send popup event using ekg events processor.
                ekg::event::dispatch(ekg::event::POPUP, static_cast<void *>(P.get()), nullptr);
            }

            this->Pressed = false;
            this->Focused = "";

            break;
        }

        case SDL_FINGERDOWN: {
            float FX = Event.tfinger.x;
            float FY = Event.tfinger.y;

            ekg::scaled_finger_pos(FX, FY);

            if (this->hovered) {
                float Y = this->get_y();
                float X, W, H;

                EKG_Data Component;
                Component.Name = "";

                for (const EKG_Data &Components : this->List) {
                    X = this->get_x();

                    W = X + this->get_width();
                    H = Y + Components.DataHeight;

                    if (Components.Tag == "1" && FX > X && FX < W && FY > Y && FY < H) {
                        Component = Components;
                        break;
                    }

                    Y += Components.DataHeight;
                }

                this->Focused = Component.Name;

                if (!this->Focused.empty()) {
                    this->Pressed = true;

                    if (Component.Id != 0) {
                        auto* ElementOut = (ekg_ui_element_popup*) EKG_CORE->get_element_by_id(
                                Component.Id);

                        if (ElementOut != NULL) {
                            ElementOut->set_visibility_flag(ekg::visibility::VISIBLE);
                            ElementOut->Place(this->get_x() + this->get_width(), Y, this->get_x());

                            this->Activy = Component.Name;

                            for (const EKG_Data &Components : this->List) {
                                auto *ElementIn = EKG_CORE->get_element_by_id(Components.Id);

                                if (ElementIn != NULL && ElementOut->get_id() != Components.Id) {
                                    ElementIn->set_visibility_flag(ekg::visibility::INVISIBLE);
                                }
                            }
                        }
                    }
                }
            }

            bool FingerNotHoveredMasterOrCurrentFocusedIsNotPopup =
                    ekg::current_focused_type() != "popup";

            if (!this->PopupMaster && this->get_master_id() != 0 && FingerNotHoveredMasterOrCurrentFocusedIsNotPopup) {
                auto* Element = EKG_CORE->get_element_by_id(this->get_master_id());
                FingerNotHoveredMasterOrCurrentFocusedIsNotPopup = Element != NULL && !Element->collide_with_pos(
                        FX, FY);
            }

            if (FingerNotHoveredMasterOrCurrentFocusedIsNotPopup) {
                this->kill();
                this->Focused = "";
                this->Pressed = false;
            }

            break;
        }
    }
}

void ekg_ui_element_popup::on_post_event(SDL_Event Event) {
    ekg_abstract_element::on_post_event(Event);
}

void ekg_ui_element_popup::on_update(const float &DeltaTicks) {
    ekg_abstract_element::on_update(DeltaTicks);
}

void ekg_ui_element_popup::on_render(const float &PartialTicks) {
    ekg_abstract_element::on_render(PartialTicks);

    // render only if is open.
    if (this->Show) {
        this->rect.H = EKG_AnimationSmooth(100, SDL_GetTicks() - this->LastTicks) * this->MaximumHeight;
        this->SmoothPressed.NextFactory = !this->Focused.empty() ? EKG_CORE->color_theme.WidgetActivy[3] : 0;
        this->SmoothPressed.Update(PartialTicks);

        // Background container.
        EKG_Color Color(EKG_CORE->color_theme.ContainerBackground);
        ekg_draw_filled_rect(this->rect, Color);

        // Now reset color to activy in theme.
        Color.Set(EKG_CORE->color_theme.WidgetActivy, SmoothPressed.Factory);

        // We need calc a runtime height to render property the positions.
        float FullHeight = this->get_y();

        ekg_scissor((int) this->rect.X, (int) this->rect.Y, (int) this->rect.W, (int) this->rect.H);

        // Iterate all component data.
        for (const EKG_Data &Components : this->List) {
            // Background when is focused.
            if (Components.Name == this->Focused) {
                ekg_draw_filled_shape(this->get_x(), FullHeight, this->rect.W,
                                      Components.DataHeight, Color);
            }

            // Draw name of component.
            EKG_CORE->font_renderer.DrawString(Components.Name,
                                              this->get_x() + 2.0F + this->TextOffset, FullHeight + this->TextScale, Components.Tag != "1" ? EKG_CORE->color_theme.StringFadeColor : EKG_CORE->color_theme.StringColor);

            // update the height to the next element be rendered property.
            FullHeight += Components.DataHeight;
        }

        ekg_end_scissor();

        // Outline (YOU CAN NOT DISABLED IT SORRY).
        ekg_draw_outline_rect(this->rect, 1.5f, EKG_CORE->color_theme.StringColor);
    }
}

EKG_Data ekg_ui_element_popup::GetHoveredComponent(float FX, float FY) {
    float Y = this->get_y();
    float X, W, H;

    for (const EKG_Data &Components : this->List) {
        X = this->get_x();

        W = X + this->get_width();
        H = Y + Components.DataHeight;

        if (Components.Tag == "1" && FX > X && FX < W && FY > Y && FY < H) {
            return Components;
        }

        Y += Components.DataHeight;
    }

    EKG_Data Component;
    Component.Name = "";

    return Component;
}

void ekg_ui_element_popup::SetScale(float Scale) {
    if (this->TextScale != Scale) {
        this->TextScale = Scale;
        this->sync_size();
    }
}

float ekg_ui_element_popup::GetScale() {
    return this->TextScale;
}

void ekg_ui_element_popup::Place(ekg_ui_element_popup* Popup) {
    if (Popup == NULL || this->children_stack.Contains(Popup->get_id()) || Popup->get_id() ==
                                                                           this->get_id()) {
        return;
    }

    bool Contains;

    for (EKG_Data &Components : this->List) {
        if (Components.Name == Popup->get_tag()) {
            Components.Id = Popup->get_id();
            Contains = true;
        }
    }

    if (Contains) {
        this->children_stack.Put(Popup->get_id());

        Popup->set_master_id(this->get_id());
        Popup->set_visibility_flag(ekg::visibility::INVISIBLE);
    }
}

void ekg_ui_element_popup::place(float X, float Y) {
    float FactoredX = X < 0 ? 0 : X;
    float FactoredY = Y < 0 ? 0 : Y;

    if (FactoredX + this->get_width() > ekg::screen_width) {
        FactoredX = ekg::screen_width - this->get_width();
    }

    if (FactoredY + this->MaximumHeight > ekg::screen_height) {
        FactoredY = ekg::screen_height - this->MaximumHeight;
    }

    ekg_abstract_element::place(FactoredX, FactoredY);
}

std::string ekg_ui_element_popup::info_class() {
    ekg_abstract_element::info_class();
    return "popup";
}

void ekg_ui_element_popup::Place(float X, float Y, float BoundingX) {
    float FactoredX = X < 0 ? 0 : X;
    float FactoredY = Y < 0 ? 0 : Y;

    if (FactoredX + this->get_width() > ekg::screen_width) {
        FactoredX = BoundingX - this->get_width();
    }

    if (FactoredY + this->MaximumHeight > ekg::screen_height) {
        FactoredY = ekg::screen_height - this->MaximumHeight;
    }

    this->rect.X = (FactoredX);
    this->rect.Y = FactoredY;
}

bool ekg_ui_element_popup::IsUpdate(float FX, float FY) {
    return ekg::current_focused_type() == "popup" && ekg::current_focused_id() != 0 && (ekg::current_focused_id() == this->master_id ||
            ekg::current_focused_id() == this->get_id() || this->children_stack.Contains(
            ekg::current_focused_id()));
}

void ekg_ui_element_popup::kill() {
    if (this->dead) {
        return;
    }

    ekg_abstract_element::kill();

    for (unsigned int IDs : this->children_stack.StackedIds) {
        auto* Element = EKG_CORE->get_element_by_id(IDs);

        if (Element != NULL) {
            Element->kill();
        }
    }

    this->children_stack.StackedIds.clear();

    if (this->get_master_id() != 0 && this->PopupMaster) {
        auto* Element = EKG_CORE->get_element_by_id(this->get_master_id());

        if (Element != NULL) {
            Element->kill();
        }
    }
}

void ekg_ui_element_popup::GetPath(std::string &PreviousPath) {
    PreviousPath = this->get_tag() + "|" + PreviousPath;

    if (this->get_master_id() != 0) {
        auto* Element = (ekg_ui_element_popup*) ekg::find(this->get_master_id());

        if (Element != NULL) {
            Element->GetPath(PreviousPath);
        }
    }
}

void ekg_ui_element_popup::set_master_id(unsigned int ElementMasterId) {
    ekg_abstract_element::set_master_id(ElementMasterId);

    // We can use this as flag to prevent visual glitch.
    this->PopupMaster = true;

    if (this->master_id != 0) {
        auto* Element = EKG_CORE->get_element_by_id(ElementMasterId);
        this->PopupMaster = Element != NULL && Element->info_class() == "popup";
    }
}

float ekg_ui_element_popup::GetMaximumWidth() {
    return this->MaximumWidth;
}

float ekg_ui_element_popup::GetMaximumHeight() {
    return this->MaximumHeight;
}

void ekg_ui_element_popup::set_visibility_flag(unsigned int VisibilityFlag) {
    if (VisibilityFlag == ekg::visibility::INVISIBLE) {
        this->rect.H = 0;
    }

    bool Flag = (VisibilityFlag == ekg::visibility::VISIBLE);

    if (this->Show != Flag) {
        this->LastTicks = SDL_GetTicks();
        ekg_abstract_element::set_visibility_flag(ekg::visibility::INVISIBLE);
        this->Show = Flag;
    }

    this->visible = VisibilityFlag;
}
