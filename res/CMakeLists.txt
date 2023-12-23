set (
    PROJECT_RESOURCES
        "res/c-plus-plus-logo.png"
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