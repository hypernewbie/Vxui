#pragma once

#include <algorithm>
#include <cstring>
#include <string>

#include "theme.h"
#include "../../vxui.h"
#include "../../vxui_menu.h"
#include "layout_contract.h"

struct vxui_demo_split_deck_visuals
{
    uint32_t body_font_id;
    uint32_t title_font_id;
    uint32_t section_font_id;
};

struct vxui_demo_status_summary_cfg
{
    const char* locale_name_key;
    const char* prompt_name_key;
    const char* top_name_key;
    int screen_count;
};

struct vxui_demo_sortie_screen_cfg
{
    vxui_menu_state* menu_state;
    int* selected_mission_index;
    int* difficulty_index;
    vxui_action_fn start_fn;
    vxui_action_cfg start_cfg;
    vxui_action_fn back_fn;
    vxui_action_cfg back_cfg;
    vxui_demo_status_summary_cfg status;
};

struct vxui_demo_loadout_screen_cfg
{
    vxui_menu_state* menu_state;
    int* selected_ship_index;
    int* selected_primary_index;
    int* selected_support_index;
    int* selected_system_index;
    vxui_action_fn back_fn;
    vxui_action_cfg back_cfg;
    vxui_demo_status_summary_cfg status;
};

struct vxui_demo_archives_screen_cfg
{
    vxui_menu_state* menu_state;
    int* archive_category_index;
    int* archive_entry_index;
    vxui_action_fn back_fn;
    vxui_action_cfg back_cfg;
    vxui_demo_status_summary_cfg status;
};

struct vxui_demo_records_screen_cfg
{
    vxui_menu_state* menu_state;
    int* records_board_index;
    int* records_entry_index;
    vxui_action_fn back_fn;
    vxui_action_cfg back_cfg;
    vxui_demo_status_summary_cfg status;
};

struct vxui_demo_shared_mission
{
    const char* name;
    const char* region;
    const char* briefing;
    const char* threat;
    const char* reward;
    const char* warning;
};

struct vxui_demo_shared_ship
{
    const char* name;
    const char* class_name;
    const char* summary;
    float speed;
    float shield;
    float output;
    float sync;
};

struct vxui_demo_shared_archive_entry
{
    const char* title;
    const char* subtitle;
    const char* detail;
    const char* meta;
    bool unlocked;
};

struct vxui_demo_shared_record
{
    const char* run_name;
    const char* pilot_name;
    const char* ship_name;
    const char* stage_name;
    const char* score_text;
    const char* clear_text;
    const char* note;
};

inline const vxui_demo_shared_mission VXUI_DEMO_SHARED_MISSIONS[] = {
    { "OP-01 Aegis Break", "Stratos Belt", "Breach the carrier screen, cut a corridor through interceptor rings, and keep the convoy beacon alive long enough to escape.", "Threat Tier 02 / Interceptor bloom", "Reward: Burst rail prototype", "Warning: Dense crossfire near the aft gate." },
    { "OP-02 Silent Meridian", "Aster Reach", "Thread a low-visibility debris lane and destroy the relay pair feeding hostile target locks into the sector grid.", "Threat Tier 03 / Sensor fog", "Reward: Shield lattice shard", "Warning: Heavy drift and delayed visual reads." },
    { "OP-03 Pale Crown", "Helios Verge", "Crack the defense lattice around the crown platform and extract the command archive before the perimeter resets.", "Threat Tier 04 / Shield bloom", "Reward: Archive clearance key", "Warning: Tight windows on the centerline." },
    { "OP-04 Last Metric", "Greywater Rim", "Stage a final stress sortie against an unstable fleet pocket built to show late-loop records and results states.", "Threat Tier 05 / Attrition field", "Reward: Demo clear marker", "Warning: Sustained pressure from every lane." },
};

inline const vxui_demo_shared_ship VXUI_DEMO_SHARED_SHIPS[] = {
    { "VF-17 Kestrel", "Balanced Interceptor", "Fast response frame with forgiving shields and an easy rhythm for menu testing.", 0.72f, 0.66f, 0.58f, 0.70f },
    { "VX-4 Halberd", "Heavy Breaker", "Slower but decisive burst output with broad lock-on windows and strong shield retention.", 0.42f, 0.88f, 0.86f, 0.51f },
    { "AR-9 Mistral", "Sweep Specialist", "High-mobility frame built around lane control, support uptime, and aggressive repositioning.", 0.91f, 0.46f, 0.62f, 0.80f },
    { "NX-0 Aurora", "Prototype Lattice", "Experimental chassis with volatile output curves and elevated system sync for flashy demo bars.", 0.65f, 0.55f, 0.95f, 0.93f },
};

inline const vxui_demo_shared_archive_entry VXUI_DEMO_SHARED_ARCHIVE_INTEL[] = {
    { "Intercept Packet 04", "Fleet route fragment", "Recovered fleet routing packet with enough integrity left to expose the next relay corridor and justify a second sortie pass.", "Status: decrypted", true },
    { "Signal Burn Log", "Telemetry noise capture", "A dense telemetry scrape used to justify the scanline toggle, warning accents, and command-deck radar vocabulary.", "Status: cleaned for archive", true },
};

inline const vxui_demo_shared_archive_entry VXUI_DEMO_SHARED_ARCHIVE_FRAMES[] = {
    { "Halberd Field Notes", "Maintenance digest", "Crew notes on thermal pacing, burst discipline, and why the heavy frame deserves the recommended badge in demo copy.", "Status: available", true },
    { "Aurora Lattice Memo", "Prototype clearance", "Experimental notes remain partially hidden to show locked-state visuals and mixed unlocked/locked archive lists.", "Status: restricted", false },
};

inline const vxui_demo_shared_archive_entry VXUI_DEMO_SHARED_ARCHIVE_SIGNALS[] = {
    { "Wake Sweep 7C", "Deep-space beacon trace", "A quiet signal layer used as the fiction excuse for the boot handshake and title ambience treatment.", "Status: tagged for sortie", true },
    { "Null Carrier Echo", "Redacted relay shard", "Still locked in the stub build, but visible so the archives screen demonstrates disabled rows without hiding content.", "Status: locked", false },
};

inline const vxui_demo_shared_record VXUI_DEMO_SHARED_RECORDS[] = {
    { "Run 01 / Kestrel", "Iris Vale", "VF-17 Kestrel", "Aegis Break", "3,284,120", "1CC / Normal", "Balanced clear used as the reference board row." },
    { "Run 02 / Halberd", "Rho Mercer", "VX-4 Halberd", "Silent Meridian", "4,012,770", "No-miss / Hard", "Heavy burst route with slower movement but higher relay damage." },
    { "Run 03 / Mistral", "Naomi Crest", "AR-9 Mistral", "Pale Crown", "3,776,980", "Normal / Fast clear", "Mobility route showcasing quicker stage times and lower shield padding." },
    { "Run 04 / Aurora", "Seta Noor", "NX-0 Aurora", "Last Metric", "4,488,400", "Prototype / Demo clear", "High-sync prototype route built to justify the command-deck warning colors." },
};

inline const char* const VXUI_DEMO_SHARED_DIFFICULTY_KEYS[] = { "difficulty.easy", "difficulty.normal", "difficulty.hard" };
inline const char* const VXUI_DEMO_SHARED_MISSION_NAMES[] = { "OP-01 Aegis Break", "OP-02 Silent Meridian", "OP-03 Pale Crown", "OP-04 Last Metric" };
inline const char* const VXUI_DEMO_SHARED_SHIP_NAMES[] = { "VF-17 Kestrel", "VX-4 Halberd", "AR-9 Mistral", "NX-0 Aurora" };
inline const char* const VXUI_DEMO_SHARED_PRIMARY_NAMES[] = { "Burst Rail", "Spread Lance", "Pulse Needler", "Vector Beam" };
inline const char* const VXUI_DEMO_SHARED_SUPPORT_NAMES[] = { "Shield Drone", "Mine Curtain", "Target Decoy", "Repair Pulse" };
inline const char* const VXUI_DEMO_SHARED_SYSTEM_NAMES[] = { "Afterburn Sync", "Mag Shield", "Trace Assist", "Risk Core" };
inline const char* const VXUI_DEMO_SHARED_ARCHIVE_CATEGORY_NAMES[] = { "Intel Bank", "Frame Logs", "Signal Vault" };
inline const char* const VXUI_DEMO_SHARED_ARCHIVE_INTEL_NAMES[] = { "Intercept Packet 04", "Signal Burn Log" };
inline const char* const VXUI_DEMO_SHARED_ARCHIVE_FRAME_NAMES[] = { "Halberd Field Notes", "Aurora Lattice Memo" };
inline const char* const VXUI_DEMO_SHARED_ARCHIVE_SIGNAL_NAMES[] = { "Wake Sweep 7C", "Null Carrier Echo" };
inline const char* const VXUI_DEMO_SHARED_RECORD_BOARD_NAMES[] = { "Global Board", "Ship Board", "Prototype Board" };
inline const char* const VXUI_DEMO_SHARED_RECORD_NAMES[] = { "Run 01 / Kestrel", "Run 02 / Halberd", "Run 03 / Mistral", "Run 04 / Aurora" };

static inline Clay_ElementId vxui_demo_split_deck_hashed_id( uint32_t id )
{
    return ( Clay_ElementId ) { .id = id, .offset = 0, .baseId = 0, .stringId = { 0 } };
}

inline bool vxui_demo_split_deck_locale_matches( const char* locale, const char* prefix )
{
    if ( !locale || !prefix ) {
        return false;
    }
    const size_t prefix_len = std::strlen( prefix );
    if ( std::strncmp( locale, prefix, prefix_len ) != 0 ) {
        return false;
    }
    return locale[ prefix_len ] == '\0' || locale[ prefix_len ] == '-' || locale[ prefix_len ] == '_';
}

inline const char* vxui_demo_shared_locale_name_key_from_locale( const char* locale )
{
    if ( vxui_demo_split_deck_locale_matches( locale, "ja" ) ) {
        return "locale.name.ja";
    }
    if ( vxui_demo_split_deck_locale_matches( locale, "ar" ) ) {
        return "locale.name.ar";
    }
    return "locale.name.en";
}

inline const char* vxui_demo_shared_prompt_name_key( int prompt_table_index )
{
    return prompt_table_index == 0 ? "prompt.name.keyboard" : "prompt.name.gamepad";
}

inline void vxui_demo_shared_emit_compact_meta_row(
    vxui_ctx* ctx,
    const char* id,
    const char* label_key,
    const char* value_key,
    bool rtl,
    const vxui_label_cfg* text_cfg = nullptr )
{
    CLAY( vxui_demo_split_deck_hashed_id( vxui_id( id ) ), {
        .layout = {
            .sizing = { CLAY_SIZING_FIT( 0 ), CLAY_SIZING_FIT( 0 ) },
            .childGap = ( uint16_t ) VXUI_DEMO_LAYOUT_INLINE_GAP,
            .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
            .layoutDirection = CLAY_LEFT_TO_RIGHT,
        },
    } ) {
        if ( rtl ) {
            VXUI_LABEL( ctx, value_key, text_cfg ? *text_cfg : ( vxui_label_cfg ) { 0 } );
            VXUI_LABEL( ctx, label_key, text_cfg ? *text_cfg : ( vxui_label_cfg ) { 0 } );
        } else {
            VXUI_LABEL( ctx, label_key, text_cfg ? *text_cfg : ( vxui_label_cfg ) { 0 } );
            VXUI_LABEL( ctx, value_key, text_cfg ? *text_cfg : ( vxui_label_cfg ) { 0 } );
        }
    }
}

inline void vxui_demo_shared_emit_prompt_pair(
    vxui_ctx* ctx,
    const char* id,
    const char* action_name,
    const char* label_key,
    const vxui_label_cfg* label_cfg = nullptr )
{
    CLAY( vxui_demo_split_deck_hashed_id( vxui_id( id ) ), {
        .layout = {
            .sizing = { CLAY_SIZING_FIT( 0 ), CLAY_SIZING_FIT( 0 ) },
            .childGap = ( uint16_t ) VXUI_DEMO_LAYOUT_INLINE_GAP,
            .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
            .layoutDirection = CLAY_LEFT_TO_RIGHT,
        },
    } ) {
        VXUI_PROMPT( ctx, action_name );
        VXUI_LABEL( ctx, label_key, label_cfg ? *label_cfg : ( vxui_label_cfg ) { 0 } );
    }
}

template <typename TEmitChildren>
inline void vxui_demo_shared_emit_footer_action_bar( vxui_ctx* ctx, const char* id, bool rtl, TEmitChildren&& emit_children )
{
    const vxui_demo_command_deck_theme& theme = vxui_demo_command_deck_theme_tokens();
    CLAY( vxui_demo_split_deck_hashed_id( vxui_id( id ) ), {
        .layout = {
            .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
            .padding = CLAY_PADDING_ALL( 10 ),
            .childGap = ( uint16_t ) VXUI_DEMO_LAYOUT_ROW_GAP,
            .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
            .layoutDirection = CLAY_LEFT_TO_RIGHT,
        },
        .backgroundColor = vxui_demo_clay_color( theme.action_fill ),
        .cornerRadius = CLAY_CORNER_RADIUS( 10 ),
        .border = vxui_demo_panel_border( theme.action_border, 1 ),
    } ) {
        emit_children();
    }
}

inline void vxui_demo_shared_emit_status_summary( vxui_ctx* ctx, const char* id, const vxui_demo_status_summary_cfg& status, bool rtl, uint32_t body_font_id )
{
    const vxui_demo_command_deck_theme& theme = vxui_demo_command_deck_theme_tokens();
    const vxui_label_cfg meta_cfg = vxui_demo_text_style( body_font_id, 16.0f, theme.muted_text );
    const vxui_value_cfg value_cfg = vxui_demo_value_style( body_font_id, 16.0f, theme.muted_text, "%.0f" );
    const std::string primary_id = std::string( id ) + ".row.primary";
    const std::string secondary_id = std::string( id ) + ".row.secondary";
    const std::string locale_id = std::string( id ) + ".locale";
    const std::string prompts_id = std::string( id ) + ".prompts";
    const std::string screens_id = std::string( id ) + ".screens";
    const std::string top_id = std::string( id ) + ".top";
    CLAY( vxui_demo_split_deck_hashed_id( vxui_id( id ) ), {
        .layout = {
            .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
            .padding = CLAY_PADDING_ALL( 10 ),
            .childGap = ( uint16_t ) VXUI_DEMO_LAYOUT_ROW_GAP,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
        .backgroundColor = vxui_demo_clay_color( theme.secondary_panel_fill ),
        .cornerRadius = CLAY_CORNER_RADIUS( 10 ),
        .border = vxui_demo_panel_border( theme.secondary_panel_border, 1 ),
    } ) {
        CLAY( vxui_demo_split_deck_hashed_id( vxui_id( primary_id.c_str() ) ), {
            .layout = {
                .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
                .childGap = ( uint16_t ) VXUI_DEMO_LAYOUT_ROW_GAP,
                .layoutDirection = CLAY_LEFT_TO_RIGHT,
            },
        } ) {
            vxui_demo_shared_emit_compact_meta_row( ctx, locale_id.c_str(), "status.label.locale", status.locale_name_key, rtl, &meta_cfg );
            vxui_demo_shared_emit_compact_meta_row( ctx, prompts_id.c_str(), "status.label.prompts", status.prompt_name_key, rtl, &meta_cfg );
        }

        CLAY( vxui_demo_split_deck_hashed_id( vxui_id( secondary_id.c_str() ) ), {
            .layout = {
                .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
                .childGap = ( uint16_t ) VXUI_DEMO_LAYOUT_ROW_GAP,
                .layoutDirection = CLAY_LEFT_TO_RIGHT,
            },
        } ) {
            CLAY( vxui_demo_split_deck_hashed_id( vxui_id( screens_id.c_str() ) ), {
                .layout = {
                    .sizing = { CLAY_SIZING_FIT( 0 ), CLAY_SIZING_FIT( 0 ) },
                },
            } ) {
                VXUI_VALUE( ctx, "status.label.screens", ( float ) status.screen_count, value_cfg );
            }
            vxui_demo_shared_emit_compact_meta_row( ctx, top_id.c_str(), "status.label.top", status.top_name_key, rtl, &meta_cfg );
        }
    }
}

template <typename TEmitChildren>
inline void vxui_demo_shared_emit_screen_surface(
    vxui_ctx* ctx, const char* root_id, const char* surface_id,
    float surface_width, float surface_max_height, bool rtl, TEmitChildren&& emit_children )
{
    const vxui_demo_command_deck_theme& theme = vxui_demo_command_deck_theme_tokens();
    CLAY( vxui_demo_split_deck_hashed_id( vxui_id( root_id ) ), {
        .layout = {
            .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_GROW( 0 ) },
            .padding = CLAY_PADDING_ALL( ( uint16_t ) VXUI_DEMO_LAYOUT_OUTER_PADDING ),
            .childAlignment = { .x = CLAY_ALIGN_X_CENTER },
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
        .backgroundColor = vxui_demo_clay_color( theme.app_background_base ),
    } ) {
        CLAY( vxui_demo_split_deck_hashed_id( vxui_id( surface_id ) ), {
            .layout = {
                .sizing = { CLAY_SIZING_FIXED( surface_width ), CLAY_SIZING_GROW( 0, surface_max_height ) },
                .padding = {
                    ( uint16_t ) VXUI_DEMO_LAYOUT_SURFACE_PADDING_X,
                    ( uint16_t ) VXUI_DEMO_LAYOUT_SURFACE_PADDING_X,
                    ( uint16_t ) VXUI_DEMO_LAYOUT_SURFACE_PADDING_Y,
                    ( uint16_t ) VXUI_DEMO_LAYOUT_SURFACE_PADDING_Y,
                },
                .childGap = ( uint16_t ) VXUI_DEMO_LAYOUT_SECTION_GAP,
                .childAlignment = { .x = rtl ? CLAY_ALIGN_X_RIGHT : CLAY_ALIGN_X_LEFT },
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
            },
            .backgroundColor = vxui_demo_clay_color( theme.primary_panel_fill ),
            .cornerRadius = CLAY_CORNER_RADIUS( 18 ),
            .border = vxui_demo_panel_border( theme.primary_panel_border, 1 ),
        } ) {
            emit_children();
        }
    }
}

inline vxui_menu_style vxui_demo_shared_menu_style_form_deck( const vxui_demo_split_deck_visuals& visuals, float label_lane_width )
{
    vxui_menu_style style = vxui_menu_style_form();
    style.option_wrap_by_default = true;
    style.value_lane_mode = VXUI_MENU_VALUE_LANE_GROW;
    style.body_font_id = visuals.body_font_id;
    style.title_font_id = visuals.section_font_id;
    style.badge_font_id = visuals.body_font_id;
    style.label_lane_width = label_lane_width;
    style.body_font_size = 22.0f;
    style.secondary_font_size = 18.0f;
    style.title_font_size = 28.0f;
    style.row_height = 48.0f;
    style.row_gap = 8.0f;
    style.section_gap = 14.0f;
    style.padding_x = 18.0f;
    style.padding_y = 16.0f;
    vxui_demo_apply_form_menu_theme( style );
    return style;
}

inline void vxui_demo_shared_emit_stat_bar( vxui_ctx* ctx, const vxui_demo_split_deck_visuals& visuals, const char* id, const char* label, float value )
{
    const vxui_demo_command_deck_theme& theme = vxui_demo_command_deck_theme_tokens();
    value = std::clamp( value, 0.0f, 1.0f );
    const std::string row_id = std::string( id ) + ".row";
    const std::string fill_id = std::string( id ) + ".fill";
    CLAY( vxui_demo_split_deck_hashed_id( vxui_id( row_id.c_str() ) ), {
        .layout = {
            .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
            .childGap = 8,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
    } ) {
        VXUI_LABEL( ctx, label, ( vxui_label_cfg ) {
            .font_id = visuals.body_font_id,
            .font_size = 18.0f,
            .color = theme.muted_text,
        } );
        CLAY( vxui_demo_split_deck_hashed_id( vxui_id( id ) ), {
            .layout = {
                .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIXED( 12 ) },
            },
            .backgroundColor = vxui_demo_clay_color( theme.stat_track ),
            .cornerRadius = CLAY_CORNER_RADIUS( 6 ),
        } ) {
            CLAY( vxui_demo_split_deck_hashed_id( vxui_id( fill_id.c_str() ) ), {
                .layout = {
                    .sizing = { CLAY_SIZING_FIXED( value * 220.0f ), CLAY_SIZING_GROW( 0 ) },
                },
                .backgroundColor = vxui_demo_clay_color( theme.stat_fill ),
                .cornerRadius = CLAY_CORNER_RADIUS( 6 ),
            } ) {}
        }
    }
}

inline void vxui_demo_shared_emit_screen_header( vxui_ctx* ctx, const char* id, const char* title_key, const vxui_demo_split_deck_visuals& visuals, float title_size )
{
    const vxui_demo_command_deck_theme& theme = vxui_demo_command_deck_theme_tokens();
    CLAY( vxui_demo_split_deck_hashed_id( vxui_id( id ) ), {
        .layout = {
            .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
            .padding = CLAY_PADDING_ALL( 18 ),
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
        .backgroundColor = vxui_demo_clay_color( theme.hero_surface_fill ),
        .cornerRadius = CLAY_CORNER_RADIUS( 14 ),
        .border = vxui_demo_panel_border( theme.hero_surface_border, 1 ),
    } ) {
        VXUI_LABEL( ctx, title_key, vxui_demo_text_style( visuals.title_font_id, title_size, theme.title_text ) );
    }
}

inline const vxui_demo_shared_archive_entry* vxui_demo_shared_archive_entries_for_category( int category_index, int* out_count, const char* const** out_names )
{
    switch ( std::clamp( category_index, 0, 2 ) ) {
        case 1:
            if ( out_count ) *out_count = ( int ) ( sizeof( VXUI_DEMO_SHARED_ARCHIVE_FRAMES ) / sizeof( VXUI_DEMO_SHARED_ARCHIVE_FRAMES[ 0 ] ) );
            if ( out_names ) *out_names = VXUI_DEMO_SHARED_ARCHIVE_FRAME_NAMES;
            return VXUI_DEMO_SHARED_ARCHIVE_FRAMES;
        case 2:
            if ( out_count ) *out_count = ( int ) ( sizeof( VXUI_DEMO_SHARED_ARCHIVE_SIGNALS ) / sizeof( VXUI_DEMO_SHARED_ARCHIVE_SIGNALS[ 0 ] ) );
            if ( out_names ) *out_names = VXUI_DEMO_SHARED_ARCHIVE_SIGNAL_NAMES;
            return VXUI_DEMO_SHARED_ARCHIVE_SIGNALS;
        case 0:
        default:
            if ( out_count ) *out_count = ( int ) ( sizeof( VXUI_DEMO_SHARED_ARCHIVE_INTEL ) / sizeof( VXUI_DEMO_SHARED_ARCHIVE_INTEL[ 0 ] ) );
            if ( out_names ) *out_names = VXUI_DEMO_SHARED_ARCHIVE_INTEL_NAMES;
            return VXUI_DEMO_SHARED_ARCHIVE_INTEL;
    }
}

inline void vxui_demo_render_sortie_screen_shared( vxui_ctx* ctx, const vxui_demo_split_deck_visuals& visuals, const vxui_demo_sortie_screen_cfg& cfg )
{
    const vxui_demo_command_deck_theme& theme = vxui_demo_command_deck_theme_tokens();
    const bool rtl = ctx->rtl;
    const float viewport_width = std::max( 0.0f, ( float ) ctx->cfg.screen_width - VXUI_DEMO_LAYOUT_OUTER_PADDING * 2.0f );
    const float surface_max_height = std::max( 0.0f, ( float ) ctx->cfg.screen_height - VXUI_DEMO_LAYOUT_OUTER_PADDING * 2.0f );
    const vxui_demo_split_deck_layout_spec layout =
        vxui_demo_resolve_split_deck_layout( VXUI_DEMO_SURFACE_SORTIE, viewport_width, surface_max_height, ctx->locale );
    vxui_menu_style form_style = vxui_demo_shared_menu_style_form_deck( visuals, 110.0f );
    form_style.body_font_size = 20.0f;
    form_style.secondary_font_size = 16.0f;
    form_style.badge_font_size = 14.0f;
    form_style.row_height = 44.0f;
    form_style.row_gap = 6.0f;

    vxui_demo_shared_emit_screen_surface( ctx, "sortie", "sortie.surface", layout.surface.surface_width, layout.surface_max_height, rtl, [&]() {
        vxui_demo_shared_emit_screen_header( ctx, "sortie.header", "menu.sortie", visuals, 48.0f );

        VXUI( ctx, "sortie.deck", {
            .layout = {
                .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_GROW( 200.0f ) },
                .childGap = ( uint16_t ) layout.deck_gap,
                .layoutDirection = CLAY_LEFT_TO_RIGHT,
            },
        } ) {
            VXUI( ctx, "sortie.menu_panel", {
                .layout = { .sizing = { CLAY_SIZING_FIXED( layout.primary_lane_width ), CLAY_SIZING_GROW( 0 ) } },
                .backgroundColor = vxui_demo_clay_color( theme.primary_panel_fill ),
                .cornerRadius = CLAY_CORNER_RADIUS( 12 ),
                .border = vxui_demo_panel_border( theme.primary_panel_border, 1 ),
            } ) {
                vxui_menu_begin( ctx, cfg.menu_state, "sortie.menu", ( vxui_menu_cfg ) {
                    .style = &form_style,
                    .viewport_height = layout.menu_viewport_height,
                } );
                vxui_menu_section( ctx, cfg.menu_state, "operations", "sortie.section.operations", ( vxui_menu_section_cfg ) { 0 } );
                vxui_menu_option( ctx, cfg.menu_state, "mission", "Mission", cfg.selected_mission_index, const_cast< const char** >( VXUI_DEMO_SHARED_MISSION_NAMES ), 4, ( vxui_menu_row_cfg ) {
                    .badge_text_key = "badge.selected",
                }, ( vxui_option_cfg ) { 0 } );
                vxui_menu_section( ctx, cfg.menu_state, "setup", "sortie.section.setup", ( vxui_menu_section_cfg ) { 0 } );
                vxui_menu_option( ctx, cfg.menu_state, "difficulty", "menu.difficulty", cfg.difficulty_index, const_cast< const char** >( VXUI_DEMO_SHARED_DIFFICULTY_KEYS ), 3, ( vxui_menu_row_cfg ) {
                    .badge_text_key = "badge.recommended",
                }, ( vxui_option_cfg ) { 0 } );
                vxui_menu_action( ctx, cfg.menu_state, "start", "menu.start_sortie", cfg.start_fn, ( vxui_menu_row_cfg ) {
                    .badge_text_key = "badge.recommended",
                }, cfg.start_cfg );
                vxui_menu_action( ctx, cfg.menu_state, "back", "menu.return_command_deck", cfg.back_fn, ( vxui_menu_row_cfg ) { 0 }, cfg.back_cfg );
                vxui_menu_end( ctx, cfg.menu_state );
            }

            const vxui_demo_shared_mission& mission = VXUI_DEMO_SHARED_MISSIONS[ std::clamp( cfg.selected_mission_index ? *cfg.selected_mission_index : 0, 0, 3 ) ];
            VXUI( ctx, "sortie.briefing", {
                .layout = {
                    .sizing = { CLAY_SIZING_FIXED( layout.secondary_lane_width ), CLAY_SIZING_GROW( 0 ) },
                    .padding = CLAY_PADDING_ALL( 20 ),
                    .childGap = 12,
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                },
                .backgroundColor = vxui_demo_clay_color( theme.secondary_panel_fill ),
                .cornerRadius = CLAY_CORNER_RADIUS( 12 ),
                .border = vxui_demo_panel_border( theme.secondary_panel_border, 1 ),
            } ) {
                VXUI_LABEL( ctx, mission.name, vxui_demo_text_style( visuals.title_font_id, 40.0f, theme.title_text ) );
                VXUI_LABEL( ctx, mission.region, vxui_demo_text_style( visuals.section_font_id, 24.0f, theme.accent_cool ) );
                CLAY( vxui_demo_split_deck_hashed_id( vxui_id( "sortie.briefing.body_viewport" ) ), {
                    .layout = {
                        .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_GROW( 160.0f ) },
                        .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    },
                    .clip = {
                        .horizontal = true,
                        .vertical = true,
                    },
                } ) {
                    VXUI( ctx, "sortie.briefing.body", {
                        .layout = {
                            .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
                            .padding = { 0, 0, 0, 12 },
                            .childGap = 12,
                            .layoutDirection = CLAY_TOP_TO_BOTTOM,
                        },
                    } ) {
                        VXUI_LABEL( ctx, mission.briefing, vxui_demo_text_style( visuals.body_font_id, 20.0f, theme.body_text ) );
                        VXUI_LABEL( ctx, mission.warning, vxui_demo_text_style( visuals.body_font_id, 17.0f, theme.warning_text ) );
                    }
                }
            }

            VXUI( ctx, "sortie.detail", {
                .layout = {
                    .sizing = { CLAY_SIZING_FIXED( layout.tertiary_lane_width ), CLAY_SIZING_GROW( 0 ) },
                    .padding = CLAY_PADDING_ALL( 18 ),
                    .childGap = 10,
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                },
                .backgroundColor = vxui_demo_clay_color( theme.secondary_panel_fill ),
                .cornerRadius = CLAY_CORNER_RADIUS( 12 ),
                .border = vxui_demo_panel_border( theme.secondary_panel_border, 1 ),
            } ) {
                VXUI_LABEL( ctx, mission.threat, vxui_demo_text_style( visuals.section_font_id, 24.0f, theme.section_text ) );
                VXUI_LABEL( ctx, mission.reward, vxui_demo_text_style( visuals.body_font_id, 20.0f, theme.body_text ) );
                VXUI_LABEL( ctx, "Challenge window stable", vxui_demo_text_style( visuals.body_font_id, 17.0f, theme.success_text ) );
            }
        }

        VXUI( ctx, "sortie.footer", {
            .layout = {
                .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
                .padding = CLAY_PADDING_ALL( 18 ),
                .childGap = ( uint16_t ) VXUI_DEMO_LAYOUT_ROW_GAP,
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
            },
            .backgroundColor = vxui_demo_clay_color( theme.utility_fill ),
            .cornerRadius = CLAY_CORNER_RADIUS( 12 ),
            .border = vxui_demo_panel_border( theme.utility_border, 1 ),
        } ) {
            vxui_demo_shared_emit_footer_action_bar( ctx, "sortie.footer.prompts", rtl, [&]() {
                const vxui_label_cfg prompt_cfg = vxui_demo_text_style( visuals.body_font_id, 18.0f, theme.utility_text );
                vxui_demo_shared_emit_prompt_pair( ctx, "sortie.prompt.confirm", "action.confirm", "menu.confirm", &prompt_cfg );
                vxui_demo_shared_emit_prompt_pair( ctx, "sortie.prompt.cancel", "action.cancel", "menu.cancel", &prompt_cfg );
            } );
            vxui_demo_shared_emit_status_summary( ctx, "sortie.footer.status", cfg.status, rtl, visuals.body_font_id );
        }
    } );
}

inline void vxui_demo_render_loadout_screen_shared( vxui_ctx* ctx, const vxui_demo_split_deck_visuals& visuals, const vxui_demo_loadout_screen_cfg& cfg )
{
    const vxui_demo_command_deck_theme& theme = vxui_demo_command_deck_theme_tokens();
    const bool rtl = ctx->rtl;
    const float viewport_width = std::max( 0.0f, ( float ) ctx->cfg.screen_width - VXUI_DEMO_LAYOUT_OUTER_PADDING * 2.0f );
    const float surface_max_height = std::max( 0.0f, ( float ) ctx->cfg.screen_height - VXUI_DEMO_LAYOUT_OUTER_PADDING * 2.0f );
    const vxui_demo_split_deck_layout_spec layout =
        vxui_demo_resolve_split_deck_layout( VXUI_DEMO_SURFACE_LOADOUT, viewport_width, surface_max_height, ctx->locale );
    vxui_menu_style form_style = vxui_demo_shared_menu_style_form_deck( visuals, 136.0f );

    vxui_demo_shared_emit_screen_surface( ctx, "loadout", "loadout.surface", layout.surface.surface_width, layout.surface_max_height, rtl, [&]() {
        vxui_demo_shared_emit_screen_header( ctx, "loadout.header", "menu.loadout", visuals, 46.0f );

        VXUI( ctx, "loadout.deck", {
            .layout = {
                .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_GROW( 200.0f ) },
                .childGap = ( uint16_t ) layout.deck_gap,
                .layoutDirection = CLAY_LEFT_TO_RIGHT,
            },
        } ) {
            VXUI( ctx, "loadout.menu_panel", {
                .layout = { .sizing = { CLAY_SIZING_FIXED( layout.primary_lane_width ), CLAY_SIZING_GROW( 0 ) } },
                .backgroundColor = vxui_demo_clay_color( theme.primary_panel_fill ),
                .cornerRadius = CLAY_CORNER_RADIUS( 12 ),
                .border = vxui_demo_panel_border( theme.primary_panel_border, 1 ),
            } ) {
                vxui_menu_begin( ctx, cfg.menu_state, "loadout.menu", ( vxui_menu_cfg ) {
                    .style = &form_style,
                    .viewport_height = layout.menu_viewport_height,
                } );
                vxui_menu_section( ctx, cfg.menu_state, "frames", "loadout.section.ships", ( vxui_menu_section_cfg ) { 0 } );
                vxui_menu_option( ctx, cfg.menu_state, "ship", "Frame", cfg.selected_ship_index, const_cast< const char** >( VXUI_DEMO_SHARED_SHIP_NAMES ), 4, ( vxui_menu_row_cfg ) {
                    .badge_text_key = "badge.recommended",
                }, ( vxui_option_cfg ) { 0 } );
                vxui_menu_section( ctx, cfg.menu_state, "systems", "loadout.section.weapons", ( vxui_menu_section_cfg ) { 0 } );
                vxui_menu_option( ctx, cfg.menu_state, "primary", "Primary", cfg.selected_primary_index, const_cast< const char** >( VXUI_DEMO_SHARED_PRIMARY_NAMES ), 4, ( vxui_menu_row_cfg ) { 0 }, ( vxui_option_cfg ) { 0 } );
                vxui_menu_option( ctx, cfg.menu_state, "support", "Support", cfg.selected_support_index, const_cast< const char** >( VXUI_DEMO_SHARED_SUPPORT_NAMES ), 4, ( vxui_menu_row_cfg ) { 0 }, ( vxui_option_cfg ) { 0 } );
                vxui_menu_option( ctx, cfg.menu_state, "system", "System", cfg.selected_system_index, const_cast< const char** >( VXUI_DEMO_SHARED_SYSTEM_NAMES ), 4, ( vxui_menu_row_cfg ) { 0 }, ( vxui_option_cfg ) { 0 } );
                vxui_menu_action( ctx, cfg.menu_state, "back", "menu.return_command_deck", cfg.back_fn, ( vxui_menu_row_cfg ) { 0 }, cfg.back_cfg );
                vxui_menu_end( ctx, cfg.menu_state );
            }

            const vxui_demo_shared_ship& ship = VXUI_DEMO_SHARED_SHIPS[ std::clamp( cfg.selected_ship_index ? *cfg.selected_ship_index : 0, 0, 3 ) ];
            VXUI( ctx, "loadout.detail", {
                .layout = {
                    .sizing = { CLAY_SIZING_FIXED( layout.secondary_lane_width ), CLAY_SIZING_GROW( 0 ) },
                    .childGap = ( uint16_t ) VXUI_DEMO_LAYOUT_SECTION_GAP,
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                },
            } ) {
                CLAY( vxui_demo_split_deck_hashed_id( vxui_id( "loadout.detail.body_viewport" ) ), {
                    .layout = {
                        .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_GROW( 160.0f ) },
                        .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    },
                    .clip = {
                        .horizontal = true,
                        .vertical = true,
                    },
                } ) {
                    VXUI( ctx, "loadout.detail.body", {
                        .layout = {
                            .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
                            .padding = { 0, 0, 0, 12 },
                            .childGap = ( uint16_t ) VXUI_DEMO_LAYOUT_SECTION_GAP,
                            .layoutDirection = CLAY_TOP_TO_BOTTOM,
                        },
                    } ) {
                        VXUI( ctx, "loadout.frame_card", {
                            .layout = {
                                .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
                            .padding = CLAY_PADDING_ALL( 20 ),
                            .childGap = 10,
                            .layoutDirection = CLAY_TOP_TO_BOTTOM,
                        },
                        .backgroundColor = vxui_demo_clay_color( theme.secondary_panel_fill ),
                        .cornerRadius = CLAY_CORNER_RADIUS( 12 ),
                        .border = vxui_demo_panel_border( theme.secondary_panel_border, 1 ),
                    } ) {
                        VXUI_LABEL( ctx, ship.name, vxui_demo_text_style( visuals.title_font_id, 40.0f, theme.title_text ) );
                        VXUI_LABEL( ctx, ship.class_name, vxui_demo_text_style( visuals.section_font_id, 24.0f, theme.accent_cool ) );
                        VXUI_LABEL( ctx, ship.summary, vxui_demo_text_style( visuals.body_font_id, 20.0f, theme.body_text ) );
                    }

                        VXUI( ctx, "loadout.stat_card", {
                            .layout = {
                                .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
                            .padding = CLAY_PADDING_ALL( 18 ),
                            .childGap = 10,
                            .layoutDirection = CLAY_TOP_TO_BOTTOM,
                        },
                        .backgroundColor = vxui_demo_clay_color( theme.secondary_panel_fill ),
                        .cornerRadius = CLAY_CORNER_RADIUS( 12 ),
                        .border = vxui_demo_panel_border( theme.secondary_panel_border, 1 ),
                    } ) {
                            vxui_demo_shared_emit_stat_bar( ctx, visuals, "loadout.stat.speed", "Speed", ship.speed );
                            vxui_demo_shared_emit_stat_bar( ctx, visuals, "loadout.stat.shield", "Shield", ship.shield );
                            vxui_demo_shared_emit_stat_bar( ctx, visuals, "loadout.stat.output", "Output", ship.output );
                            vxui_demo_shared_emit_stat_bar( ctx, visuals, "loadout.stat.sync", "Sync", ship.sync );
                            VXUI_LABEL( ctx, VXUI_DEMO_SHARED_PRIMARY_NAMES[ std::clamp( cfg.selected_primary_index ? *cfg.selected_primary_index : 0, 0, 3 ) ], vxui_demo_text_style( visuals.body_font_id, 17.0f, theme.section_text ) );
                            VXUI_LABEL( ctx, VXUI_DEMO_SHARED_SUPPORT_NAMES[ std::clamp( cfg.selected_support_index ? *cfg.selected_support_index : 0, 0, 3 ) ], vxui_demo_text_style( visuals.body_font_id, 17.0f, theme.body_text ) );
                            VXUI_LABEL( ctx, VXUI_DEMO_SHARED_SYSTEM_NAMES[ std::clamp( cfg.selected_system_index ? *cfg.selected_system_index : 0, 0, 3 ) ], vxui_demo_text_style( visuals.body_font_id, 17.0f, theme.success_text ) );
                        }
                    }
                }
            }
        }

        VXUI( ctx, "loadout.footer", {
            .layout = {
                .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
                .padding = CLAY_PADDING_ALL( 18 ),
                .childGap = ( uint16_t ) VXUI_DEMO_LAYOUT_ROW_GAP,
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
            },
            .backgroundColor = vxui_demo_clay_color( theme.utility_fill ),
            .cornerRadius = CLAY_CORNER_RADIUS( 12 ),
            .border = vxui_demo_panel_border( theme.utility_border, 1 ),
        } ) {
            vxui_demo_shared_emit_footer_action_bar( ctx, "loadout.footer.prompts", rtl, [&]() {
                const vxui_label_cfg prompt_cfg = vxui_demo_text_style( visuals.body_font_id, 18.0f, theme.utility_text );
                vxui_demo_shared_emit_prompt_pair( ctx, "loadout.prompt.confirm", "action.confirm", "menu.confirm", &prompt_cfg );
                vxui_demo_shared_emit_prompt_pair( ctx, "loadout.prompt.cancel", "action.cancel", "menu.cancel", &prompt_cfg );
            } );
            vxui_demo_shared_emit_status_summary( ctx, "loadout.footer.status", cfg.status, rtl, visuals.body_font_id );
        }
    } );
}

inline void vxui_demo_render_archives_screen_shared( vxui_ctx* ctx, const vxui_demo_split_deck_visuals& visuals, const vxui_demo_archives_screen_cfg& cfg )
{
    const vxui_demo_command_deck_theme& theme = vxui_demo_command_deck_theme_tokens();
    const bool rtl = ctx->rtl;
    const float viewport_width = std::max( 0.0f, ( float ) ctx->cfg.screen_width - VXUI_DEMO_LAYOUT_OUTER_PADDING * 2.0f );
    const float surface_max_height = std::max( 0.0f, ( float ) ctx->cfg.screen_height - VXUI_DEMO_LAYOUT_OUTER_PADDING * 2.0f );
    const vxui_demo_split_deck_layout_spec layout =
        vxui_demo_resolve_split_deck_layout( VXUI_DEMO_SURFACE_ARCHIVES, viewport_width, surface_max_height, ctx->locale );
    vxui_menu_style form_style = vxui_demo_shared_menu_style_form_deck( visuals, 132.0f );

    vxui_demo_shared_emit_screen_surface( ctx, "archives", "archives.surface", layout.surface.surface_width, layout.surface_max_height, rtl, [&]() {
        vxui_demo_shared_emit_screen_header( ctx, "archives.header", "menu.archives", visuals, 46.0f );

        VXUI( ctx, "archives.deck", {
            .layout = {
                .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_GROW( 200.0f ) },
                .childGap = ( uint16_t ) layout.deck_gap,
                .layoutDirection = CLAY_LEFT_TO_RIGHT,
            },
        } ) {
            VXUI( ctx, "archives.menu_panel", {
                .layout = { .sizing = { CLAY_SIZING_FIXED( layout.primary_lane_width ), CLAY_SIZING_GROW( 0 ) } },
                .backgroundColor = vxui_demo_clay_color( theme.primary_panel_fill ),
                .cornerRadius = CLAY_CORNER_RADIUS( 12 ),
                .border = vxui_demo_panel_border( theme.primary_panel_border, 1 ),
            } ) {
                int entry_count = 0;
                const char* const* entry_names = nullptr;
                vxui_demo_shared_archive_entries_for_category( cfg.archive_category_index ? *cfg.archive_category_index : 0, &entry_count, &entry_names );
                if ( cfg.archive_entry_index ) {
                    *cfg.archive_entry_index = std::clamp( *cfg.archive_entry_index, 0, std::max( 0, entry_count - 1 ) );
                }

                vxui_menu_begin( ctx, cfg.menu_state, "archives.menu", ( vxui_menu_cfg ) {
                    .style = &form_style,
                    .viewport_height = layout.menu_viewport_height,
                } );
                vxui_menu_option( ctx, cfg.menu_state, "category", "menu.category", cfg.archive_category_index, const_cast< const char** >( VXUI_DEMO_SHARED_ARCHIVE_CATEGORY_NAMES ), 3, ( vxui_menu_row_cfg ) { 0 }, ( vxui_option_cfg ) { 0 } );
                vxui_menu_option( ctx, cfg.menu_state, "entry", "Entry", cfg.archive_entry_index, const_cast< const char** >( entry_names ), entry_count, ( vxui_menu_row_cfg ) {
                    .badge_text_key = "badge.demo",
                }, ( vxui_option_cfg ) { 0 } );
                vxui_menu_action( ctx, cfg.menu_state, "back", "menu.return_command_deck", cfg.back_fn, ( vxui_menu_row_cfg ) { 0 }, cfg.back_cfg );
                vxui_menu_end( ctx, cfg.menu_state );
            }

            int entry_count = 0;
            const char* const* entry_names = nullptr;
            const vxui_demo_shared_archive_entry* entries = vxui_demo_shared_archive_entries_for_category( cfg.archive_category_index ? *cfg.archive_category_index : 0, &entry_count, &entry_names );
            const int entry_index = std::clamp( cfg.archive_entry_index ? *cfg.archive_entry_index : 0, 0, std::max( 0, entry_count - 1 ) );
            const vxui_demo_shared_archive_entry& entry = entries[ entry_index ];
            VXUI( ctx, "archives.detail", {
                .layout = {
                    .sizing = { CLAY_SIZING_FIXED( layout.secondary_lane_width ), CLAY_SIZING_GROW( 0 ) },
                    .padding = CLAY_PADDING_ALL( 20 ),
                    .childGap = 12,
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                },
                .backgroundColor = vxui_demo_clay_color( theme.secondary_panel_fill ),
                .cornerRadius = CLAY_CORNER_RADIUS( 12 ),
                .border = vxui_demo_panel_border( theme.secondary_panel_border, 1 ),
            } ) {
                VXUI_LABEL( ctx, entry.title, vxui_demo_text_style( visuals.title_font_id, 40.0f, theme.title_text ) );
                VXUI_LABEL( ctx, entry.subtitle, vxui_demo_text_style( visuals.section_font_id, 24.0f, theme.accent_cool ) );
                CLAY( vxui_demo_split_deck_hashed_id( vxui_id( "archives.detail.body_viewport" ) ), {
                    .layout = {
                        .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_GROW( 160.0f ) },
                        .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    },
                    .clip = {
                        .horizontal = true,
                        .vertical = true,
                    },
                } ) {
                    VXUI( ctx, "archives.detail.body", {
                        .layout = {
                            .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
                            .padding = { 0, 0, 0, 12 },
                            .childGap = 12,
                            .layoutDirection = CLAY_TOP_TO_BOTTOM,
                        },
                    } ) {
                        VXUI_LABEL( ctx, entry.detail, vxui_demo_text_style( visuals.body_font_id, 20.0f, theme.body_text ) );
                        VXUI_LABEL( ctx, entry.meta, vxui_demo_text_style( visuals.body_font_id, 17.0f, entry.unlocked ? theme.section_text : theme.alert_text ) );
                    }
                }
            }
        }

        VXUI( ctx, "archives.footer", {
            .layout = {
                .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
                .padding = CLAY_PADDING_ALL( 18 ),
                .childGap = ( uint16_t ) VXUI_DEMO_LAYOUT_ROW_GAP,
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
            },
            .backgroundColor = vxui_demo_clay_color( theme.utility_fill ),
            .cornerRadius = CLAY_CORNER_RADIUS( 12 ),
            .border = vxui_demo_panel_border( theme.utility_border, 1 ),
        } ) {
            vxui_demo_shared_emit_footer_action_bar( ctx, "archives.footer.prompts", rtl, [&]() {
                const vxui_label_cfg prompt_cfg = vxui_demo_text_style( visuals.body_font_id, 18.0f, theme.utility_text );
                vxui_demo_shared_emit_prompt_pair( ctx, "archives.prompt.confirm", "action.confirm", "menu.confirm", &prompt_cfg );
                vxui_demo_shared_emit_prompt_pair( ctx, "archives.prompt.cancel", "action.cancel", "menu.cancel", &prompt_cfg );
            } );
            vxui_demo_shared_emit_status_summary( ctx, "archives.footer.status", cfg.status, rtl, visuals.body_font_id );
        }
    } );
}

inline void vxui_demo_render_records_screen_shared( vxui_ctx* ctx, const vxui_demo_split_deck_visuals& visuals, const vxui_demo_records_screen_cfg& cfg )
{
    const vxui_demo_command_deck_theme& theme = vxui_demo_command_deck_theme_tokens();
    const bool rtl = ctx->rtl;
    const float viewport_width = std::max( 0.0f, ( float ) ctx->cfg.screen_width - VXUI_DEMO_LAYOUT_OUTER_PADDING * 2.0f );
    const float surface_max_height = std::max( 0.0f, ( float ) ctx->cfg.screen_height - VXUI_DEMO_LAYOUT_OUTER_PADDING * 2.0f );
    const vxui_demo_split_deck_layout_spec layout =
        vxui_demo_resolve_split_deck_layout( VXUI_DEMO_SURFACE_RECORDS, viewport_width, surface_max_height, ctx->locale );
    if ( cfg.records_entry_index ) {
        *cfg.records_entry_index = std::clamp( *cfg.records_entry_index, 0, 3 );
    }
    vxui_menu_style form_style = vxui_demo_shared_menu_style_form_deck( visuals, 132.0f );

    vxui_demo_shared_emit_screen_surface( ctx, "records", "records.surface", layout.surface.surface_width, layout.surface_max_height, rtl, [&]() {
        vxui_demo_shared_emit_screen_header( ctx, "records.header", "menu.records", visuals, 46.0f );

        VXUI( ctx, "records.deck", {
            .layout = {
                .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_GROW( 200.0f ) },
                .childGap = ( uint16_t ) layout.deck_gap,
                .layoutDirection = CLAY_LEFT_TO_RIGHT,
            },
        } ) {
            VXUI( ctx, "records.menu_panel", {
                .layout = { .sizing = { CLAY_SIZING_FIXED( layout.primary_lane_width ), CLAY_SIZING_GROW( 0 ) } },
                .backgroundColor = vxui_demo_clay_color( theme.primary_panel_fill ),
                .cornerRadius = CLAY_CORNER_RADIUS( 12 ),
                .border = vxui_demo_panel_border( theme.primary_panel_border, 1 ),
            } ) {
                vxui_menu_begin( ctx, cfg.menu_state, "records.menu", ( vxui_menu_cfg ) {
                    .style = &form_style,
                    .viewport_height = layout.menu_viewport_height,
                } );
                vxui_menu_option( ctx, cfg.menu_state, "board", "menu.board", cfg.records_board_index, const_cast< const char** >( VXUI_DEMO_SHARED_RECORD_BOARD_NAMES ), 3, ( vxui_menu_row_cfg ) { 0 }, ( vxui_option_cfg ) { 0 } );
                vxui_menu_option( ctx, cfg.menu_state, "run", "Run", cfg.records_entry_index, const_cast< const char** >( VXUI_DEMO_SHARED_RECORD_NAMES ), 4, ( vxui_menu_row_cfg ) {
                    .badge_text_key = "badge.demo",
                }, ( vxui_option_cfg ) { 0 } );
                vxui_menu_action( ctx, cfg.menu_state, "back", "menu.return_command_deck", cfg.back_fn, ( vxui_menu_row_cfg ) { 0 }, cfg.back_cfg );
                vxui_menu_end( ctx, cfg.menu_state );
            }

            const vxui_demo_shared_record& record = VXUI_DEMO_SHARED_RECORDS[ std::clamp( cfg.records_entry_index ? *cfg.records_entry_index : 0, 0, 3 ) ];
            VXUI( ctx, "records.detail", {
                .layout = {
                    .sizing = { CLAY_SIZING_FIXED( layout.secondary_lane_width ), CLAY_SIZING_GROW( 0 ) },
                    .padding = CLAY_PADDING_ALL( 20 ),
                    .childGap = 12,
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                },
                .backgroundColor = vxui_demo_clay_color( theme.secondary_panel_fill ),
                .cornerRadius = CLAY_CORNER_RADIUS( 12 ),
                .border = vxui_demo_panel_border( theme.secondary_panel_border, 1 ),
            } ) {
                VXUI_LABEL( ctx, record.run_name, vxui_demo_text_style( visuals.title_font_id, 40.0f, theme.title_text ) );
                VXUI_LABEL( ctx, record.pilot_name, vxui_demo_text_style( visuals.section_font_id, 24.0f, theme.accent_cool ) );
                CLAY( vxui_demo_split_deck_hashed_id( vxui_id( "records.detail.body_viewport" ) ), {
                    .layout = {
                        .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_GROW( 160.0f ) },
                        .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    },
                    .clip = {
                        .horizontal = true,
                        .vertical = true,
                    },
                } ) {
                    VXUI( ctx, "records.detail.body", {
                        .layout = {
                            .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
                            .padding = { 0, 0, 0, 12 },
                            .childGap = 12,
                            .layoutDirection = CLAY_TOP_TO_BOTTOM,
                        },
                    } ) {
                        VXUI_LABEL( ctx, record.ship_name, vxui_demo_text_style( visuals.body_font_id, 20.0f, theme.body_text ) );
                        VXUI_LABEL( ctx, record.stage_name, vxui_demo_text_style( visuals.body_font_id, 20.0f, theme.section_text ) );
                        VXUI_LABEL( ctx, record.score_text, vxui_demo_text_style( visuals.section_font_id, 26.0f, theme.title_text ) );
                        VXUI_LABEL( ctx, record.clear_text, vxui_demo_text_style( visuals.body_font_id, 18.0f, theme.success_text ) );
                        VXUI_LABEL( ctx, record.note, vxui_demo_text_style( visuals.body_font_id, 17.0f, theme.muted_text ) );
                    }
                }
            }
        }

        VXUI( ctx, "records.footer", {
            .layout = {
                .sizing = { CLAY_SIZING_GROW( 0 ), CLAY_SIZING_FIT( 0 ) },
                .padding = CLAY_PADDING_ALL( 18 ),
                .childGap = ( uint16_t ) VXUI_DEMO_LAYOUT_ROW_GAP,
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
            },
            .backgroundColor = vxui_demo_clay_color( theme.utility_fill ),
            .cornerRadius = CLAY_CORNER_RADIUS( 12 ),
            .border = vxui_demo_panel_border( theme.utility_border, 1 ),
        } ) {
            vxui_demo_shared_emit_footer_action_bar( ctx, "records.footer.prompts", rtl, [&]() {
                const vxui_label_cfg prompt_cfg = vxui_demo_text_style( visuals.body_font_id, 18.0f, theme.utility_text );
                vxui_demo_shared_emit_prompt_pair( ctx, "records.prompt.confirm", "action.confirm", "menu.confirm", &prompt_cfg );
                vxui_demo_shared_emit_prompt_pair( ctx, "records.prompt.cancel", "action.cancel", "menu.cancel", &prompt_cfg );
            } );
            vxui_demo_shared_emit_status_summary( ctx, "records.footer.status", cfg.status, rtl, visuals.body_font_id );
        }
    } );
}
