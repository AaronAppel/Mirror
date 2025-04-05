⚠️ [Experimental Branch] ⚠️

This branch contains unstable changes for features under development.

For Mirror documentation, please refer to the main branch [README.md](https://github.com/AaronAppel/Mirror/blob/main/README.md)

Use MIRROR_EXPERIMENTAL macro to enable/disable experimental features.

# Compiler Considerations (Experimental branch)
Mirror is currently developed using MSVC compiler. No efforts have been made to compile on GCC, Clang, or other compilers, at this time.
Builds with strict conformance (/permissive-) fail due to template compiler Error C2672 in multiple MIR_Mirror.h functions.

MSVC compiler versions have been known to change critical features that prevent experimental Mirror features from compiling.
Use with caution, and see [Known Working Compiler Versions](https://github.com/AaronAppel/Mirror?tab=readme-ov-file#known-working-compiler-versions) as outlined below.

# Known Working Compiler Versions
Using Visual Studio 2022
| Compiler | Version | Conformance        |
| -------- | ------- | ------------------ |
| MSVC     | 1938    | No                 |
| MSVC     | 1942    | Default            |
| MSVC     | 1943    | Default            |

ConstexprCounter.h the main problematic and non-conforming code, as well as functions for generating type info for containers and pairs.

# FAQ and Common Errors
#TODO List expected/common errors and their solutions
- Conformance command line argument /permissive compile errors
