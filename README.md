# wasm_conways_game_of_life
conway's game of life in wasm

Instructions:

1. Install Emscripten:

  http://emscripten.org

2. build

```
make clean all
```

or

```
emcc sdl_ui.cpp -s WASM=1 -s USE_SDL=2 -O3 -o index.html
```

3. run

run http server with python:

```
python3 -m http.server 8080
```

or `python -m SimpleHTTPServer 8080` in python2

then open http://127.0.0.1:8080 in web browser