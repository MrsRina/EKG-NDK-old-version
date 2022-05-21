/**
 * @author Rina
 * @since 21/03/2022 at 21:05PM
 */
#pragma once
#include "ekg_includes.h"
#include "ekg_abstract_ui_element.h"
#include "ekg_shader.h"
#include "ekg_tessellator.h"

/**
 * The core of ekg, where everything is processed.
 */
class ekg_core {
protected:
    /* Buffers element used in context gui. */
    std::vector<ekg_abstract_element*> buffer_update, buffer_copy, buffer_new;
    std::array<ekg_abstract_element*, 256> buffer_render = {};

    uint8_t buffer_size;
    bool should_swap_buffers;

    /* Stacks. */
    ekg_stack stack_ids_selected;

    /* Focused id & most high id to generate unique ids. */
    unsigned int focused_element_id, previous_id_used = 1;

    /* References. */
    std::string focused_element_tag, focused_element_info_class;

    /* Stack handler methods. */
    void reorder_stack(unsigned int element_id);
    void refresh_stack();
    void free_stack();
    void swap_stack();

    /* Environment. */
    bool task_blocked, task_refresh, task_reorder, task_refocus, task_free;
    unsigned int id_from_task;
public:
    ekg_core() {}

    /* Start of managers and controllers to ekg. */
    ekg_shader_manager shader_manager;
    ekg_font_renderer font_renderer;
    ekg_color_theme color_theme;
    ekg_timing* timing;
    /* End of managers and controllers. */

    /* Start of setters & getters. */
    int get_size_of_update_elements();
    int get_size_of_render_elements();
    unsigned int get_focused_element_id();

    std::string get_focused_element_tag();
    std::string get_focused_element_info_class();
    /* End of setters & getters. */

    /* Start of important methods. */
    void init();
    void quit();

    void add_element_to_queue(ekg_abstract_element* element);
    void remove_element_by_id(unsigned int element_id);

    ekg_abstract_element* get_element_by_id(unsigned int Id);
    unsigned int next_id();

    void sync_stack_scaled_metrics(float x, float y, float w, float h, const ekg_stack &stack);
    void sync_scissor(ekg_abstract_element* element);
    /* End of important methods. */

    /* Start of concurrent methods. */
    void add_task_to_queue(unsigned int task, unsigned int id = 0);
    bool verify_task(unsigned int task);

    void on_event(SDL_Event event);
    void on_update(const float &delta_ticks);
    void OnRender(const float &render_ticks);
    /* End of concurrent methods. */
};