#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "config.h"

// Custom strdup implementation for portability
static char* my_strdup(const char* s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s) + 1;
    char* copy = malloc(len);
    if (copy) {
        memcpy(copy, s, len);
    }
    return copy;
}

// Custom strtok_r implementation for portability
static char* my_strtok_r(char* str, const char* delim, char** saveptr) {
    char* token;
    
    if (str != NULL) {
        *saveptr = str;
    }
    
    token = *saveptr;
    if (token == NULL) {
        return NULL;
    }
    
    // Skip leading delimiters
    token += strspn(token, delim);
    if (*token == '\0') {
        *saveptr = NULL;
        return NULL;
    }
    
    // Find end of token
    char* end = token + strcspn(token, delim);
    if (*end != '\0') {
        *end = '\0';
        *saveptr = end + 1;
    } else {
        *saveptr = NULL;
    }
    
    return token;
}

// Helper: Trim whitespace from string
static char* trim_whitespace(char* str) {
    if (!str) return NULL;
    
    char* end;
    
    // Trim leading space
    while (isspace((unsigned char)*str)) str++;
    
    if (*str == 0) return str;  // All spaces?
    
    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    
    end[1] = '\0';
    return str;
}

// Helper: Get value from environment with default
static char* get_env_value(const char* key, const char* default_val) {
    char* env_val = getenv(key);
    if (env_val != NULL && strlen(env_val) > 0) {
        return my_strdup(env_val);
    }
    return default_val ? my_strdup(default_val) : NULL;
}

// Parse a single line from .env file
static int parse_env_line(BlogConfig* config, const char* line) {
    char* work_line = my_strdup(line);
    char* saveptr = NULL;
    
    if (!work_line) return -1;
    
    // Skip empty lines and comments
    char* trimmed = trim_whitespace(work_line);
    if (strlen(trimmed) == 0 || trimmed[0] == '#') {
        free(work_line);
        return 0;
    }
    
    // Split key=value
    char* key = my_strtok_r(work_line, "=", &saveptr);
    char* value = my_strtok_r(NULL, "\n", &saveptr);
    
    if (!key || !value) {
        free(work_line);
        return -1;
    }
    
    key = trim_whitespace(key);
    value = trim_whitespace(value);
    
    // Remove quotes if present
    if (value[0] == '"' && value[strlen(value)-1] == '"') {
        value[strlen(value)-1] = '\0';
        value++;
    }

    if (strcmp(key, "BASE_DIR") == 0) {
        free(config->base_dir);
        config->base_dir = my_strdup(value);
    }else if (strcmp(key, "BLOG_DB_PATH") == 0) {// Set the appropriate config field
        free(config->db_path);
        config->db_path = my_strdup(value);
    }
    else if (strcmp(key, "BLOGS_PATH") == 0) {
        free(config->blogs_path);
        config->blogs_path = my_strdup(value);
    }
    else if (strcmp(key, "BLOGS_ARCHIVE") == 0) {
        free(config->blogs_archive);
        config->blogs_archive = my_strdup(value);
    }
    else if (strcmp(key, "CSS_PATH") == 0) {
        free(config->css_path);
        config->css_path = my_strdup(value);
    }
    else if (strcmp(key, "JS_PATH") == 0) {
        free(config->js_path);
        config->js_path = my_strdup(value);
    }
    else if (strcmp(key, "IMGS_PATH") == 0) {
        free(config->imgs_path);
        config->imgs_path = my_strdup(value);
    }
    else if (strcmp(key, "LOG_FILE") == 0) {
        free(config->log_file);
        config->log_file = my_strdup(value);
    }
    else if (strcmp(key, "HTML_PATH") == 0) {
        free(config->html_path);
        config->html_path = my_strdup(value);
    }
   
    
    free(work_line);
    return 0;
}

// Try to find .env file in common locations
static char* find_env_file(void) {
    const char* locations[] = {
        ".env",
        "../.env",
        "../../.env",
        "config/.env",
        NULL
    };
    
    for (int i = 0; locations[i] != NULL; i++) {
        FILE* f = fopen(locations[i], "r");
        if (f) {
            fclose(f);
            return my_strdup(locations[i]);
        }
    }
    return NULL;
}

BlogConfig* config_create(void) {
    BlogConfig* config = malloc(sizeof(BlogConfig));
    if (!config) return NULL;
    
    // Initialize with NULL pointers
    memset(config, 0, sizeof(BlogConfig));
    return config;
}

void config_free(BlogConfig* config) {
    if (!config) return;
    
    free(config->db_path);
    free(config->blogs_path);
    free(config->blogs_archive);
    free(config->css_path);
    free(config->js_path);
    free(config->imgs_path);
    free(config->log_file);
    free(config->html_path);
    free(config);
}

int config_load_from_file(BlogConfig* config, const char* env_path) {
    FILE* file = fopen(env_path, "r");
    if (!file) {
        return -1;  // File not found
    }
    
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        parse_env_line(config, line);
    }
    
    fclose(file);
    return 0;
}

int config_load_from_env(BlogConfig* config) {
    // Load from environment variables only
    config->db_path = get_env_value("BLOG_DB_PATH", "./data/blog.db");
    config->blogs_path = get_env_value("BLOGS_PATH", "./posts");
    config->blogs_archive = get_env_value("BLOGS_ARCHIVE", "./archive");
    config->css_path = get_env_value("CSS_PATH", "./public/css");
    config->js_path = get_env_value("JS_PATH", "./public/js");
    config->imgs_path = get_env_value("IMGS_PATH", "./public/images");
    config->log_file = get_env_value("LOG_FILE", "blog.log");
    config->html_path = get_env_value("HTML_PATH", "./public/html");
    
    return 0;
}

int config_load(BlogConfig* config) {
    // Try to find and load .env file
    char* env_path = find_env_file();
    if (env_path) {
        int result = config_load_from_file(config, env_path);
        free(env_path);
        if (result == 0) {
            return 0;  // Successfully loaded from .env file
        }
    }
    
    // Fallback to environment variables
    return config_load_from_env(config);
}

static char* expand_variables(const char* value, BlogConfig* config) {
    if (!value) return NULL;
    
    char buffer[1024];
    char* result = my_strdup(value);
    if (!result) return NULL;
    
    if (config->base_dir) {
        char search[32], replace[1024];
        snprintf(search, sizeof(search), "${BASE_DIR}");
        snprintf(replace, sizeof(replace), "%s", config->base_dir);
        
        char* pos = strstr(result, search);
        if (pos) {
            size_t new_len = strlen(result) - strlen(search) + strlen(replace) + 1;
            char* expanded = malloc(new_len);
            if (expanded) {
                strncpy(expanded, result, pos - result);
                expanded[pos - result] = '\0';
                strcat(expanded, replace);
                strcat(expanded, pos + strlen(search));
                free(result);
                result = expanded;
            }
        }
    }
    

    
    return result;
}


// Helper: Replace all occurrences of a substring
static char* string_replace(const char* str, const char* search, const char* replace) {
    if (!str || !search || !replace) return NULL;
    
    size_t search_len = strlen(search);
    size_t replace_len = strlen(replace);
    
    // Count occurrences
    size_t count = 0;
    const char* pos = str;
    while ((pos = strstr(pos, search)) != NULL) {
        count++;
        pos += search_len;
    }
    
    if (count == 0) return my_strdup(str);
    
    // Calculate new length
    size_t new_len = strlen(str) + count * (replace_len - search_len) + 1;
    char* result = malloc(new_len);
    if (!result) return NULL;
    
    // Perform replacement
    char* current = result;
    const char* start = str;
    
    while (count--) {
        pos = strstr(start, search);
        size_t segment_len = pos - start;
        
        // Copy segment before search term
        memcpy(current, start, segment_len);
        current += segment_len;
        
        // Copy replacement
        memcpy(current, replace, replace_len);
        current += replace_len;
        
        start = pos + search_len;
    }
    
    // Copy remaining string
    strcpy(current, start);
    return result;
}

// Expand variables in a single string
static char* expand_string_variables(const char* value, BlogConfig* config) {
    if (!value) return NULL;
    
    char* result = my_strdup(value);
    if (!result) return NULL;
    
    // Expand ${BASE_DIR}
    if (config->base_dir) {
        char* expanded = string_replace(result, "${BASE_DIR}", config->base_dir);
        if (expanded) {
            free(result);
            result = expanded;
        }
    }
    
    
    
    return result;
}

// Main variable expansion function
int config_expand_variables(BlogConfig* config) {
    if (!config) return -1;
    
    char* expanded;
    
    // Expand base_dir itself first (in case it has variables)
    if (config->base_dir) {
        expanded = expand_string_variables(config->base_dir, config);
        if (expanded) {
            free(config->base_dir);
            config->base_dir = expanded;
        }
    }
    
   
    
    // Expand all other paths that might contain variables
    if (config->db_path) {
        expanded = expand_string_variables(config->db_path, config);
        if (expanded) {
            free(config->db_path);
            config->db_path = expanded;
        }
    }
    
    if (config->blogs_path) {
        expanded = expand_string_variables(config->blogs_path, config);
        if (expanded) {
            free(config->blogs_path);
            config->blogs_path = expanded;
        }
    }
    
    if (config->blogs_archive) {
        expanded = expand_string_variables(config->blogs_archive, config);
        if (expanded) {
            free(config->blogs_archive);
            config->blogs_archive = expanded;
        }
    }
    
    if (config->css_path) {
        expanded = expand_string_variables(config->css_path, config);
        if (expanded) {
            free(config->css_path);
            config->css_path = expanded;
        }
    }
    
    if (config->js_path) {
        expanded = expand_string_variables(config->js_path, config);
        if (expanded) {
            free(config->js_path);
            config->js_path = expanded;
        }
    }
    
    if (config->imgs_path) {
        expanded = expand_string_variables(config->imgs_path, config);
        if (expanded) {
            free(config->imgs_path);
            config->imgs_path = expanded;
        }
    }
    
    if (config->log_file) {
        expanded = expand_string_variables(config->log_file, config);
        if (expanded) {
            free(config->log_file);
            config->log_file = expanded;
        }
    }

    if (config->html_path) {
        expanded = expand_string_variables(config->html_path, config);
        if (expanded) {
            free(config->html_path);
            config->html_path = expanded;
        }
    }
    
    return 0;
}
// Getters with fallback defaults
const char* config_get_db_path(const BlogConfig* config) {
    return config->db_path ? config->db_path : "./data/blogdbV2.db";
}

const char* config_get_blogs_path(const BlogConfig* config) {
    return config->blogs_path ? config->blogs_path : "./public/blogs";
}

const char* config_get_blogs_archive(const BlogConfig* config) {
    return config->blogs_archive ? config->blogs_archive : "./public/archive";
}

const char* config_get_css_path(const BlogConfig* config) {
    return config->css_path ? config->css_path : "./src/static/template/css";
}

const char* config_get_js_path(const BlogConfig* config) {
    return config->js_path ? config->js_path : "./src/static/template/js";
}

const char* config_get_imgs_path(const BlogConfig* config) {
    return config->imgs_path ? config->imgs_path : "./src/static/template/images";
}

const char* config_get_log_file(const BlogConfig* config) {
    return config->log_file ? config->log_file : "./src/bloglog.txt";
}
const char* config_get_html_path(const BlogConfig* config) {
    return config->html_path ? config->html_path : "./src/static/template/html";
}
