import re
import subprocess
from pathlib import Path

test_files = sorted(Path("src").rglob("test_*.c"))

lines: list[str] = []

for f in test_files:
    lines.append(f'#include "{f}"')

lines.append("")
lines.append(Path("base_test.c").read_text())
lines.append("i32 main() {")

for f in test_files:
    result = subprocess.run(["rg", "i32 test_", str(f)], capture_output=True, text=True)
    for match in re.finditer(r"test_[^(]+", result.stdout):
        func = match.group().rstrip()
        print(f"Found {func} in {f}")
        lines.append(f"    test({func});")

lines.append("""
    printf("Success: %d, Failures: %d\\n", success, failures);

    return failures;
}
""")

Path("test.c").write_text("\n".join(lines))
