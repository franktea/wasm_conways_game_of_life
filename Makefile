all:
	emcc sdl_ui.cpp -s WASM=1 -s USE_SDL=2 -O3 -o index.html

clean:
	rm -f index.html *.js *.wasm