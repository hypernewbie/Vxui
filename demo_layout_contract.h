#pragma once

#include <algorithm>
#include <cstring>

inline constexpr float VXUI_DEMO_LAYOUT_OUTER_PADDING = 18.0f;
inline constexpr float VXUI_DEMO_LAYOUT_SURFACE_PADDING_X = 28.0f;
inline constexpr float VXUI_DEMO_LAYOUT_SURFACE_PADDING_Y = 24.0f;
inline constexpr float VXUI_DEMO_LAYOUT_SECTION_GAP = 16.0f;
inline constexpr float VXUI_DEMO_LAYOUT_ROW_GAP = 12.0f;
inline constexpr float VXUI_DEMO_LAYOUT_INLINE_GAP = 8.0f;

enum vxui_demo_surface_kind
{
    VXUI_DEMO_SURFACE_SETTINGS = 0,
    VXUI_DEMO_SURFACE_MAIN_MENU,
};

struct vxui_demo_surface_contract
{
    float content_min_width;
    float content_target_width;
    float content_max_width;
    float desktop_min_surface_width;
    float target_fraction;
};

struct vxui_demo_surface_metrics
{
    float surface_width;
    float content_width;
    float label_lane_width;
    float control_lane_width;
};

inline bool vxui_demo_locale_needs_wide_label_lane( const char* locale )
{
    return locale && ( std::strncmp( locale, "ja", 2 ) == 0 || std::strncmp( locale, "ar", 2 ) == 0 );
}

inline const char* vxui_demo_root_id( vxui_demo_surface_kind kind )
{
    return kind == VXUI_DEMO_SURFACE_SETTINGS ? "settings" : "main_menu";
}

inline const char* vxui_demo_surface_id( vxui_demo_surface_kind kind )
{
    return kind == VXUI_DEMO_SURFACE_SETTINGS ? "settings.surface" : "main.surface";
}

inline vxui_demo_surface_contract vxui_demo_get_surface_contract( vxui_demo_surface_kind kind )
{
    switch ( kind ) {
        case VXUI_DEMO_SURFACE_SETTINGS:
            return {
                760.0f,
                900.0f,
                960.0f,
                820.0f,
                0.74f,
            };

        case VXUI_DEMO_SURFACE_MAIN_MENU:
        default:
            return {
                720.0f,
                840.0f,
                900.0f,
                760.0f,
                0.69f,
            };
    }
}

inline float vxui_demo_compute_surface_width( float viewport_width, vxui_demo_surface_kind kind )
{
    vxui_demo_surface_contract contract = vxui_demo_get_surface_contract( kind );
    float preferred = viewport_width * contract.target_fraction;
    preferred = viewport_width >= contract.content_target_width
        ? std::max( preferred, contract.content_target_width )
        : std::min( preferred, contract.content_target_width );
    preferred = std::clamp( preferred, contract.content_min_width, contract.content_max_width );
    return std::min( preferred, viewport_width );
}

inline vxui_demo_surface_metrics vxui_demo_compute_surface_metrics(
    float viewport_width,
    const char* locale,
    vxui_demo_surface_kind kind,
    float surface_padding_x = VXUI_DEMO_LAYOUT_SURFACE_PADDING_X,
    float control_gap = VXUI_DEMO_LAYOUT_ROW_GAP )
{
    float surface_width = vxui_demo_compute_surface_width( viewport_width, kind );
    float content_width = std::max( 0.0f, surface_width - surface_padding_x * 2.0f );
    float label_min_width = vxui_demo_locale_needs_wide_label_lane( locale ) ? 180.0f : 160.0f;
    float label_lane_width = std::clamp( content_width * 0.24f, label_min_width, 220.0f );
    float control_lane_width = std::max( 0.0f, content_width - label_lane_width - control_gap );
    return {
        surface_width,
        content_width,
        label_lane_width,
        control_lane_width,
    };
}
