#include "EKG_core.h"
#include "EKG.h"

void EKG_Core::RemoveElement(unsigned int ElementId) {
    std::vector<EKG_AbstractElement*> NewBufferUpdate;

    this->BufferRender.fill(0);
    this->BufferSize = 0;

    for (EKG_AbstractElement* &Element : this->BufferUpdate) {
        if (Element->GetId() == ElementId) {
            Element->Kill();

            delete Element;
            continue;
        }

        NewBufferUpdate.push_back(Element);

        if (Element->GetVisibility() == EKG::Visibility::VISIBLE) {
            this->BufferRender[this->BufferSize++] = Element;
        }
    }

    this->BufferUpdate = NewBufferUpdate;
}

void EKG_Core::AddElement(EKG_AbstractElement* Element) {
    this->BufferNew.push_back(Element);
    this->ShouldSwapBuffers = true;
}

void EKG_Core::OnEvent(SDL_Event Event) {
    if (Event.type != SDL_FINGERDOWN && Event.type != SDL_FINGERUP && Event.type != SDL_FINGERMOTION && Event.type != EKG::Event::POPUP) {
        return;
    }

    if (Event.type == SDL_FINGERDOWN) {
        this->TaskBlocked = false;
    }

    this->FocusedId = 0;

    // Get focusing element id.
    for (EKG_AbstractElement* &Element : this->BufferUpdate) {
        if (Element == nullptr || Element->IsDead()) {
            continue;
        }

        // Update flags.
        Element->OnPreEvent(Event);

        // The next superior element is set.
        if (Element->IsHovered() && Element->GetVisibility() == EKG::Visibility::VISIBLE) {
            this->FocusedId = Element->GetId();
        }

        // Reset flags.
        Element->OnPostEvent(Event);
    }

    this->BufferRender.fill(0);
    this->BufferSize = 0;

    // Call all events.
    for (EKG_AbstractElement* &Element : this->BufferUpdate) {
        if (Element == nullptr || Element->IsDead()) {
            continue;
        }

        if (Element->GetId() == this->FocusedId) {
            Element->OnPreEvent(Event);

            // Communicate for EKG environment the current focused element.
            this->FocusedTag = Element->GetTag();
            this->FocusedType = Element->InfoClass();
        }

        Element->OnEvent(Event);

        if (Element->GetVisibility() == EKG::Visibility::VISIBLE) {
            this->BufferRender[this->BufferSize++] = Element;
        }

        this->SyncScissor(Element);
    }

    if (this->ShouldSwapBuffers) {
        this->SwapBuffers();
    }

    if (Event.type == SDL_FINGERDOWN || Event.type == SDL_FINGERUP) {
        if (this->TaskRefocus) {
            for (EKG_AbstractElement* &Element : this->BufferUpdate) {
                if (Element == nullptr || Element->IsDead()) {
                    continue;
                }

                if (Element->IsHovered() && Element->GetVisibility() == EKG::Visibility::VISIBLE) {
                    this->FocusedId = Element->GetId();

                    // Communicate for EKG environment the current focused element.
                    this->FocusedTag = Element->GetTag();
                    this->FocusedType = Element->InfoClass();
                }
            }

            this->TaskRefocus = false;
        }

        if (this->FocusedId == 0) {
            this->FocusedTag = "NULL";
            this->FocusedType = "NULL";
        } else {
            this->ReorderStack(this->FocusedId);
        }
    }
}

void EKG_Core::OnUpdate(const float &DeltaTicks) {
    if (this->TaskRefresh) {
        this->RefreshStack();
        this->TaskRefresh = false;
    }

    if (this->ShouldSwapBuffers) {
        this->SwapBuffers();
    }

    if (this->TaskReorder) {
        this->ReorderStack(this->IdFromTask);
        this->IdFromTask = 0;
        this->TaskReorder = false;
    }

    if (this->TaskFree) {
        this->ReorderStack(this->IdFromTask);
        this->IdFromTask = 0;
        this->TaskFree = false;
    }
}

void EKG_Core::OnRender(const float &PartialTicks) {
    for (EKG_AbstractElement* &Element : this->BufferRender) {
        if (Element == nullptr) {
            continue;
        }

        Element->OnRender(PartialTicks);
    }
}

void EKG_Core::Init() {
    this->ShaderManager.Init();
    this->FontRenderer.Init();
    this->ColorTheme.Init();

    this->Timing = new EKG_Timing();
}

void EKG_Core::Quit() {
    for (EKG_AbstractElement* &Element : this->BufferUpdate) {
        Element->Kill();
        delete Element;
    }

    this->BufferUpdate.clear();
    EKG_Log("Killed all alive elements.");

    this->ShaderManager.Quit();
    this->FontRenderer.Quit();
    this->ColorTheme.Quit();

    EKG_Log("Shutdown complete.");
}

void EKG_Core::ReorderStack(unsigned int ElementId) {
    // Reset if is 0.
    if (ElementId == 0) {
        return;
    }

    EKG_Stack Current;
    EKG_Stack Focused;

    for (EKG_AbstractElement* &Element : this->BufferUpdate) {
        if (Element == nullptr) {
            continue;
        }

        // Ignore if contains at focused.
        if (Focused.Contains(Element->GetId())) {
            continue;
        }

        // Verify child in this element.
        EKG_Stack Stack;
        Element->Stack(Stack);

        // If contains element id focused we set the stack value in focused.
        if (Stack.Contains(ElementId)) {
            Focused = Stack;
        } else {
            // Else, we add in current stack.
            Current.Put(Element->GetId());
        }
    }

    std::vector<EKG_AbstractElement*> NewBufferOfUpdate;

    this->BufferRender.fill(0);
    this->BufferSize = 0;

    // Put current.
    for (unsigned int IDs : Current.StackedIds) {
        auto* Element = (EKG_AbstractElement*) this->GetElementById(IDs);

        if (Element == nullptr) {
            continue;
        }

        NewBufferOfUpdate.push_back(Element);

        if (Element->GetVisibility() == EKG::Visibility::VISIBLE) {
            this->BufferRender[this->BufferSize++] = Element;
        }
    }

    // Put the focused ids at top of list.
    for (unsigned int IDs : Focused.StackedIds) {
        auto* Element = (EKG_AbstractElement*) this->GetElementById(IDs);

        if (Element == nullptr) {
            continue;
        }

        NewBufferOfUpdate.push_back(Element);

        if (Element->GetVisibility() == EKG::Visibility::VISIBLE) {
            this->BufferRender[this->BufferSize++] = Element;
        }
    }

    this->BufferUpdate = NewBufferOfUpdate;
}

EKG_AbstractElement* EKG_Core::GetElementById(unsigned int Id) {
    for (EKG_AbstractElement* &Element : this->BufferNew) {
        if (Element->GetId() == Id) {
            return Element;
        }
    }

    for (EKG_AbstractElement* &Element : this->BufferUpdate) {
        if (Element->GetId() == Id) {
            return Element;
        }
    }

    return NULL;
}

unsigned int EKG_Core::NewId() {
    this->HighId++;
    return this->HighId;
}

void EKG_Core::Sync(float X, float Y, float W, float H, const EKG_Stack &Stack) {
    for (unsigned int IDs : Stack.StackedIds) {
        auto* Element =  this->GetElementById(IDs);

        if (Element == nullptr) {
            continue;
        }

        Element->SetScaled(X, Y, W, H);
        Element->SyncPos();

        auto PreviousVisibility = (EKG::Visibility) Element->GetVisibility();
        bool Visible = Element->GetRect().CollideWithRect(X, Y, W, H);

        if (PreviousVisibility == EKG::Visibility::VISIBLE && !Visible) {
            Element->Visibility(EKG::Visibility::LOW_PRIORITY);
        } else if (PreviousVisibility == EKG::LOW_PRIORITY && Visible) {
            Element->Visibility(EKG::Visibility::VISIBLE);
        }

        if (Element->IsMaster()) {
            EKG_Core::Sync(Element->GetX(), Element->GetY(), Element->GetWidth(), Element->GetHeight(), Element->GetChildren());
        }
    }
}

void EKG_Core::SyncScissor(EKG_AbstractElement* Element) {
    Element->SetScissor(Element->GetX(), Element->GetY(), Element->GetWidth(), Element->GetHeight());

    if (!Element->IsMaster()) {
        return;
    }

    int X = Element->GetScissorX();
    int Y = Element->GetScissorY();
    int W = Element->GetScissorW();
    int H = Element->GetScissorH();

    // if (X < (int) Element->GetX()) {
    //     Element->SetScissor((int) Element->GetX(), Element->GetScissorY(), Element->GetScissorW(), Element->GetScissorH());
    // }

    // if (Y < (int) Element->GetY()) {
    //     Element->SetScissor(Element->GetScissorX(), (int) Element->GetY(), Element->GetScissorW(), Element->GetScissorH());
    // }

    // if (W < (int) Element->GetWidth()) {
    //     Element->SetScissor(Element->GetScissorX(), Element->GetScissorY(), (int) Element->GetWidth(), Element->GetScissorH());
    // }

    // if (H < (int) Element->GetHeight()) {
    //     Element->SetScissor(Element->GetScissorX(), Element->GetScissorY(), Element->GetScissorW(), (int) Element->GetHeight());
    //}

    for (unsigned int Ids : Element->GetChildren().StackedIds) {
        auto* Elements = (EKG_AbstractElement*) this->GetElementById(Ids);

        // if (X < (int) Elements->GetX()) {
        //     Elements->SetScissor((int) Elements->GetX(), Elements->GetScissorY(), Elements->GetScissorW(), Elements->GetScissorH());
        // }
//
        // if (Y < (int) Element->GetY()) {
        //     Elements->SetScissor(Elements->GetScissorX(), (int) Elements->GetY(), Elements->GetScissorW(), Elements->GetScissorH());
        // }
//
        // if (W < (int) Elements->GetWidth()) {
        //     Elements->SetScissor(Elements->GetScissorX(), Elements->GetScissorY(), (int) Elements->GetWidth(), Elements->GetScissorH());
        // }
//
        // if (H < (int) Elements->GetHeight()) {
        //     Elements->SetScissor(Elements->GetScissorX(), Elements->GetScissorY(), Elements->GetScissorW(), (int) Elements->GetHeight());
        // }
    }
}

int EKG_Core::GetSizeOfUpdateElements() {
    return this->BufferUpdate.size();
}

int EKG_Core::GetSizeOfRenderElements() {
    return this->BufferSize;
}

unsigned int EKG_Core::GetFocusedElementId() {
    return this->FocusedId;
}

std::string EKG_Core::GetFocusedTag() {
    return this->FocusedTag;
}

std::string EKG_Core::GetFocusedType() {
    return this->FocusedType;
}

void EKG_Core::RefreshStack() {
    this->BufferRender.fill(0);
    this->BufferSize = 0;

    for (EKG_AbstractElement* &Element : this->BufferUpdate) {
        if (Element == nullptr || Element->IsDead()) {
            continue;
        }

        if (Element->GetVisibility() == EKG::Visibility::VISIBLE) {
            this->BufferRender[this->BufferSize++] = Element;
        }
    }
}


void EKG_Core::FreeStack() {
    this->BufferRender.fill(0);
    this->BufferSize = 0;

    std::vector<EKG_AbstractElement*> NewBuffer;

    for (EKG_AbstractElement* &Element : this->BufferUpdate) {
        if (Element == nullptr) {
            continue;
        }

        if (Element->IsDead()) {
            delete Element;
            continue;
        }

        NewBuffer.push_back(Element);

        if (Element->GetVisibility() == EKG::Visibility::VISIBLE) {
            this->BufferRender[this->BufferSize++] = Element;
        }
    }

    this->BufferUpdate = NewBuffer;
}


void EKG_Core::SwapBuffers() {
    this->BufferRender.fill(0);
    this->BufferSize = 0;

    std::vector<EKG_AbstractElement*> NewBuffer;

    for (EKG_AbstractElement* &Element : this->BufferUpdate) {
        if (Element == nullptr) {
            continue;
        }

        NewBuffer.push_back(Element);

        if (Element->GetVisibility() == EKG::Visibility::VISIBLE) {
            this->BufferRender[this->BufferSize++] = Element;
        }
    }

    for (EKG_AbstractElement* &Element : this->BufferNew) {
        if (Element == nullptr) {
            continue;
        }

        NewBuffer.push_back(Element);

        if (Element->GetVisibility() == EKG::Visibility::VISIBLE) {
            this->BufferRender[this->BufferSize++] = Element;
        }
    }

    this->BufferUpdate = NewBuffer;
    this->BufferNew.clear();
    this->ShouldSwapBuffers = false;
}

void EKG_Core::Task(unsigned int Task, unsigned int Id) {
    if (EKG_FlagContains(Task, EKG::Task::REFRESH)) {
        this->TaskRefresh = true;
    }

    if (EKG_FlagContains(Task, EKG::Task::REORDER)) {
        this->TaskReorder = true;
        this->IdFromTask = Id;
    }

    if (EKG_FlagContains(Task, EKG::Task::BLOCKED)) {
        this->TaskBlocked = true;
    }

    if (EKG_FlagContains(Task, EKG::Task::REFOCUS)) {
        this->TaskRefocus = true;
    }

    if (EKG_FlagContains(Task, EKG::Task::FREE)) {
        this->TaskFree = true;
    }
}

bool EKG_Core::VerifyTask(unsigned int Task) {
    switch (Task) {
        case EKG::Task::REORDER: {
            return this->TaskReorder;
        }

        case EKG::Task::REFOCUS: {
            return this->TaskRefocus;
        }

        case EKG::Task::BLOCKED: {
            return this->TaskBlocked;
        }

        case EKG::Task::REFRESH: {
            return this->TaskRefresh;
        }

        case EKG::Task::FREE: {
            return this->TaskFree;
        }
    }

    return false;
}