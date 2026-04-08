import re
from pathlib import Path

bench_files = sorted(Path("src").rglob("bench_*.c"))

lines: list[str] = []

for f in bench_files:
    lines.append(f'#include "{f}"')

lines.append("")
lines.append(Path("base_bench.c").read_text())
lines.append("i32 main() {")

for f in bench_files:
    # group label from filename: bench_vec.c -> "vec", bench_vec_sort.c -> "vec sort"
    stem = f.stem.removeprefix("bench_").replace("_", " ")

    src = f.read_text()

    # find setup function (setup_* in this file)
    setup_match = re.search(r"void (setup_\w+)\(", src)
    setup = setup_match.group(1) if setup_match else None

    funcs = re.findall(r"i32 (bench_\w+)\(", src)

    if not funcs:
        continue

    reference = funcs[0]
    print(f"Group '{stem}' from {f}: {funcs}")
    lines.append(f'    bench_group("{stem}");')
    for i, func in enumerate(funcs):
        compare_arg = f'"{reference}"' if i > 0 else "NULL"
        lines.append(f'    bench("{func}", {setup}, {func}, {compare_arg});')

lines.append("    return 0;\n}\n")

Path("bench.c").write_text("\n".join(lines))
