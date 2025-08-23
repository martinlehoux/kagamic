# Kagamic

## TODO

- Vec_get not safe, I made a type mistake. And should be more efficient for loops
- Fix tests in release
- Static functions for private?
- `valgrind --leak-check=full ./your_program`
- web server, Json parsing, struct codegen
- what is a const char \*
- whitechar simd
- assert not in release

## Benchmarks

| version        | result                                                              | throughput  | ram     |
| -------------- | ------------------------------------------------------------------- | ----------- | ------- |
| json.loads     | 652 ms ± 6.65 ms per loop (mean ± std. dev. of 7 runs, 1 loop each) |             |         |
| v1             | 336 ms ± 163 ms per loop (mean ± std. dev. of 10 runs, 1 loop each) |             |         |
| no 0 mem alloc | 299 ms ± 8 ms per loop (mean ± std. dev. of 10 runs, 1 loop each)   |             |         |
| Str_view       | 272 ms ± 6 ms per loop (mean ± std. dev. of 10 runs, 1 loop each)   | 330.46 MB/s | 1255 MB |
| simd           | 254 ms ± 6 ms per loop (mean ± std. dev. of 10 runs, 1 loop each)   | 353.36 MB/s | 1255 MB |

```c
void print_bytes(byte_32 *data) {
    for (i32 i = 0; i < 32; i++) {
        byte c = ((byte *)data)[i];
        if (c >= 32 && c <= 126) {
            printf("%c", c);
        } else {
            printf(".");
        }
    }
}
```

## Resources

- https://nullprogram.com/blog/2023/10/08/
- https://danielchasehooper.com/posts/typechecked-generic-c-data-structures/
