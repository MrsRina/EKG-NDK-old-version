/**
 * @author Rina
 * @since 21/03/2022 at 21:05PM
 */
#pragma once
#include "ekg_includes.h"
#include "ekg_util.h"

#ifndef EKG_ABSTRACT_UI_ELEMENT_H
#define EKG_ABSTRACT_UI_ELEMENT_H

/**
 * Abstract element.
 */
class ekg_abstract_element {
protected:
    /* The tag of element. */
    std::string tag;

    /* The element ID. */
    unsigned int id;

    /* Master, children & collided stacks. */
    unsigned int master_id;
    ekg_stack children_stack;

    /* rect of this element and scaled metrics. */
    ekg_rect rect;
    float scaled_x, scaled_y, scaled_width, scale_height, sync_x, sync_y;

    /* scissor_sync metrics (very important to scissor works). */
    int scissor_x, scissor_y, scissor_w, scissor_h;

    /* Main flags. */
    bool hovered, disabled, dead;
    unsigned int visible;
public:
    ekg_abstract_element();
    ~ekg_abstract_element();

    /* Start of setters and getters. */
    bool is_master();
    bool collide_with_pos(float x, float y);

    void set_tag(const std::string &new_tag);
    std::string get_tag();

    void set_id(unsigned int element_id);
    unsigned int get_id();

    virtual void set_master_id(unsigned int element_id);
    unsigned int get_master_id();

    void set_children_stack(const ekg_stack &the_stack);
    ekg_stack GetChildren();

    void set_rect(const ekg_rect &rectangle);
    ekg_rect get_rect();

    float get_x();
    float get_y();
    float get_width();
    float get_height();

    void set_scaled(float x, float y, float width, float height);
    void set_scissor(int x, int y, int w, int h);

    void set_hover_state(bool state);
    bool is_hovered();

    virtual void set_visibility_flag(unsigned int flag);
    unsigned int get_visibility_flag();

    void set_disable_state(bool state);
    bool is_disabled();

    void set_dead_state(bool state);
    bool is_dead();

    int get_scissor_x();
    int get_scissor_y();
    int get_scissor_w();
    int get_scissor_h();
    /* End of setters and getters. */

    /* Start of action methods. */
    virtual void place(float x, float y);
    virtual void remove(unsigned int element_id);
    virtual void kill();

    virtual void sync_pos();
    virtual void sync_size();
    /* End of action methods. */

    /* Start of invokable update methods (important to make works). */
    void scissor_sync();
    void push_children_ids_to_stack(ekg_stack &stack);
    /* End of invokable update methods. */

    /* Start of overrides methods. */
    virtual std::string info_class();

    virtual void on_parent_killed(unsigned int element_id);
    virtual void on_master_killed(unsigned int element_id);

    virtual void on_pre_event(SDL_Event Event);
    virtual void on_event(SDL_Event Event);
    virtual void on_post_event(SDL_Event Event);

    virtual void on_update(const float &DeltaTicks);
    virtual void on_render(const float &PartialTicks);
    /* End of overrides methods. */
};

#endif