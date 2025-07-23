# Kagamic

## TODO

- Vec_get not safe, I made a type mistake. And should be more efficient for loops
- Fix tests in release
- Benchmark 100MB file
  - json.loads: 526 ms ± 77 ms per loop (mean ± std. dev. of 7 runs, 1 loop each)
  - self: 348 ms (mean of 7 runs, 1 loop each). 2079 MB/s
- Static functions for private?
- `valgrind --leak-check=full ./your_program`
- assert with error msg?
- negative numbers

## Resources

- https://nullprogram.com/blog/2023/10/08/
- https://danielchasehooper.com/posts/typechecked-generic-c-data-structures/
