# VXUI

Too many game UI libraries are obsessed with widgets. Dropdowns, checkboxes, modals, tree views, property grids, select boxes, tab panels.

When was the last time you saw a checkbox on an arcade machine?

VXUI is built around motion and the primitives that actually ship in games. Everything animates: springs under the hood, sequenced transitions for screen changes, ambient effects for the sci-fi blip-and-spin stuff. Labels, cycling options, sliders, values that count up, input prompts. That's your lot.

It sits on top of [Clay](https://github.com/nicbarker/clay) and [VEFontCache](https://github.com/hypernewbie/vefontcache): single-header, C-first, draw list out. Their larder bro.

Your renderer, your main loop.

*VXUI is in active development. See the roadmap below.*

## What It Is

VXUI is a retained-animated, immediate-authored game UI layer built as a single-header library in [`vxui.h`](vxui.h), with an optional BR-flavored menu/form helper in [`vxui_menu.h`](vxui_menu.h).

You author UI each frame with Clay-style blocks and VXUI primitives:

- `VXUI_LABEL`
- `VXUI_VALUE`
- `VXUI_ACTION`
- `VXUI_OPTION`
- `VXUI_SLIDER`
- `VXUI_PROMPT`
- `VXUI_LIST_BEGIN` / `VXUI_LIST_ITEM` / `VXUI_LIST_END`
- `VXUI_TRAIT`

VXUI handles:

- Clay id-compatible hashing via `vxui_id()` / `vxui_idi()`
- focus and controller navigation
- per-element spring state
- disappearing-element retention and fade/settle eviction
- screen stack enter/exit snapshots
- C-defined and TOML-loaded sequences
- locale-specific font switching and RTL support
- prompt glyph tables
- ambient trait hooks
- draw-command translation from Clay into a renderer-facing VXUI draw list

VXUI does not own your renderer. `vxui_end()` returns a draw list. You consume it.

## Design Notes

- Single header on purpose.
- C-style API on purpose.
- `vxui_ctx` is publicly visible on purpose.
- The demo and tests reach into internals on purpose.
- This is closer in spirit to VEFontCache than to a hidden OO widget toolkit.

If you want a sealed framework with a private engine core, this is not that repo.

Shared demo-only support now lives under `demo/internal/` and is used by the demo runtime plus tests; it is not part of the public library surface.

## Current Feature Set

### Core Runtime

- `vxui_min_memory_size()`, `vxui_create_arena()`, and `vxui_init()` for fixed-memory setup
- `vxui_begin()` / `vxui_end()` frame lifecycle
- explicit input latches: nav, confirm, cancel, tab
- focus ownership and animated focus ring state
- renderer-facing `vxui_draw_list` with these command types:
  - `VXUI_CMD_RECT`
  - `VXUI_CMD_RECT_ROUNDED`
  - `VXUI_CMD_BORDER`
  - `VXUI_CMD_IMAGE`
  - `VXUI_CMD_TEXT`
  - `VXUI_CMD_CLIP_PUSH`
  - `VXUI_CMD_CLIP_POP`

### Motion

- spring-driven opacity, scale, slide X, and slide Y state
- first-seen snap behavior
- retained commands for fading/exiting elements
- per-frame settle detection and eviction

### Navigation

- default focus acquisition
- spatial neighbor search
- explicit directional overrides on actions and options
- focus suppression per declaration
- nested list/input consumption rules

### Localisation And Prompts

- text callback via `vxui_set_text_fn()`
- locale setter via `vxui_set_locale()`
- locale font table via `vxui_set_locale_font()`
- prompt glyph table via `vxui_set_input_table()`
- RTL-aware output path

### Screen Stack And Sequences

- `vxui_push_screen()`
- `vxui_pop_screen()`
- `vxui_replace_screen()`
- `vxui_register_seq()`
- `vxui_fire_seq()`
- `vxui_stop_seq()`
- `vxui_seq_playing()`
- `vxui_find_seq()`
- `vxui_load_seq_toml()`
- `vxui_generate_seq_c()`
- `vxui_generate_seq_toml()`

### Traits

Built-in trait ids currently include:

- `VXUI_TRAIT_PULSE`
- `VXUI_TRAIT_SCANLINE`
- `VXUI_TRAIT_SPIN`
- `VXUI_TRAIT_GLOW`
- `VXUI_TRAIT_SHAKE`
- `VXUI_TRAIT_BLIP`

You can also register your own trait callbacks with `vxui_register_trait()`.

### Optional Menu Layer

[`vxui_menu.h`](vxui_menu.h) is a separate single-header extension that sits on top of VXUI without adding a second input or focus model.

It provides:

- public POD state via `vxui_menu_state`
- public style/config structs for menu rows, sections, badges, and prompt bars
- immediate row emitters like `vxui_menu_action()`, `vxui_menu_option()`, and `vxui_menu_slider()`
- BR-inspired preset styles via `vxui_menu_style_br_title()` and `vxui_menu_style_br_panel()`
- generic form presets via `vxui_menu_style_form()` and `vxui_menu_style_compact()`
- menu-owned spring scroll state with focused-row auto-scroll scoped to the current menu
- explicit focus decorations via `VXUI_MENU_FOCUS_DECOR_NONE`, `VXUI_MENU_FOCUS_DECOR_GLOW`, and `VXUI_MENU_FOCUS_DECOR_PULSE`

Preset styles default option rows to wrapping. Override per row with `vxui_menu_row_cfg.wrap_mode`; `vxui_option_cfg` callbacks, userdata, and nav overrides do not change menu wrap behavior.

### Debug Tooling

When built with `VXUI_DEBUG`, VXUI also exposes:

- watched sequence files
- hot reload polling
- preview snapshot capture
- generated C/TOML sequence output

The demo uses this for an in-app sequence debug overlay.

## Build

The repo uses CMake and currently exposes three targets:

- `vxui_demo`
- `vxui_tests`
- `vxui_release_probe`

Top-level options:

- `VXUI_BUILD_DEMO=ON|OFF`
- `VXUI_BUILD_TESTS=ON|OFF`

### Configure And Build

```powershell
cmake -S . -B build
cmake --build build
```

### Run Tests

```powershell
ctest --test-dir build --output-on-failure
```

### Demo Dependencies

When `VXUI_BUILD_DEMO=ON`, CMake fetches demo-only FreeType and HarfBuzz with `FetchContent` and builds the TinyWindow/OpenGL demo from [`main.cpp`](main.cpp).

The canonical demo is now a playable stub shmup-style "Command Deck" front end:

- boot + title flow
- multi-screen command deck navigation
- `vxui_menu`-driven menu/form surfaces
- locale and prompt-table switching
- in-app sequence debug overlay

It still mirrors the VEFontCache renderer stack:

- TinyWindow
- OpenGL
- GLAD
- FreeType raster path
- HarfBuzz shaping path

The test binary does not require the demo renderer path.

## Quick Start

In one translation unit, define the implementations you need:

```cpp
#define CLAY_IMPLEMENTATION
#include "clay/clay.h"

#define VE_FONTCACHE_IMPL
#include "vefc/ve_fontcache.h"

#define VXUI_IMPL
#include "vxui.h"

#define VXUI_MENU_IMPL
#include "vxui_menu.h"
```

Then create a context, hand it fixed memory, and build UI each frame:

```cpp
static const char* game_text( const char* key, void* userdata )
{
    ( void ) userdata;
    return key;
}

std::vector< uint8_t > memory( ( size_t ) vxui_min_memory_size() );
vxui_ctx ctx = {};

vxui_init(
    &ctx,
    vxui_create_arena( ( uint64_t ) memory.size(), memory.data() ),
    ( vxui_config ) {
        .screen_width = 1280,
        .screen_height = 720,
        .max_elements = 256,
        .max_anim_states = 256,
        .max_sequences = 16,
    } );

vxui_set_text_fn( &ctx, game_text, nullptr );
ctx.default_font_id = 0;
ctx.default_font_size = 24.0f;
ctx.default_text_color = ( vxui_color ) { 255, 255, 255, 255 };
```

If you want proper VEFontCache-backed text measurement, set a live cache on the context with `vxui_set_fontcache( &ctx, &cache )`.

Per frame:

```cpp
vxui_begin( &ctx, dt );

VXUI( &ctx, "main_menu", {
    .layout = {
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
        .childGap = 12,
    },
} ) {
    VXUI_LABEL( &ctx, "menu.title", ( vxui_label_cfg ) {
        .font_size = 40.0f,
    } );

    VXUI_ACTION( &ctx, "menu.start", "menu.start", nullptr, ( vxui_action_cfg ) { 0 } );
    VXUI_OPTION( &ctx, "menu.difficulty", &difficulty_index, difficulty_labels, difficulty_count, ( vxui_option_cfg ) {
        .wrap = true,
    } );
    VXUI_SLIDER( &ctx, "menu.volume", &volume, 0.0f, 1.0f, ( vxui_slider_cfg ) {
        .show_value = true,
    } );
    VXUI_PROMPT( &ctx, "action.confirm" );
}

vxui_draw_list list = vxui_end( &ctx );
```

Render the returned commands in your backend. After you consume the frame's text commands, call:

```cpp
vxui_flush_text( &ctx );
```

`text_queue` is transient staging data. The draw list is the contract.

### `vxui_menu.h` Quick Examples

A BR-style title menu:

```cpp
vxui_menu_style title_style = vxui_menu_style_br_title();
vxui_menu_state title_menu = {};

VXUI( &ctx, "screen.title", {} ) {
    vxui_menu_begin( &ctx, &title_menu, "title_menu", ( vxui_menu_cfg ) {
        .style = &title_style,
        .title_key = "menu.title",
        .subtitle_key = "menu.subtitle",
        .viewport_height = 280.0f,
    } );
    vxui_menu_action( &ctx, &title_menu, "start", "menu.start", start_game, ( vxui_menu_row_cfg ) { 0 }, ( vxui_action_cfg ) { 0 } );
    vxui_menu_action( &ctx, &title_menu, "options", "menu.options", open_options, ( vxui_menu_row_cfg ) { 0 }, ( vxui_action_cfg ) { 0 } );
    vxui_menu_action( &ctx, &title_menu, "quit", "menu.quit", quit_game, ( vxui_menu_row_cfg ) { 0 }, ( vxui_action_cfg ) { 0 } );
    vxui_menu_end( &ctx, &title_menu );
}
```

A settings form:

```cpp
vxui_menu_style form_style = vxui_menu_style_form();
vxui_menu_state settings_menu = {};

VXUI( &ctx, "screen.settings", {} ) {
    vxui_menu_begin( &ctx, &settings_menu, "settings_menu", ( vxui_menu_cfg ) {
        .style = &form_style,
        .title_key = "settings.title",
        .viewport_height = 320.0f,
    } );
    vxui_menu_section( &ctx, &settings_menu, "audio", "settings.audio", ( vxui_menu_section_cfg ) { 0 } );
    vxui_menu_option( &ctx, &settings_menu, "difficulty", "settings.difficulty", &difficulty, difficulty_keys, difficulty_count, ( vxui_menu_row_cfg ) {
        .secondary_key = "settings.difficulty.help",
        .wrap_mode = VXUI_MENU_WRAP_FORCE_OFF,
    }, ( vxui_option_cfg ) {
        .on_change = on_difficulty_changed,
    } );
    vxui_menu_slider( &ctx, &settings_menu, "volume", "settings.volume", &volume, 0.0f, 1.0f, ( vxui_menu_row_cfg ) {
        .badge_text_key = "settings.badge.live",
    }, ( vxui_slider_cfg ) {
        .step = 0.05f,
        .format = "%.2f",
    } );
    vxui_menu_end( &ctx, &settings_menu );
}
```

Menu-local auto-scroll only follows rows emitted by that menu in the current frame. For option rows, the default wrap policy comes from `vxui_menu_style.option_wrap_by_default`, and `vxui_menu_row_cfg.wrap_mode` can force wrapping on or off for a specific row.

A compact pause menu with prompts:

```cpp
vxui_menu_style compact_style = vxui_menu_style_compact();
vxui_menu_state pause_menu = {};
const char* pause_prompt_actions[] = { "action.confirm", "action.cancel" };
const char* pause_prompt_labels[] = { "prompt.resume", "prompt.back" };
vxui_menu_prompt_bar_cfg pause_prompts = {
    .action_names = pause_prompt_actions,
    .label_keys = pause_prompt_labels,
    .count = 2,
};

VXUI( &ctx, "screen.pause", {} ) {
    vxui_menu_begin( &ctx, &pause_menu, "pause_menu", ( vxui_menu_cfg ) {
        .style = &compact_style,
        .viewport_height = 220.0f,
    } );
    vxui_menu_action( &ctx, &pause_menu, "resume", "pause.resume", resume_game, ( vxui_menu_row_cfg ) { 0 }, ( vxui_action_cfg ) { 0 } );
    vxui_menu_action( &ctx, &pause_menu, "quit", "pause.quit", quit_to_title, ( vxui_menu_row_cfg ) {
        .disabled = !can_quit_now,
    }, ( vxui_action_cfg ) { 0 } );
    vxui_menu_prompt_bar( &ctx, &pause_menu, "pause_prompts", &pause_prompts );
    vxui_menu_end( &ctx, &pause_menu );
}
```

The demo now uses the menu extension to drive a larger front-end sample rather than the earlier main/settings utility flow.

## Public API Surface

The main public groups in [`vxui.h`](vxui.h) are:

- lifecycle: `vxui_min_memory_size`, `vxui_create_arena`, `vxui_init`, `vxui_begin`, `vxui_end`, `vxui_flush_text`
- services: `vxui_set_fontcache`, `vxui_set_text_fn`
- input/navigation: `vxui_input_nav`, `vxui_input_confirm`, `vxui_input_cancel`, `vxui_input_tab`, `vxui_focused_id`, `vxui_set_focus`
- locale/prompt setup: `vxui_set_input_table`, `vxui_set_locale`, `vxui_set_locale_font`
- bounds lookup: `vxui_find_anim_bounds`
- screens/sequences: `vxui_push_screen`, `vxui_pop_screen`, `vxui_replace_screen`, `vxui_register_seq`, `vxui_fire_seq`, `vxui_stop_seq`, `vxui_seq_playing`, `vxui_find_seq`, `vxui_load_seq_toml`
- tooling: `vxui_generate_seq_c`, `vxui_generate_seq_toml`
- traits: `vxui_register_trait`, `VXUI_TRAIT`
- primitives: `VXUI_LABEL`, `VXUI_VALUE`, `VXUI_ACTION`, `VXUI_OPTION`, `VXUI_SLIDER`, `VXUI_PROMPT`
- menu extension: `vxui_menu_begin`, `vxui_menu_end`, `vxui_menu_section`, `vxui_menu_label`, `vxui_menu_action`, `vxui_menu_option`, `vxui_menu_slider`, `vxui_menu_badge`, `vxui_menu_prompt_bar`

## Repo Layout

- [`vxui.h`](vxui.h): the library
- [`vxui_menu.h`](vxui_menu.h): optional menu/form extension
- [`main.cpp`](main.cpp): Windows TinyWindow/OpenGL demo
- [`tests/`](tests): `utest.h`-based unit, integration, and soak coverage
- [`third_party/utest.h`](third_party/utest.h): test framework
- [`third_party/tomlc99/`](third_party/tomlc99): TOML parser used for sequence loading
- [`clay/`](clay): Clay dependency
- [`vefc/`](vefc): VEFontCache dependency and demo assets

## Testing

VXUI ships with a broad `utest.h` suite covering:

- core setup and arena behavior
- Clay id compatibility
- Clay-to-draw-command translation
- basic primitives
- advanced primitives and input consumption
- motion and retained-state behavior
- navigation and focus ring behavior
- locale/font/prompt/RTL behavior
- screen stack and snapshot transitions
- traits and nested declaration scoping
- sequence TOML loading, generation, and hot reload
- long-run soak behavior

The tests are intentionally white-box in places. This repo does not pretend the internals are private.

## Current Caveats

- The demo path is Windows/TinyWindow/OpenGL-focused right now.
- The API is usable, but still moving.
- `vxui_ctx` is intentionally exposed, so internal layout may change during active development.
- This is a game-menu runtime, not a general desktop GUI framework.

## Roadmap

- stabilize and polish the demo path
- tighten renderer examples and integration docs
- keep hardening sequence tooling and debug overlay workflow
- expand usage examples for real game-menu patterns
- continue broad test coverage as the API settles

## Why The Shape Is Like This

Because a lot of game UI isn't forms. It's focus, motion, state, prompts, transitions, count-ups, glow, and menu flow.

VXUI is trying to stay close to that.
