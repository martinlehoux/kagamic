# Kagamic

## TODO

- Vec_get not safe, I made a type mistake. And should be more efficient for loops. maybe i don't use that many vec and i can define the types manually
- `valgrind --leak-check=full ./your_program`
- web server, Json parsing, struct codegen
- assert not in release
- python based generate
- escape chars
- unicode
- fail on any control character in string
- benchmark kind of tests

```c
SDL_GetError();
SDL_Window *window = SDL_CreateWindow("Hello World!", 100, 100, width, height, SDL_WINDOW_SHOWN);
SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
SDL_RenderClear(renderer);
SDL_RenderPresent(renderer);
SDL_Delay(500);
SDL_Rect rect = {c_position->x, c_position->y, 4, 4};
SDL_RenderFillRect(renderer, &rect);
SDL_Quit();
SDL_DestroyRenderer(renderer);
SDL_DestroyWindow(window);
```

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
