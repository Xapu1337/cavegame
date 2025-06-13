# AGENT INSTRUCTIONS

This repository uses an automated code assistant powered by GPT. The assistant reads the closest `AGENTS.md` file whenever it modifies files. These documents describe style conventions, testing requirements, and other rules.

## Guidelines
- **Variables** must use camelCase.
- **Functions** use PascalCase.
- **Constants** remain in SCREAMING_SNAKE case.

## Testing Procedure
When you change code, always run the following commands before committing:
1. `gcc -std=c11 -Wall -Wextra -O2 build_tool.c -o build_tool`
2. `./build_tool --verbose`
3. Optionally run `timeout 5 ./build/game` to ensure the program launches.

## Precedence
If multiple `AGENTS.md` files exist, the one located closest to the changed file overrides others. System and user instructions still override the contents of any `AGENTS.md`.

Modify or create an `AGENTS.md` file to change these rules. The assistant will read the updated instructions on the next run.
