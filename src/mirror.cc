#include <gcc-plugin.h>
#include <plugin-version.h>
#include <tree.h>
#include <tree-pass.h>
#include <gimple.h>
#include <context.h>
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

static unsigned int test(void) {
    printf("test\n");

    return 0;
}

class gimple_pass : public gimple_opt_pass {
    public:

        gimple_pass (const pass_data& data, gcc::context *ctxt) : gimple_opt_pass (data, ctxt) {}

        bool gate (function* gate_fun)
        {
            return true;
        }

        unsigned int execute(function* exec_fun)
        {
            return test();
        }
};

static tree handle_mirror_attribute(tree *node, tree name, tree args, int flags, bool *no_add_attrs) {
    printf("Found attribute\n");

    printf("\tnode = ");
    print_generic_stmt(stdout, *node, TDF_NONE);

    printf("\tname = ");
    print_generic_stmt(stdout, name, TDF_NONE);

    return NULL_TREE;
}

static void register_attr(void *gcc_data, void *user_data) {
    static struct attribute_spec attr = {
        .name = ATTRIBUTE_NAME,
        .min_length = 0,
        .max_length = 0, // strlen(ATTRIBUTE_NAME)?
                         // TODO: change when starting more support
        .decl_required = false,
        .type_required = true,
        .function_type_required = false,
        .affects_type_identity = false, // ?
        .handler = handle_mirror_attribute,
        .exclude = NULL
    };

    register_attribute(&attr);
}

int plugin_init(struct plugin_name_args *plugin_info, struct plugin_gcc_version *version) {
    if(!plugin_default_version_check(version, &gcc_version))
        return 1;

    register_callback(PLUGIN_NAME, PLUGIN_INFO, NULL, &plugin_info);
    printf("loaded gcc plugin: %s v%s\n", PLUGIN_NAME, PLUGIN_VERSION);

    struct pass_data pass_data =  {
        .type = GIMPLE_PASS,
        .name = PLUGIN_NAME,
        .optinfo_flags = OPTGROUP_NONE,
        .tv_id = TV_NONE,
        .properties_required = PROP_gimple_any,
        .properties_provided = 0,
        .properties_destroyed = 0,
        .todo_flags_start = 0,
        .todo_flags_finish = TODO_update_ssa | TODO_cleanup_cfg
    };
    gimple_pass pass = gimple_pass(pass_data, g);

    struct register_pass_info pass_info = {
        .pass = &pass,
        .reference_pass_name = "ssa",
        .ref_pass_instance_number = 1,
        .pos_op = PASS_POS_INSERT_AFTER
    };
    register_callback(PLUGIN_NAME, PLUGIN_PASS_MANAGER_SETUP, NULL, &pass_info);

    register_callback(PLUGIN_NAME, PLUGIN_ATTRIBUTES, register_attr, NULL);

    return 0;
}
