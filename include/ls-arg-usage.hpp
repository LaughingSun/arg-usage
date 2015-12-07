
#ifndef LS_ARG_USAGE_HPP
#define LS_ARG_USAGE_HPP

#include <cstdlib>
#include <cstring>
#include <functional>
#include <string>
#include <iostream>


namespace ls 
{
  class ArgUsage;
  
  typedef struct arg_usage_pos 
  {
    char** args;        // argument list (probably argv)
    int args_i;         // current args index
    int args_i_o;       // current char index within args[args_i]
    int args_c;         // end of args
    int marked_i;       // last marked args index
    int marked_i_b;     // last marked char begin index within args[marked_i]
    int marked_i_e;     // last marked char end index within args[marked_i]
  } arg_usage_pos_t;
  
  typedef int arg_usage_uid_t;
  typedef enum predefined_arg_usage_uid : arg_usage_uid_t 
  {
    auid_undef                            // arguid is undefined
    , auid_terminator  = 0x200000         // "--" request stop argument parsing
    , auid_double_dash = auid_terminator  // alias auid_terminator
    , auid_stdin                          // "-"  represents stdin as argument
    , auid_dash = auid_stdin              // alias auid_stdin_arg
  } predefined_arg_usage_uid_t;
  
  typedef enum arg_usage_flags : size_t
  {
    auf_default       = 0           // use defaults
    
    , auf_alias       = 0x00000001  // alias of previous definition
    
    , auf_short       = 0x00000002  // command is short form (single char)
                                    // auid is used as the name.  if there is
                                    // also a non-zero name field then the auid
                                    // name is appended as an alias.  shorts 
                                    // are prefixed with a short form (default 
                                    // '-') first character of the argument
                                    // the first character of the 
    , auf_short_single= 0x00000004  // no trailing commands
                                    // within the same argument
    
    , auf_cmdarg      = 0x00000008  // command accepts an argument
    , auf_cmdarg_opt  = 0x00000010  // command argument is optional
    , auf_cmdarg_file = 0x00000020  // command argument considered as file or 
                                    // stream name
    
    , auf_cmdsep      = 0x00000040  // command uses a seperator
    , auf_cmdsep_opt  = 0x00000080  // command seperator is optional
    
    , auf_cmdicase    = 0x00000100  // command is case insensative
    
    , auf_cmdlast     = 0x00000200  // command is terminator, all following 
                                    // args considered as non-command arguments
    
    , auf_no_prefix   = 0x00000400  // command doesn't require "--" prefix
    
    , auf_extend_desc = 0x00000800  // the desc field is appended to previous
                                    // seperated by endls.  all other fields 
                                    // are ignored unless the alias flags is 
                                    // also set
    
    , auf_child_link   = 0x00001000 // command expects child commands and 
                                    // au_link is considered as child def
    
    , END_OF_DEFINITION_LIST = (size_t)-1
  } arg_usage_flags_t;
  
  typedef enum arg_usage_errnos : int
  {
    aue_ok
    , aue_command_undefined
    , aue_command_warning
    , aue_command_error
    , aue_seperator_required
    , aue_argument_required
  } arg_usage_errno_t;
  
  typedef struct arg_usage_def
  {
    size_t flags;                 // flags
    arg_usage_uid_t auid;         // command auid (and short name if auf_short)
    const char* name;             // name
    const char* arg;              // argument(s)
    const char* desc;             // description
    int (*caller) (               // caller
      arg_usage_errno_t au_errno, struct arg_usage_def* audef
      , arg_usage_pos_t* au_pos, ArgUsage* au, void* userdata );
    struct arg_usage_def* au_link;// extended (or child) command definition list
  } arg_usage_def_t;
  
  typedef arg_usage_errno_t (*command_caller_t) ( arg_usage_errno_t au_errno
      , arg_usage_def_t* audef, arg_usage_pos_t* aupos, ArgUsage* au
      , void* userdata );
  
  typedef struct usage_options
  {
    const char*   name_prefix;
    const char**  short_form_prefix;
    const char**  seperators;
    int (*user_caller) (               // caller
      arg_usage_errno_t au_errno, arg_usage_def_t* audef, arg_usage_pos_t* aupos
      , ArgUsage* au, void* userdata );
    bool          allow_undefined;
  } usage_options_t;
  
  class ArgUsage
  {
    
    
    
public:
    
    ArgUsage ( const char* program_name
//        , const char* program_version
        , const char* argument_names
        , const char* program_summary
        , arg_usage_def_t* usage_definitions
        , usage_options_t* options );
  
    
    
private:
    
    const char* _program_name;
    const char* _program_version;
    const char* _argument_names;
    std::string _program_summary;
    arg_usage_def_t* _usage_definitions;
    usage_options_t* _options;

  };  // class ArgUsage
  
}; //namespace ls

#endif  // LS_ARG_USAGE_HPP

