/**
 * @author Rina
 * @since 28/04/2022 at 16:28
 **/

#pragma once

#include "ekg_includes.h"
#include "ekg_abstract_ui_element.h"

#ifndef EKG_UI_ELEMENT_COMBOBOX_H
#define EKG_UI_ELEMENT_COMBOBOX_H

/**
 * Name: Combobox
 * Type: Widget
 * Description: A selection box with elements.
 * Features: Lists elements and set current.
 **/
class ekg_ui_element_combobox : public ekg_abstract_element {
protected:
    /* Settings. */
    std::string current = " ";
    std::vector<std::string> popup_element_list;

    /* States. */
    bool pressed, activy;

    /* Metrics of text. */
    float align_offset_text, offset_text, text_scale, text_width, text_height;
    unsigned int align_text_docking;

    /* Animations. */
    EKG_Smooth smooth_pressed;
public:
    /* Start of configurable methods. */
    void add(std::string &string);
    void remove(std::string &string);
    void align_text(unsigned int dock);
    /* End of configurable methods. */

    /* Start of setters & getters. */
    void set_list(const std::vector<std::string> &list);
    void set_width(float width);

    void set_offset_text(float offset);
    float get_offset_text();

    void set_scale(float scale);
    float get_scale();

    void set_press_state(bool state);
    bool is_pressed();

    void set_current(const std::string &string);
    std::string get_current();
    /* End of setters & getters. */

    /* Start of override methods. */
    std::string info_class();
    void sync_size();
    void on_parent_killed(unsigned int element_id);
    void on_pre_event(SDL_Event event);
    void on_event(SDL_Event event);
    void on_post_event(SDL_Event event);
    void on_update(const float &delta_ticks);
    void on_render(const float &render_ticks);
    /* End of override methods. */
};

#endif
