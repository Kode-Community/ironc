# Ironc

C port of the [iron](https://github.com/armory3d/iron) haxe library.

Goals:

- Allocation free library(extending languages will be responsible for the memory)
- Integration with multiple languages(through the kincmake idl to language generator)
- On par with the haxe version(i.e. become it's replacement eventually)

Roadmap:

- [ ] API parity with the haxe version
    - [X] Add the Kha scheduler
    - [X] Have traits
    - [ ] Recreate the Object class
    - [ ] Recreate the App class
    - [ ] Recreate the Scene class
- [ ] Test language generators
    - [ ] Beef
    - [ ] C#
    - [ ] Rust
    - [ ] Lua/Teal
    - [ ] Wren 
    - [ ] Jai
    - [ ] Other community-driven
        - [ ] Odin
        - [ ] Zig
- [ ] Add a idl to haxe generator to kincmake(to replace iron)
    - [ ] Add Krom gc
    - [ ] Add hxcpp gc
    - [ ] hl maybe, even though it's slow ?