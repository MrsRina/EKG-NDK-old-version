/**
 * @author Rina
 * @since 21/03/2022 at 21:05PM
 */
#pragma once
#include "EKG_includes.h"
#include "EKG_util.h"

#define FRAME  1
#define BUTTON 2
#define SLIDER 3
#define POPUP  4

class EKG_AbstractElement {
protected:
    /* The tag of element. */
    std::string Tag;

    /* The element ID. */
    unsigned int Id;

    /* Master, children & collided stacks. */
    unsigned int MasterId;
    EKG_Stack Children, Collided;

    /* Rect of this element and scaled metrics. */
    EKG_Rect Rect;
    float ScaledX, ScaledY, ScaledWidth, ScaledHeight, SyncX, SyncY;

    /* Scissor metrics (very important to scissor works). */
    int ScissorX, ScissorY, ScissorW, ScissorH;

    /* Main flags. */
    bool Hovered, Visible = true, Disabled, Dead, NoRender;
public:
    EKG_AbstractElement() {}
    ~EKG_AbstractElement() {}

    /* Start of setters and getters. */
    bool IsMaster();
    bool IsFingerOver(float X, float Y);

    void SetTag(const std::string &NewTag);
    std::string GetTag();

    void SetId(unsigned int ElementId);
    unsigned int GetId();

    void SetMasterId(unsigned int ElementMasterId);
    unsigned int GetMasterId();

    void SetChildren(const EKG_Stack &Stack);
    EKG_Stack GetChildren();

    void SetCollided(const EKG_Stack &Stack);
    EKG_Stack GetCollided();

    void SetRect(const EKG_Rect &Rectangle);
    EKG_Rect GetRect();

    void SetRender(bool ApplyNoRender) ;
    bool IsRender();

    float GetX();
    float GetY();
    float GetWidth();
    float GetHeight();

    void SetScaled(float X, float Y, float Width, float Height);
    void SetScissor(int X, int Y, int W, int H);

    void SetHovered(bool State);
    bool IsHovered();

    void SetVisible(bool Visible);
    bool IsVisible();

    void SetDisabled(bool State);
    bool IsDisabled();

    void SetDead(bool State);
    bool IsDead();

    int GetScissorX();
    int GetScissorY();
    int GetScissorW();
    int GetScissorH();
    /* End of setters and getters. */

    /* Start of action methods. */
    virtual void Place(float X, float Y);
    virtual EKG_AbstractElement* Kill();

    virtual void SyncPos();
    virtual void SyncSize();
    /* End of action methods. */

    /* Start of invokable update methods (important to make works). */
    void Scissor();
    void Stack(EKG_Stack &Stack);
    /* End of invokable update methods. */

    /* Start of overrides methods. */
    virtual void OnKilled();
    virtual void OnCreated();

    virtual void OnPreEvent(SDL_Event Event);
    virtual void OnEvent(SDL_Event Event);
    virtual void OnPostEvent(SDL_Event Event);

    virtual void OnUpdate(float DeltaTicks);
    virtual void OnRender(float PartialTicks);
    /* End of overrides methods. */
};