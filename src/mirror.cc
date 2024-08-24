#include <gcc-plugin.h>
#include <plugin-version.h>
#include <tree.h>
#include <tree-pass.h>
#include <gimple.h>
#include <context.h>
#include <stringpool.h>
#include <attribs.h>
#include <tree-pretty-print.h>

#define ATTRIBUTE_NAME "mirrored"

#define PLUGIN_NAME "mirror"
#define PLUGIN_VERSION "0.1"
#define PLUGIN_HELP "Usage: registers an attribute " ATTRIBUTE_NAME

#define PLUGIN_GCC_BASEVER "9.2.1"

int plugin_is_GPL_compatible;

static struct plugin_info plugin_info = {
    .version = PLUGIN_VERSION,
    .help = PLUGIN_HELP
};

static struct plugin_gcc_version plugin_version = {
    .basever = PLUGIN_GCC_BASEVER
};

static void register_attr(void *gcc_data, void *user_data) {
    static struct attribute_spec attr = {
        .name = ATTRIBUTE_NAME,
        // TODO: maybe some args for different reflection functionality
        .min_length = 0,
        .max_length = 0,
        .decl_required = false,
        .type_required = false,
        .function_type_required = false,
        .affects_type_identity = false,
        .handler = NULL,
        .exclude = NULL
    };

    register_attribute(&attr);
}

static void mirror(void *gcc_data, void *user_data) {
    tree type = (tree)gcc_data;
    if(!RECORD_OR_UNION_CHECK(type) || TREE_CODE(type) != RECORD_TYPE || TYPE_FIELDS(type) == NULL_TREE)
        return;

    tree attr = lookup_attribute(ATTRIBUTE_NAME, TYPE_ATTRIBUTES(type));
    if(attr == NULL_TREE)
        return;
    print_generic_stmt(stdout, type);

    if(!COMPLETE_TYPE_P(type))
        return;

    // tree name = TYPE_IDENTIFIER(type);
    // if(name == NULL)
    //     return;
    // const char *name_ser = IDENTIFIER_POINTER(name);
    // printf("%s\n", name_ser);

    print_generic_stmt(stdout, type->type_common.size);
}

int plugin_init(struct plugin_name_args *plugin_info, struct plugin_gcc_version *version) {
    if(!plugin_default_version_check(version, &gcc_version))
        return 1;

    register_callback(PLUGIN_NAME, PLUGIN_INFO, NULL, &plugin_info);
    printf("loaded gcc plugin: %s v%s\n", PLUGIN_NAME, PLUGIN_VERSION);

    register_callback(PLUGIN_NAME, PLUGIN_ATTRIBUTES, register_attr, NULL);
    register_callback(PLUGIN_NAME, PLUGIN_FINISH_TYPE, mirror, NULL);

    return 0;
}
