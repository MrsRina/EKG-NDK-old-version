#include "EKG_abstract_ui_element.h"
#include "EKG.h"

void EKG_AbstractElement::SetTag(const std::string &NewTag) {
    if (this->Tag != NewTag) {
        this->Tag = NewTag;
        this->SyncSize();
    }
}

std::string EKG_AbstractElement::GetTag() {
    return this->Tag;
}

void EKG_AbstractElement::SetId(unsigned int ElementId) {
    this->Id = ElementId;
}

unsigned int EKG_AbstractElement::GetId() {
    return this->Id;
}

void EKG_AbstractElement::SetMasterId(unsigned int ElementMasterId) {
    this->MasterId = ElementMasterId;
}

unsigned int EKG_AbstractElement::GetMasterId() {
    return this->MasterId;
}

void EKG_AbstractElement::SetChildren(const EKG_Stack &Stack) {
    this->Children = Stack;
}

EKG_Stack EKG_AbstractElement::GetChildren() {
    return this->Children;
}

void EKG_AbstractElement::SetCollided(const EKG_Stack &Stack) {
    this->Collided = Stack;
}

EKG_Stack EKG_AbstractElement::GetCollided() {
    return this->Collided;
}

void EKG_AbstractElement::SetRect(const EKG_Rect &Rectangle) {
     this->Rect = Rectangle;
}

EKG_Rect EKG_AbstractElement::GetRect() {
    return this->Rect;
}

float EKG_AbstractElement::GetX() {
    return this->Rect.X;
}

float EKG_AbstractElement::GetY() {
    return this->Rect.Y;
}

float EKG_AbstractElement::GetWidth() {
    return this->Rect.W;
}

float EKG_AbstractElement::GetHeight() {
    return this->Rect.H;
}

void EKG_AbstractElement::SetScaled(float X, float Y, float Width, float Height) {
    this->ScaledX = X;
    this->ScaledY = Y;
    this->ScaledWidth = Width;
    this->ScaledHeight = Height;
}

void EKG_AbstractElement::SetHovered(bool State) {
    this->Hovered = State;
}

bool EKG_AbstractElement::IsHovered() {
    return this->Hovered;
}

void EKG_AbstractElement::SetVisible(bool State) {
    this->Visible = State;
}

bool EKG_AbstractElement::IsVisible() {
    return this->Visible && !this->Dead;
}

void EKG_AbstractElement::SetDisabled(bool State) {
    this->Disabled = State;
}

bool EKG_AbstractElement::IsDisabled() {
    return this->Disabled;
}

void EKG_AbstractElement::SetDead(bool State) {
    this->Dead = State;
}

bool EKG_AbstractElement::IsDead() {
    return this->Dead;
}

void EKG_AbstractElement::Kill() {
    this->Dead = true;
    this->NoRender = true;
    this->Visible = false;

    if (this->MasterId != 0) {
        auto* Element = EKG_CORE->GetElementById(this->MasterId);

        if (Element != NULL) {
            Element->Remove(this->GetId());
        }
    }

    // Set all children master ID to 0.
    for (unsigned int IDs : this->Children.StackedIds) {
        auto* Element = EKG_CORE->GetElementById(IDs);

        if (Element != NULL) {
            Element->SetMasterId(0);
        }
    }

    EKG_CORE->Refresh();
}

void EKG_AbstractElement::OnKilled() {

}

void EKG_AbstractElement::OnCreated() {

}

void EKG_AbstractElement::OnPreEvent(SDL_Event Event) {
    if (Event.type == SDL_FINGERDOWN || Event.type == SDL_FINGERMOTION || Event.type == SDL_FINGERUP) {
        float FX = Event.tfinger.x;
        float FY = Event.tfinger.y;

        EKG::ScaledFingerPos(FX, FY);
        this->Hovered = this->IsFingerOver(FX, FY);
    }
}

void EKG_AbstractElement::OnEvent(SDL_Event Event) {

}

void EKG_AbstractElement::OnPostEvent(SDL_Event Event) {
    this->Hovered = false;
}

void EKG_AbstractElement::OnUpdate(const float &DeltaTicks) {

}

void EKG_AbstractElement::OnRender(const float &PartialTicks) {

}

void EKG_AbstractElement::Scissor() {

}

void EKG_AbstractElement::Stack(EKG_Stack &Stack) {
    if (Stack.Contains(this->Id)) {
        return;
    }

    Stack.Put(this->Id);

    for (unsigned int Ids : this->Children.StackedIds) {
        auto* Element = (EKG_AbstractElement*) EKG_CORE->GetElementById(Ids);

        if (Element == NULL) {
            continue;
        }

        Element->Stack(Stack);
    }
}

bool EKG_AbstractElement::IsMaster() {
    return !this->Children.StackedIds.empty();
}

void EKG_AbstractElement::SetScissor(int X, int Y, int W, int H) {
    this->ScissorX = X;
    this->ScissorY = Y;
    this->ScissorW = W;
    this->ScissorH = H;
}

int EKG_AbstractElement::GetScissorX() {
    return this->ScissorX;
}

int EKG_AbstractElement::GetScissorY() {
    return this->ScissorY;
}

int EKG_AbstractElement::GetScissorW() {
    return this->ScissorW;
}

int EKG_AbstractElement::GetScissorH() {
    return this->ScissorH;
}

bool EKG_AbstractElement::IsFingerOver(float X, float Y) {
    bool Flag = this->Rect.CollideWithPoint(X, Y);

    if (Flag && this->GetMasterId() != 0 && !EKG_CORE->GetElementById(this->GetMasterId())->IsFingerOver(X, Y)) {
        return false;
    }

    return Flag;
}

void EKG_AbstractElement::Place(float X, float Y) {
    if (this->GetMasterId() == 0) {
        this->Rect.X = X;
        this->Rect.Y = Y;
    } else {
        this->SyncX = X;
        this->SyncY = Y;

        this->Rect.X = this->ScaledX + this->SyncX;
        this->Rect.Y = this->ScaledY + this->SyncY;
    }
}

void EKG_AbstractElement::SyncPos() {
    this->Place(this->SyncX, this->SyncY);
}

void EKG_AbstractElement::SyncSize() {

}

bool EKG_AbstractElement::IsRender() {
    return !this->NoRender && !this->Dead;
}

void EKG_AbstractElement::SetRender(bool ApplyNoRender) {
    this->NoRender = !ApplyNoRender;
}

EKG_AbstractElement::EKG_AbstractElement() {

}

EKG_AbstractElement::~EKG_AbstractElement() {

}

std::string EKG_AbstractElement::InfoClass() {
    return "abstract";
}

void EKG_AbstractElement::Remove(unsigned int ElementId) {
    this->Children.Rem(ElementId);
}
