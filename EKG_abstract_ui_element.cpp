#include "EKG_abstract_ui_element.h"
#include "EKG.h"

void EKG_AbstractElement::SetTag(const std::string &NewTag) {
    if (this->Tag != NewTag) {
        this->Tag = NewTag;
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

void EKG_AbstractElement::SetX(float X) {
    this->Rect.X = X;
}

float EKG_AbstractElement::GetX() {
    return this->Rect.X;
}

void EKG_AbstractElement::SetY(float Y) {
    this->Rect.Y = Y;
}

float EKG_AbstractElement::GetY() {
    return this->Rect.Y;
}

void EKG_AbstractElement::SetWidth(float Width) {
    this->Rect.W = Width;
}

float EKG_AbstractElement::GetWidth() {
    return this->Rect.W;
}

void EKG_AbstractElement::SetHeight(float Height) {
    this->Rect.H = Height;
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
    return this->Visible;
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

}

void EKG_AbstractElement::OnKilled() {

}

void EKG_AbstractElement::OnCreated() {

}

void EKG_AbstractElement::OnPreEvent(SDL_Event Event) {
    if (Event.type == SDL_FINGERDOWN || Event.type == SDL_FINGERMOTION) {
        float X = Event.motion.x;
        float Y = Event.motion.y;

        EKG::ScaledFingerPos(X, Y);
        this->Hovered = this->IsFingerOver(X, Y);
    }
}

void EKG_AbstractElement::OnEvent(SDL_Event Event) {

}

void EKG_AbstractElement::OnPostEvent(SDL_Event Event) {
    this->Hovered = false;
}

void EKG_AbstractElement::OnUpdate(float DeltaTicks) {

}

void EKG_AbstractElement::OnRender(float PartialTicks) {

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

void EKG_AbstractElement::SetScissorX(int X) {
    this->ScissorX = X;
}

int EKG_AbstractElement::GetScissorX() {
    return this->ScissorX;
}

void EKG_AbstractElement::SetScissorY(int Y) {
    this->ScissorY = Y;
}

int EKG_AbstractElement::GetScissorY() {
    return this->ScissorY;
}

void EKG_AbstractElement::SetScissorW(int W) {
    this->ScissorW = W;
}

int EKG_AbstractElement::GetScissorW() {
    return this->ScissorW;
}

void EKG_AbstractElement::SetScissorH(int H) {
    this->ScissorH = H;
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
        this->SetX(X);
        this->SetY(Y);
    } else {
        this->SyncX = X;
        this->SyncY = Y;

        this->SetX(this->ScaledX + this->SyncX);
        this->SetY(this->ScaledY + this->SyncY);
    }
}

void EKG_AbstractElement::SyncPos() {
    this->Place(this->SyncX, this->SyncY);
}

void EKG_AbstractElement::SyncSize() {

}