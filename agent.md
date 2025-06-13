# Agent Instructions

This repository uses an automated code assistant powered by GPT. The assistant follows the guidelines in any `AGENTS.md` files found within the repository tree. These files specify style conventions, testing requirements and other rules the assistant must obey when making changes.

The assistant operates by examining these instructions before applying modifications. If multiple `AGENTS.md` files exist, the one closest to a modified file takes precedence. System level instructions and user prompts override the contents of `AGENTS.md`.

To modify the behavior of the assistant, edit or create an `AGENTS.md` file and commit it to the repository. The assistant will read the file on subsequent runs and adapt its behaviour accordingly.

