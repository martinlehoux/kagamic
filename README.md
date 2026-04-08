# Kagamic

## TODO

- Vec_get not safe, I made a type mistake. And should be more efficient for loops. maybe i don't use that many vec and i can define the types manually
- `valgrind --leak-check=full ./your_program`
- web server, Json parsing, struct codegen
- assert not in release
- unicode
- benchmark kind of tests

**JSON**
- fail on any control character in string
- escape chars


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

## Resources

- https://nullprogram.com/blog/2023/10/08/
- https://danielchasehooper.com/posts/typechecked-generic-c-data-structures/
- Hashing: https://github.com/aappleby/smhasher/
