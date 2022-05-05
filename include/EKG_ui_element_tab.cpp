#include "EKG_ui_element_tab.h"
#include "EKG.h"

void EKG_Tab::SetOffsetText(float OffsetText) {
    this->TextOffset = OffsetText;
}

float EKG_Tab::GetOffsetText() {
    return this->TextOffset;
}

void EKG_Tab::SetPressed(bool State) {
    this->Pressed = State;
}

bool EKG_Tab::IsPressed() {
    return this->Pressed;
}

void EKG_Tab::SetWidth(float Width) {
    if (this->Rect.W != Width) {
        this->Rect.W = Width;
        this->SyncSize();
    }
}

void EKG_Tab::SetMasterId(unsigned int Id) {
    EKG_AbstractElement::SetMasterId(Id);
}

void EKG_Tab::SetScale(float Scale) {
    if (this->TextScale != Scale) {
        this->TextScale = Scale;
        this->SyncSize();
    }
}

float EKG_Tab::GetScale() const {
    return this->TextScale;
}

std::vector<EKG_Texture> &EKG_Tab::GetList() {
    return this->List;
}

std::string EKG_Tab::InfoClass() {
    return EKG_AbstractElement::InfoClass();
}

void EKG_Tab::SyncSize() {
    EKG_AbstractElement::SyncSize();
    std::vector<EKG_Texture> NewList;

    for (EKG_Texture &Components : this->List) {
        Components.Width = this->ButtonSize;
        Components.Height = (this->TextScale * 2) + EKG_CORE->FontRenderer.GetStringHeight(Components.Tag);

        if (Components.Height > this->MinimumHeight) {
            this->MinimumHeight = Components.Height;
        }

        NewList.push_back(Components);
    }

    this->List = NewList;
}

void EKG_Tab::OnPreEvent(SDL_Event Event) {
    EKG_AbstractElement::OnPreEvent(Event);
}

void EKG_Tab::OnEvent(SDL_Event Event) {
    EKG_AbstractElement::OnEvent(Event);

    switch (Event.type) {
        case SDL_FINGERUP: {
            break;
        }

        case SDL_FINGERDOWN: {
            float FX = Event.tfinger.x;
            float FY = Event.tfinger.y;

            EKG::ScaledFingerPos(FX, FY);
            EKG_Texture Component = this->GetComponentHovered(FX, FY);

            if (!Component.Name.empty()) {
                this->Focused = Component.Name;
            }

            break;
        }
    }
}

void EKG_Tab::OnPostEvent(SDL_Event Event) {
    EKG_AbstractElement::OnPostEvent(Event);
}

void EKG_Tab::OnUpdate(const float &DeltaTicks) {
    EKG_AbstractElement::OnUpdate(DeltaTicks);
}

void EKG_Tab::OnRender(const float &PartialTicks) {
    EKG_AbstractElement::OnRender(PartialTicks);

    // Background.
    EKG_Color Color(EKG_CORE->ColorTheme.ContainerBackground);
    EKG_DrawFilledRect(this->Rect, Color);

    this->Size = 0;
    float X = this->GetX(), Y;

    switch (this->DockTab) {
        case EKG::Dock::BOTTOM: {
            Y = this->Rect.Y = this->Rect.W - this->MinimumHeight;
            break;
        }
    }

    for (EKG_Texture &Component : this->List) {
        Color.Set(EKG_CORE->ColorTheme.ContainerActivy);

        if (this->Focused == Component.Tag) {
            Color.Set(EKG_CORE->ColorTheme.ContainerBackground);
        }

        EKG_DrawFilledRect(this->Rect, Color);

        EKG_CORE->FontRenderer.DrawString(Component.Tag, X + this->Size, Y, Color);
        this->Size += this->ButtonSize;
    }
}

void EKG_Tab::SetButtonSize(float Amount) {
    this->ButtonSize = Amount;
}

float EKG_Tab::GetButtonSize() {
    return this->ButtonSize;
}

void EKG_Tab::Insert(const std::vector<std::string> &ToAddList) {
    this->List.clear();

    for (const std::string& String : ToAddList) {
        if (String.empty()) {
            continue;
        }

        EKG_Texture Component;

        Component.Name = String;
        Component.Tag = "1";

        this->List.push_back(Component);
    }
}

void EKG_Tab::Delete(const std::string &Pattern) {
    std::vector<EKG_Texture> NewList;

    for (const EKG_Texture &Components : this->List) {
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

    std::vector<EKG_Texture> NewList;

    for (EKG_Texture Components : this->List) {
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
    std::vector<EKG_Texture> NewList;

    for (EKG_Texture Components : this->List) {
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
    } else {

    }
}

EKG_Texture EKG_Tab::GetComponentHovered(float FX, float FY) {
    float X = this->GetX(), Y;

    switch (this->DockTab) {
        case EKG::Dock::BOTTOM: {
            Y = this->Rect.Y = this->Rect.W - this->MinimumHeight;
            break;
        }
    }

    EKG_Texture Component;

    for (EKG_Texture &Components : this->List) {
        if (!Components.Tag.empty() && FX > X && FX < X + Components.Width && FY < Y && FY < Y + Components.Height) {
            Component = Components;
            break;
        }

        X += Components.Width;
    }

    return Component;
}
