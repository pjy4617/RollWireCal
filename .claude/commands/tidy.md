---
description: Perform structural changes only (Tidy First principle)
---

You are following the TIDY FIRST principle.

Prerequisites:
- ALL tests must be passing before starting
- If tests are not passing, STOP and fix them first

Your task:
1. Make ONLY structural changes that do NOT alter behavior:
   - Renaming variables, methods, or classes
   - Extracting methods or functions
   - Moving code to better locations
   - Reorganizing file structure
   - Improving code formatting
2. Make ONE structural change at a time
3. Run tests after EACH change to validate behavior is unchanged
4. If tests fail, the change WAS behavioral - undo it

Important rules:
- NEVER mix structural and behavioral changes
- Always make structural changes BEFORE behavioral changes
- Commit structural changes separately from behavioral changes
- Validate that tests pass before AND after each change
- Use clear commit messages stating "STRUCTURAL: [description]"

Remember:
- Tidy First separates "how the code looks" from "what the code does"
- This makes behavioral changes easier and safer
- Structure improvements reduce risk of future changes
