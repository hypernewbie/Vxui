#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <string>

#include "clay/clay.h"
typedef struct ve_fontcache ve_fontcache;

#define VXUI( ctx, id_str, ... ) \
    CLAY( CLAY_ID( id_str ), __VA_ARGS__ )

typedef struct vxui_vec2
{
    float x;
    float y;
} vxui_vec2;

typedef struct vxui_rect
{
    float x;
    float y;
    float w;
    float h;
} vxui_rect;

typedef struct vxui_color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} vxui_color;

typedef struct vxui_arena
{
    uint8_t* memory;
    uint64_t capacity;
    uint64_t offset;
} vxui_arena;

typedef enum vxui_cmd_type
{
    VXUI_CMD_RECT,
    VXUI_CMD_RECT_ROUNDED,
    VXUI_CMD_IMAGE,
    VXUI_CMD_TEXT,
    VXUI_CMD_CLIP_PUSH,
    VXUI_CMD_CLIP_POP,
} vxui_cmd_type;

typedef struct vxui_draw_cmd_rect
{
    vxui_rect bounds;
    vxui_color color;
} vxui_draw_cmd_rect;

typedef struct vxui_draw_cmd_rect_rounded
{
    vxui_rect bounds;
    vxui_color color;
    float radius;
} vxui_draw_cmd_rect_rounded;

typedef struct vxui_draw_cmd_image
{
    vxui_rect bounds;
    void* image_data;
} vxui_draw_cmd_image;

typedef struct vxui_draw_cmd_text
{
    vxui_vec2 pos;
    uint32_t font_id;
    float size;
    vxui_color color;
    const char* text;
} vxui_draw_cmd_text;

typedef struct vxui_cmd
{
    vxui_cmd_type type;
    vxui_rect clip_rect;
    union
    {
        vxui_draw_cmd_rect rect;
        vxui_draw_cmd_rect_rounded rect_rounded;
        vxui_draw_cmd_image image;
        vxui_draw_cmd_text text;
        struct
        {
            vxui_rect rect;
        } clip;
    };
} vxui_cmd;

typedef struct vxui_focus_ring_cfg
{
    vxui_color color;
    float border_width;
    float corner_radius;
    float spring_stiffness;
    float spring_damping;
} vxui_focus_ring_cfg;

typedef struct vxui_config
{
    int screen_width;
    int screen_height;
    int exit_anim_max_time;
    vxui_focus_ring_cfg focus_ring;
    int max_elements;
    int max_anim_states;
    int max_sequences;
    int max_seq_steps;
} vxui_config;

typedef struct vxui_draw_list
{
    vxui_cmd* commands;
    int length;
} vxui_draw_list;

typedef struct vxui_ctx
{
    vxui_config cfg;
    vxui_arena arena;

    uint64_t frame_index;
    float delta_time;

    ve_fontcache* fontcache;

    vxui_cmd* commands;
    int command_count;
    int command_capacity;

    vxui_draw_cmd_text* text_queue;
    int text_queue_count;
    int text_queue_capacity;

    uint32_t pending_nav_mask;
    bool pending_confirm;
    bool pending_cancel;
    int pending_tab;

    Clay_Context* clay_ctx;
    Clay_Arena clay_arena;
    Clay_ErrorHandler clay_error_handler;
    const char* ( *text_fn )( const char* key, void* userdata );
    void* text_fn_userdata;
    Clay_RenderCommandArray clay_render_commands;
    vxui_rect current_clip_rect;
    bool clip_active;

    vxui_rect* clip_stack;
    int clip_stack_count;
    int clip_stack_capacity;

    char* frame_string_buffer;
    int frame_string_count;
    int frame_string_capacity;

    /* vxui_anim_state* anim_states; */
    /* int anim_state_count; */
    /* int anim_state_capacity; */

    /* uint32_t focused_id; */
} vxui_ctx;

uint64_t vxui_min_memory_size( void );
vxui_arena vxui_create_arena( uint64_t size, void* memory );
void vxui_init( vxui_ctx* ctx, vxui_arena arena, vxui_config cfg );
void vxui_begin( vxui_ctx* ctx, float delta_time );
vxui_draw_list vxui_end( vxui_ctx* ctx );
void vxui_flush_text( vxui_ctx* ctx );
void vxui_set_fontcache( vxui_ctx* ctx, ve_fontcache* cache );
void vxui_set_text_fn( vxui_ctx* ctx, const char* ( *fn )( const char* key, void* userdata ), void* userdata );
uint32_t vxui_id( const char* label );
uint32_t vxui_idi( const char* label, int index );

#ifdef VXUI_IMPL

#include "vefc/ve_fontcache.h"

enum
{
    VXUI__DEFAULT_SCREEN_WIDTH = 1920,
    VXUI__DEFAULT_SCREEN_HEIGHT = 1080,
    VXUI__DEFAULT_EXIT_ANIM_MAX_TIME = 500,
    VXUI__DEFAULT_MAX_ELEMENTS = 8192,
    VXUI__DEFAULT_MAX_ANIM_STATES = 1024,
    VXUI__DEFAULT_MAX_SEQUENCES = 64,
    VXUI__DEFAULT_MAX_SEQ_STEPS = 1024,
    VXUI__DEFAULT_FRAME_STRING_BYTES = VXUI__DEFAULT_MAX_ELEMENTS * 128,
};

static void* vxui__arena_alloc( vxui_arena* arena, uint64_t size, uint64_t align );
static uint32_t vxui__hash_bytes( const void* data, size_t size, uint32_t seed );
static Clay_String vxui__clay_string_from_cstr( const char* text );
static vxui_rect vxui__rect_from_clay_box( Clay_BoundingBox box );
static vxui_color vxui__color_from_clay( Clay_Color color );
static void vxui__handle_clay_error( Clay_ErrorData errorData );
static Clay_Dimensions vxui__measure_text( Clay_StringSlice text, Clay_TextElementConfig* cfg, void* userdata );
static void vxui__reset_frame_buffers( vxui_ctx* ctx );
static vxui_cmd* vxui__push_cmd( vxui_ctx* ctx, vxui_cmd_type type );
static vxui_draw_cmd_text* vxui__push_text( vxui_ctx* ctx );
static const char* vxui__push_frame_string( vxui_ctx* ctx, const char* text, size_t length );
static const char* vxui__copy_slice_text( vxui_ctx* ctx, Clay_StringSlice text );
static void vxui__translate_clay_commands( vxui_ctx* ctx );
static vxui_config vxui__sanitize_config( vxui_config cfg );

static void* vxui__arena_alloc( vxui_arena* arena, uint64_t size, uint64_t align )
{
    if ( align == 0 ) {
        align = 1;
    }

    uint64_t mask = align - 1;
    uint64_t base = ( uint64_t ) ( uintptr_t ) arena->memory;
    uint64_t at = ( ( base + arena->offset + mask ) & ~mask ) - base;
    if ( at + size > arena->capacity ) {
        return nullptr;
    }

    void* ret = arena->memory + at;
    arena->offset = at + size;
    return ret;
}

static uint32_t vxui__hash_bytes( const void* data, size_t size, uint32_t seed )
{
    const uint8_t* bytes = ( const uint8_t* ) data;
    uint32_t hash = 2166136261u ^ seed;

    for ( size_t i = 0; i < size; ++i ) {
        hash ^= bytes[ i ];
        hash *= 16777619u;
    }

    return hash;
}

static Clay_String vxui__clay_string_from_cstr( const char* text )
{
    Clay_String ret = {};
    ret.isStaticallyAllocated = false;
    ret.length = text ? ( int32_t ) std::strlen( text ) : 0;
    ret.chars = text ? text : "";
    return ret;
}

static vxui_rect vxui__rect_from_clay_box( Clay_BoundingBox box )
{
    vxui_rect rect = {};
    rect.x = box.x;
    rect.y = box.y;
    rect.w = box.width;
    rect.h = box.height;
    return rect;
}

static vxui_color vxui__color_from_clay( Clay_Color color )
{
    vxui_color ret = {};

    float r = color.r < 0.0f ? 0.0f : ( color.r > 255.0f ? 255.0f : color.r );
    float g = color.g < 0.0f ? 0.0f : ( color.g > 255.0f ? 255.0f : color.g );
    float b = color.b < 0.0f ? 0.0f : ( color.b > 255.0f ? 255.0f : color.b );
    float a = color.a < 0.0f ? 0.0f : ( color.a > 255.0f ? 255.0f : color.a );

    ret.r = ( uint8_t ) r;
    ret.g = ( uint8_t ) g;
    ret.b = ( uint8_t ) b;
    ret.a = ( uint8_t ) a;
    return ret;
}

static void vxui__handle_clay_error( Clay_ErrorData errorData )
{
    std::fprintf(
        stderr,
        "vxui clay error: %.*s\n",
        errorData.errorText.length,
        errorData.errorText.chars ? errorData.errorText.chars : "" );
}

static Clay_Dimensions vxui__measure_text( Clay_StringSlice text, Clay_TextElementConfig* cfg, void* userdata )
{
    vxui_ctx* ctx = ( vxui_ctx* ) userdata;
    Clay_Dimensions dims = {};
    dims.height = cfg ? ( float ) cfg->fontSize : 0.0f;

    if ( !ctx || !ctx->fontcache || !cfg ) {
        return dims;
    }

    ve_fontcache* cache = ctx->fontcache;
    if ( cfg->fontId >= cache->entry.size() ) {
        return dims;
    }

    size_t verts = cache->drawlist.vertices.size();
    size_t indices = cache->drawlist.indices.size();
    size_t dcalls = cache->drawlist.dcalls.size();
    ve_fontcache_vec2 cursor = cache->cursor_pos;

    std::u8string temp( ( const char8_t* ) text.chars, ( size_t ) text.length );
    ve_fontcache_draw_text( cache, ( ve_font_id ) cfg->fontId, temp, 0.0f, 0.0f, 1.0f, 1.0f, true );
    ve_fontcache_vec2 end = ve_fontcache_get_cursor_pos( cache );

    cache->drawlist.vertices.resize( verts );
    cache->drawlist.indices.resize( indices );
    cache->drawlist.dcalls.resize( dcalls );
    cache->cursor_pos = cursor;

    dims.width = end.x;
    return dims;
}

static void vxui__reset_frame_buffers( vxui_ctx* ctx )
{
    ctx->command_count = 0;
    ctx->text_queue_count = 0;
    ctx->pending_nav_mask = 0;
    ctx->pending_confirm = false;
    ctx->pending_cancel = false;
    ctx->pending_tab = 0;
    ctx->clay_render_commands = Clay_RenderCommandArray {};
    ctx->current_clip_rect = vxui_rect {};
    ctx->clip_active = false;
    ctx->clip_stack_count = 0;
    ctx->frame_string_count = 0;
}

static vxui_cmd* vxui__push_cmd( vxui_ctx* ctx, vxui_cmd_type type )
{
    if ( ctx->command_count >= ctx->command_capacity ) {
        return nullptr;
    }

    vxui_cmd* cmd = &ctx->commands[ ctx->command_count++ ];
    *cmd = vxui_cmd {};
    cmd->type = type;
    return cmd;
}

static vxui_draw_cmd_text* vxui__push_text( vxui_ctx* ctx )
{
    if ( ctx->text_queue_count >= ctx->text_queue_capacity ) {
        return nullptr;
    }

    vxui_draw_cmd_text* text = &ctx->text_queue[ ctx->text_queue_count++ ];
    *text = vxui_draw_cmd_text {};
    return text;
}

static const char* vxui__push_frame_string( vxui_ctx* ctx, const char* text, size_t length )
{
    if ( !ctx->frame_string_buffer || ctx->frame_string_count + ( int ) length + 1 > ctx->frame_string_capacity ) {
        assert( false && "vxui frame string buffer exhausted" );
        return "";
    }

    char* dest = ctx->frame_string_buffer + ctx->frame_string_count;
    if ( length > 0 ) {
        std::memcpy( dest, text, length );
    }
    dest[ length ] = '\0';
    ctx->frame_string_count += ( int ) length + 1;
    return dest;
}

static const char* vxui__copy_slice_text( vxui_ctx* ctx, Clay_StringSlice text )
{
    if ( !text.chars || text.length <= 0 ) {
        return vxui__push_frame_string( ctx, "", 0 );
    }
    return vxui__push_frame_string( ctx, text.chars, ( size_t ) text.length );
}

static void vxui__translate_clay_commands( vxui_ctx* ctx )
{
    ctx->current_clip_rect = vxui_rect {};
    ctx->clip_active = false;
    ctx->clip_stack_count = 0;

    for ( int32_t i = 0; i < ctx->clay_render_commands.length; ++i ) {
        Clay_RenderCommand* src = Clay_RenderCommandArray_Get( &ctx->clay_render_commands, i );
        if ( !src ) {
            continue;
        }

        switch ( src->commandType ) {
            case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
                const Clay_RectangleRenderData* rect = &src->renderData.rectangle;
                float radius = rect->cornerRadius.topLeft;
                vxui_cmd* cmd = vxui__push_cmd( ctx, radius > 0.0f ? VXUI_CMD_RECT_ROUNDED : VXUI_CMD_RECT );
                if ( !cmd ) {
                    break;
                }

                cmd->clip_rect = ctx->current_clip_rect;
                if ( radius > 0.0f ) {
                    cmd->rect_rounded.bounds = vxui__rect_from_clay_box( src->boundingBox );
                    cmd->rect_rounded.color = vxui__color_from_clay( rect->backgroundColor );
                    cmd->rect_rounded.radius = radius;
                } else {
                    cmd->rect.bounds = vxui__rect_from_clay_box( src->boundingBox );
                    cmd->rect.color = vxui__color_from_clay( rect->backgroundColor );
                }
                break;
            }

            case CLAY_RENDER_COMMAND_TYPE_TEXT: {
                const Clay_TextRenderData* text = &src->renderData.text;
                const char* copied = vxui__copy_slice_text( ctx, text->stringContents );
                vxui_draw_cmd_text* queued = vxui__push_text( ctx );
                vxui_cmd* cmd = vxui__push_cmd( ctx, VXUI_CMD_TEXT );
                if ( !queued || !cmd ) {
                    break;
                }

                queued->pos = ( vxui_vec2 ) { src->boundingBox.x, src->boundingBox.y };
                queued->font_id = text->fontId;
                queued->size = ( float ) text->fontSize;
                queued->color = vxui__color_from_clay( text->textColor );
                queued->text = copied;

                cmd->clip_rect = ctx->current_clip_rect;
                cmd->text = *queued;
                break;
            }

            case CLAY_RENDER_COMMAND_TYPE_IMAGE: {
                const Clay_ImageRenderData* image = &src->renderData.image;
                vxui_cmd* cmd = vxui__push_cmd( ctx, VXUI_CMD_IMAGE );
                if ( !cmd ) {
                    break;
                }

                cmd->clip_rect = ctx->current_clip_rect;
                cmd->image.bounds = vxui__rect_from_clay_box( src->boundingBox );
                cmd->image.image_data = image->imageData;
                break;
            }

            case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START: {
                if ( ctx->clip_active ) {
                    assert( ctx->clip_stack_count < ctx->clip_stack_capacity );
                    if ( ctx->clip_stack_count < ctx->clip_stack_capacity ) {
                        ctx->clip_stack[ ctx->clip_stack_count++ ] = ctx->current_clip_rect;
                    }
                }

                ctx->current_clip_rect = vxui__rect_from_clay_box( src->boundingBox );
                ctx->clip_active = true;

                vxui_cmd* cmd = vxui__push_cmd( ctx, VXUI_CMD_CLIP_PUSH );
                if ( !cmd ) {
                    break;
                }

                cmd->clip_rect = ctx->current_clip_rect;
                cmd->clip.rect = ctx->current_clip_rect;
                break;
            }

            case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END: {
                vxui_cmd* cmd = vxui__push_cmd( ctx, VXUI_CMD_CLIP_POP );
                if ( cmd ) {
                    cmd->clip_rect = ctx->current_clip_rect;
                    cmd->clip.rect = ctx->current_clip_rect;
                }

                if ( ctx->clip_stack_count > 0 ) {
                    ctx->current_clip_rect = ctx->clip_stack[ --ctx->clip_stack_count ];
                    ctx->clip_active = true;
                } else {
                    ctx->current_clip_rect = vxui_rect {};
                    ctx->clip_active = false;
                }
                break;
            }

            case CLAY_RENDER_COMMAND_TYPE_NONE:
            case CLAY_RENDER_COMMAND_TYPE_BORDER:
            case CLAY_RENDER_COMMAND_TYPE_CUSTOM:
            default:
                assert( false && "unsupported clay render command" );
                break;
        }
    }
}

static vxui_config vxui__sanitize_config( vxui_config cfg )
{
    if ( cfg.screen_width <= 0 ) {
        cfg.screen_width = VXUI__DEFAULT_SCREEN_WIDTH;
    }
    if ( cfg.screen_height <= 0 ) {
        cfg.screen_height = VXUI__DEFAULT_SCREEN_HEIGHT;
    }
    if ( cfg.exit_anim_max_time <= 0 ) {
        cfg.exit_anim_max_time = VXUI__DEFAULT_EXIT_ANIM_MAX_TIME;
    }

    if ( cfg.focus_ring.color.a == 0 ) {
        cfg.focus_ring.color = ( vxui_color ) { 0, 255, 200, 255 };
    }
    if ( cfg.focus_ring.border_width <= 0.0f ) {
        cfg.focus_ring.border_width = 2.0f;
    }
    if ( cfg.focus_ring.corner_radius <= 0.0f ) {
        cfg.focus_ring.corner_radius = 4.0f;
    }
    if ( cfg.focus_ring.spring_stiffness <= 0.0f ) {
        cfg.focus_ring.spring_stiffness = 200.0f;
    }
    if ( cfg.focus_ring.spring_damping <= 0.0f ) {
        cfg.focus_ring.spring_damping = 20.0f;
    }

    if ( cfg.max_elements <= 0 ) {
        cfg.max_elements = VXUI__DEFAULT_MAX_ELEMENTS;
    }
    if ( cfg.max_anim_states <= 0 ) {
        cfg.max_anim_states = VXUI__DEFAULT_MAX_ANIM_STATES;
    }
    if ( cfg.max_sequences <= 0 ) {
        cfg.max_sequences = VXUI__DEFAULT_MAX_SEQUENCES;
    }
    if ( cfg.max_seq_steps <= 0 ) {
        cfg.max_seq_steps = VXUI__DEFAULT_MAX_SEQ_STEPS;
    }

    return cfg;
}

uint64_t vxui_min_memory_size( void )
{
    uint64_t clay_bytes = ( uint64_t ) Clay_MinMemorySize();
    uint64_t command_bytes = ( uint64_t ) VXUI__DEFAULT_MAX_ELEMENTS * ( uint64_t ) sizeof( vxui_cmd );
    uint64_t text_bytes = ( uint64_t ) VXUI__DEFAULT_MAX_ELEMENTS * ( uint64_t ) sizeof( vxui_draw_cmd_text );
    uint64_t clip_bytes = ( uint64_t ) VXUI__DEFAULT_MAX_ELEMENTS * ( uint64_t ) sizeof( vxui_rect );
    uint64_t frame_bytes = ( uint64_t ) VXUI__DEFAULT_FRAME_STRING_BYTES;
    uint64_t slack_bytes = 64u * 1024u;
    return clay_bytes + command_bytes + text_bytes + clip_bytes + frame_bytes + slack_bytes;
}

vxui_arena vxui_create_arena( uint64_t size, void* memory )
{
    vxui_arena arena = {};
    arena.memory = ( uint8_t* ) memory;
    arena.capacity = size;
    arena.offset = 0;
    return arena;
}

void vxui_init( vxui_ctx* ctx, vxui_arena arena, vxui_config cfg )
{
    *ctx = vxui_ctx {};
    ctx->cfg = vxui__sanitize_config( cfg );
    ctx->arena = arena;

    void* clay_memory = vxui__arena_alloc(
        &ctx->arena,
        ( uint64_t ) Clay_MinMemorySize(),
        64 );
    if ( clay_memory ) {
        Clay_SetCurrentContext( nullptr );
        ctx->clay_error_handler.errorHandlerFunction = vxui__handle_clay_error;
        ctx->clay_error_handler.userData = ctx;
        ctx->clay_arena = Clay_CreateArenaWithCapacityAndMemory( Clay_MinMemorySize(), clay_memory );
        ctx->clay_ctx = Clay_Initialize(
            ctx->clay_arena,
            Clay_Dimensions { ( float ) ctx->cfg.screen_width, ( float ) ctx->cfg.screen_height },
            ctx->clay_error_handler );
        Clay_SetMeasureTextFunction( vxui__measure_text, ctx );
    }

    ctx->commands = ( vxui_cmd* ) vxui__arena_alloc(
        &ctx->arena,
        ( uint64_t ) ctx->cfg.max_elements * ( uint64_t ) sizeof( vxui_cmd ),
        ( uint64_t ) alignof( vxui_cmd ) );
    if ( ctx->commands ) {
        ctx->command_capacity = ctx->cfg.max_elements;
    }

    ctx->text_queue = ( vxui_draw_cmd_text* ) vxui__arena_alloc(
        &ctx->arena,
        ( uint64_t ) ctx->cfg.max_elements * ( uint64_t ) sizeof( vxui_draw_cmd_text ),
        ( uint64_t ) alignof( vxui_draw_cmd_text ) );
    if ( ctx->text_queue ) {
        ctx->text_queue_capacity = ctx->cfg.max_elements;
    }

    ctx->clip_stack = ( vxui_rect* ) vxui__arena_alloc(
        &ctx->arena,
        ( uint64_t ) ctx->cfg.max_elements * ( uint64_t ) sizeof( vxui_rect ),
        ( uint64_t ) alignof( vxui_rect ) );
    if ( ctx->clip_stack ) {
        ctx->clip_stack_capacity = ctx->cfg.max_elements;
    }

    ctx->frame_string_capacity = ctx->cfg.max_elements * 128;
    ctx->frame_string_buffer = ( char* ) vxui__arena_alloc(
        &ctx->arena,
        ( uint64_t ) ctx->frame_string_capacity,
        1 );
    if ( !ctx->frame_string_buffer ) {
        ctx->frame_string_capacity = 0;
    }

    vxui__reset_frame_buffers( ctx );
}

void vxui_begin( vxui_ctx* ctx, float delta_time )
{
    ctx->frame_index += 1;
    ctx->delta_time = delta_time;
    vxui__reset_frame_buffers( ctx );

    if ( ctx->clay_ctx ) {
        Clay_SetCurrentContext( ctx->clay_ctx );
        Clay_SetLayoutDimensions( Clay_Dimensions { ( float ) ctx->cfg.screen_width, ( float ) ctx->cfg.screen_height } );
        Clay_BeginLayout();
    }
}

vxui_draw_list vxui_end( vxui_ctx* ctx )
{
    vxui_draw_list list = {};

    if ( ctx->clay_ctx ) {
        Clay_SetCurrentContext( ctx->clay_ctx );
        ctx->clay_render_commands = Clay_EndLayout();
        vxui__translate_clay_commands( ctx );
    }

    list.commands = ctx->commands;
    list.length = ctx->command_count;
    return list;
}

void vxui_flush_text( vxui_ctx* ctx )
{
    ( void ) ctx;
}

void vxui_set_fontcache( vxui_ctx* ctx, ve_fontcache* cache )
{
    ctx->fontcache = cache;
}

void vxui_set_text_fn( vxui_ctx* ctx, const char* ( *fn )( const char* key, void* userdata ), void* userdata )
{
    ctx->text_fn = fn;
    ctx->text_fn_userdata = userdata;
}

uint32_t vxui_id( const char* label )
{
    Clay_ElementId id = Clay_GetElementId( vxui__clay_string_from_cstr( label ) );
    return id.id;
}

uint32_t vxui_idi( const char* label, int index )
{
    Clay_ElementId id = Clay_GetElementIdWithIndex( vxui__clay_string_from_cstr( label ), ( uint32_t ) index );
    return id.id;
}

#endif // VXUI_IMPL
