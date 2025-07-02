---
name: Bug Report
about: Create a report to help us improve the library
title: '[BUG] '
labels: ['bug']
assignees: ''

---

## Bug Description
A clear and concise description of what the bug is.

## Environment
- **OS**: [e.g., Ubuntu 22.04, Yocto Kirkstone]
- **Architecture**: [e.g., x86_64, ARM64, ARM32]
- **Compiler**: [e.g., GCC 11.2, Clang 14]
- **Build System**: [e.g., CMake 3.22, Autotools]
- **Library Version**: [e.g., 1.0.0, git commit hash]

## Hardware Setup (if applicable)
- **Sensor Model**: [e.g., BGT60TR13C]
- **SPI Interface**: [e.g., /dev/spidev0.0]
- **GPIO Configuration**: [e.g., Reset: GPIO18, CS: GPIO24]
- **Platform**: [e.g., Raspberry Pi 4, BeagleBone Black]

## Steps to Reproduce
1. Go to '...'
2. Click on '....'
3. Scroll down to '....'
4. See error

## Expected Behavior
A clear and concise description of what you expected to happen.

## Actual Behavior
A clear and concise description of what actually happened.

## Code Sample
If applicable, provide a minimal code sample that reproduces the issue:

```c
#include "xensiv_bgt60trxx.h"
#include "xensiv_bgt60trxx_linux.h"

int main() {
    // Your code here
    return 0;
}
```

## Build Command
The exact command used to build the library:
```bash
./build.sh --clean --examples
```

## Error Output
If applicable, paste the complete error output:
```
Error messages here...
```

## Additional Context
Add any other context about the problem here, such as:
- Does this work with the original ModusToolbox implementation?
- Are there any workarounds you've found?
- Any relevant system logs or debug output?

## Checklist
- [ ] I have searched existing issues to ensure this is not a duplicate
- [ ] I have provided all the requested information above
- [ ] I have tested with the latest version of the library
- [ ] I have included a minimal reproducible example (if applicable)