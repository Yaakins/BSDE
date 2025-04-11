# BSDE (BullShit Desktop Environment)

### What is this for ?

Litterally nothing. I found it fun to play with different low-level features on my computer, and I wanted to assemble them into a project.
One of the main idea was to make it lib-free, meaning I could only use code I wrote, and I understood (i.e. I can reuse a lib if I apply the algorithm to my case)

### How to use it ?

You will need `gcc` to build it and a linux distribution (only tested on Ubuntu so far).
clone the repo and simply use `make` to build, then `make run` to use the programm
It requires superuser privileges, so running it will ask you your password

### TODO

- add texture clipping when blitting, to avoid segfault on lower part of the screen
- setup keyboard event support
- implement texture loading (probably means reimplementing the whole libpng, so not right now)
- perhaps setup some audio output ?

### Contact

If you want to make the project progress, do not hesitate to fork/pull request, or contact me on discord, my username is `yaakin`
Hoping you like this project ^^
