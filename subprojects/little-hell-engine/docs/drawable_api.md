# Drawable API Design Document

# Creating a Drawable

```c
DrawableTextures *textures = drawable_get_textures();
Drawable drawable = {
    .x = 0, 
    .y = SCREEN_HEIGHT - 32,
    .enabled = enabled,
    .value.texture = textures->status_bar,
    .type = DRAWABLE_TEXTURE
};
    
// Under the hood this uses the DrawableRegistry
drawable_create_drawable(&drawable, "statusbar"); 
```

# Destroying a drawable
    
```c 
// Again, using the DrawableRegistry under the hood
drawable_destroy_drawable(drawable, "statusbar"); 

// All Drawables can be destroyed (such as at exit), just a wrapper
around the DrawableRegistry.
drawables_destroy_all();
```

## Accessing a Drawable

```c
// Again, wrapper around the Drawable registry
Drawable *drawable = drawable_get_drawable(drawable, "statusbar");
*drawable->value.number = &plyr_ammo;
```
