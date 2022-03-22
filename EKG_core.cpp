#include "EKG_core.h"

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

}

EKG_ShaderManager EKG_Core::GetShaderManager() {
    return this->ShaderManager;
}

void EKG_Core::Init() {
    this->ShaderManager.Init();
}

void EKG_Core::Quit() {
    this->ShaderManager.Quit();
}