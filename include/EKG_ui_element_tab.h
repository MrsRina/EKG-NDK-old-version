/**
 * @author Rina
 * @since 03/05/2022 at 16:47
 **/
#pragma once

#include "ekg_includes.h"
#include "ekg_ui_element_frame.h"

#ifndef EKG_UI_ELEMENT_TAB_H
#define EKG_UI_ELEMENT_TAB_H

/**
 * name: Tab
 * Type: Container
 * Description: A container with frames.
 * Features: Lists elements and set current.
 **/
class ekg_ui_element_tab : public ekg_ui_element_frame {
protected:
    /* Settings. */
    std::vector<ekg_data> list;
    std::string focused, activy;

    /* Metrics of tab & buttons. */
    float border_offset, size, text_offset, text_scale, button_size;

    /* Configuration. */
    unsigned int dock_tab;
    EKG_Smooth smooth_activy;

    /* Start of helper methods. */
    ekg_data get_component_by_pos(float x, float y);

    void sync_layout();
    void sort_close_component();
    /* End of helper methods. */
public:
    /* Start of configurable methods. */
    void remove(const std::string &pattern);
    void disable(const std::string &pattern);
    void enable(const std::string &pattern);
    void tab_side(unsigned int dock);
    void place(ekg_ui_element_frame* frame);
    void open(const std::string &name);
    void open(ekg_data &component);
    /* End of configurable methods. */

    /* Start of setters & getters. */
    void set_offset_text(float offset);
    float get_offset_text();

    void set_scale(float scale);
    float get_scale();

    float get_size();

    void set_border_offset(float offset);
    float get_border_offset();

    void set_button_size(float size_val);
    float get_button_size();

    std::vector<ekg_data> &get_list();
    /* End of setters & getters. */

    /* Start of override methods. */
    std::string info_class();
    void place(float x, float y);
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