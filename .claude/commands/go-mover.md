---
description: Find the next unmarked test in RollWireMover plan.md and execute the TDD cycle
---

Follow the instructions in CLAUDE.md strictly.

1. Read docs/RollWireMover/plan.md to find the next unmarked test
2. If docs/RollWireMover/plan.md doesn't exist, ask the user what feature to implement next
3. Execute the complete TDD cycle for that test:
   - RED: Write a failing test for the next small increment of functionality
   - GREEN: Implement only the minimum code needed to make that test pass
   - REFACTOR: Improve code structure while keeping tests green

Important reminders:
- Write ONE test at a time
- Use meaningful test names that describe behavior
- Implement the simplest solution that could possibly work
- Run all tests (except long-running tests) after each step
- Separate structural changes from behavioral changes
- Only commit when ALL tests pass and there are NO warnings
- Mark the test in docs/RollWireMover/plan.md when complete
