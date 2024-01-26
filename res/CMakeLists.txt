set (
    PROJECT_RESOURCES
        "res/music/world_bg.mp3"
        "res/characters/player.png"
        "res/characters/slime.png"
        "res/objects/rock_in_water_06.png"
        "res/sfx/01_chest_open_1.mp3"
        "res/sfx/02_chest_close_1.mp3"
        "res/sfx/05_door_open_1.mp3"
        "res/sfx/06_door_close_1.mp3"
        "res/sfx/07_human_atk_sword_1.mp3"
        "res/sfx/14_human_death_spin.mp3"
        "res/sfx/26_sword_hit_1.mp3"
        "res/sfx/26_sword_hit_2.mp3"
        "res/sfx/26_sword_hit_3.mp3"
        "res/tilesets/grass.png"
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
