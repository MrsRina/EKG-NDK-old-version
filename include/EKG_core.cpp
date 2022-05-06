#include "EKG_core.h"
#include "EKG.h"

void EKG_Core::RemoveElement(unsigned int ElementId) {
    std::vector<EKG_AbstractElement*> NewBufferRender;
    std::vector<EKG_AbstractElement*> NewBufferUpdate;

    this->BufferRender.fill(0);
    this->BufferSize = 0;

    for (EKG_AbstractElement* Elements : this->BufferUpdate) {
        if (Elements->GetId() == ElementId) {
            Elements->Kill();

            delete Elements;
            continue;
        }

        NewBufferUpdate.push_back(Elements);

        if (Elements->IsVisible() && Elements->IsRender()) {
            this->BufferRender[this->BufferSize++] = Elements;
        }
    }

    this->BufferUpdate = NewBufferUpdate;
}

void EKG_Core::AddElement(EKG_AbstractElement* Element) {
    this->BufferUpdate.push_back(Element);

    if (Element->IsVisible()) {
        this->BufferRender[this->BufferSize++] = Element;
    }
}

void EKG_Core::OnEvent(SDL_Event Event) {
    if (Event.type != SDL_FINGERDOWN && Event.type != SDL_FINGERUP && Event.type != SDL_FINGERMOTION && Event.type != EKG::Event::POPUP) {
        return;
    }

    if (Event.type == SDL_FINGERDOWN) {
        this->ActionHappening = false;
    }

    this->FocusedId = 0;

    // Get focusing element id.
    for (EKG_AbstractElement* Element : this->BufferUpdate) {
        // Update flags.
        Element->OnPreEvent(Event);

        // The next superior element is set.
        if (Element->IsHovered() && Element->IsVisible()) {
            this->FocusedId = Element->GetId();
        }

        // Reset flags.
        Element->OnPostEvent(Event);
    }

    this->BufferRender.fill(0);
    this->BufferSize = 0;

    // Call all events.
    for (EKG_AbstractElement* Element : this->BufferUpdate) {
        if (Element == NULL) {
            continue;
        }

        if (Element->GetId() == this->FocusedId) {
            Element->OnPreEvent(Event);

            // Communicate for EKG environment the current focused element.
            this->FocusedTag = Element->GetTag();
            this->FocusedType = Element->InfoClass();
        }

        Element->OnEvent(Event);

        if (Element->IsVisible() && Element->IsRender()) {
            this->BufferRender[this->BufferSize++] = Element;
        }

        this->SyncScissor(Element);
    }

    if (Event.type == SDL_FINGERDOWN || Event.type == SDL_FINGERUP) {
        if (this->NeededReorder) {
            for (EKG_AbstractElement* Element : this->BufferUpdate) {
                if (Element->IsHovered() && Element->IsVisible()) {
                    this->FocusedId = Element->GetId();

                    // Communicate for EKG environment the current focused element.
                    this->FocusedTag = Element->GetTag();
                    this->FocusedType = Element->InfoClass();
                }
            }

            this->NeededReorder = false;
        }

        this->ReorderStack();
    }
}

void EKG_Core::OnUpdate(const float &DeltaTicks) {
    if (this->NeededRefresh) {
        this->ResetStack();
        this->RefreshStack();
        this->NeededRefresh = false;
    }
}

void EKG_Core::OnRender(const float &PartialTicks) {
    for (EKG_AbstractElement* &Element : this->BufferRender) {
        if (Element == NULL) {
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
    for (EKG_AbstractElement* Element : this->BufferUpdate) {
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

void EKG_Core::ResetStack() {
    std::vector<EKG_AbstractElement*> NewBufferOfUpdate;

    this->BufferRender.fill(0);
    this->BufferSize = 0;

    // Get current elements list.
    for (EKG_AbstractElement* Element : this->BufferUpdate) {
        if (Element->IsDead()) {
            delete Element;
            continue;
        }

        // If is not one master we add in new list.
        if (Element->GetMasterId() == 0 && !Element->IsMaster()) {
            NewBufferOfUpdate.push_back(Element);

            if (Element->IsVisible() && Element->IsRender()) {
                this->BufferRender[this->BufferSize++] = Element;
            }
        }
    }

    EKG_Stack Stack;

    // Get current elements list.
    for (EKG_AbstractElement* Element : this->BufferUpdate) {
        if (Element->IsDead()) {
            delete Element;
            continue;
        }

        if (Element->IsMaster()) {
            EKG_Stack ConcurrentStack;
            Element->Stack(ConcurrentStack);

            // Update stack now pushing back the fixed stack list.
            for (unsigned int IDs : ConcurrentStack.StackedIds) {
                auto* Elements = (EKG_AbstractElement *) this->GetElementById(IDs);

                if (Elements != NULL) {
                    NewBufferOfUpdate.push_back(Elements);

                    if (Elements->IsVisible() && Elements->IsRender()) {
                        this->BufferRender[this->BufferSize++] = Elements;
                    }
                }
            }

            Stack = ConcurrentStack;
        }
    }

    this->BufferUpdate = NewBufferOfUpdate;
}

void EKG_Core::ReorderStack() {
    // Reset if is 0.
    if (this->FocusedId == 0) {
        this->FocusedTag = "NULL";
        this->FocusedType = "NULL";
        return;
    }

    EKG_Stack Current;
    EKG_Stack Focused;

    for (EKG_AbstractElement* Element : this->BufferUpdate) {
        if (Element->IsDead()) {
            delete Element;
            continue;
        }

        // Ignore if contains in focused.
        if (Focused.Contains(Element->GetId())) {
            continue;
        }

        // Verify child in this element.
        EKG_Stack Stack;
        Element->Stack(Stack);

        // If contains element id focused we set the stack value in focused.
        if (Stack.Contains(this->FocusedId)) {
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

        if (Element == NULL) {
            continue;
        }

        NewBufferOfUpdate.push_back(Element);

        if (Element->IsVisible() && Element->IsRender()) {
            this->BufferRender[this->BufferSize++] = Element;
        }
    }

    // Put the focused ids at top of list.
    for (unsigned int IDs : Focused.StackedIds) {
        auto* Element = (EKG_AbstractElement*) this->GetElementById(IDs);

        if (Element == NULL) {
            continue;
        }

        NewBufferOfUpdate.push_back(Element);

        if (Element->IsVisible() && Element->IsRender()) {
            this->BufferRender[this->BufferSize++] = Element;
        }
    }

    this->BufferUpdate = NewBufferOfUpdate;
}

EKG_AbstractElement* EKG_Core::GetElementById(unsigned int Id) {
    for (EKG_AbstractElement* Element : this->BufferUpdate) {
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
        auto* Element = (EKG_AbstractElement*) this->GetElementById(IDs);

        if (Element == NULL) {
            continue;
        }

        Element->SetScaled(X, Y, W, H);
        Element->SyncPos();
        Element->SetRender(Element->GetRect().CollideWithRect(X, Y, W, H));

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

int EKG_Core::GetSizeOfRenderElements() const {
    return this->BufferSize;
}

int EKG_Core::GetFocusedElementId() const {
    return (int) this->FocusedId;
}

std::string &EKG_Core::GetFocusedTag() {
    return this->FocusedTag;
}

std::string &EKG_Core::GetFocusedType() {
    return this->FocusedType;
}

void EKG_Core::ActionHappen() {
    this->ActionHappening = true;
}

bool EKG_Core::IsActionHappening() const {
    return this->ActionHappening;
}

void EKG_Core::Refresh() {
    this->NeededRefresh = true;
}

void EKG_Core::RefreshStack() {
    std::vector<EKG_AbstractElement*> NewBufferOfUpdate;
    this->BufferRender.fill(0);
    this->BufferSize = 0;

    for (EKG_AbstractElement* Element : this->BufferUpdate) {
        if (Element->IsDead()) {
            delete Element;
            continue;
        }

        NewBufferOfUpdate.push_back(Element);

        if (Element->IsRender() && Element->IsVisible()) {
            this->BufferRender[this->BufferSize++] = Element;
        }
    }

    this->BufferUpdate = NewBufferOfUpdate;
}

void EKG_Core::RefreshNeeded() {
    this->NeededReorder = true;
}
