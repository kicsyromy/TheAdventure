set (
    PROJECT_RESOURCES
        "res/tilesets/grass.png"
        "res/characters/player.png"
        "res/objects/rock_in_water_06.png"
)

set (RES_FIXED_PATH)
foreach (resource IN LISTS PROJECT_RESOURCES)
    list (APPEND RES_FIXED_PATH "${PROJECT_SOURCE_DIR}/${resource}")
endforeach ()
embed_resources (
    TARGET
        ${PROJECT_ID}
    RESOURCES
        ${RES_FIXED_PATH}
)
