#include "EKG_ui_element_popup.h"
#include "EKG.h"

void EKG_Popup::Insert(const std::list<std::string> &ToAddList) {
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
    // There is no problem in finger pos out of master bound.
    if (Event.type == SDL_FINGERDOWN || Event.type == SDL_FINGERMOTION) {
        float FX = Event.tfinger.x;
        float FY = Event.tfinger.y;

        EKG::ScaledFingerPos(FX, FY);
        this->Hovered = this->Rect.CollideWithPoint(FX, FY);
    }
}

void EKG_Popup::OnEvent(SDL_Event Event) {
    EKG_AbstractElement::OnEvent(Event);

    switch (Event.type) {
        case SDL_FINGERUP: {
            float FX = Event.tfinger.x;
            float FY = Event.tfinger.y;

            EKG::ScaledFingerPos(FX, FY);
            EKG_Texture Component = this->GetHoveredComponent(FX, FY);

            this->Selected = "NULL";

            if (this->Pressed && this->Focused != "NULL" && this->Focused == Component.Name) {
                this->Clicked = true;
                this->Selected = this->Focused;
            }

            if (EKG::CurrentFocusedType() != "Popup" || (this->Selected != "NULL" && Component.Id == 0) || (EKG_CORE->GetElementById(Component.Id) == NULL && Component.Id != 0)) {
                this->Kill();
            }

            if (this->Focused != "NULL" && Component.Id == 0) {
                if (EKG_POPUPCALLBACK != ((Uint32) -1)) {
                    auto* Callback = new EKG_PopupCallback;
                    Callback->Info = "oi";

                    SDL_Event event;
                    SDL_memset(&event, 0, sizeof(event)); /* or SDL_zero(event) */
                    event.type = SDL_USEREVENT;
                    event.user.code = EKG_POPUPCALLBACK;
                    event.user.data1 = &Callback;
                    event.user.data2 = 0;
                    SDL_PushEvent(&event);

                    EKG_Log("Evento foi");
                    delete Callback;
                }
            }

            this->Pressed = false;
            this->Focused = "NULL";

            break;
        }

        case SDL_FINGERDOWN: {
            float FX = Event.tfinger.x;
            float FY = Event.tfinger.y;

            EKG::ScaledFingerPos(FX, FY);
            EKG_Texture Component = this->GetHoveredComponent(FX, FY);

            this->Focused = Component.Name;
            this->Pressed = this->Focused != "NULL";

            if (this->Pressed) {
                float FullHeight = this->GetY();

                for (const EKG_Texture &Components : this->List) {
                    bool Flag = Components.Name == Component.Name;

                    if (Components.Id != 0) {
                        auto* Element = (EKG_Popup*) EKG_CORE->GetElementById(Components.Id);

                        if (Element != NULL) {
                            Element->SetShow(Flag);
                            Element->Place(this->GetX() +  this->GetWidth(), FullHeight, this->GetX(), this->GetY());

                            this->Activy = Component.Name;
                        }
                    }

                    FullHeight += Components.Height;
                }

                EKG_CORE->ActionHappen();
            }

            break;
        }
    }
}

void EKG_Popup::OnPostEvent(SDL_Event Event) {
    EKG_AbstractElement::OnPostEvent(Event);
}

void EKG_Popup::OnUpdate(const float &DeltaTicks) {
    EKG_AbstractElement::OnUpdate(DeltaTicks);
}

void EKG_Popup::OnRender(const float &PartialTicks) {
    EKG_AbstractElement::OnRender(PartialTicks);

    // Animation of rect.
    this->Rect.H = EKG_LinearInterpolation(this->Rect.H, this->Show ? this->MaximumHeight : 0.0F, PartialTicks);

    // Render only if is open.
    if (this->Rect.H > 10) {
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
            EKG_CORE->FontRenderer.DrawString(Components.Name,this->GetX() + 2.0F + this->TextOffset, FullHeight + this->TextScale, EKG_CORE->ColorTheme.StringColor);

            // Update the height to the next element be rendered property.
            FullHeight += Components.Height;
        }

        EKG_EndScissor();

        // Outline (YOU CAN NOT DISABLED IT SORRY).
        EKG_DrawOutlineRect(this->Rect, 1.5f, EKG_CORE->ColorTheme.StringColor);
    }
}

EKG_Texture EKG_Popup::GetHoveredComponent(float FX, float FY) {
    float Y = this->GetY();
    float X, W, H;

    for (const EKG_Texture &Components : this->List) {
        X = this->GetX();

        W = X + this->GetWidth();
        H = Y + Components.Height;

        if (Components.Tag == "1" && FX > X && FX < W && FY > Y && FY < H) {
            return Components;
        }

        Y += Components.Height;
    }

    EKG_Texture Component;
    Component.Name = "NULL";

    return Component;
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

void EKG_Popup::Place(EKG_Popup* Popup) {
    if (Popup == NULL || this->Children.Contains(Popup->GetId()) || Popup->GetId() == this->GetId()) {
        return;
    }

    bool Contains;
    std::vector<EKG_Texture> NewList;

    for (EKG_Texture Components : this->List) {
        if (Components.Name == Popup->GetTag()) {
            Components.Id = Popup->GetId();
            Contains = true;
        }

        NewList.push_back(Components);
    }

    if (Contains) {
        this->Children.Put(Popup->GetId());

        Popup->SetMasterId(this->GetId());
        Popup->SetShow(false);
    }

    this->List = NewList;
}

void EKG_Popup::SetShow(bool State) {
    this->Show = State;
}

bool EKG_Popup::IsShow() {
    return this->Show;
}

void EKG_Popup::Place(float X, float Y) {
    float FactoredX = X < 0 ? 0 : X;
    float FactoredY = Y < 0 ? 0 : Y;

    if (FactoredX + this->GetWidth() > EKG::DeviceScreenWidth) {
        FactoredX = EKG::DeviceScreenWidth - this->GetWidth();
    }

    if (FactoredY + this->MaximumHeight > EKG::DeviceScreenHeight) {
        FactoredY = EKG::DeviceScreenHeight - this->MaximumHeight;
    }

    EKG_AbstractElement::Place(FactoredX, FactoredY);
}

std::string EKG_Popup::InfoClass() {
    EKG_AbstractElement::InfoClass();
    return "Popup";
}

void EKG_Popup::Place(float X, float Y, float BoundingX, float BoundingY) {
    float FactoredX = X < 0 ? 0 : X;
    float FactoredY = Y < 0 ? 0 : Y;

    if (FactoredX + this->GetWidth() > EKG::DeviceScreenWidth) {
        FactoredX = BoundingX - this->GetWidth();
    }

    if (FactoredY + this->MaximumHeight > EKG::DeviceScreenHeight) {
        FactoredY = EKG::DeviceScreenHeight - this->MaximumHeight;
    }

    this->Rect.X = (FactoredX);
    this->Rect.Y = FactoredY;
}

bool EKG_Popup::IsUpdate(float FX, float FY) {
    return EKG::CurrentFocusedType() == "Popup" && EKG::CurrentFocusedId() != 0 && (EKG::CurrentFocusedId() == this->MasterId || EKG::CurrentFocusedId() == this->GetId() || this->Children.Contains(EKG::CurrentFocusedId()));
}

void EKG_Popup::Kill() {
    if (this->Dead) {
        return;
    }

    EKG_AbstractElement::Kill();

    for (unsigned int IDs : this->Children.StackedIds) {
        auto* Element = (EKG_Popup*) EKG_CORE->GetElementById(IDs);

        if (Element != NULL) {
            Element->Kill();
        }
    }

    this->Children.StackedIds.clear();

    if (this->GetMasterId() != 0) {
        auto* Element = (EKG_Popup*) EKG_CORE->GetElementById(this->GetMasterId());

        if (Element != NULL) {
            Element->Kill();
        }
    }
}
