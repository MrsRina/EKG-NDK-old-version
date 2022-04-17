#include "EKG_ui_element_popup.h"
#include "EKG.h"

void EKG_Popup::Insert(const std::string StringList[32]) {
    this->List.clear();

    for (int I = 0; I < 32; I++) {
        std::string String = StringList[I];

        if (String.empty()) {
            continue;
        }

        EKG_Log(String);
        EKG_Texture Component;

        Component.Name = String;
        Component.Tag = "1";

        this->List.push_back(Component);
    }
}

void EKG_Popup::Delete(const std::string &Pattern) {
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

void EKG_Popup::Disable(const std::string &Pattern) {
    std::vector<EKG_Texture> NewList;

    for (EKG_Texture Components : this->List) {
        if (EKG_StringContains(Pattern, Components.Name)) {
            Components.Tag = "0";
        }

        NewList.push_back(Components);
    }

    bool ShouldSync = this->List.size() != NewList.size();
    this->List = NewList;

    if (ShouldSync) {
        this->SyncSize();
    }
}

void EKG_Popup::Enable(const std::string &Pattern) {
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

void EKG_Popup::SetOffsetText(float OffsetText) {
    this->TextOffset = OffsetText;
}

float EKG_Popup::GetOffsetText() {
    return this->TextOffset;
}

void EKG_Popup::SetPressed(bool State) {
    this->Pressed = State;
}

bool EKG_Popup::IsPressed() {
    return this->Pressed;
}

void EKG_Popup::SetClicked(bool State) {
    this->Clicked = State;
}

bool EKG_Popup::IsClicked() {
    return this->Clicked;
}

void EKG_Popup::SetWidth(float Width) {
    if (this->Rect.W != Width) {
        this->Rect.W = Width;
        this->SyncSize();
    }
}

std::vector<EKG_Texture> EKG_Popup::GetList() {
    return this->List;
}

std::string EKG_Popup::GetFocused() {
    return this->Focused;
}

std::string EKG_Popup::GetClicked() {
    return this->Selected;
}

void EKG_Popup::SyncSize() {
    EKG_AbstractElement::SyncSize();

    // This method is invoked too many times.
    // So we call 'ConcurrentList' instead 'NewList'.
    std::vector<EKG_Texture> ConcurrentList;

    this->MaximumHeight = 0.0F;

    for (EKG_Texture &Components : this->List) {
        // It cost too many ticks (hardware) to get string height.
        Components.Height = EKG_CORE->FontRenderer.GetStringHeight(Components.Name) + (this->TextScale * 2);
        ConcurrentList.push_back(Components);

        this->MaximumHeight += Components.Height;
    }

    this->List = ConcurrentList;
}

void EKG_Popup::OnPreEvent(SDL_Event Event) {
    EKG_AbstractElement::OnPreEvent(Event);
}

void EKG_Popup::OnEvent(SDL_Event Event) {
    EKG_AbstractElement::OnEvent(Event);

    switch (Event.type) {
        case SDL_FINGERUP: {
            float FX = Event.tfinger.x;
            float FY = Event.tfinger.y;

            EKG::ScaledFingerPos(FX, FY);

            if (!this->IsFingerOver(FX, FY)) {
                this->Kill();
            } else {
                if (this->Pressed && this->Focused != "NULL" && this->Focused == this->GetHoveredComponent(FX, FY)) {
                    this->Clicked = true;
                    this->Selected = this->Focused;
                }
            }

            this->Pressed = false;
            this->Focused = "NULL";

            break;
        }

        case SDL_FINGERDOWN: {
            if (!this->Hovered) {
                this->Kill();
                return;
            }

            float FX = Event.tfinger.x;
            float FY = Event.tfinger.y;

            EKG::ScaledFingerPos(FX, FY);

            this->Focused = this->GetHoveredComponent(FX, FY);
            this->Pressed = this->Focused != "NULL";

            break;
        }
    }
}

void EKG_Popup::OnPostEvent(SDL_Event Event) {
    EKG_AbstractElement::OnPostEvent(Event);
}

void EKG_Popup::OnUpdate(float DeltaTicks) {
    EKG_AbstractElement::OnUpdate(DeltaTicks);
}

void EKG_Popup::OnRender(float PartialTicks) {
    EKG_AbstractElement::OnRender(PartialTicks);

    // Background container.
    EKG_Color Color(EKG_CORE->ColorTheme.ContainerBackground);
    EKG_DrawFilledRect(this->Rect, Color);

    // Now reset color to activy in theme.
    Color.Set(EKG_CORE->ColorTheme.WidgetActivy);

    // We need calc a runtime height to render property the positions.
    float FullHeight = this->GetY();

    EKG_Scissor((int) this->Rect.X, (int) this->Rect.Y, (int) this->Rect.W, (int) this->Rect.H);

    // I do not like iterations in loops but need use here.
    for (const EKG_Texture &Components : this->List) {
        // Background when is focused.
        if (Components.Name == this->Focused) {
            EKG_DrawFilledShape(this->GetX(), FullHeight, this->Rect.W, Components.Height, Color);
        }

        // Draw the name of component.
        EKG_CORE->FontRenderer.DrawString(Components.Name, this->GetX() + 2.0F + this->TextOffset, FullHeight + this->TextScale, EKG_CORE->ColorTheme.StringColor);

        // Update the height to the next element be rendered property.
        FullHeight += Components.Height;
    }

    EKG_EndScissor();

    // Animation of rect.
    this->Rect.H = EKG_LinearInterpolation(this->Rect.H, this->MaximumHeight, PartialTicks);

    // Outline (YOU CAN NOT DISABLED IT SORRY).
    EKG_DrawOutlineRect(this->Rect, 1.5f, EKG_CORE->ColorTheme.StringColor);
}

std::string EKG_Popup::GetHoveredComponent(float FX, float FY) {
    float FullHeight = this->GetY();
    float X, Y, W, H;

    for (const EKG_Texture &Components : this->List) {
        X = this->GetX();
        Y = FullHeight;

        W = X + this->GetWidth();
        H = Y + Components.Height;

        if (Components.Tag == "1" && FX > X && FX < W && FY > Y && FY < H) {
            return Components.Name;
        }

        FullHeight += Components.Height;
    }

    return "NULL";
}

void EKG_Popup::SetScale(float Scale) {
    if (this->TextScale != Scale) {
        this->TextScale = Scale;
        this->SyncSize();
    }
}

float EKG_Popup::GetScale() {
    return this->TextScale;
}

void EKG_Popup::Place(EKG_AbstractElement* Element, const std::string &Component) {
    bool Contains;
    std::vector<EKG_Texture> NewList;

    for (EKG_Texture Components : this->List) {
        Contains = Components.Name == Component;

        if (Contains) {
            Components.Id = Element->GetId();
        }

        NewList.push_back(Components);
    }

    if (Contains) {
        this->List = NewList;
    }
}

void EKG_Popup::SetShow(bool State) {
    this->Show = State;
}

bool EKG_Popup::IsShow() {
    return this->Show;
}

void EKG_Popup::Place(float X, float Y) {
    EKG_AbstractElement::Place(X, Y);
}