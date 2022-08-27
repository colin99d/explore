# Explore

A video game where you explore the surroundings.

## Compiling the Game

The C component of this project is dependent on the following packages:
- [sdl2](https://github.com/libsdl-org/SDL/releases/tag/release-2.24.0)
- [sdl2-image](https://github.com/libsdl-org/SDL_image/releases)
- [sdl2-ttf](https://github.com/libsdl-org/SDL_ttf/releases)
- [sdl2-mixer](https://github.com/libsdl-org/SDL_mixer/releases)

On mac all of these can be installed using homebrew. If C has issues finding the files you may
need to include the following code in your bash/zsh config file:
```
export CPATH=/opt/homebrew/include
export LIBRARY_PATH=/opt/homebrew/lib
```

To build from source:
- cd into project
- ./configure
- make
- make install

## Generating the Resources

The game uses python to dynamically create images. Run the following steps to create:
- Install python
- Add python and pip to your PATH
- run `pip install pillow`
- run `python make_images.py`
