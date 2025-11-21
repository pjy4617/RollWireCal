---
description: Refactor code while keeping tests green (REFACTOR phase of TDD cycle)
---

You are in the REFACTOR phase of the TDD cycle.

Prerequisites:
- ALL tests must be passing before starting
- If tests are not passing, STOP and fix them first

Your task:
1. Improve code structure WITHOUT changing behavior
2. Focus on:
   - Eliminating duplication
   - Improving clarity and naming
   - Extracting methods for better organization
   - Making dependencies explicit
   - Simplifying complex logic
3. Make ONE refactoring change at a time
4. Run tests after EACH refactoring step
5. If tests fail, undo the change immediately

Remember:
- This is the third step of Red → Green → Refactor
- Behavior must not change (tests prove this)
- Use established refactoring patterns with their proper names
- Keep methods small and focused on single responsibility
- After refactoring, consider if ready to commit or start next cycle
