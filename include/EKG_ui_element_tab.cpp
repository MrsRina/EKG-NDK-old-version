#include "EKG_ui_element_tab.h"
#include "EKG.h"

void EKG_Tab::SetOffsetText(float OffsetText) {
    this->TextOffset = OffsetText;
}

float EKG_Tab::GetOffsetText() {
    return this->TextOffset;
}

void EKG_Tab::SetScale(float Scale) {
    if (this->TextScale != Scale) {
        this->TextScale = Scale;
        this->SyncSize();
    }
}

float EKG_Tab::GetScale() {
    return this->TextScale;
}

std::vector<EKG_Data> &EKG_Tab::GetList() {
    return this->List;
}

std::string EKG_Tab::InfoClass() {
    return "frame-tab";
}

void EKG_Tab::SyncSize() {
    EKG_Frame::SyncSize();
    std::vector<EKG_Data> NewList;

    this->MinimumWidth = 0.0F;

    for (EKG_Data &Components : this->List) {
        float StringNameWidth = EKG_CORE->FontRenderer.GetStringWidth(Components.Name);

        Components.DataWidth = (this->ButtonSize * 2) + StringNameWidth;
        Components.DataHeight = (this->TextScale * 2) + EKG_CORE->FontRenderer.GetStringHeight(Components.Name);

        if (Components.DataWidth > this->MinimumWidth) {
            this->MinimumWidth = Components.DataWidth;
        }

        if (Components.DataHeight > this->MinimumHeight) {
            this->MinimumHeight = Components.DataHeight;
        }

        Components.DataX = (Components.DataWidth / 2.0F) - (StringNameWidth / 2.0F);
        NewList.push_back(Components);
    }

    this->List = NewList;
    NewList.clear();

    float DivMinWidth = this->MinimumWidth / 2.0f;
    float DivBorder = this->BorderOffset / 2.0F;

    // We do not want small buttons, after sync the most largest component,
    // we set the components less than largest size divided by 2.
    for (EKG_Data &Components : this->List) {
        if (Components.DataWidth < DivMinWidth) {
            Components.DataWidth = DivMinWidth;

            float StringNameWidth = EKG_CORE->FontRenderer.GetStringWidth(Components.Name);
            Components.DataX = (Components.DataWidth / 2.0F) - (StringNameWidth / 2.0F);
        }

        Components.DataHeight = this->MinimumHeight - DivBorder;
        NewList.push_back(Components);
    }

    this->List = NewList;
}

void EKG_Tab::OnPreEvent(SDL_Event Event) {
    EKG_Frame::OnPreEvent(Event);
}

void EKG_Tab::OnEvent(SDL_Event Event) {
    EKG_Frame::OnEvent(Event);

    switch (Event.type) {
        case SDL_FINGERUP: {
            this->Activy = "";
            break;
        }

        case SDL_FINGERMOTION: {
            for (unsigned int IDs : this->Children.StackedIds) {
                auto* Element = (EKG_Frame*) EKG_CORE->GetElementById(IDs);

                if (Element != NULL) {
                    Element->SetSize(this->GetWidth() - this->BorderOffset * 2, this->GetHeight() - this->MinimumHeight - this->BorderOffset);

                    switch (this->DockTab) {
                        case EKG::Dock::TOP: {
                            Element->Place(this->GetBorderOffset(), this->MinimumHeight);
                            break;
                        }

                        case EKG::Dock::BOTTOM: {
                            Element->Place(this->GetBorderOffset(), 0);
                            break;
                        }
                    }
                }
            }

            break;
        }

        case SDL_FINGERDOWN: {
            if (!this->Hovered) {
                return;
            }

            float FX = Event.tfinger.x;
            float FY = Event.tfinger.y;

            EKG::ScaledFingerPos(FX, FY);
            float YDiff = this->DockTab == EKG::Dock::TOP ? this->GetY() : this->GetY() + this->GetHeight() - this->GetMinimumHeight();

            if (!(FX > this->GetX() && FX < this->GetX() + this->GetSize() && FY > YDiff && FY < YDiff + this->MinimumHeight)) {
                return;
            }

            EKG_Data Component = this->GetComponentHovered(FX, FY);

            if (!Component.Name.empty()) {
                this->Dragging = false;
                this->Resizing = false;

                for (EKG_Data &Components : this->List) {
                    auto* Element = EKG_CORE->GetElementById(Components.Id);

                    if (Element != NULL) {
                        Element->SetVisible(Element->GetTag() == Component.Name);
                    }
                }

                this->Focused = Component.Name;
                this->Activy = Component.Name;

                EKG_CORE->ActionHappen();
            }

            break;
        }
    }
}

void EKG_Tab::OnPostEvent(SDL_Event Event) {
    EKG_Frame::OnPostEvent(Event);
}

void EKG_Tab::OnUpdate(const float &DeltaTicks) {
    EKG_Frame::OnUpdate(DeltaTicks);
}

void EKG_Tab::OnRender(const float &PartialTicks) {
    // Background.
    EKG_Color Color(EKG_CORE->ColorTheme.ContainerBackground);
    EKG_DrawFilledRect(this->Rect, Color);

    this->Size = 0;
    float X = this->GetX(), Y = this->GetY();

    switch (this->DockTab) {
        case EKG::Dock::BOTTOM: {
            Y = this->GetY() + this->Rect.W - this->MinimumHeight;
            break;
        }
    }

    for (EKG_Data &Component : this->List) {
        Color.Set(EKG_CORE->ColorTheme.ContainerBackground);

        if (Component.Name == this->Activy) {
            Color.Set(EKG_CORE->ColorTheme.ContainerActivy);
            EKG_DrawFilledShape(X + this->Size, Y, Component.DataWidth, Component.DataHeight, Color);
        }

        if (this->Focused == Component.Name) {
            float UniqueHeight = Component.DataHeight / 10;
            float UniqueY = Y + Component.DataHeight - UniqueHeight - (1);

            Color.Set(EKG_CORE->ColorTheme.ContainerActivy);
            EKG_DrawOutlineShape(X + this->Size, UniqueY, Component.DataWidth, 1, 1.5f, EKG_CORE->ColorTheme.StringColor);
        }

        EKG_CORE->FontRenderer.DrawString(Component.Name, X + this->Size + Component.DataX, Y + this->TextScale, Component.Tag.empty() ? EKG_CORE->ColorTheme.StringFadeColor : EKG_CORE->ColorTheme.StringColor);
        this->Size += Component.DataWidth;
    }
}

void EKG_Tab::SetButtonSize(float Amount) {
    this->ButtonSize = Amount;
}

float EKG_Tab::GetButtonSize() {
    return this->ButtonSize;
}

void EKG_Tab::Delete(const std::string &Pattern) {
    std::vector<EKG_Data> NewList;

    for (const EKG_Data &Components : this->List) {
        if (EKG_StringContains(Pattern, Components.Name)) {
            continue;
        }

        NewList.push_back(Components);
    }

    bool ShouldSync = this->List.size() != NewList.size();
    this->List = NewList;

    if (ShouldSync) {
        this->SyncSize();
    }
}

void EKG_Tab::Disable(const std::string &Pattern) {
    if (Pattern.empty()) {
        return;
    }

    std::vector<EKG_Data> NewList;

    for (EKG_Data Components : this->List) {
        if (EKG_StringContains(Pattern, Components.Name)) {
            Components.Tag = "";
        }

        NewList.push_back(Components);
    }

    bool ShouldSync = this->List.size() != NewList.size();
    this->List = NewList;

    if (ShouldSync) {
        this->SyncSize();
    }
}

void EKG_Tab::Enable(const std::string &Pattern) {
    std::vector<EKG_Data> NewList;

    for (EKG_Data Components : this->List) {
        if (EKG_StringContains(Pattern, Components.Name)) {
            Components.Tag = "1";
        }

        NewList.push_back(Components);
    }

    bool ShouldSync = this->List.size() != NewList.size();
    this->List = NewList;

    if (ShouldSync) {
        this->SyncSize();
    }
}

void EKG_Tab::TabSide(unsigned int Dock) {
    if (Dock == EKG::Dock::TOP || Dock == EKG::Dock::BOTTOM) {
        this->DockTab = Dock;
    }
}

EKG_Data EKG_Tab::GetComponentHovered(float FX, float FY) {
    float X = this->GetX(), Y = this->GetY();

    switch (this->DockTab) {
        case EKG::Dock::BOTTOM: {
            Y = this->Rect.Y + this->Rect.H - this->MinimumHeight;
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

void EKG_Tab::Place(EKG_Frame *Frame) {
    if (Frame == NULL || this->Children.Contains(Frame->GetId()) || Frame->GetId() == this->GetId()) {
        return;
    }

    EKG_Data Component;

    Component.Name = Frame->GetTag();
    Component.Tag = "0";
    Component.Id = Frame->GetId();

    this->Children.Put(Component.Id);
    this->List.push_back(Component);

    Frame->SetMasterId(this->GetId());
    Frame->Draggable(NULL);
    Frame->Resizable(NULL);
    this->SyncSize();

    Frame->SetScaled(this->GetX(), this->GetY(), this->GetWidth(), this->GetHeight());
    Frame->Place(0, this->DockTab == EKG::Dock::TOP ? this->MinimumHeight : 0);
    Frame->SetHeight(this->GetHeight() - this->MinimumHeight);

    EKG_CORE->Refresh();
}

void EKG_Tab::Place(float X, float Y) {
    EKG_Frame::Place(X, Y);
}

float EKG_Tab::GetSize() {
    return this->Size;
}

void EKG_Tab::SetBorderOffset(float Offset) {
    this->BorderOffset = Offset < 0.0f ? 5.0f : Offset;
}

float EKG_Tab::GetBorderOffset() {
    return this->BorderOffset;
}
