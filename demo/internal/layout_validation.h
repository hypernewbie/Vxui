#pragma once

#include <cmath>
#include <cstring>
#include <string>
#include <vector>

#include "layout_contract.h"
#include "main_menu_shared.h"
#include "vxui.h"

typedef bool ( *vxui_demo_layout_lookup_by_id_fn )( void* userdata, const char* id, vxui_rect* out );
typedef bool ( *vxui_demo_layout_lookup_by_hash_fn )( void* userdata, uint32_t id, vxui_rect* out );

struct vxui_demo_layout_probe
{
    const vxui_ctx* ctx;
    const vxui_draw_list* list;
    const char* screen_name;
    void* userdata;
    vxui_demo_layout_lookup_by_id_fn get_bounds_by_id;
    vxui_demo_layout_lookup_by_hash_fn get_bounds_by_hash;
    vxui_demo_layout_lookup_by_hash_fn get_anim_bounds_by_hash;
};

inline bool vxui_demo_probe_bounds( const vxui_demo_layout_probe& probe, const char* id, vxui_rect* out )
{
    return probe.get_bounds_by_id && id && probe.get_bounds_by_id( probe.userdata, id, out );
}

inline bool vxui_demo_probe_hash_bounds( const vxui_demo_layout_probe& probe, uint32_t id, vxui_rect* out )
{
    return probe.get_bounds_by_hash && probe.get_bounds_by_hash( probe.userdata, id, out );
}

inline bool vxui_demo_probe_anim_bounds( const vxui_demo_layout_probe& probe, uint32_t id, vxui_rect* out )
{
    return probe.get_anim_bounds_by_hash && probe.get_anim_bounds_by_hash( probe.userdata, id, out );
}

inline bool vxui_demo_rect_inside( vxui_rect parent, vxui_rect child, float pad = 0.0f )
{
    return child.x >= parent.x + pad - 1.0f
        && child.y >= parent.y + pad - 1.0f
        && child.x + child.w <= parent.x + parent.w - pad + 1.0f
        && child.y + child.h <= parent.y + parent.h - pad + 1.0f;
}

inline bool vxui_demo_element_fully_visible_inside( vxui_rect parent, vxui_rect child, float pad = 0.0f )
{
    return child.w > 0.0f && child.h > 0.0f && vxui_demo_rect_inside( parent, child, pad );
}

inline bool vxui_demo_rects_non_overlapping( vxui_rect a, vxui_rect b, float gap = 0.0f )
{
    return a.x + a.w + gap <= b.x || b.x + b.w + gap <= a.x || a.y + a.h + gap <= b.y || b.y + b.h + gap <= a.y;
}

inline bool vxui_demo_vertical_stack_order( vxui_rect top, vxui_rect bottom, float min_gap )
{
    return top.y + top.h + min_gap <= bottom.y + 1.0f;
}

inline bool vxui_demo_horizontal_split_order( vxui_rect left, vxui_rect right, float min_gap, bool rtl )
{
    return rtl ? right.x + right.w + min_gap <= left.x + 1.0f : left.x + left.w + min_gap <= right.x + 1.0f;
}

inline bool vxui_demo_elements_form_vertical_stack( const vxui_rect* elements, int count, float min_gap )
{
    if ( !elements || count <= 0 ) {
        return false;
    }
    for ( int i = 1; i < count; ++i ) {
        if ( !vxui_demo_vertical_stack_order( elements[ i - 1 ], elements[ i ], min_gap ) ) {
            return false;
        }
    }
    return true;
}

inline bool vxui_demo_elements_non_overlapping( const vxui_rect* elements, int count, float gap = 0.0f )
{
    if ( !elements || count <= 0 ) {
        return false;
    }
    for ( int i = 0; i < count; ++i ) {
        for ( int j = i + 1; j < count; ++j ) {
            if ( !vxui_demo_rects_non_overlapping( elements[ i ], elements[ j ], gap ) ) {
                return false;
            }
        }
    }
    return true;
}

inline bool vxui_demo_element_group_fully_visible( vxui_rect parent, const vxui_rect* elements, int count, float pad = 0.0f )
{
    if ( !elements || count <= 0 ) {
        return false;
    }
    for ( int i = 0; i < count; ++i ) {
        if ( !vxui_demo_element_fully_visible_inside( parent, elements[ i ], pad ) ) {
            return false;
        }
    }
    return true;
}

inline bool vxui_demo_region_has_single_overflow_owner( vxui_rect container, vxui_rect viewport, vxui_rect content )
{
    if ( !vxui_demo_rect_inside( container, viewport, 0.0f ) ) {
        return false;
    }
    return content.w > 0.0f && content.h > 0.0f;
}

template <typename TFilter>
inline bool vxui_demo_text_commands_inside_region(
    const vxui_draw_list* list,
    vxui_rect region,
    TFilter&& filter,
    bool allow_clip = false )
{
    if ( !list ) {
        return false;
    }

    bool found = false;
    for ( int i = 0; i < list->length; ++i ) {
        const vxui_cmd& cmd = list->commands[ i ];
        if ( cmd.type != VXUI_CMD_TEXT || !cmd.text.text || !filter( cmd ) ) {
            continue;
        }
        found = true;
        if ( allow_clip && cmd.clip_rect.w > 0.0f && cmd.clip_rect.h > 0.0f ) {
            if ( !vxui_demo_rect_inside( region, cmd.clip_rect, 0.0f ) ) {
                return false;
            }
            continue;
        }
        const vxui_rect text_rect = { cmd.text.pos.x, cmd.text.pos.y, cmd.text.size, cmd.text.size };
        if ( text_rect.w > 0.0f && text_rect.h > 0.0f ) {
            if ( !vxui_demo_rect_inside( region, text_rect, 1.0f ) ) {
                return false;
            }
        } else if ( cmd.text.pos.x < region.x - 1.0f || cmd.text.pos.x > region.x + region.w + 1.0f
            || cmd.text.pos.y < region.y - 1.0f || cmd.text.pos.y > region.y + region.h + 1.0f ) {
            return false;
        }
        if ( cmd.clip_rect.w > 0.0f && cmd.clip_rect.h > 0.0f && !vxui_demo_rect_inside( region, cmd.clip_rect, 0.0f ) ) {
            return false;
        }
    }
    return found;
}

template <typename TFilter>
inline bool vxui_demo_clipped_text_stays_inside_viewport( const vxui_draw_list* list, vxui_rect viewport, TFilter&& filter )
{
    if ( !list ) {
        return false;
    }

    bool found = false;
    for ( int i = 0; i < list->length; ++i ) {
        const vxui_cmd& cmd = list->commands[ i ];
        if ( cmd.type != VXUI_CMD_TEXT || !cmd.text.text || !filter( cmd ) ) {
            continue;
        }
        found = true;
        if ( cmd.clip_rect.w <= 0.0f || cmd.clip_rect.h <= 0.0f ) {
            return false;
        }
        if ( !vxui_demo_rect_inside( viewport, cmd.clip_rect, 0.0f ) ) {
            return false;
        }
        if ( cmd.text.pos.y < viewport.y - cmd.text.size || cmd.text.pos.y > viewport.y + viewport.h + cmd.text.size ) {
            return false;
        }
    }
    return found;
}

template <typename TFilter>
inline bool vxui_demo_no_text_in_band( const vxui_draw_list* list, float y0, float y1, TFilter&& filter )
{
    if ( !list ) {
        return true;
    }

    for ( int i = 0; i < list->length; ++i ) {
        const vxui_cmd& cmd = list->commands[ i ];
        if ( cmd.type != VXUI_CMD_TEXT || !cmd.text.text || !filter( cmd ) ) {
            continue;
        }
        vxui_rect visible = { cmd.text.pos.x, cmd.text.pos.y, cmd.text.size, cmd.text.size };
        if ( cmd.clip_rect.w > 0.0f && cmd.clip_rect.h > 0.0f ) {
            const float ix = std::max( visible.x, cmd.clip_rect.x );
            const float iy = std::max( visible.y, cmd.clip_rect.y );
            const float ir = std::min( visible.x + visible.w, cmd.clip_rect.x + cmd.clip_rect.w );
            const float ib = std::min( visible.y + visible.h, cmd.clip_rect.y + cmd.clip_rect.h );
            if ( ir <= ix || ib <= iy ) {
                continue;
            }
            visible = { ix, iy, ir - ix, ib - iy };
        }
        if ( visible.y < y1 + 1.0f && visible.y + visible.h > y0 - 1.0f ) {
            return false;
        }
    }
    return true;
}

template <typename TFilter>
inline int vxui_demo_find_text_clip_failures( const vxui_draw_list* list, vxui_rect region, TFilter&& filter )
{
    if ( !list ) {
        return 1;
    }

    int failures = 0;
    for ( int i = 0; i < list->length; ++i ) {
        const vxui_cmd& cmd = list->commands[ i ];
        if ( cmd.type != VXUI_CMD_TEXT || !cmd.text.text || !filter( cmd ) ) {
            continue;
        }
        const vxui_rect text_rect = { cmd.text.pos.x, cmd.text.pos.y, cmd.text.size, cmd.text.size };
        const vxui_rect visible_rect = ( cmd.clip_rect.w > 0.0f && cmd.clip_rect.h > 0.0f ) ? cmd.clip_rect : text_rect;
        if ( !vxui_demo_rect_inside( region, visible_rect, 0.0f ) || !vxui_demo_rect_inside( region, text_rect, 0.0f ) ) {
            ++failures;
            continue;
        }
        if ( cmd.clip_rect.w > 0.0f && cmd.clip_rect.h > 0.0f ) {
            if ( text_rect.x + text_rect.w > cmd.clip_rect.x + cmd.clip_rect.w + 1.0f
                || text_rect.y + text_rect.h > cmd.clip_rect.y + cmd.clip_rect.h + 1.0f ) {
                ++failures;
            }
        }
    }
    return failures;
}

template <typename TFilter>
inline bool vxui_demo_text_group_fully_visible( const vxui_draw_list* list, vxui_rect region, TFilter&& filter )
{
    if ( !list ) {
        return false;
    }

    bool found = false;
    for ( int i = 0; i < list->length; ++i ) {
        const vxui_cmd& cmd = list->commands[ i ];
        if ( cmd.type != VXUI_CMD_TEXT || !cmd.text.text || !filter( cmd ) ) {
            continue;
        }
        found = true;
    }
    return found && vxui_demo_find_text_clip_failures( list, region, filter ) == 0;
}

inline bool vxui_demo_focus_ring_inside_surface( const vxui_demo_layout_probe& probe, const char* surface_id )
{
    if ( !probe.ctx || probe.ctx->focused_id == 0 || !surface_id ) {
        return true;
    }

    vxui_rect surface = {};
    vxui_rect focused = {};
    if ( !vxui_demo_probe_bounds( probe, surface_id, &surface ) ) {
        return false;
    }
    if ( !vxui_demo_probe_anim_bounds( probe, probe.ctx->focused_id, &focused ) ) {
        return true;
    }
    return vxui_demo_rect_inside( surface, focused, 0.0f );
}

inline void vxui_demo_collect_sequence_warnings( const vxui_demo_layout_probe& probe, std::vector<std::string>* warnings )
{
    if ( !probe.ctx || !warnings || !probe.screen_name ) {
        return;
    }

    const char* enter_seq_name = vxui_demo_enter_sequence_name_for_screen( probe.screen_name );
    const char* exit_seq_name = std::strcmp( probe.screen_name, "settings" ) == 0 ? "settings_exit" : nullptr;
    const char* root_id = probe.screen_name;
    const char* surface_id = vxui_demo_surface_id_for_screen( probe.screen_name );
    const uint32_t root_anim_id = vxui_id( root_id );
    const uint32_t surface_anim_id = vxui_id( surface_id );

    const vxui_registered_seq* enter_seq = enter_seq_name ? vxui_find_seq( const_cast< vxui_ctx* >( probe.ctx ), enter_seq_name ) : nullptr;
    if ( enter_seq ) {
        for ( int i = 0; i < enter_seq->count; ++i ) {
            if ( enter_seq->steps[ i ].id == root_anim_id ) {
                warnings->emplace_back( "screen enter animation is still bound to the full-screen root instead of the bounded surface." );
                break;
            }
        }
        bool targets_surface = false;
        for ( int i = 0; i < enter_seq->count; ++i ) {
            if ( enter_seq->steps[ i ].id == surface_anim_id ) {
                targets_surface = true;
                break;
            }
        }
        if ( !targets_surface ) {
            warnings->emplace_back( "screen enter animation stopped targeting the bounded surface." );
        }
    }

    const vxui_registered_seq* exit_seq = exit_seq_name ? vxui_find_seq( const_cast< vxui_ctx* >( probe.ctx ), exit_seq_name ) : nullptr;
    if ( exit_seq ) {
        for ( int i = 0; i < exit_seq->count; ++i ) {
            if ( exit_seq->steps[ i ].id == root_anim_id ) {
                warnings->emplace_back( "screen exit animation is still bound to the full-screen root instead of the bounded surface." );
                break;
            }
        }
        bool targets_surface = false;
        for ( int i = 0; i < exit_seq->count; ++i ) {
            if ( exit_seq->steps[ i ].id == surface_anim_id ) {
                targets_surface = true;
                break;
            }
        }
        if ( !targets_surface ) {
            warnings->emplace_back( "screen exit animation stopped targeting the bounded surface." );
        }
    }
}

inline void vxui_demo_collect_layout_warnings( const vxui_demo_layout_probe& probe, std::vector<std::string>* warnings )
{
    if ( !warnings || !probe.screen_name ) {
        return;
    }

    const vxui_demo_surface_kind kind = vxui_demo_surface_kind_from_screen_id( probe.screen_name );
    const vxui_demo_surface_contract surface_contract = vxui_demo_get_surface_contract( kind );
    const char* root_id = probe.screen_name;
    const char* surface_id = vxui_demo_surface_id_for_screen( probe.screen_name );
    vxui_rect root = {};
    vxui_rect surface = {};

    if ( vxui_demo_probe_bounds( probe, root_id, &root ) && probe.ctx ) {
        if ( std::fabs( root.w - ( float ) probe.ctx->cfg.screen_width ) > 1.0f
            || std::fabs( root.h - ( float ) probe.ctx->cfg.screen_height ) > 1.0f ) {
            warnings->emplace_back( "screen root stopped filling the viewport; the static backdrop contract regressed." );
        }
    }
    if ( vxui_demo_probe_bounds( probe, surface_id, &surface ) && probe.ctx ) {
        const vxui_demo_viewport_class viewport_class =
            vxui_demo_classify_viewport( ( float ) probe.ctx->cfg.screen_width, ( float ) probe.ctx->cfg.screen_height );
        if ( viewport_class != VXUI_DEMO_VIEWPORT_UNSUPPORTED
            && viewport_class != VXUI_DEMO_VIEWPORT_DESKTOP_STRESS_TIGHT
            && surface.w < surface_contract.desktop_min_surface_width ) {
            warnings->emplace_back(
                "surface width fell below the desktop minimum; the demo is collapsing back toward a narrow mobile column." );
        }
        if ( !vxui_demo_rect_inside( root, surface, 0.0f ) ) {
            warnings->emplace_back( "bounded surface slipped outside the viewport root." );
        }
    }

    vxui_demo_collect_sequence_warnings( probe, warnings );

    if ( !vxui_demo_focus_ring_inside_surface( probe, surface_id ) ) {
        warnings->emplace_back( "focused element drifted outside the bounded surface." );
    }

    switch ( vxui_demo_surface_kind_from_screen_id( probe.screen_name ) ) {
        case VXUI_DEMO_SURFACE_TITLE: {
            vxui_rect hero = {};
            vxui_rect action_band = {};
            if ( vxui_demo_probe_bounds( probe, "title.hero", &hero )
                && vxui_demo_probe_bounds( probe, "title.action_band", &action_band )
                && !vxui_demo_vertical_stack_order( hero, action_band, 8.0f ) ) {
                warnings->emplace_back( "title hero and confirm band are crowding together." );
            }
            break;
        }

        case VXUI_DEMO_SURFACE_MAIN_MENU: {
            const vxui_demo_main_menu_contract contract = vxui_demo_get_main_menu_contract();
            vxui_rect hero = {};
            vxui_rect deck = {};
            vxui_rect footer = {};
            vxui_rect command_panel = {};
            vxui_rect preview_panel = {};
            vxui_rect preview_header = {};
            vxui_rect preview_viewport = {};
            vxui_rect preview_content = {};
            vxui_rect help = {};
            vxui_rect help_title = {};
            vxui_rect help_lines[ 4 ] = {};
            vxui_demo_controls_block_contract controls_contract = vxui_demo_get_controls_block_contract( surface.h );
            if ( vxui_demo_probe_bounds( probe, "main.hero", &hero ) && vxui_demo_probe_bounds( probe, "main.deck", &deck )
                && vxui_demo_probe_bounds( probe, "main.footer", &footer ) ) {
                if ( !vxui_demo_vertical_stack_order( hero, deck, contract.hero_to_deck_gap_min )
                    || !vxui_demo_vertical_stack_order( deck, footer, contract.deck_to_footer_gap_min ) ) {
                    warnings->emplace_back( "main menu sections lost their intended hero/deck/footer rhythm." );
                }
            }
            if ( vxui_demo_probe_bounds( probe, "main.command_panel", &command_panel )
                && vxui_demo_probe_bounds( probe, "main.preview_panel", &preview_panel ) ) {
                if ( command_panel.w < contract.command_min_width ) {
                    warnings->emplace_back( "main menu command lane collapsed below the intended desktop width." );
                }
                if ( preview_panel.w < contract.preview_min_width ) {
                    warnings->emplace_back( "main menu preview panel collapsed below the intended desktop width." );
                }
                if ( !vxui_demo_horizontal_split_order( command_panel, preview_panel, contract.deck_gap, false ) ) {
                    warnings->emplace_back( "main menu command and preview panels now overlap or reversed unexpectedly." );
                }
            }
            if ( vxui_demo_probe_bounds( probe, "main.preview_header", &preview_header )
                && vxui_demo_probe_bounds( probe, "main.preview_body_viewport", &preview_viewport )
                && vxui_demo_probe_bounds( probe, "main.preview_body_content", &preview_content )
                && vxui_demo_probe_bounds( probe, "main.help", &help ) ) {
                controls_contract = vxui_demo_get_controls_block_contract( surface.h, help.w );
                const vxui_demo_controls_block_copy resolved_controls_copy =
                    vxui_demo_controls_block_copy_for_locale( probe.ctx ? probe.ctx->locale : nullptr, controls_contract.compact_copy );
                if ( !vxui_demo_rect_inside( preview_panel, preview_header, 0.0f )
                    || !vxui_demo_rect_inside( preview_panel, preview_viewport, 0.0f )
                    || !vxui_demo_rect_inside( preview_panel, help, 0.0f ) ) {
                    warnings->emplace_back( "main menu preview regions escaped their owning panel." );
                }
                if ( !vxui_demo_region_has_single_overflow_owner( preview_viewport, preview_viewport, preview_content ) ) {
                    warnings->emplace_back( "main menu preview panel lost its explicit viewport/content overflow contract." );
                }
                if ( !vxui_demo_vertical_stack_order( preview_header, preview_viewport, contract.preview_header_gap ) ) {
                    warnings->emplace_back( "main menu preview header and body viewport are crowding together." );
                }
                if ( !vxui_demo_vertical_stack_order( preview_viewport, help, 8.0f ) ) {
                    warnings->emplace_back( "main menu preview help block is crowding or falling back into the clipped body viewport." );
                }
                if ( vxui_demo_probe_bounds( probe, "main.help.title", &help_title )
                    && vxui_demo_probe_bounds( probe, "main.help.line.0", &help_lines[ 0 ] )
                    && vxui_demo_probe_bounds( probe, "main.help.line.1", &help_lines[ 1 ] )
                    && vxui_demo_probe_bounds( probe, "main.help.line.2", &help_lines[ 2 ] )
                    && vxui_demo_probe_bounds( probe, "main.help.line.3", &help_lines[ 3 ] ) ) {
                    vxui_rect help_stack[ 5 ] = { help_title, help_lines[ 0 ], help_lines[ 1 ], help_lines[ 2 ], help_lines[ 3 ] };
                    if ( !vxui_demo_element_group_fully_visible( help, help_stack, 5, 0.0f )
                        || !vxui_demo_elements_form_vertical_stack( help_stack, 5, controls_contract.line_gap_min )
                        || !vxui_demo_elements_non_overlapping( help_stack, 5, 0.0f ) ) {
                        warnings->emplace_back( "main menu help lines lost their readable stacked layout." );
                    }
                }
                if ( vxui_demo_probe_anim_bounds( probe, vxui_id( resolved_controls_copy.title ), &help_title )
                    && vxui_demo_probe_anim_bounds( probe, vxui_id( resolved_controls_copy.lines[ 0 ] ), &help_lines[ 0 ] )
                    && vxui_demo_probe_anim_bounds( probe, vxui_id( resolved_controls_copy.lines[ 1 ] ), &help_lines[ 1 ] )
                    && vxui_demo_probe_anim_bounds( probe, vxui_id( resolved_controls_copy.lines[ 2 ] ), &help_lines[ 2 ] )
                    && vxui_demo_probe_anim_bounds( probe, vxui_id( resolved_controls_copy.lines[ 3 ] ), &help_lines[ 3 ] ) ) {
                    vxui_rect help_text_stack[ 5 ] = { help_title, help_lines[ 0 ], help_lines[ 1 ], help_lines[ 2 ], help_lines[ 3 ] };
                    if ( !vxui_demo_element_group_fully_visible( help, help_text_stack, 5, 0.0f )
                        || !vxui_demo_elements_form_vertical_stack( help_text_stack, 5, controls_contract.line_gap_min )
                        || !vxui_demo_elements_non_overlapping( help_text_stack, 5, 0.0f ) ) {
                        warnings->emplace_back( "main menu help text owners no longer fit the readable compact stack." );
                    }
                }
            }
            break;
        }

        case VXUI_DEMO_SURFACE_SORTIE:
        case VXUI_DEMO_SURFACE_LOADOUT:
        case VXUI_DEMO_SURFACE_ARCHIVES:
        case VXUI_DEMO_SURFACE_RECORDS: {
            const vxui_demo_split_deck_contract contract = vxui_demo_get_split_deck_contract( kind );
            const char* deck_id = kind == VXUI_DEMO_SURFACE_SORTIE ? "sortie.deck"
                : kind == VXUI_DEMO_SURFACE_LOADOUT         ? "loadout.deck"
                : kind == VXUI_DEMO_SURFACE_ARCHIVES        ? "archives.deck"
                                                           : "records.deck";
            const char* menu_panel_id = kind == VXUI_DEMO_SURFACE_SORTIE ? "sortie.menu_panel"
                : kind == VXUI_DEMO_SURFACE_LOADOUT                      ? "loadout.menu_panel"
                : kind == VXUI_DEMO_SURFACE_ARCHIVES                     ? "archives.menu_panel"
                                                                         : "records.menu_panel";
            const char* detail_id = kind == VXUI_DEMO_SURFACE_SORTIE ? "sortie.briefing"
                : kind == VXUI_DEMO_SURFACE_LOADOUT                  ? "loadout.detail"
                : kind == VXUI_DEMO_SURFACE_ARCHIVES                 ? "archives.detail"
                                                                     : "records.detail";
            const char* tertiary_id = kind == VXUI_DEMO_SURFACE_SORTIE ? "sortie.detail" : nullptr;
            const std::string footer_id = std::string( vxui_demo_root_id( kind ) ) + ".footer";

            vxui_rect deck = {};
            vxui_rect footer = {};
            vxui_rect menu_panel = {};
            vxui_rect detail = {};
            if ( vxui_demo_probe_bounds( probe, deck_id, &deck ) && vxui_demo_probe_bounds( probe, footer_id.c_str(), &footer )
                && !vxui_demo_vertical_stack_order( deck, footer, contract.deck_to_footer_gap_min ) ) {
                warnings->emplace_back( "split-deck screen footer is crowding the main deck body." );
            }
            if ( vxui_demo_probe_bounds( probe, menu_panel_id, &menu_panel ) && menu_panel.w < contract.primary_min_width ) {
                warnings->emplace_back( "split-deck menu lane collapsed below the intended desktop width." );
            }
            if ( vxui_demo_probe_bounds( probe, detail_id, &detail ) && detail.w < contract.secondary_min_width ) {
                warnings->emplace_back( "split-deck detail lane collapsed below the intended desktop width." );
            }
            if ( tertiary_id ) {
                vxui_rect tertiary = {};
                if ( vxui_demo_probe_bounds( probe, tertiary_id, &tertiary ) && tertiary.w < contract.tertiary_min_width ) {
                    warnings->emplace_back( "split-deck tertiary lane collapsed below the intended desktop width." );
                }
            }
            break;
        }

        case VXUI_DEMO_SURFACE_SETTINGS: {
            const vxui_demo_settings_contract contract = vxui_demo_get_settings_contract();
            vxui_rect header = {};
            vxui_rect body_panel = {};
            vxui_rect footer = {};
            vxui_rect menu_viewport = {};
            vxui_rect menu_content = {};
            if ( vxui_demo_probe_bounds( probe, "settings.header", &header )
                && vxui_demo_probe_bounds( probe, "settings.body_panel", &body_panel )
                && vxui_demo_probe_bounds( probe, "settings.footer", &footer ) ) {
                if ( !vxui_demo_vertical_stack_order( header, body_panel, contract.header_to_body_gap_min )
                    || !vxui_demo_vertical_stack_order( body_panel, footer, contract.body_to_footer_gap_min ) ) {
                    warnings->emplace_back( "settings sections lost their header/body/footer rhythm." );
                }
                if ( footer.y + footer.h > surface.y + surface.h + 1.0f ) {
                    warnings->emplace_back( "settings footer slipped below the bounded surface." );
                }
            }
            if ( vxui_demo_probe_hash_bounds( probe, vxui_idi( "settings.body_menu", 1 ), &menu_viewport )
                && vxui_demo_probe_hash_bounds( probe, vxui_idi( "settings.body_menu", 2 ), &menu_content ) ) {
                if ( menu_viewport.w < surface.w * contract.menu_viewport_min_width_fraction ) {
                    warnings->emplace_back( "settings body menu viewport collapsed below a usable desktop width." );
                }
                if ( !vxui_demo_region_has_single_overflow_owner( body_panel, menu_viewport, menu_content ) ) {
                    warnings->emplace_back( "settings body menu stopped producing a single explicit overflow owner." );
                }
            }
            break;
        }

        case VXUI_DEMO_SURFACE_CREDITS: {
            vxui_rect stack = {};
            vxui_rect footer = {};
            if ( vxui_demo_probe_bounds( probe, "credits.stack", &stack )
                && vxui_demo_probe_bounds( probe, "credits.footer", &footer )
                && !vxui_demo_vertical_stack_order( stack, footer, 8.0f ) ) {
                warnings->emplace_back( "credits footer is crowding the stack content." );
            }
            break;
        }

        case VXUI_DEMO_SURFACE_LAUNCH_STUB:
        case VXUI_DEMO_SURFACE_RESULTS_STUB: {
            const std::string footer_id = std::string( probe.screen_name ) + ".footer";
            vxui_rect footer = {};
            if ( vxui_demo_probe_bounds( probe, footer_id.c_str(), &footer ) && footer.y + footer.h > surface.y + surface.h + 1.0f ) {
                warnings->emplace_back( "stub flow footer slipped outside the bounded surface." );
            }
            break;
        }

        default:
            break;
    }
}
