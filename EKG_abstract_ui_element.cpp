#include "EKG_abstract_ui_element.h"

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

}

void EKG_AbstractElement::OnEvent(SDL_Event Event) {

}

void EKG_AbstractElement::OnPostEvent(SDL_Event Event) {

}

void EKG_AbstractElement::OnUpdate(float DeltaTicks) {

}

void EKG_AbstractElement::OnRender(float PartialTicks) {

}

void EKG_AbstractElement::Scissor() {

}
