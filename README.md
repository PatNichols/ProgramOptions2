# ProgramOptions2

A set of header files to allow one to enter program options in a sane way.
1) Options may be set by add_option or by parsing a file containing the options.
2) The options can be set by reading the command line, a file or environmental variables
3) for command line parsing -key=value, -key value, --key value and --key=value are allowed.
4) for option set file a single option is given per line with key=value or key value syntax
5) environmental values are through a chosen prefix (can be null) e.g. setenv PREFIX_KEY VALUE or export PREFIX_KEY=value
6) options are entered into file to create options in one of the following way with a quoted description string:
   a) keyword "description which can be any length" value required
   b) keyword "description which can be any length" value
   c) keyword "description which can be any length" required
   d) keyword "description ..."
   e) keyword value required
   f) keyword value
   g) keyword required
   h) keyword
  note the required string will denote that the option in question must have a value!  
