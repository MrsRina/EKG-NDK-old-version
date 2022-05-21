/**
 * @author Rina
 * @since 23/03/2022 at 15:22pm
 **/
#pragma once

#include "ekg_includes.h"
#include "ekg_abstract_ui_element.h"

#ifndef EKG_UI_ELEMENT_BUTTON_H
#define EKG_UI_ELEMENT_BUTTON_H

/**
 * Name: button
 * Type: Widget
 * Description: A normal button state & check.
 * Features: Check box or normal click.
 **/
class ekg_ui_element_button : public ekg_abstract_element {
protected:
    /* States. */
    bool pressed, clicked, box_scaled, checked, box, hovered_box;
    std::string button_mode;

    /* Checkbox texture, metrics. */
    EKG_Data texture_box;
    float box_rect[4], align_offset_text, align_offset_box, offset_text, offset_box, text_width, text_height, size;
    unsigned int align_text_docking, align_box_docking;

    /* Animations. */
    EKG_Smooth smooth_box_pressed;
    EKG_Smooth smooth_pressed;
    EKG_Smooth smooth_box_activy;

    /* Start of helper methods to box (checkbox set_mode on). */
    void set_box_rect(float x, float y, float w, float h);
    bool detect_point_collide_box(float x, float y);
    /* End of helper methods. */
public:
    /* Start of configurable methods. */
    void align_box(unsigned int dock);
    void align_text(unsigned int dock);
    /* end of configurable methods. */

    /* Start of setters & getters. */
    void set_box_texture(const EKG_Data &texture);
    EKG_Data &get_check_box_texture();

    void set_mode(const std::string& mode);
    std::string get_mode();

    void set_press_state(bool state);
    bool is_pressed();

    void set_click_state(bool state);
    bool is_clicked();

    void set_check_state(bool state);
    bool is_checked();

    bool is_check_box();

    void set_offset_text(float offset);
    float get_offset_text();

    void set_offset_box(float offset);
    float get_offset_box();

    float* get_box_rect();

    void set_hovered_box_state(bool state);
    bool is_hovered_box();

    void set_size(float button_size);
    float get_size();

    float get_text_width();
    float get_text_height();

    void SetWidth(float Width);
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