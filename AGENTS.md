# Agent & Skill Configuration

## Recommended Agents by Task Type

### Code Review & Quality
- **code-review** — Security, bugs, reuse cleanup. Use `low` for correctness, `high` for thorough analysis.
- **simplify** — Efficiency & reuse cleanups only (no bug hunting).

### Architecture & Planning
- **Plan** — Design multi-feature implementations, test strategies, refactoring approaches.
- **Explore** — Fast codebase navigation, find files/symbols, analyze dependencies.

### Development & Debugging
- **run** — Launch interactive demos, test features in terminal.
- **verify** — Confirm memory safety, test Valgrind output, validate behavior changes.

---

## Installed Skills

| Skill | What it does |
|-------|-------------|
| memory-safety-patterns | C memory management (malloc/free), Valgrind, heap analysis |
| architecture-patterns | Design patterns, DSA structure decisions, system architecture |
| code-review-excellence | Deep code review, bug hunting, security analysis |
| c-data-structures | C-specific DSA implementations, pointers, memory patterns |
| codebase-summarizer | Generate project overviews, understand structure |
| conventional-commit | Format commits to conventional standard (feat/fix/etc) |
| github-actions-docs | GitHub Actions workflow documentation & patterns |

---

## Usage Examples

### Adding a New Data Structure
```bash
/plan "Add Doubly Circular Linked List (DCLL) with step-by-step demo"
```

### Before Pushing Changes
```bash
/code-review --fix       # Auto-fix reuse issues
/verify                  # Confirm Valgrind passes
```

### Understanding Codebase
```bash
/explore "quick" "find all BST-related files"
```

### Optimizing Algorithm
```bash
/simplify               # Check efficiency
```

---

## CI/CD Integration
- GitHub Actions runs on every push (compile + test + Valgrind)
- Merge freeze if any test fails or memory error detected
- All PRs must include Valgrind clean certification
