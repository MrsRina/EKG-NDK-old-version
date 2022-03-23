#include "EKG_core.h"
#include "EKG.h"

void EKG_Core::RemoveElement(EKG_AbstractElement* Element) {
    std::vector<EKG_AbstractElement*> NewBufferUpdate;

    for (EKG_AbstractElement* Elements : this->BufferUpdate) {
        if (Elements->GetId() == Element->GetId()) {
            continue;
        }

        NewBufferUpdate.push_back(Elements);
    }

    this->BufferUpdate = NewBufferUpdate;
}

void EKG_Core::AddElement(EKG_AbstractElement *Element) {
    this->BufferUpdate.push_back(Element);
}

void EKG_Core::OnEvent(SDL_Event Event) {

}

void EKG_Core::OnUpdate(float DeltaTicks) {

}

void EKG_Core::OnRender(float PartialTicks) {
    this->FontRenderer.DrawString("oi eu sou linda linda linda", 10, 10, EKG_Color(0, 0, 255));
}

void EKG_Core::Init() {
    this->ShaderManager.Init();
    this->FontRenderer.Init();
}

void EKG_Core::Quit() {
    this->ShaderManager.Quit();
    this->FontRenderer.Quit();
}