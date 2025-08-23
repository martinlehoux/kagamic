# Kagamic

## TODO

- Vec_get not safe, I made a type mistake. And should be more efficient for loops
- `valgrind --leak-check=full ./your_program`
- web server, Json parsing, struct codegen
- assert not in release
- python based generate
- test helpers to load json files
- add test with backspaces weird
- remove t_assert

## Benchmarks

| version          | result                                                              | throughput  | ram     |
| ---------------- | ------------------------------------------------------------------- | ----------- | ------- |
| json.loads       | 652 ms ± 6.65 ms per loop (mean ± std. dev. of 7 runs, 1 loop each) | 137.73 MB/s |         |
| orjson.loads     | 535 ms ± 8.64 ms per loop (mean ± std. dev. of 7 runs, 1 loop each) | 167.85 MB/s |         |
| v1               | 336 ms ± 163 ms per loop (mean ± std. dev. of 10 runs, 1 loop each) |             |         |
| no 0 mem alloc   | 299 ms ± 8 ms per loop (mean ± std. dev. of 10 runs, 1 loop each)   |             |         |
| Str_view         | 272 ms ± 6 ms per loop (mean ± std. dev. of 10 runs, 1 loop each)   | 330.46 MB/s | 1255 MB |
| simd             | 254 ms ± 6 ms per loop (mean ± std. dev. of 10 runs, 1 loop each)   | 353.36 MB/s | 1255 MB |
| negative         | 206 ms ± 6 ms per loop (mean ± std. dev. of 10 runs, 1 loop each)   | 436.25 MB/s | 1255 MB |
| init 6 obj array | 186 ms ± 6 ms per loop (mean ± std. dev. of 10 runs, 1 loop each)   | 483.96 MB/s | 1026 MB |

## Resources

- https://nullprogram.com/blog/2023/10/08/
- https://danielchasehooper.com/posts/typechecked-generic-c-data-structures/
