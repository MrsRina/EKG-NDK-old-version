/**
 * @author Rina
 * @since 21/03/2022 at 21:05PM
 */
#pragma once
#include "EKG_includes.h"
#include "EKG_util.h"

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
    float ScaledX, ScaledY, ScaledWidth, ScaledHeight;

    /* Scissor metrics (very important to scissor works). */
    int ScissorX, ScissorY, ScissorW, ScissorH;

    /* Main flags. */
    bool Hovered, Visible, Disabled, Dead;
public:
    /* Start of setters and getters. */
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

    void SetX(float X);
    float GetX();

    void SetY(float Y);
    float GetY();

    void SetWidth(float Width);
    float GetWidth();

    void SetHeight(float Height);
    float GetHeight();

    void SetScaled(float X, float Y, float Width, float Height);

    void SetHovered(bool State);
    bool IsHovered();

    void SetVisible(bool Visible);
    bool IsVisible();

    void SetDisabled(bool State);
    bool IsDisabled();

    void SetDead(bool State);
    bool IsDead();
    /* End of setters and getters. */

    /* Start of action methods. */
    virtual void Kill();
    /* End of action methods. */

    /* Start of invokable update methods (important to make works). */
    void Scissor();
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