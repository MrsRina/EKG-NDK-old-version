/**
 * @author Rina
 * @since 16/04/2022 at 20:16pm
 **/
#pragma once

#include "ekg_includes.h"
#include "ekg_abstract_ui_element.h"

#ifndef EKG_UI_ELEMENT_POPUP_H
#define EKG_UI_ELEMENT_POPUP_H

/**
 * name: popup
 * Type: Container
 * Description: list elements and make selectable
 * Features: enable or disable elements
 **/
class ekg_ui_element_popup : public ekg_abstract_element {
protected:
    /* Settings. */
    std::vector<ekg_data> list;
    std::string selected, focused, activy;

    /* States. */
    bool pressed, clicked, is_master_popup, show;

    /* Metrics of popup & buttons. */
    float max_width, max_height, text_offset, text_scale;

    /* Animations. */
    float last_tick;
    EKG_Smooth smooth_pressed;

    /* Help to return hovered components. */
    ekg_data get_component_by_pos(float x, float y);
public:
    /* Start of configurable methods. */
    void insert(const std::vector<std::string> &new_list);
    void remove(const std::string &pattern);
    void disable(const std::string &pattern);
    void enable(const std::string &pattern);
    /* End of configurable methods. */

    /* Start of setters & getters. */
    void set_offset_text(float offset_text);
    float get_offset_text();

    void set_press_state(bool state);
    bool is_pressed();

    void set_click_state(bool state);
    bool is_clicked();

    void set_width(float width);
    void set_master_id(unsigned int id);

    void set_scale(float scale);
    float get_scale();

    std::vector<ekg_data> &get_list();
    std::string &get_focused();
    std::string &get_clicked();

    void get_path(std::string &previous_path);

    float get_max_width();
    float get_max_height();
    /* End of setters & getters. */

    /* Start of action methods. */
    void place(ekg_ui_element_popup* element);
    void place(float x, float y, float bounding_y);
    /* End of action methods. */

    /* Start of override methods. */
    std::string info_class();
    void set_visibility_flag(unsigned int flag);

    void kill();
    void place(float x, float y);
    void sync_size();
    void on_pre_event(SDL_Event event);
    void on_event(SDL_Event event);
    void on_post_event(SDL_Event event);
    void on_update(const float &delta_ticks);
    void on_render(const float &render_ticks);
    /* End of override methods. */
};

#endif