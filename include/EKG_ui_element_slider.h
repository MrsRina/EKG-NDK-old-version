/**
 * @author Rina
 * @since 16/04/2022 at 20:16pm
 **/
#pragma once

#include "ekg_includes.h"
#include "ekg_abstract_ui_element.h"

#ifndef EKG_UI_ELEMENT_SLIDER_H
#define EKG_UI_ELEMENT_SLIDER_H

/**
 * name: Slider
 * Type: Widget
 * Description: Control a bar with value min & max.
 * Features: Set draggable or make a progress bar,
 * two align (Horizontal - Vertical)
 **/
class ekg_ui_element_slider : public ekg_abstract_element {
protected:
    /* For render value. */
    std::string format;

    /* Settings. */
    double min, max, value;

    /* States. */
    bool pressed, drag, dragging, text_visible;
    unsigned int bar_orientation;

    /* Bar metrics. */
    float bar_rect[4], size, scale;

    /* Label metrics & docking of the label. */
    float text_align_x, text_align_y, text_width, text_height, offset_text, align_text;
    unsigned int text_align_docking;

    /* update slider progress with a pos factory. */
    void sync_bar(float pos_factor);
public:
    /* Start of configurable methods. */
    void orientation(const std::string& side);
    void draggable(bool state);
    void text_align(unsigned int docking);
    void text_visibility(bool visible);
    /* End of configurable methods. */

    /* Start of setters & getters. */
    void set_offset_text(float offset);
    float get_offset_text();

    void set_scale(float amount);
    float GetScale();

    void set_size(float bar_side);
    float get_size();

    void set_max(double maximum);
    double get_max();

    void set_min(double minimum);
    double get_min();

    void set_value(double val);
    double get_value();
    /* End of setters & getters. */

    /* Start of override methods. */
    std::string info_class();
    void sync_size();
    void on_pre_event(SDL_Event event);
    void on_event(SDL_Event event);
    void on_post_event(SDL_Event event);
    void on_update(const float &delta_ticks);
    void on_render(const float &render_ticks);
    /* End of override methods. */
};

#endif