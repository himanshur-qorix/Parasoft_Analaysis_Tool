# Source Code Files

Place your module's source code files (.c, .h, .cpp, .hpp) in this directory.

## Structure

```
source/
├── ModuleName_File1.c
├── ModuleName_File1.h
├── ModuleName_File2.c
├── ModuleName_File2.h
└── ...
```

## Requirements

- **Format**: C/C++ source and header files
- **Encoding**: UTF-8
- **Quality**: Code should be reviewed and cleaned
- **Justifications**: Include suppression comments in source

## Example

```c
// File: Mka_Main.c

#include "Mka_Main.h"

// Reason: Global state required for interrupt-safe operation. The state 
// machine must be accessible from both main context and CAN interrupt 
// handler. Static allocation ensures predictable memory usage (ASIL-D).
// Deviation approved in DR-2024-089.
// parasoft-suppress MISRA2012-RULE-8_9 "Interrupt safety requirement"
static MkaState_t g_MkaState;

void Mka_Init(void) {
    // Initialization code
}
```

## Tips

- Include representative files that show your module's patterns
- Ensure suppression comments are clear and detailed
- Add both simple and complex files for varied learning
- Keep file count reasonable (5-20 files typical)

---

**Place your source files here and remove this README.md**
