#include <stdio.h>
#include "config.h"

int main() {
    printf("Testing Config Library...\n");
    
    BlogConfig* config = config_create();
    if (!config) {
        printf("Failed to create config!\n");
        return 1;
    }
    
    // Try to load configuration
    if (config_load(config) == 0) {
        printf("Configuration loaded successfully!\n");
    } else {
        printf("Using default configuration\n");
    }
    
    // Print all config values
    printf("DB Path: %s\n", config_get_db_path(config));
    printf("Blogs Path: %s\n", config_get_blogs_path(config));
    printf("Blogs Archive: %s\n", config_get_blogs_archive(config));
    printf("CSS Path: %s\n", config_get_css_path(config));
    printf("JS Path: %s\n", config_get_js_path(config));
    printf("Images Path: %s\n", config_get_imgs_path(config));
    printf("Log File: %s\n", config_get_log_file(config));
    
    config_free(config);
    return 0;
}