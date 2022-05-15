#include "EKG_ui_element_popup.h"
#include "EKG.h"

void EKG_Popup::Insert(const std::vector<std::string> &ToAddList) {
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

void EKG_Popup::Delete(const std::string &Pattern) {
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

void EKG_Popup::Disable(const std::string &Pattern) {
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

void EKG_Popup::Enable(const std::string &Pattern) {
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

std::vector<EKG_Data> &EKG_Popup::GetList() {
    return this->List;
}

std::string &EKG_Popup::GetFocused() {
    return this->Focused;
}

std::string &EKG_Popup::GetClicked() {
    return this->Selected;
}

void EKG_Popup::SyncSize() {
    EKG_AbstractElement::SyncSize();

    // This method is invoked too many times.
    // So we call 'ConcurrentList' instead 'NewList'.
    std::vector<EKG_Data> ConcurrentList;

    this->MaximumHeight = 0.0F;

    for (EKG_Data &Components : this->List) {
        // It cost too many ticks (hardware) to get string height.
        Components.DataHeight = EKG_CORE->FontRenderer.GetStringHeight(Components.Name) + (this->TextScale * 2);
        ConcurrentList.push_back(Components);

        this->MaximumHeight += Components.DataHeight;
    }

    this->List = ConcurrentList;
}

void EKG_Popup::OnPreEvent(SDL_Event Event) {
    // There is no problem in finger pos out of master bound.
    if (Event.type == SDL_FINGERDOWN || Event.type == SDL_FINGERMOTION || Event.type == SDL_FINGERUP) {
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
            EKG_Data Component = this->GetHoveredComponent(FX, FY);

            this->Selected = "";

            if (this->Pressed && !this->Focused.empty() && this->Focused == Component.Name) {
                this->Clicked = true;
                this->Selected = this->Focused;
            }

            bool FingerNotHoveredMasterOrCurrentFocusedIsNotPopup = EKG::CurrentFocusedType() != "popup";

            if (!this->PopupMaster && this->GetMasterId() != 0 && FingerNotHoveredMasterOrCurrentFocusedIsNotPopup) {
                auto* Element = EKG_CORE->GetElementById(this->GetMasterId());
                FingerNotHoveredMasterOrCurrentFocusedIsNotPopup = Element != NULL && !Element->IsFingerOver(FX, FY);
            }

            if (FingerNotHoveredMasterOrCurrentFocusedIsNotPopup || (!this->Focused.empty() && !this->Hovered) || (!this->Selected.empty() && Component.Id == 0) || (EKG_CORE->GetElementById(Component.Id) == NULL && Component.Id != 0)) {
                this->Kill();
            }

            if (!this->Focused.empty() && (Component.Name == this->Focused || EKG::CurrentFocusedType() == "popup") && Component.Id == 0) {
                std::string Path = this->Focused;
                this->GetPath(Path);

                // We use smart pointers instead directly pointers.
                std::shared_ptr<void> P(new std::string (Path));

                // Send popup event using EKG events processor.
                EKG::Event::Dispatch(EKG::Event::POPUP, static_cast<void*>(P.get()), nullptr);
            }

            this->Pressed = false;
            this->Focused = "";

            break;
        }

        case SDL_FINGERDOWN: {
            float FX = Event.tfinger.x;
            float FY = Event.tfinger.y;

            EKG::ScaledFingerPos(FX, FY);

            if (this->Hovered) {
                float Y = this->GetY();
                float X, W, H;

                EKG_Data Component;
                Component.Name = "";

                for (const EKG_Data &Components : this->List) {
                    X = this->GetX();

                    W = X + this->GetWidth();
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
                        auto* ElementOut = (EKG_Popup*) EKG_CORE->GetElementById(Component.Id);

                        if (ElementOut != NULL) {
                            ElementOut->Visibility(EKG::Visibility::VISIBLE);
                            ElementOut->Place(this->GetX() + this->GetWidth(), Y, this->GetX());

                            this->Activy = Component.Name;

                            for (const EKG_Data &Components : this->List) {
                                auto *ElementIn = EKG_CORE->GetElementById(Components.Id);

                                if (ElementIn != NULL && ElementOut->GetId() != Components.Id) {
                                    ElementIn->Visibility(EKG::Visibility::INVISIBLE);
                                }
                            }
                        }
                    }
                }
            }

            bool FingerNotHoveredMasterOrCurrentFocusedIsNotPopup = EKG::CurrentFocusedType() != "popup";

            if (!this->PopupMaster && this->GetMasterId() != 0 && FingerNotHoveredMasterOrCurrentFocusedIsNotPopup) {
                auto* Element = EKG_CORE->GetElementById(this->GetMasterId());
                FingerNotHoveredMasterOrCurrentFocusedIsNotPopup = Element != NULL && !Element->IsFingerOver(FX, FY);
            }

            if (FingerNotHoveredMasterOrCurrentFocusedIsNotPopup) {
                this->Kill();
                this->Focused = "";
                this->Pressed = false;
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

    // Render only if is open.
    if (this->Show) {
        this->Rect.H = EKG_AnimationSmooth(100, SDL_GetTicks() - this->LastTicks) * this->MaximumHeight;
        this->SmoothPressed.NextFactory = !this->Focused.empty() ? EKG_CORE->ColorTheme.WidgetActivy[3] : 0;
        this->SmoothPressed.Update(PartialTicks);

        // Background container.
        EKG_Color Color(EKG_CORE->ColorTheme.ContainerBackground);
        EKG_DrawFilledRect(this->Rect, Color);

        // Now reset color to activy in theme.
        Color.Set(EKG_CORE->ColorTheme.WidgetActivy, SmoothPressed.Factory);

        // We need calc a runtime height to render property the positions.
        float FullHeight = this->GetY();

        EKG_Scissor((int) this->Rect.X, (int) this->Rect.Y, (int) this->Rect.W, (int) this->Rect.H);

        // I do not like iterations in loops but need use here.
        for (const EKG_Data &Components : this->List) {
            // Background when is focused.
            if (Components.Name == this->Focused) {
                EKG_DrawFilledShape(this->GetX(), FullHeight, this->Rect.W, Components.DataHeight, Color);
            }

            // Draw the name of component.
            EKG_CORE->FontRenderer.DrawString(Components.Name,this->GetX() + 2.0F + this->TextOffset, FullHeight + this->TextScale, Components.Tag != "1" ? EKG_CORE->ColorTheme.StringFadeColor : EKG_CORE->ColorTheme.StringColor);

            // Update the height to the next element be rendered property.
            FullHeight += Components.DataHeight;
        }

        EKG_EndScissor();

        // Outline (YOU CAN NOT DISABLED IT SORRY).
        EKG_DrawOutlineRect(this->Rect, 1.5f, EKG_CORE->ColorTheme.StringColor);
    }
}

EKG_Data EKG_Popup::GetHoveredComponent(float FX, float FY) {
    float Y = this->GetY();
    float X, W, H;

    for (const EKG_Data &Components : this->List) {
        X = this->GetX();

        W = X + this->GetWidth();
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
    std::vector<EKG_Data> NewList;

    for (EKG_Data Components : this->List) {
        if (Components.Name == Popup->GetTag()) {
            Components.Id = Popup->GetId();
            Contains = true;
        }

        NewList.push_back(Components);
    }

    if (Contains) {
        this->Children.Put(Popup->GetId());

        Popup->SetMasterId(this->GetId());
        Popup->Visibility(EKG::Visibility::INVISIBLE);
    }

    this->List = NewList;
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
    return "popup";
}

void EKG_Popup::Place(float X, float Y, float BoundingX) {
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
    return EKG::CurrentFocusedType() == "popup" && EKG::CurrentFocusedId() != 0 && (EKG::CurrentFocusedId() == this->MasterId || EKG::CurrentFocusedId() == this->GetId() || this->Children.Contains(EKG::CurrentFocusedId()));
}

void EKG_Popup::Kill() {
    if (this->Dead) {
        return;
    }

    EKG_AbstractElement::Kill();

    for (unsigned int IDs : this->Children.StackedIds) {
        auto* Element = EKG_CORE->GetElementById(IDs);

        if (Element != NULL) {
            Element->Kill();
        }
    }

    this->Children.StackedIds.clear();

    if (this->GetMasterId() != 0 && this->PopupMaster) {
        auto* Element = EKG_CORE->GetElementById(this->GetMasterId());

        if (Element != NULL) {
            Element->Kill();
        }
    }
}

void EKG_Popup::GetPath(std::string &PreviousPath) {
    PreviousPath = this->GetTag() + "|" + PreviousPath;

    if (this->GetMasterId() != 0) {
        auto* Element = (EKG_Popup*) EKG::Find(this->GetMasterId());

        if (Element != NULL) {
            Element->GetPath(PreviousPath);
        }
    }
}

void EKG_Popup::SetMasterId(unsigned int ElementMasterId) {
    EKG_AbstractElement::SetMasterId(ElementMasterId);

    // We can use this as flag to prevent visual glitch.
    this->PopupMaster = true;

    if (this->MasterId != 0) {
        auto* Element = EKG_CORE->GetElementById(ElementMasterId);
        this->PopupMaster = Element != NULL && Element->InfoClass() == "popup";
    }
}

float EKG_Popup::GetMaximumWidth() {
    return this->MaximumWidth;
}

float EKG_Popup::GetMaximumHeight() {
    return this->MaximumHeight;
}

void EKG_Popup::Visibility(unsigned int VisibilityFlag) {
    if (VisibilityFlag == EKG::Visibility::INVISIBLE) {
        this->Rect.H = 0;
    }

    bool Flag = (VisibilityFlag == EKG::Visibility::VISIBLE);

    if (this->Show != Flag) {
        this->LastTicks = SDL_GetTicks();
        EKG_AbstractElement::Visibility(EKG::Visibility::INVISIBLE);
        this->Show = Flag;
    }

    this->Visible = VisibilityFlag;
}
