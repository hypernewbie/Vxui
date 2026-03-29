#pragma once

// Shared infrastructure for user-story layout tests.
// Content packs, supported viewport matrix, and readable-outcome assertions.

#include <cmath>
#include <cstring>
#include <cstdio>

// ---------------------------------------------------------------------------
// Supported viewport matrix
// ---------------------------------------------------------------------------

struct story_viewport
{
    const char* name;
    int width;
    int height;
};

inline constexpr story_viewport STORY_VIEWPORTS[] = {
    { "preferred_desktop",    1600, 900 },
    { "desktop_normal",       1280, 720 },
    { "desktop_short",        1280, 680 },
    { "desktop_compact",      1280, 648 },
    { "desktop_tight",        1100, 720 },
    { "desktop_tight_compact",1100, 648 },
    { "desktop_stress",        960, 720 },
};

inline constexpr int STORY_VIEWPORT_COUNT =
    ( int ) ( sizeof( STORY_VIEWPORTS ) / sizeof( STORY_VIEWPORTS[ 0 ] ) );

inline constexpr const char* STORY_LOCALES[] = { "en", "ja-JP", "ar" };
inline constexpr int STORY_LOCALE_COUNT = 3;

// ---------------------------------------------------------------------------
// Content packs — deterministic realistic strings for authored screens
// ---------------------------------------------------------------------------

enum story_content_class
{
    STORY_CONTENT_SHORT = 0,
    STORY_CONTENT_NORMAL,
    STORY_CONTENT_LONG,
    STORY_CONTENT_EXTREME,
    STORY_CONTENT_DENSE_BADGE,
};

struct story_menu_row_content
{
    const char* label;
    const char* value;
    const char* badge;
    const char* secondary;
};

struct story_content_pack
{
    const char* name;
    story_content_class content_class;
    const char* locale;

    const char* title;
    const char* subtitle;
    const char* body;

    story_menu_row_content rows[ 8 ];
    int row_count;

    const char* footer_status;
    const char* footer_prompt;

    const char* help_title;
    const char* help_lines[ 4 ];
};

inline story_content_pack story_content_pack_en_normal()
{
    return {
        "en_normal", STORY_CONTENT_NORMAL, "en",
        "Command Deck", "Production-shape front-end stub",
        "Mission briefing and tactical overview for the current sortie assignment.",
        {
            { "Sortie",   nullptr, "REC",  nullptr },
            { "Loadout",  nullptr, "DEMO", nullptr },
            { "Archives", nullptr, "DEMO", nullptr },
            { "Settings", nullptr, "REC",  nullptr },
            { "Records",  nullptr, "DEMO", nullptr },
            { "Credits",  nullptr, "DEMO", nullptr },
            { "Quit Demo",nullptr, nullptr, nullptr },
        },
        7,
        "Locale: English  Prompts: Keyboard",
        "Confirm",
        "Controls",
        { "Up/Down: Navigate", "Left/Right: Adjust", "Enter: Confirm", "Esc: Back" },
    };
}

inline story_content_pack story_content_pack_en_long()
{
    return {
        "en_long", STORY_CONTENT_LONG, "en",
        "Command Deck", "Production-shape front-end stub with extended subtitle copy",
        "Extended mission briefing that describes the full tactical situation with warnings about hostile radar coverage and recommended approach vectors for the squadron.",
        {
            { "Primary Sortie Assignment",  "Mission Alpha-7", "RECOMMENDED", "High-priority tasking" },
            { "Loadout Configuration",      "Custom Set B",    "DEMO",        "Modified standard kit" },
            { "Archives & Intelligence",    "3 new entries",   "DEMO",        nullptr },
            { "Advanced Settings Panel",    "Modified",        "RECOMMENDED", "User changes pending" },
            { "Combat Records & Rankings",  "Updated",         "DEMO",        nullptr },
            { "Credits & Acknowledgements", nullptr,           "DEMO",        nullptr },
            { "Quit Demo Application",      nullptr,           nullptr,       nullptr },
        },
        7,
        "Locale: English  Prompts: Keyboard  Screens: 1  Top: Command Deck",
        "Confirm",
        "Controls",
        { "Up/Down: Navigate menus", "Left/Right: Adjust values", "Enter: Confirm selection", "Esc: Return to previous" },
    };
}

inline story_content_pack story_content_pack_en_extreme()
{
    return {
        "en_extreme", STORY_CONTENT_EXTREME, "en",
        "Command Deck", "Production-shape front-end validation stub with LONGTOKEN_LONGTOKEN stress",
        "Extended extreme briefing copy with repeated status bands tactical warnings and LONGTOKEN_LONGTOKEN_LONGTOKEN to verify that text origins and clip rectangles stay inside the panel surface boundaries under height pressure.",
        {
            { "Primary Sortie Assignment Override", "Mission Alpha-7 Extended Deployment", "RECOMMENDED+PRIORITY", "High-priority tactical tasking with deadline" },
            { "Loadout Configuration Extended",     "Custom Loadout Set B (Modified)",      "DEMO+MODIFIED",       "Modified standard kit with override" },
            { "Archives Intelligence Database",     "3 new classified entries",             "DEMO+NEW",            "Classification pending review" },
            { "Advanced Settings Configuration",    "User Modified Settings Applied",       "RECOMMENDED+CHANGED", "User configuration changes pending save" },
            { "Combat Records Rankings Board",      "Updated Scoreboard Available",         "DEMO+UPDATED",        "New rankings posted this session" },
            { "Credits Acknowledgements List",      nullptr,                                "DEMO",                nullptr },
            { "Quit Demo Application Now",          nullptr,                                nullptr,               nullptr },
        },
        7,
        "Locale: English  Prompts: Keyboard  Screens: 1  Top: Command Deck",
        "Confirm",
        "Controls",
        { "Up/Down arrows: Navigate through menu options", "Left/Right arrows: Adjust selected values", "Enter key: Confirm current selection", "Escape key: Return to the previous screen" },
    };
}

inline story_content_pack story_content_pack_ja_normal()
{
    return {
        "ja_normal", STORY_CONTENT_NORMAL, "ja-JP",
        "コマンドデッキ", "本番形状フロントエンドスタブ",
        "現在の出撃任務に関する作戦概要と戦術的状況報告。",
        {
            { "出撃",       nullptr, "推奨",   nullptr },
            { "ロードアウト", nullptr, "デモ",   nullptr },
            { "アーカイブ",   nullptr, "デモ",   nullptr },
            { "設定",       nullptr, "推奨",   nullptr },
            { "記録",       nullptr, "デモ",   nullptr },
            { "クレジット",   nullptr, "デモ",   nullptr },
            { "デモ終了",    nullptr, nullptr, nullptr },
        },
        7,
        "言語: 日本語  プロンプト: キーボード",
        "決定",
        "操作方法",
        { "上下: メニュー移動", "左右: 値の変更", "決定: 確認", "ESC: 戻る" },
    };
}

inline story_content_pack story_content_pack_ar_normal()
{
    return {
        "ar_normal", STORY_CONTENT_NORMAL, "ar",
        "سطح القيادة", "واجهة تجريبية بشكل إنتاجي",
        "إحاطة المهمة والنظرة العامة التكتيكية لمهمة الطلعة الحالية.",
        {
            { "الطلعة",     nullptr, "موصى",  nullptr },
            { "العتاد",     nullptr, "تجريبي", nullptr },
            { "الأرشيف",    nullptr, "تجريبي", nullptr },
            { "الإعدادات",   nullptr, "موصى",  nullptr },
            { "السجلات",    nullptr, "تجريبي", nullptr },
            { "الاعتمادات", nullptr, "تجريبي", nullptr },
            { "إنهاء العرض", nullptr, nullptr, nullptr },
        },
        7,
        "اللغة: العربية  الأزرار: لوحة مفاتيح",
        "تأكيد",
        "أدوات التحكم",
        { "أعلى/أسفل: التنقل", "يمين/يسار: التعديل", "إدخال: التأكيد", "خروج: الرجوع" },
    };
}

inline story_content_pack story_content_pack_en_dense_badge()
{
    return {
        "en_dense_badge", STORY_CONTENT_DENSE_BADGE, "en",
        "Command Deck", "Badge-heavy validation stub",
        "Every row has a badge, value, and secondary text simultaneously.",
        {
            { "Sortie",   "Mission Alpha",    "RECOMMENDED", "Priority tasking" },
            { "Loadout",  "Custom Set B",     "MODIFIED",    "Changed from default" },
            { "Archives", "3 new entries",    "UPDATED",     "New intel available" },
            { "Settings", "User modified",    "CHANGED",     "Pending save" },
            { "Records",  "Board A selected", "NEW",         "Rankings posted" },
            { "Credits",  "Full list",        "DEMO",        nullptr },
            { "Quit",     nullptr,            nullptr,       nullptr },
        },
        7,
        "Locale: English  Prompts: Keyboard",
        "Confirm",
        "Controls",
        { "Up/Down: Navigate", "Left/Right: Adjust", "Enter: Confirm", "Esc: Back" },
    };
}

// Content pack registry
inline const story_content_pack STORY_CONTENT_PACKS[] = {
    story_content_pack_en_normal(),
    story_content_pack_en_long(),
    story_content_pack_en_extreme(),
    story_content_pack_ja_normal(),
    story_content_pack_ar_normal(),
    story_content_pack_en_dense_badge(),
};

inline constexpr int STORY_CONTENT_PACK_COUNT = 6;

// ---------------------------------------------------------------------------
// Readable-outcome assertion helpers
// ---------------------------------------------------------------------------
// These complement the geometric assertions in layout_validation.h with
// higher-level readability checks designed for story tests.

// Minimum gap in pixels for two vertically stacked text lines to be readable.
inline constexpr float STORY_MIN_READABLE_LINE_GAP = 2.0f;

// Minimum height in pixels for a text region to be considered readable.
inline constexpr float STORY_MIN_READABLE_HEIGHT = 10.0f;

// Check that a rectangle has non-degenerate readable dimensions.
inline bool story_rect_is_readable( vxui_rect r )
{
    return r.w >= STORY_MIN_READABLE_HEIGHT && r.h >= STORY_MIN_READABLE_HEIGHT;
}

// Check that an element fits fully inside a viewport (no clipping unless in a scroll owner).
inline bool story_element_fully_inside( vxui_rect parent, vxui_rect child, float tolerance = 1.0f )
{
    return child.x >= parent.x - tolerance
        && child.y >= parent.y - tolerance
        && ( child.x + child.w ) <= ( parent.x + parent.w ) + tolerance
        && ( child.y + child.h ) <= ( parent.y + parent.h ) + tolerance;
}

// Check that two elements do not vertically overlap.
inline bool story_no_vertical_overlap( vxui_rect a, vxui_rect b, float tolerance = 0.5f )
{
    float a_bottom = a.y + a.h;
    float b_bottom = b.y + b.h;
    return ( a_bottom <= b.y + tolerance ) || ( b_bottom <= a.y + tolerance );
}

// Check that a sequence of rects form a readable vertical stack (each below previous, readable gap).
inline bool story_readable_vertical_stack( const vxui_rect* elements, int count, float min_gap = STORY_MIN_READABLE_LINE_GAP )
{
    for ( int i = 1; i < count; ++i ) {
        float prev_bottom = elements[ i - 1 ].y + elements[ i - 1 ].h;
        float gap = elements[ i ].y - prev_bottom;
        if ( gap < min_gap - 0.5f ) {
            return false;
        }
    }
    return true;
}

// Check that no pair of rects in a group overlap vertically.
inline bool story_group_no_overlaps( const vxui_rect* elements, int count, float tolerance = 0.5f )
{
    for ( int i = 0; i < count; ++i ) {
        for ( int j = i + 1; j < count; ++j ) {
            if ( !story_no_vertical_overlap( elements[ i ], elements[ j ], tolerance ) ) {
                return false;
            }
        }
    }
    return true;
}

// Check that every rect in a group is fully inside a parent region.
inline bool story_group_fully_visible( vxui_rect parent, const vxui_rect* elements, int count, float tolerance = 1.0f )
{
    for ( int i = 0; i < count; ++i ) {
        if ( !story_element_fully_inside( parent, elements[ i ], tolerance ) ) {
            return false;
        }
    }
    return true;
}

// Check that two regions are separated vertically (top is above bottom with min gap).
inline bool story_vertical_order( vxui_rect top, vxui_rect bottom, float min_gap = 0.0f )
{
    float top_bottom = top.y + top.h;
    return ( bottom.y - top_bottom ) >= ( min_gap - 0.5f );
}

// Check that two elements do not overlap horizontally.
inline bool story_no_horizontal_overlap( vxui_rect a, vxui_rect b, float tolerance = 0.5f )
{
    float a_right = a.x + a.w;
    float b_right = b.x + b.w;
    return ( a_right <= b.x + tolerance ) || ( b_right <= a.x + tolerance );
}

// Check that a badge does not visually occupy the same horizontal space as a value.
inline bool story_badge_does_not_cover_value( vxui_rect value, vxui_rect badge, float tolerance = 2.0f )
{
    if ( badge.w < 1.0f || badge.h < 1.0f ) {
        return true;  // No badge visible
    }
    // Badge must not significantly overlap value text horizontally
    float value_right = value.x + value.w;
    float badge_right = badge.x + badge.w;
    float overlap_left = std::fmax( value.x, badge.x );
    float overlap_right = std::fmin( value_right, badge_right );
    float horizontal_overlap = std::fmax( 0.0f, overlap_right - overlap_left );
    // Also must not overlap vertically
    float overlap_top = std::fmax( value.y, badge.y );
    float overlap_bottom = std::fmin( value.y + value.h, badge.y + badge.h );
    float vertical_overlap = std::fmax( 0.0f, overlap_bottom - overlap_top );
    float overlap_area = horizontal_overlap * vertical_overlap;
    float value_area = value.w * value.h;
    // Allow up to tolerance pixels of area overlap
    return overlap_area <= tolerance * tolerance || value_area < 1.0f;
}

// Diagnostic: print a rect for debugging.
inline void story_print_rect( const char* label, vxui_rect r )
{
    std::printf( "  %s: x=%.1f y=%.1f w=%.1f h=%.1f\n", label, r.x, r.y, r.w, r.h );
}
