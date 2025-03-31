; COIL configuration for development environment
[processor]
execution_mode = "interpreter"
debug_level = 3
memory_model = "strict"
bounds_checking = true
optimization_level = 0
max_memory = 1073741824    ; 1 GB
stack_size = 8388608       ; 8 MB

[assembler]
include_paths = ["./include", "${ENV:COIL_INCLUDE_PATH}"]
defines = {"DEBUG": "1", "VERSION": "1.0.0"}
warning_level = 3
treat_warnings_as_errors = true
generate_debug_info = true
optimization_level = 0

[linker]
library_paths = ["./lib", "${ENV:COIL_LIB_PATH}"]
libraries = ["stdmem", "stdio"]
entry_point = "main"
generate_map_file = true
strip_debug_info = false
merge_sections = true

[debug]
source_paths = ["./src"]
debug_level = 3
break_on_error = true
watchpoints = []
log_file = "debug.log"

[security]
stack_protection = true
executable_stack = false
memory_randomization = true
control_flow_protection = true
sanitize_options = ["address", "undefined"]

[target]
processor = 0x01           ; CPU
architecture = 0x01        ; x86
mode = 0x03                ; 64-bit
capabilities = ["sse4.2", "avx2", "fma3"]

; Platform-specific overrides
[processor:windows]
execution_mode = "interpreter"

[processor:linux]
execution_mode = "jit"