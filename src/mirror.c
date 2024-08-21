#include <gcc-plugin.h>
#include <plugin-version.h>
#include <c-tree.h>
#include <tree-pass.h>

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

void test(void *gcc_data, void *user_data) {
    printf("%s\n", ATTRIBUTE_NAME);
}

int plugin_init(struct plugin_name_args *plugin_info, struct plugin_gcc_version *version) {
    if(!plugin_default_version_check(version, &gcc_version))
        return 1;

    register_callback(PLUGIN_NAME, PLUGIN_INFO, NULL, &plugin_info);
    printf("loaded gcc plugin: %s v%s\n", PLUGIN_NAME, PLUGIN_VERSION);

    register_callback(PLUGIN_NAME, PLUGIN_FINISH_TYPE, test, NULL);

    struct pass_data pass_data =  {
        .opt_pass_type = GIMPLE_PASS,
        .name = PLUGIN_NAME,
        .optinfo_flags = OPTGROUP_NONE,
        .tv_id = TV_NONE,
        .properties_required = PROP_gimple_any,
        .properties_provided = 0,
        .properties_destroyed = 0,
        .todo_flags_start = 0,
        .todo_flags_finish = TODO_update_ssa | TODO_cleanup_cfg
    };

    struct register_pass_info pass_info = {
        .pass = &opt_pass,
        .reference_pass_name = "ssa",
        .ref_pass_instance_number = 1,
        .pos_op = PASS_POS_INSERT_AFTER
    };
    register_callback(PLUGIN_NAME, PLUGIN_PASS_MANAGER_SETUP, NULL, &pass_info);

    return 0;
}
