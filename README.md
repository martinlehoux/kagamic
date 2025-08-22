# Kagamic

## TODO

- Vec_get not safe, I made a type mistake. And should be more efficient for loops
- Fix tests in release
- Static functions for private?
- `valgrind --leak-check=full ./your_program`
- assert with error msg?
- negative numbers
- web server, Json parsing, struct codegen

## Benchmarks

| version | result | throughput | ram|
|---|---|---|---|
|json.loads|652 ms ± 6.65 ms per loop (mean ± std. dev. of 7 runs, 1 loop each)|||
|v1|336 ms ± 163 ms per loop (mean ± std. dev. of 10 runs, 1 loop each)|||
|no 0 mem alloc|299 ms ±   8 ms per loop (mean ± std. dev. of 10 runs, 1 loop each)|||
|Str_view|272 ms ±   6 ms per loop (mean ± std. dev. of 10 runs, 1 loop each)|330.46 MB/s|1255 MB|

## Resources

- https://nullprogram.com/blog/2023/10/08/
- https://danielchasehooper.com/posts/typechecked-generic-c-data-structures/
