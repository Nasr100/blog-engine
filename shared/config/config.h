#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    char* db_path;
    char* blogs_path; 
    char* blogs_archive;
    char* css_path;
    char* js_path;
    char* imgs_path;
    char* log_file;
    char* base_dir;
    char* html_path;
} BlogConfig;

// Constructor/Destructor
BlogConfig* config_create(void);
void config_free(BlogConfig* config);

// Load configuration
int config_load(BlogConfig* config);  // Auto-find .env
int config_load_from_file(BlogConfig* config, const char* env_path);
int config_load_from_env(BlogConfig* config);  // Only environment variables
int config_expand_variables(BlogConfig* config);
// Getters (fixed const correctness)
const char* config_get_db_path(const BlogConfig* config);
const char* config_get_blogs_path(const BlogConfig* config);
const char* config_get_blogs_archive(const BlogConfig* config);
const char* config_get_css_path(const BlogConfig* config);
const char* config_get_js_path(const BlogConfig* config);
const char* config_get_imgs_path(const BlogConfig* config);
const char* config_get_log_file(const BlogConfig* config);
const char* config_get_html_path(const BlogConfig* config);
;

#endif