"""Analyze distribution quality of a polynomial rolling hash function."""

import math
import random
import string
from collections import Counter


MAX_BUCKETS = 1000
MULTIPLIER = 31


def hashstring(s: str) -> int:
    h = 0
    for c in s:
        h = h * MULTIPLIER + ord(c)
    return h % MAX_BUCKETS


def chi_squared(counts: list[int], n_samples: int) -> float:
    """Chi-squared statistic vs uniform distribution."""
    expected = n_samples / MAX_BUCKETS
    return sum((c - expected) ** 2 / expected for c in counts)


def generate_words(n: int) -> list[str]:
    """Random lowercase alpha strings of length 3-10."""
    rng = random.Random(42)
    return [
        "".join(rng.choices(string.ascii_lowercase, k=rng.randint(3, 10)))
        for _ in range(n)
    ]


def analyze(label: str, keys: list[str]) -> None:
    buckets = Counter(hashstring(k) for k in keys)
    counts = [buckets.get(i, 0) for i in range(MAX_BUCKETS)]

    filled = sum(1 for c in counts if c > 0)
    max_count = max(counts)
    min_count = min(counts)
    mean = len(keys) / MAX_BUCKETS
    variance = sum((c - mean) ** 2 for c in counts) / MAX_BUCKETS
    std_dev = math.sqrt(variance)
    chi2 = chi_squared(counts, len(keys))
    # Degrees of freedom = MAX_BUCKETS - 1 = 999
    # chi2 critical value at p=0.05 is approx 1073.6
    chi2_critical = 1073.6

    print(f"\n=== {label} ({len(keys)} keys) ===")
    print(
        f"  Filled buckets : {filled} / {MAX_BUCKETS} ({100 * filled / MAX_BUCKETS:.1f}%)"
    )
    print(f"  Min / Mean / Max count : {min_count} / {mean:.2f} / {max_count}")
    print(f"  Std dev : {std_dev:.3f}  (ideal ≈ {math.sqrt(mean):.3f})")
    print(f"  Chi² : {chi2:.1f}  (critical at p=0.05: {chi2_critical})")
    print(f"  Verdict: {'GOOD' if chi2 < chi2_critical else 'POOR'} distribution")

    # Simple ASCII histogram of top/bottom 5 buckets
    top5 = sorted(range(MAX_BUCKETS), key=lambda i: -counts[i])[:5]
    print("  Top-5 hottest buckets:")
    for b in top5:
        bar = "#" * min(counts[b], 60)
        print(f"    bucket {b:4d}: {counts[b]:4d}  {bar}")


def sequential_integers(n: int) -> list[str]:
    return [str(i) for i in range(n)]


def short_keys(n: int) -> list[str]:
    """Single-character and two-character keys to expose low-entropy edge cases."""
    keys: list[str] = []
    for c in string.ascii_lowercase:
        keys.append(c)
    for a in string.ascii_lowercase:
        for b in string.ascii_lowercase:
            keys.append(a + b)
            if len(keys) >= n:
                return keys
    return keys


def main() -> None:
    print(f"Hash parameters: MAX_BUCKETS={MAX_BUCKETS}, MULTIPLIER={MULTIPLIER}")

    analyze("Random strings", generate_words(10_000))
    analyze("Sequential integers", sequential_integers(10_000))
    analyze("Short keys (1-2 chars)", short_keys(700))


if __name__ == "__main__":
    main()
