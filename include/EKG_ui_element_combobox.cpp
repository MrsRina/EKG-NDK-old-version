/**
 * @author Diyng
 * @since 28/04/2022 at 16:27
 **/

#include "EKG_ui_element_combobox.h"
#include "EKG.h"

void EKG_Combobox::Add(std::string &String) {
    this->PopupElementList.push_back(String);
}

void EKG_Combobox::Remove(std::string &Pattern) {
    std::vector<std::string> NewList;

    for (const std::string &String : this->PopupElementList) {
        if (EKG_StringContains(Pattern, String)) {
            continue;
        }

        NewList.push_back(String);
    }

    this->PopupElementList = NewList;
}

void EKG_Combobox::AlignText(unsigned int Dock) {
    bool Flag = (Dock == EKG::Dock::LEFT || Dock == EKG::Dock::CENTER || Dock == EKG::Dock::RIGHT);

    if (this->AlignTextDocking != Dock && Flag) {
        this->AlignTextDocking = Dock;
        this->OffsetText = 0.0F;
        this->SyncSize();
    } else {
        EKG_Log(EKG_Print(this->Tag, this->Id) + "Incorrect text align: only accept (LEFT - CENTER - RIGHT)");
    }
}

void EKG_Combobox::SetWidth(float Width) {
    if (this->Rect.W != Width) {
        this->Rect.W = Width;
        this->SyncSize();
    }
}

void EKG_Combobox::SetList(const std::vector<std::string> &List) {
    this->PopupElementList = List;
}

void EKG_Combobox::SetOffsetText(float Offset) {
    this->OffsetText = Offset;
}

float EKG_Combobox::GetOffsetText() const {
    return this->OffsetText;
}

void EKG_Combobox::SetScale(float Scale) {
    this->TextScale = Scale;
}

float EKG_Combobox::GetScale() const {
    return this->TextScale;
}

void EKG_Combobox::SetPressed(bool State) {
    this->Pressed = State;
}

bool EKG_Combobox::IsPressed() const {
    return this->Pressed;
}

void EKG_Combobox::SetCurrent(const std::string &String) {
    bool ShouldSync = this->PopupElementList.empty();

    if (ShouldSync && this->Current != " ") {
        this->Current = " ";
        this->SyncSize();
    }

    if (!ShouldSync) {
        ShouldSync = false;

        for (const std::string &Strings : this->PopupElementList) {
            if (Strings == String) {
                ShouldSync = true;
                break;
            }
        }

        if (!ShouldSync && this->Current != this->PopupElementList.at(0)) {
            this->Current = this->PopupElementList.at(0);
            this->SyncSize();
        }
    }

    if (ShouldSync && this->Current != String) {
        this->Current = String;
        this->SyncSize();
    }
}

std::string &EKG_Combobox::GetCurrent() {
    return this->Current;
}

std::string EKG_Combobox::InfoClass() {
    return "combobox";
}

void EKG_Combobox::SyncSize() {
    EKG_AbstractElement::SyncSize();

    this->TextWidth = EKG_CORE->FontRenderer.GetStringWidth(this->Current);
    this->TextHeight = EKG_CORE->FontRenderer.GetStringHeight(this->Current);

    this->Rect.H = this->TextHeight + (this->TextScale * 2);

    switch (this->AlignTextDocking) {
        case EKG::Dock::LEFT: {
            this->AlignOffsetText = 4.0f;
            break;
        }

        case EKG::Dock::RIGHT: {
            this->AlignOffsetText = this->Rect.W - this->TextWidth - 4.0f;
            break;
        }

        case EKG::Dock::CENTER: {
            this->AlignOffsetText = (this->Rect.W / 2) - (this->TextWidth / 2);
            break;
        }
    }
}

void EKG_Combobox::OnPreEvent(SDL_Event Event) {
    EKG_AbstractElement::OnPreEvent(Event);
}

void EKG_Combobox::OnEvent(SDL_Event Event) {
    EKG_AbstractElement::OnEvent(Event);

    switch (Event.type) {
        default: {
            EKG_Event CustomEvent = EKG::Event::Read(Event);

            if (CustomEvent.Type == EKG::Event::POPUP && this->Activy) {
                EKG_Log(CustomEvent.Popup.Info);

                for (const std::string &Strings : this->PopupElementList) {
                    if (EKG_StringContains(CustomEvent.Popup.Info, Strings)) {
                        this->SetCurrent(Strings);
                        break;
                    }
                }

                this->Activy = false;
            }

            break;
        }

        case SDL_FINGERDOWN: {
            this->Pressed = this->Hovered;

            break;
        }

        case SDL_FINGERUP: {
            if (this->Pressed && this->Hovered && EKG::CurrentFocusedType() == "combobox" && this->Children.StackedIds.empty()) {
                EKG_Popup* Popup = EKG::Popup(this->GetTag() + "-combobox", EKG::NOPOS, EKG::NOPOS, this->PopupElementList);

                Popup->SetWidth(this->GetWidth());
                Popup->Place(this->GetX(), this->GetY() + this->GetHeight());
                Popup->SetShow(true);
                Popup->SetScale(this->TextScale);
                Popup->SetHovered(true);
                Popup->SetMasterId(this->GetId());

                this->Children.Put(Popup->GetId());
                this->Activy = true;

                EKG_CORE->RefreshNeeded();
                EKG_CORE->ActionHappen();
            }

            break;
        }
    }
}

void EKG_Combobox::OnPostEvent(SDL_Event Event) {
    EKG_AbstractElement::OnPostEvent(Event);
}

void EKG_Combobox::OnUpdate(const float &DeltaTicks) {
    EKG_AbstractElement::OnUpdate(DeltaTicks);
}

void EKG_Combobox::OnRender(const float &PartialTicks) {
    EKG_AbstractElement::OnRender(PartialTicks);

    // Update animations.
    this->SmoothPressed.Update(PartialTicks);
    this->SmoothBoxPressed.Update(PartialTicks);
    this->SmoothBoxActivy.Update(PartialTicks);

    // Enable scissor test and cut off the fragments.
    EKG_Scissor(this->GetScissorX(), this->GetScissorY(), this->GetScissorW(), this->GetScissorH());

    // Background
    EKG_Color Color(EKG_CORE->ColorTheme.WidgetBackground);
    EKG_DrawFilledRect(this->Rect, Color);

    // Border
    if (EKG_CORE->ColorTheme.IsOutlineButtonEnabled()) {
        EKG_DrawOutlineRect(this->Rect, 1.5f, EKG_CORE->ColorTheme.StringColor);
    }

    // Pressed.
    if (this->SmoothPressed.Factory > 10) {
        Color.Set(EKG_CORE->ColorTheme.WidgetPressed, (unsigned int) this->SmoothPressed.Factory);
        EKG_DrawFilledRect(this->Rect, Color);
    }

    // String.
    EKG_CORE->FontRenderer.DrawString(this->Current, this->Rect.X + this->AlignOffsetText + this->OffsetText, this->Rect.Y + this->TextScale, EKG_CORE->ColorTheme.StringColor);

    // End scissor.
    EKG_EndScissor();
}
