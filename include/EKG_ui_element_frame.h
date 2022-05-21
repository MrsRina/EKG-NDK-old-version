/**
 * @author Rina
 * @since 23/03/2022 at 15:22pm
 **/
#pragma once

#include "ekg_includes.h"
#include "ekg_abstract_ui_element.h"

#ifndef EKG_UI_ELEMENT_FRAME_H
#define EKG_UI_ELEMENT_FRAME_H

/**
 * Name: frame
 * Type: frame
 * Description: A master element to place others elements.
 * Features: Set draggable, resizable & add child (elements).
 **/
class ekg_ui_element_frame : public ekg_abstract_element {
protected:
    /* Flags used to save build config in frame. */
    unsigned int flags_dock_draggable, flags_dock_resizable;

    /* States. */
    bool dragging, pressed, free_drag_and_drop, alternative_alpha;
    int resizing;

    /* Drag and resizable references. */
    float drag_x, drag_y, drag_w, drag_h, previous_x, previous_y, previous_w, previous_h;

    /* Limits of metric and offsets. */
    float min_width = 50, min_height = 50, drag_offset_normal = 0.70f, resize_offset_normal = 0.70f;

    /* Design additional. */
    unsigned int alpha;
    EKG_Color border = EKG_Color(255, 255, 255, 255);
public:
    /* Start of configurable methods. */
    void draggable(unsigned int area);
    void resizable(unsigned int area);
    void custom_alpha(unsigned int alpha255);
    void place(ekg_abstract_element* element, float x, float y);
    void sync_parents_metric();
    /* End of configurable methods. */

    /* Start of setters and getters. */
    void set_drag_state(bool state);
    bool is_dragging();

    void set_resize_state(bool state);
    bool is_resizing();

    void set_press_state(bool state);
    bool is_pressed();

    void set_resize_offset(float offset);
    float get_resize_offset();

    void set_drag_offset(float offset);
    float get_drag_offset();

    void set_width(float width);
    void set_height(float height);

    void set_size(float width, float height);
    void SetLimit(float minimum_width, float minimum_height);

    void set_border_color(unsigned int r, unsigned int g, unsigned b, unsigned int a);
    EKG_Color &get_border_color();

    float get_min_width();
    float get_min_height();
    /* End of setters and getters. */

    /* Start of override methods. */
    std::string info_class();
    void place(float X, float Y);
    void sync_size();
    void on_pre_event(SDL_Event Event);
    void on_event(SDL_Event Event);
    void on_post_event(SDL_Event Event);
    void on_update(const float &DeltaTicks);
    void on_render(const float &PartialTicks);
    /* End of override methods. */
};

#endif