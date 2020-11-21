# Game of Life

ref: https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life

just for fun :))

## Usage

### Compile

```sh
$ make
```

### Run
```sh
$ ./gol --help
-h, --height <h>       board height (default: number of rows of terminal)
-w, --width <w>        board width (default: number of columns of terminal)
-m, --millisecond <m>  refresh interval (default: 150)
--help                 print help message
```

```sh
# Example
$ ./gol --height 30 --width 40 -m 150
```

> Notice: since the board will be framed up with a box, the final height and width to be displayed is height+2 and width+2.  
