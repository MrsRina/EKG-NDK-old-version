#include "ekg_ui_element_tab.h"
#include "ekg.h"

void ekg_ui_element_tab::SetOffsetText(float OffsetText) {
    this->TextOffset = OffsetText;
}

float ekg_ui_element_tab::GetOffsetText() {
    return this->TextOffset;
}

void ekg_ui_element_tab::SetScale(float Scale) {
    if (this->TextScale != Scale) {
        this->TextScale = Scale;
        this->sync_size();
    }
}

float ekg_ui_element_tab::GetScale() {
    return this->TextScale;
}

std::vector<EKG_Data> &ekg_ui_element_tab::GetList() {
    return this->List;
}

std::string ekg_ui_element_tab::info_class() {
    return "frame-tab";
}

void ekg_ui_element_tab::sync_size() {
    ekg_ui_element_frame::sync_size();
    std::vector<EKG_Data> NewList;

    this->MinimumWidth = 0.0F;

    for (EKG_Data &Components : this->List) {
        float StringNameWidth = EKG_CORE->font_renderer.GetStringWidth(Components.Name);

        Components.DataWidth = (this->ButtonSize * 2) + StringNameWidth;
        Components.DataHeight = (this->TextScale * 2) + EKG_CORE->font_renderer.GetStringHeight(Components.Name);

        if (Components.DataWidth > this->MinimumWidth) {
            this->MinimumWidth = Components.DataWidth;
        }

        if (Components.DataHeight > this->MinimumHeight) {
            this->MinimumHeight = Components.DataHeight;
        }

        Components.DataX = (Components.DataWidth / 2.0F) - (StringNameWidth / 2.0F);
    }

    float DivMinWidth = this->MinimumWidth / 2.0f;
    float DivBorder = this->BorderOffset / 2.0F;

    // We do not want small buttons, after sync_stack_scaled_metrics the most largest component,
    // we set the components less than largest size divided by 2.
    for (EKG_Data &Components : this->List) {
        if (Components.DataWidth < DivMinWidth) {
            Components.DataWidth = DivMinWidth;

            float StringNameWidth = EKG_CORE->font_renderer.GetStringWidth(Components.Name);
            Components.DataX = (Components.DataWidth / 2.0F) - (StringNameWidth / 2.0F);
        }

        Components.DataHeight = this->MinimumHeight - DivBorder;
    }
}

void ekg_ui_element_tab::on_pre_event(SDL_Event Event) {
    ekg_ui_element_frame::on_pre_event(Event);
}

void ekg_ui_element_tab::on_event(SDL_Event Event) {
    ekg_ui_element_frame::on_event(Event);

    switch (Event.type) {
        case SDL_FINGERUP: {
            this->Activy = "";
            break;
        }

        case SDL_FINGERMOTION: {
            this->SyncLayout();
            break;
        }

        case SDL_FINGERDOWN: {
            if (!this->hovered) {
                return;
            }

            float FX = Event.tfinger.x;
            float FY = Event.tfinger.y;

            ekg::scaled_finger_pos(FX, FY);
            float YDiff = this->DockTab == ekg::dock::TOP ? this->get_y() : this->get_y() +
                    this->get_height() - this->get_min_height();

            if (!(FX > this->get_x() && FX < this->get_x() + this->GetSize() && FY > YDiff && FY < YDiff + this->MinimumHeight)) {
                return;
            }

            EKG_Data Component = this->GetComponentHovered(FX, FY);
            this->Open(Component);
            this->SyncLayout();
            this->Activy = Component.Name;

            break;
        }
    }
}

void ekg_ui_element_tab::on_post_event(SDL_Event Event) {
    ekg_ui_element_frame::on_post_event(Event);
}

void ekg_ui_element_tab::on_update(const float &DeltaTicks) {
    ekg_ui_element_frame::on_update(DeltaTicks);
}

void ekg_ui_element_tab::on_render(const float &PartialTicks) {
    this->SmoothActivy.NextFactory = this->Activy.empty() ? 0 : EKG_CORE->color_theme.FrameActivy[3];
    this->SmoothActivy.Update(PartialTicks);

    // Background.
    EKG_Color Color(EKG_CORE->color_theme.ContainerBackground);
    ekg_draw_filled_rect(this->rect, Color);

    this->Size = 0;
    float X = this->get_x(), Y = this->get_y();

    switch (this->DockTab) {
        case ekg::dock::BOTTOM: {
            Y = this->get_y() + this->rect.W - this->MinimumHeight;
            break;
        }
    }

    for (EKG_Data &Component : this->List) {
        if (Component.Name == this->Activy) {
            Color.Set(EKG_CORE->color_theme.ContainerActivy, this->SmoothActivy.Factory);
            ekg_draw_filled_shape(X + this->Size, Y, Component.DataWidth, Component.DataHeight,
                                  Color);
        }

        if (Component.Name == this->Focused) {
            float UniqueHeight = Component.DataHeight / 10;
            float UniqueY = Y + Component.DataHeight - UniqueHeight - (1);

            ekg_draw_outline_shape(X + this->Size, UniqueY, Component.DataWidth, 1, 1.5f,
                                   EKG_CORE->color_theme.StringColor);
        }

        EKG_CORE->font_renderer.DrawString(Component.Name, X + this->Size + Component.DataX, Y + this->TextScale, Component.Tag.empty() ? EKG_CORE->color_theme.StringFadeColor : EKG_CORE->color_theme.StringColor);
        this->Size += Component.DataWidth;
    }
}

void ekg_ui_element_tab::SetButtonSize(float Amount) {
    this->ButtonSize = Amount;
}

float ekg_ui_element_tab::GetButtonSize() {
    return this->ButtonSize;
}

void ekg_ui_element_tab::Delete(const std::string &Pattern) {
    if (Pattern.empty()) {
        return;
    }

    std::vector<EKG_Data> NewList;
    bool Flag;

    for (const EKG_Data &Components : this->List) {
        if (ekg_string_in(Pattern, Components.Name)) {
            Flag = this->Focused == Components.Name;
            continue;
        }

        NewList.push_back(Components);
    }

    bool ShouldSync = this->List.size() != NewList.size();
    this->List = NewList;

    if (Flag) {
        this->SortCloseComponent();
    }

    if (ShouldSync) {
        this->sync_size();
    }
}

void ekg_ui_element_tab::Disable(const std::string &Pattern) {
    if (Pattern.empty()) {
        return;
    }

    bool ShouldSort;

    for (EKG_Data &Components : this->List) {
        if (ekg_string_in(Pattern, Components.Name)) {
            Components.Tag = "";
            ShouldSort = this->Focused == Components.Name;
        }
    }

    if (ShouldSort) {
        this->SortCloseComponent();
        this->sync_size();
    }
}

void ekg_ui_element_tab::Enable(const std::string &Pattern) {
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

void ekg_ui_element_tab::TabSide(unsigned int Dock) {
    if (Dock == ekg::dock::TOP || Dock == ekg::dock::BOTTOM) {
        this->DockTab = Dock;
    }
}

EKG_Data ekg_ui_element_tab::GetComponentHovered(float FX, float FY) {
    float X = this->get_x(), Y = this->get_y();

    switch (this->DockTab) {
        case ekg::dock::BOTTOM: {
            Y = this->rect.Y + this->rect.H - this->MinimumHeight;
            break;
        }
    }

    EKG_Data Component;

    for (EKG_Data &Components : this->List) {
        if (!Components.Tag.empty() && FX > X && FX < X + Components.DataWidth && FY > Y && FY < Y + Components.DataHeight) {
            Component = Components;
            break;
        }

        X += Components.DataWidth;
    }

    return Component;
}

void ekg_ui_element_tab::Place(ekg_ui_element_frame *Frame) {
    if (Frame == nullptr || this->children_stack.Contains(Frame->get_id()) || Frame->get_id() ==
                                                                              this->get_id()) {
        return;
    }

    EKG_Data Component;

    Component.Name = Frame->get_tag();
    Component.Tag = "0";
    Component.Id = Frame->get_id();

    this->children_stack.Put(Frame->get_id());
    this->List.push_back(Component);

    Frame->set_master_id(this->get_id());
    Frame->draggable(NULL);
    Frame->resizable(NULL);
    this->sync_size();

    Frame->set_scaled(this->get_x(), this->get_y(), this->get_width(), this->get_height());
    Frame->place(0, this->DockTab == ekg::dock::TOP ? this->MinimumHeight : 0);
    Frame->set_height(this->get_height() - this->MinimumHeight);
    Frame->sync_parents_metric();

    if (this->Focused.empty()) {
        this->Focused = Frame->get_tag();
        ekg::task(ekg::task::REORDER, Frame->get_id());
    }

    this->Open(this->Focused);
    this->SyncLayout();

    ekg::task(ekg::task::REFRESH);
}

void ekg_ui_element_tab::place(float X, float Y) {
    ekg_ui_element_frame::place(X, Y);
}

float ekg_ui_element_tab::GetSize() {
    return this->Size;
}

void ekg_ui_element_tab::SetBorderOffset(float Offset) {
    this->BorderOffset = Offset < 0.0f ? 5.0f : Offset;
}

float ekg_ui_element_tab::GetBorderOffset() {
    return this->BorderOffset;
}

void ekg_ui_element_tab::Open(const std::string &Name) {
    EKG_Data Component;

    for (EKG_Data &Components : this->List) {
        if (Components.Name == Name) {
            Component = Components;
            break;
        }
    }

    this->Open(Component);
}

void ekg_ui_element_tab::Open(EKG_Data &Component) {
    if (!Component.Name.empty()) {
        this->dragging = false;
        this->resizing = false;

        for (EKG_Data &Components : this->List) {
            auto* Element = EKG_CORE->get_element_by_id(Components.Id);

            if (Element != nullptr && !Element->is_dead()) {
                Element->set_visibility_flag(
                        Element->get_tag() == Component.Name ? ekg::visibility::VISIBLE
                                                             : ekg::visibility::INVISIBLE);
            }
        }

        this->Focused = Component.Name;
        ekg::task(ekg::task::BLOCKED);
    }
}

void ekg_ui_element_tab::SyncLayout() {
    for (unsigned int IDs : this->children_stack.StackedIds) {
        auto* Element = (ekg_ui_element_frame*) EKG_CORE->get_element_by_id(IDs);

        if (Element != nullptr && !Element->is_dead() && Element->get_tag() == this->Focused) {
            Element->set_size(this->get_width() - this->BorderOffset * 2,
                              this->get_height() - this->MinimumHeight - this->BorderOffset);

            switch (this->DockTab) {
                case ekg::dock::TOP: {
                    Element->place(this->GetBorderOffset(), this->MinimumHeight);
                    break;
                }

                case ekg::dock::BOTTOM: {
                    Element->place(this->GetBorderOffset(), 0);
                    break;
                }
            }
        }
    }
}

void ekg_ui_element_tab::on_parent_killed(unsigned int ChildElementId) {
    auto* Element = EKG_CORE->get_element_by_id(ChildElementId);

    if (Element != nullptr) {
        this->Disable(Element->get_tag());
        this->SortCloseComponent();
        this->Open(this->Focused);
    }
}

void ekg_ui_element_tab::SortCloseComponent() {
    if (this->List.empty()) {
        this->Focused = "";
        return;
    }

    int CurrentIndexPos = 0;

    if (this->List.size() != 1) {
        int ConcurrentIndexPos = -1;
        CurrentIndexPos = -1;
        bool Phase = true;

        for (int I = 0; I < this->List.size(); I++) {
            Phase = this->List.at(I).Name == this->Focused;

            if (Phase) {
                ConcurrentIndexPos = I;
                break;
            }
        }

        if (Phase) {
            int LeftIndexFound = 0, RightIndexFound = 0;
            bool Direction;

            for (int I = 0; I < this->List.size(); I++) {
                if (!Direction) {
                    int Factor = (ConcurrentIndexPos - I);
                    LeftIndexFound = this->List.at(Factor == -1 ? 0 : Factor).Tag.empty() ? I : LeftIndexFound;
                } else {
                    RightIndexFound = this->List.at(I).Tag.empty() ? I : RightIndexFound;
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
        this->Focused = "";
        return;
    }

    EKG_Data Component = this->List.at(CurrentIndexPos);
    this->Focused = Component.Tag.empty() ? "" : this->List.at(CurrentIndexPos).Name;
}
