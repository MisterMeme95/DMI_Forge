#include "dmm_handler.h"
#include <windows.h>
#include <ctype.h>
#include "../dmi/dmi.h"
#include <stdarg.h>
extern int max_num_of_elements;
//#define DEBUG_ON 1
#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>



int get_icon_local_id_for_atom(DMI_BYOND *icon, const AtomInstance *atom) {
    if (!icon || !atom || !atom->icon_state) {
        return -1;  // invalid input
    }

    // If the atom has no state name, keep your old behavior (default = 0).
    if (atom->icon_state[0] == '\0') {
        return 0;
    }

    // Normalize the atom's icon_state if needed.
    // If icon_state strings in icon_state->state are already unquoted, you
    // can just use atom->icon_state directly.
    const char *state_name = atom->icon_state;

    char adjusted[256];
    size_t len = strlen(state_name);
    if (len >= 2 && (state_name[0] == '"' || state_name[0] == '\'')
        && state_name[len - 1] == state_name[0]) {

        size_t copy_len = len - 2;
        if (copy_len >= sizeof(adjusted)) {
            copy_len = sizeof(adjusted) - 1;
        }
        memcpy(adjusted, state_name + 1, copy_len);
        adjusted[copy_len] = '\0';
        state_name = adjusted;
    }

    // Build the composite key: (name, movement=0)
    icon_state_key key = {
            .state_name = state_name,
            .movement   = 0  // only non-movement states
    };

//    // Look it up in the per-icon state_set
//    if (!icon->state_set) {
//        // fallbac
//        k: you haven't built the hash yet; you could call the old linear version here
//        return -1;
//    }

    icon_state *found = (icon_state *)icon_state_look_key(&icon->state_set, &key);
    if (!found) {
        return -1;  // not found, or only exists as movement != 0
    }

    return found->local_id;
}


/* winner = higher plane; if tie: higher layer; if tie: keep original order */
static inline int should_move_right(const AtomInstance *left, const AtomInstance *right) {
    if (left->plane > right->plane) return 1;      // winner (left) moves right
    if (left->plane < right->plane) return 0;
    if (left->layer > right->layer) return 1;      // winner (left) moves right
    if (left->layer < right->layer) return 0;
    return 0;  // equal -> keep original order (stable)
}

void sort_atoms(Vector *atoms) {
    if (!atoms || atoms->current_capacity == 0) return;
    /* NOTE: if your Vector tracks logical count (size), use that instead of capacity */
    size_t n = atoms->current_capacity;
    for (size_t i = 0; i < n; i++) {
        int swapped = 0;
        for (size_t j = 0; j + 1 < n - i; j++) {
            AtomInstance *a = (AtomInstance*)((char*)atoms->data + j * sizeof(AtomInstance));
            AtomInstance *b = (AtomInstance*)((char*)atoms->data + (j + 1) * sizeof(AtomInstance));
            if (should_move_right(a, b)) {
                AtomInstance tmp;
                memcpy(&tmp, a, sizeof(AtomInstance));
                memcpy(a, b, sizeof(AtomInstance));
                memcpy(b, &tmp, sizeof(AtomInstance));
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}


static inline int is_delim(unsigned char c) {
    return (c == '\0' || c == '\n' || c == '\t' || c == ' ');
}

void list_files_recursive(const char *basePath, hash_table *htable, hash_table *hset, Vector* vect) {
    char searchPath[MAX_PATH];
    WIN32_FIND_DATAA findData;
    set *new_set = (set *)malloc(sizeof(set));
    initialize_set(new_set, 100, hset_insert, hset_lookup_element, string_match, string_match,
                   hash_string, hash_string, NULL, NULL);
    snprintf(searchPath, MAX_PATH, "%s/*", basePath);
#ifdef DEBUG_ON
    printf("Search - %s\n", searchPath);
    if(strcmp(searchPath, "C:/Users/Atomix/Downloads/Ninja_Online (1)/Ninja Online/Pixel Art/Buildings/Land of Fire/Academy/Interior/*") == 0){
        int stop = 1;
    }
#endif
    HANDLE hFind = FindFirstFileA(searchPath, &findData);
    if (hFind == INVALID_HANDLE_VALUE) return;
    do {
        if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0) continue;
        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            char fullPath[MAX_PATH];
            snprintf(fullPath, MAX_PATH, "%s/%s", basePath, findData.cFileName);

            // lowercase both filename and path before use
            char *filename_lc = strdup_lower(findData.cFileName);
            char *fullPath_lc = strdup_lower(fullPath);
            const char *ext = get_file_extension(filename_lc);  // extension from lowercase filename
            if (strcmp(ext, "dmi") == 0 || strcmp(ext, "png") == 0) {
                htable->insert(htable, filename_lc, basePath);  // store lowercase filename
                new_set->insert(new_set, filename_lc);          // ownership passed to set
            }
            else if (strcmp(ext, "dme") == 0) {
                char **path_copy = (char**)malloc(sizeof(char*));
                *path_copy = fullPath_lc;  // already lowercase
                vect->push_back(vect, path_copy);
            } else {
                free(filename_lc);
                free(fullPath_lc);
            }
        }

    } while (FindNextFileA(hFind, &findData));
    hset->insert(hset, basePath, new_set);
    FindClose(hFind);

    // ---------- SECOND PASS: Recurse into subdirectories ----------
    hFind = FindFirstFileA(searchPath, &findData);
    if (hFind == INVALID_HANDLE_VALUE) return;

    do {
        if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0) continue;
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            char subDir[MAX_PATH];
            snprintf(subDir, MAX_PATH, "%s/%s", basePath, findData.cFileName);
            list_files_recursive(subDir, htable, hset, vect);
        }
    } while (FindNextFileA(hFind, &findData));
    FindClose(hFind);
}

typedef struct {
    char* data;
    size_t length;
    size_t capacity;
} StringBuffer;

void init_buffer(StringBuffer* buf) {
    buf->capacity = 1024;
    buf->length = 0;
    buf->data = malloc(buf->capacity);
    buf->data[0] = '\0';
}
void append_to_buffer(StringBuffer* buf, const char* format, ...) {
    va_list args;
    va_start(args, format);

    size_t available = buf->capacity - buf->length;
    int needed = vsnprintf(buf->data + buf->length, available, format, args);
    if (needed >= (int)available) {
        buf->capacity = buf->length + needed + 1;
        buf->data = realloc(buf->data, buf->capacity);
        va_end(args);
        va_start(args, format); // restart va_list after realloc
        vsnprintf(buf->data + buf->length, buf->capacity - buf->length, format, args);
    }
    buf->length += needed;
    va_end(args);
}

void free_buffer(StringBuffer* buf) {
    free(buf->data);
    buf->data = NULL;
    buf->length = buf->capacity = 0;
}
MapEntry *initialize_map_pop() {
    MapEntry *entry = (MapEntry *) malloc(sizeof(MapEntry));
    vector_init(&entry->atoms, sizeof(AtomInstance), 10, NULL, NULL);
    return entry;
}

MapEntry *get_entry_data(hash_table* table, char* type_path){
    int hash_value = (int)(table->hash_function(type_path) % table->buckets);
    list *bucket_list = &table->table[hash_value];
    node *element;

    for (element = list_head(&table->table[hash_value]); element != NULL; element = list_next(element)) {
        if (string_match2(type_path, list_data(element))) {
            key_value_pair *pair = (key_value_pair*) list_data(element);
            return pair->value;
        }
    }
    return NULL;
}
int insert_hash_table_icon(hash_table *table, const void *key, const void *value) {
    if (!table || !key || !value)
        return -1;

    unsigned long index = table->hash_function(key) % table->buckets;
    list *bucket_list = &table->table[index];
    node *new_node = (node *) malloc(sizeof(node));
    key_value_pair *new_pair = malloc(sizeof(key_value_pair));
    if (!new_pair)
        return -1;

    new_pair->key = malloc(strlen(key) + 1);
    if (!new_pair->key) {
        free(new_pair);
        return -1;
    }
    memcpy(new_pair->key, key, strlen(key) + 1);

    new_pair->value = malloc(sizeof(IconData));
    IconData *test_data;
    if (!new_pair->value) {
        free(new_pair->key);
        free(new_pair);
        return -1;
    }
    test_data = (IconData*)value;
    memcpy(new_pair->value, value, sizeof(IconData));
    test_data = (IconData*)new_pair->value;

    if (list_ins_tail(bucket_list, new_pair) != 0) {
        free(new_pair->value);
        free(new_pair->key);
        free(new_pair);
        return -1;
    }
    return 0;
}

int insert_hash_table_pop(hash_table *table, const void *key, const void *value) {
    if (!table || !key || !value)
        return -1;

    unsigned long index = table->hash_function(key) % table->buckets;
    list *bucket_list = &table->table[index];
    node *new_node = (node *) malloc(sizeof(node));
    key_value_pair *new_pair = malloc(sizeof(key_value_pair));
    if (!new_pair)
        return -1;

    new_pair->key = malloc(strlen(key) + 1);
    if (!new_pair->key) {
        free(new_pair);
        return -1;
    }
    memcpy(new_pair->key, key, strlen(key) + 1);

    new_pair->value = malloc(sizeof(MapEntry));
    MapEntry *test_data;
    MapEntry *test_data2;

    if (!new_pair->value) {
        free(new_pair->key);
        free(new_pair);
        return -1;
    }
    test_data = (MapEntry*)value;
    memcpy(new_pair->value, value, sizeof(MapEntry));
    test_data2 = (MapEntry*)new_pair->value;
    AtomInstance *check1 = (AtomInstance*)test_data->atoms.data;
    AtomInstance *check2 = (AtomInstance*)test_data2->atoms.data;

    if (list_ins_tail(bucket_list, new_pair) != 0) {
        free(new_pair->value);
        free(new_pair->key);
        free(new_pair);
        return -1;
    }
    return 0;
}

/**
 * This function is utilized to populate the "entry" data structure. Map Entry contains all of the instances
 * of map pops.
 * **/
void parse_map_pop(char *buffer, MapEntry* entry, hash_table* table, StringBuffer* tsx_buffer, int* gid_tracker,
                   set *dmi_set, set *png_set, const char* root_path, DM_Tracker *tracker) {

    //We want to move beyond the first \" character so that we can get the id.
    // Note that we also look for the closing quote.
    char *start = buffer+1;
    char *end = strstr(buffer+1, "\"");
    int pop_counter = 0;
    if (!end) {
        fprintf(stderr, "Error: closing quote not found.\n");
        return;
    }



    /* Just a few mechanisms to copy the id to the map entry. */
    char store_value = end[0];
    end[0] = '\0';
    entry->id = (char*)malloc((end - start + 1) * sizeof(char));
    strcpy(entry->id, start);
    if(strcmp(entry->id, "cf") == 0){
        int pop_stop = 1;
    }
    end[1] = store_value;


    // Next we need to find the equal sign. . .
    char *equal_sign = strchr(end + 1, '=');
    if (!equal_sign) return;
    char *paren_start = strchr(equal_sign, '(');
    char *paren_end = strrchr(equal_sign, ')');
    if (!paren_start || !paren_end || paren_start >= paren_end) return;

    *paren_end = '\0';
    char *atom_string = paren_start + 1;
    char *cursor = atom_string;

    while (cursor && *cursor != '\0') {
        if(*cursor == '\n') break;
        char *atom_start = cursor;
        int in_single_quote = 0;
        int in_double_quote = 0;
        int brace_depth = 0;
        for (; *cursor != '\0'; cursor++) {
            if (*cursor == '"' && (cursor == atom_start || *(cursor - 1) != '\\')) in_double_quote = !in_double_quote;
            else if (*cursor == '\'' && (cursor == atom_start || *(cursor - 1) != '\\')) in_single_quote = !in_single_quote;
            if (!in_single_quote && !in_double_quote) {
                if (*cursor == '{') brace_depth++;
                else if (*cursor == '}') brace_depth--;
                else if (*cursor == ',' && brace_depth == 0) break;
            }
        }
        *cursor = '\0';
        AtomInstance *atomInstance = (AtomInstance*)malloc(sizeof(AtomInstance));
        atomInstance->type_path = NULL;
        atomInstance->icon_state = NULL;
        atomInstance->icon = NULL;
        atomInstance->layer = 0.0;
        atomInstance->plane = 0.0;

        char *verify_if_instance = strchr(atom_start, '{');

        //If we have an opening curly bracket
            // If so, then we know we need to get instance-specific data.
        if (verify_if_instance) {
            int type_length = (int)(verify_if_instance - atom_start);
            atomInstance->type_path = (char*) malloc(sizeof(char) * (type_length + 1));
            *verify_if_instance = '\0';
            strcpy(atomInstance->type_path, atom_start);
            atomInstance->type_path[type_length] = '\0';
            char *instance_block = verify_if_instance + 1;
            char *end_brace = strchr(instance_block, '}');
            if (!end_brace) {
                fprintf(stderr, "Error: unmatched '{' in instance definition.\n");
                return;
            }

            *end_brace = '\0';
            char *icon_var = strstr(instance_block, "icon =");
            if (icon_var) {
                icon_var += strlen("icon =");
                while (*icon_var == ' ' || *icon_var == '\t') icon_var++;
                if (*icon_var == '\'' || *icon_var == '"') {
                    char quote = *icon_var++;
                    char *end_quote = strchr(icon_var, quote);
                    if (end_quote) {
                        int string_size = (int)(end_quote - icon_var) + 1;
                        *end_quote = '\0';
                        atomInstance->icon = (char*)malloc(sizeof(char) * string_size);
                        strcpy(atomInstance->icon, icon_var);
                        atomInstance->icon[string_size-1] = '\0';
                        *end_quote = quote;
                    }
                }
            }
            char *state_var = strstr(instance_block, "icon_state =");
            if (state_var) {
                state_var += strlen("icon_state =");
                while (*state_var == ' ' || *state_var == '\t') state_var++;
                if (*state_var == '\'' || *state_var == '"') {
                    char quote = *state_var++;
                    char *end_quote = strchr(state_var, quote);
                    if (end_quote) {
                        *end_quote = '\0';
                        int string_size = (int)(end_quote - state_var) + 1;
                        atomInstance->icon_state = (char*)malloc(sizeof(char) * string_size);
                        strcpy(atomInstance->icon_state, state_var);
                        atomInstance->icon_state[string_size-1] = '\0';
                        *end_quote = quote;
                    }
                }
            }
            *end_brace = '}';
        } else {
            int length_of_type = (int)strlen(atom_start);
            atomInstance->type_path = (char*)malloc(sizeof(char) * (length_of_type + 1));
            strcpy(atomInstance->type_path, atom_start);
            atomInstance->type_path[length_of_type] = '\0';
        }
        cursor++;

        if(atomInstance->icon == NULL) {
            object* get_object = ht_find_object_by_type(tracker->object_map,atomInstance->type_path);
            if(get_object != NULL){
                if(get_object->icon == NULL){
                    atomInstance->icon = (char*)malloc(sizeof(char));
                    atomInstance->icon[0] = '\0';
                }
                else {
                    atomInstance->icon = get_object->full_icon_path;
                    atomInstance->layer = get_object->layer;
                    atomInstance->plane = get_object->plane;
                }
            }
        }
        else {
            char *new_pointer = lookup_hash_table(&tracker->map_of_icons, atomInstance->icon);
            if (new_pointer != NULL) {
                size_t len1 = strlen(new_pointer);
                size_t len2 = strlen(atomInstance->icon);
                int needs_slash = (len1 > 0 && new_pointer[len1 - 1] != '/');
                size_t total_len = len1 + (needs_slash ? 1 : 0) + len2 + 1;
                char *concat = malloc(total_len);
                if (!concat) {
                    perror("malloc failed");
                    exit(1);
                }
                strcpy(concat, new_pointer);
                if (needs_slash) strcat(concat, "/");
                strcat(concat, atomInstance->icon);

                /* free the old icon if you own it */
                free(atomInstance->icon);
                atomInstance->icon = concat;
            }
        }
        if(atomInstance->icon_state == NULL) {
            object* get_object = ht_find_object_by_type(tracker->object_map,
                                                        atomInstance->type_path);
            if(get_object != NULL) {
                if (get_object->icon_state == NULL) {
                    atomInstance->icon_state = (char *) malloc(sizeof(char));
                    atomInstance->icon_state[0] = '\0';
                } else {
                    atomInstance->icon_state = get_object->icon_state;
                }
            }
        }
        char *find_png = strstr(atomInstance->icon, ".png");
        if(find_png == NULL){
            DMI_BYOND* icon = dmi_set->lookup(dmi_set, atomInstance->icon);
            if(icon == NULL && strcmp(atomInstance->icon, "") != 0){
                char full_name[300] = {0};
                strcat(full_name, atomInstance->icon);
                icon = (DMI_BYOND*)malloc(sizeof(DMI_BYOND));
                initialize_dmi_struct(icon, full_name);
                icon->global_id = *gid_tracker;
                char tsx_path[512];
                char *last_slash = strrchr(root_path, '/');
                if (!last_slash) {
                    fprintf(stderr, "Invalid root_path: must contain at least one '/'\n");
                    return;
                }
                long long parent_len = last_slash - root_path;
                const char *icon_path = atomInstance->icon;
                char proj_dir[PATH_MAX];
                strncpy(proj_dir, tracker->project_directory, sizeof(proj_dir));
                proj_dir[sizeof(proj_dir)-1] = '\0';
                size_t p_len = strlen(proj_dir);
                while (p_len > 0 && (proj_dir[p_len-1] == '/' || proj_dir[p_len-1] == '\\')) {
                    proj_dir[--p_len] = '\0';
                }

                char parent_of_proj[PATH_MAX];
                strncpy(parent_of_proj, proj_dir, sizeof(parent_of_proj));
                parent_of_proj[sizeof(parent_of_proj)-1] = '\0';
                char *last_slash_f = strrchr(parent_of_proj, '/');
                char *last_slash_b = strrchr(parent_of_proj, '\\');
                char *last = last_slash_f;
                if (last_slash_b && (!last || last_slash_b > last)) last = last_slash_b;
                if (last) {
                    *last = '\0';
                } else {
                    // If no parent separator, fall back to "."
                    strncpy(parent_of_proj, ".", sizeof(parent_of_proj));
                    parent_of_proj[sizeof(parent_of_proj)-1] = '\0';
                }

                // Compute path of the icon relative to project_directory (e.g., "icon/image.dmi")
                const char *rel = icon_path;
                size_t proj_len = strlen(proj_dir);
                if (proj_len > 0 &&
                    strncmp(icon_path, proj_dir, proj_len) == 0 &&
                    (icon_path[proj_len] == '/' || icon_path[proj_len] == '\\')) {
                    rel = icon_path + proj_len + 1; // skip separator
                } else if (strncmp(icon_path, proj_dir, proj_len) == 0 && icon_path[proj_len] == '\0') {
                    // identical path (unlikely); treat as filename only
                    const char *b1 = strrchr(icon_path, '/');
                    const char *b2 = strrchr(icon_path, '\\');
                    rel = (b2 && (!b1 || b2 > b1)) ? b2 + 1 : (b1 ? b1 + 1 : icon_path);
                } else {
                    // If icon isn't under project_directory, fall back to basename
                    const char *b1 = strrchr(icon_path, '/');
                    const char *b2 = strrchr(icon_path, '\\');
                    rel = (b2 && (!b1 || b2 > b1)) ? b2 + 1 : (b1 ? b1 + 1 : icon_path);
                }

                // Drop extension and build rel_no_ext (e.g., "icon/image")
                char rel_no_ext[PATH_MAX];
                strncpy(rel_no_ext, rel, sizeof(rel_no_ext));
                rel_no_ext[sizeof(rel_no_ext)-1] = '\0';
                char *dot = strrchr(rel_no_ext, '.');
                if (dot) *dot = '\0';

                // Build final TSX path: <parent_of_proj>/_implement_file/<rel_no_ext>.tsx
                snprintf(tsx_path, sizeof(tsx_path),
                         "%s/_implement_file/%s.tsx",
                         parent_of_proj, rel_no_ext);

                // >>> NEW: ensure the directory exists <<<
                {
                    char dirbuf[PATH_MAX];
                    strncpy(dirbuf, tsx_path, sizeof(dirbuf));
                    dirbuf[sizeof(dirbuf)-1] = '\0';
                    char *last_slash2 = strrchr(dirbuf, '/');
                    if (last_slash2) {
                        *last_slash2 = '\0'; // cut off filename
                        if (mkdir_p(dirbuf) != 0) {
                            fprintf(stderr, "Failed to create directory: %s\n", dirbuf);
                        }
                    }
                }

                // Emit tileset line
                append_to_buffer(tsx_buffer,
                                 "\t<tileset firstgid=\"%d\" source=\"%s\"/>\n",
                                 icon->global_id, tsx_path);
//

                *gid_tracker += icon->frame_count;
                icon->name = full_name;

                convert_dmi_to_tsx(icon, tsx_path);
                icon->name = atomInstance->icon;
                dmi_set->insert(dmi_set, icon);
            }
        } //If we did find the icon in the dmi_set, then we go to the else.
        else {
            char full_name[300] = {0};
            strcat(full_name, atomInstance->icon);
            Image* image = png_set->lookup(png_set, full_name);
            if (image == NULL && strcmp(atomInstance->icon, "") != 0) {
                image = (Image*)malloc(sizeof(Image));
                load_image2(image, full_name);
                image->global_id = *gid_tracker;

                char tsx_path[512];

                // --- compute parent of project dir from root_path ---
                char *last_slash = strrchr(root_path, '/');
                if (!last_slash) {
                    fprintf(stderr, "Invalid root_path: must contain at least one '/'\n");
                    return;
                }
                long long parent_len = last_slash - root_path;
                char parent_dir[300];
                strncpy(parent_dir, root_path, parent_len);
                parent_dir[parent_len] = '\0';

                // --- compute relative path of icon under project dir ---
                const char *rel = atomInstance->icon;
                size_t proj_len = strlen(root_path);
                if (proj_len > 0 &&
                    strncmp(atomInstance->icon, root_path, proj_len) == 0 &&
                    (atomInstance->icon[proj_len] == '/' || atomInstance->icon[proj_len] == '\\')) {
                    rel = atomInstance->icon + proj_len + 1; // skip project dir + slash
                }

                // --- strip extension ---
                char rel_no_ext[300];
                strncpy(rel_no_ext, rel, sizeof(rel_no_ext));
                rel_no_ext[sizeof(rel_no_ext)-1] = '\0';
                char *dot = strrchr(rel_no_ext, '.');
                if (dot) *dot = '\0';

                // --- build tsx path under _implement_file ---
                snprintf(tsx_path, sizeof(tsx_path),
                         "%s/_implement_file/%s.tsx",
                         parent_dir, rel_no_ext);

                // --- ensure directories exist ---
                {
                    char dirbuf[512];
                    strncpy(dirbuf, tsx_path, sizeof(dirbuf));
                    dirbuf[sizeof(dirbuf)-1] = '\0';
                    char *ls = strrchr(dirbuf, '/');
                    if (ls) {
                        *ls = '\0'; // cut off filename
                        if (mkdir_p(dirbuf) != 0) {
                            fprintf(stderr, "Failed to create directory: %s\n", dirbuf);
                        }
                    }
                }

                // --- append XML + emit tsx file ---
                append_to_buffer(tsx_buffer,
                                 "<tileset firstgid=\"%d\" source=\"%s\"/>\n",
                                 image->global_id, tsx_path);

                *gid_tracker += 1;
                png2tsx(image, tsx_path);

                png_set->insert(png_set, image);
            }
        }

        pop_counter++;
        entry->atoms.push_back(&entry->atoms, atomInstance);
    }

    sort_atoms(&entry->atoms);
    if(pop_counter > max_num_of_elements){
        max_num_of_elements = pop_counter;
    }
}

#include <time.h>
#include <stdlib.h>
char* parse_dmm(const char* file_name, hash_table* table, const char* file_name2, DM_Tracker *tracker){
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    FILE* map_file = fopen(file_name, "r");
    if (!map_file) {
        perror("Failed to open map file");
        return NULL;
    }

    char buffer[10000];
    int size_of_pop = -1;
    int first_map_pop = -1;
    int end_loop = 0;
    int map_width = 0;
    int map_height = 0;

    StringBuffer tsx_output;
    init_buffer(&tsx_output);

    int firstgid = 1;

    set* dmi_set = (set*)malloc(sizeof(set));
    initialize_set(dmi_set, 5000, hset_insert, dmi_look_key,
                   dmi_match_key, dmi_match_element,
                   hash_string, hash_dmi, dmi_get_key, NULL);

    set *png_set = (set*)malloc(sizeof(set));
    initialize_set(png_set, 5000, hset_insert, png_key_lookup, png_match,
                   png_match, hash_string, hash_png, png_get_key, NULL);

    hash_table *map_pops = (hash_table*)malloc(sizeof(hash_table ));
    init_hash_table(map_pops, 5000, hash_string,
                    (int (*)(const void *, const void *))string_match2,
                    (void (*)(void)) chtbl_lookup, insert_hash_table_icon,
                    (void (*)(void *)) destroy_hash_set);

    while(fgets(buffer, sizeof(buffer), map_file)){
        // map pop parsing (unchanged)
        if(buffer[0] == '"' && buffer[1] != '}'){
            MapEntry *entry = initialize_map_pop();
            parse_map_pop(buffer, entry, table, &tsx_output, &firstgid,
                          dmi_set, png_set, file_name2, tracker);
            insert_hash_table_pop(map_pops, entry->id, entry);
            if(size_of_pop == -1){
                size_of_pop = (int)strlen(entry->id);
            }
        }


        //We're checking for (x, y, z) here to determine if this is where the map array begins.
        if(buffer[0] == '('){
            if(first_map_pop == -1){
                do {
                    int map_index = 0;
                    while(buffer[map_index] != '\0'){
                        if(isalpha(buffer[map_index]) != 0){
                            first_map_pop = map_index;
                            break;
                        }
                        map_index++;
                    }
                    if(first_map_pop != -1){
                        break;
                    }
                } while(fgets(buffer, sizeof(buffer), map_file));
            }
            LayeredAtomRef ***atom_array = (LayeredAtomRef***)calloc(max_num_of_elements, sizeof(LayeredAtomRef**));
            for (int l = 0; l < max_num_of_elements; l++) {
                atom_array[l] = (LayeredAtomRef**)calloc(1000, sizeof(LayeredAtomRef*));
                for(int p = 0; p < 1000; p++){
                    atom_array[l][p] = (LayeredAtomRef*)calloc(1000, sizeof(LayeredAtomRef));
                }
            }
            map_width = (int)(strlen(buffer) - 1) / size_of_pop;
            map_height = 0;
            do {
                int map_index = 0;
                int tile_count = 0;
                while(buffer[map_index] != '\0'){
                    if(buffer[map_index] == '\n' || buffer[map_index] == '\t' || buffer[map_index] == ' '){
                        break;
                    }
                    if(buffer[map_index] == '"' && buffer[map_index+1] == '}'){
                        end_loop = 1;
                        break;
                    }

                    char *start_pointer = &buffer[map_index];
                    char *end_pointer = start_pointer + size_of_pop;
                    char store_value = *(end_pointer);
                    *end_pointer = '\0';
                    MapEntry* pop = get_entry_data(map_pops, start_pointer);

                    // The maximum depth for this particular pop.
                    int depth = (int)pop->atoms.current_capacity;

                    //The offset is where we will begin in applying via our layer_array.
                    /* IE - If max_num_of_elements = 10, and depth = 6, we start at index 4, and work our
                     * way up. */
                    int offset = max_num_of_elements - depth;
                    for(int k = depth - 1; k >= 0; k--){
                        AtomInstance *atom = (AtomInstance*)(pop->atoms.data + k * pop->atoms.byte_size);
                        // DMI lookup
                        char* find_png = strstr(atom->icon, ".png");

                        //If our icon is a png, then we DON'T take this branch.
                        // Otherwise, we also want to verify that this isn't an empty string.
                        if(find_png == NULL && strcmp(atom->icon, "") != 0){

                            //Find the icon in our set.
                            DMI_BYOND *icon = dmi_set->lookup(dmi_set, atom->icon);
                            if(icon != NULL){
                                int local_id_incr = get_icon_local_id_for_atom(icon, atom);
                                //
                                LayeredAtomRef *slot = &atom_array[offset + (depth - 1 - k)][map_height][tile_count];
                                slot->atom      = atom;
                                slot->layer_pos = offset + (depth - 1 - k);
                            }
                        }
                        char full_name[300] = {0};
                        strcat(full_name, atom->icon);
                        Image* image = png_set->lookup(png_set, full_name);
                        if (image != NULL) {

                            LayeredAtomRef *slot = &atom_array[offset + (depth - 1 - k)][map_height][tile_count];
                            slot->atom      = atom;
                            slot->layer_pos = offset + (depth - 1 - k);
                        }
                    }
                    tile_count++;
                    *end_pointer = store_value;
                    map_index += size_of_pop;
                }
                if(end_loop == 1){
                    break;
                }
                map_height++;
            } while(fgets(buffer, sizeof(buffer), map_file));
            char tsx_path[512];
            char *last_slash = strrchr(file_name2, '/');
            if (!last_slash) {
                fprintf(stderr, "Invalid root_path: must contain at least one '/'\n");
                return NULL;
            }
            int parent_len = (int)(last_slash - file_name2);
            char parent_dir[300];
            strncpy(parent_dir, file_name2, parent_len);
            parent_dir[parent_len] = '\0';
            char *icon_basename = strrchr(file_name, '/');
            if (!icon_basename) icon_basename = (char*)file_name;
            else icon_basename += 1;
            char base_no_ext[128];
            strncpy(base_no_ext, icon_basename, sizeof(base_no_ext));
            char *dot = strrchr(base_no_ext, '.');
            if (dot) *dot = '\0';
            snprintf(tsx_path, sizeof(tsx_path), "%s/_implement_file/%s.tmx", parent_dir, base_no_ext);
            FILE* tmxfile = fopen(tsx_path, "w+");
            if (!tmxfile) {
                perror("Failed to write tmx file");
                return NULL;
            }
            fprintf(tmxfile, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
            fprintf(tmxfile,
                    "<map version=\"1.9\" tiledversion=\"1.9.2\" "
                    "orientation=\"orthogonal\" renderorder=\"right-down\" width=\"%d\" "
                    "height=\"%d\" tilewidth=\"32\" tileheight=\"32\" infinite=\"0\" nextlayerid=\"%d\" "
                    "nextobjectid=\"209\">\n",
                    map_width, map_height, max_num_of_elements);

            size_t bufcap = (size_t)map_width * (size_t)map_height * 24u + 64u;
            char *layer_buf = (char*)malloc(bufcap);

            fprintf(tmxfile, "%s\n", tsx_output.data);
            for (int q = max_num_of_elements - 1; q >= 0; q--) {
                fprintf(tmxfile, "\t<layer id=\"%d\" name=\"layer%d\" width=\"%d\" height=\"%d\">\n",
                        q, q, map_width, map_height);
                fprintf(tmxfile, "\t\t<data encoding=\"csv\">\n");

                // Start with a reasonable estimate; grow as needed.
                if (!layer_buf) {
                    perror("malloc layer_buf");
                    fclose(tmxfile);
                    return NULL;
                }

                size_t offset = 0;

                for (int e = 0; e < map_height; e++) {
                    for (int f = 0; f < map_width; f++) {
                        int out_gid = 0;
                        LayeredAtomRef atom_ref = atom_array[q][e][f];
                        if (atom_ref.atom && atom_ref.atom->icon && atom_ref.atom->icon[0] != '\0') {
                            const char *icon_path = atom_ref.atom->icon;

                            // PNG case
                            if (strstr(icon_path, ".png") != NULL) {
                                Image *image = (Image*)png_set->lookup(png_set, (void*)icon_path);
                                if (image) out_gid = image->global_id;
                                else out_gid = 0;
                            }
                                // DMI case
                            else {
                                DMI_BYOND *icon = (DMI_BYOND*)dmi_set->lookup(dmi_set, (void*)icon_path);
                                if (icon) {
                                    int local = get_icon_local_id_for_atom(icon, atom_ref.atom);
                                    if (local < 0) local = 0;
                                    out_gid = icon->global_id + local;
                                } else {
                                    out_gid = 0;
                                }
                            }
                        } else {
                            out_gid = 0;
                        }

                        // Append "out_gid," safely (grow buffer if needed)
                        while (1) {
                            size_t remaining = bufcap - offset;
                            if (remaining < 32) {
                                size_t newcap = bufcap * 2;
                                char *tmp = (char*)realloc(layer_buf, newcap);
                                if (!tmp) {
                                    perror("realloc layer_buf");
                                    free(layer_buf);
                                    fclose(tmxfile);
                                    return NULL;
                                }
                                layer_buf = tmp;
                                bufcap = newcap;
                                continue;
                            }

                            int n = snprintf(layer_buf + offset, remaining, "%d,", out_gid);
                            if (n < 0) {
                                fprintf(stderr, "snprintf failed while writing layer CSV\n");
                                free(layer_buf);
                                fclose(tmxfile);
                                return NULL;
                            }

                            if ((size_t)n >= remaining) {
                                // Need more room; grow and retry
                                size_t newcap = bufcap * 2 + (size_t)n + 1;
                                char *tmp = (char*)realloc(layer_buf, newcap);
                                if (!tmp) {
                                    perror("realloc layer_buf");
                                    free(layer_buf);
                                    fclose(tmxfile);
                                    return NULL;
                                }
                                layer_buf = tmp;
                                bufcap = newcap;
                                continue;
                            }

                            offset += (size_t)n;
                            break;
                        }
                    }
                }

                fwrite(layer_buf, 1, offset, tmxfile);
                fprintf(tmxfile, "</data>\n");
                fprintf(tmxfile, "</layer>\n");
            }

            fprintf(tmxfile, "</map>\n");
            fclose(tmxfile);
            free(layer_buf);

            // Stop timer
            clock_gettime(CLOCK_MONOTONIC, &end_time);
            double elapsed_ms = (end_time.tv_sec - start_time.tv_sec) * 1000.0 +
                                (end_time.tv_nsec - start_time.tv_nsec) / 1.0e6;
            printf("[parse_dmm] Execution time: %.3f ms\n", elapsed_ms);

            int length = (int)strlen(tsx_path);
            char* return_stuff = malloc((sizeof(char) * length)+1);
            strcpy(return_stuff, tsx_path);
            return_stuff[length] = '\0';
            return return_stuff;
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double elapsed_ms = (end_time.tv_sec - start_time.tv_sec) * 1000.0 +
                        (end_time.tv_nsec - start_time.tv_nsec) / 1.0e6;
    printf("[parse_dmm] Execution time: %.3f ms\n", elapsed_ms);
    return NULL;
}
#ifdef __cplusplus
}
#endif
