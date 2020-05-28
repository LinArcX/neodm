<h4 align="center">
  <img src="https://img.shields.io/github/languages/top/LinArcX/neodm.svg"/>  <img src="https://img.shields.io/github/repo-size/LinArcX/neodm.svg"/>  <img src="https://img.shields.io/github/tag/LinArcX/neodm.svg?colorB=green"/>
</h4>

[![asciicast](./assets/images/asciinema_nedom.png)](https://asciinema.org/a/dK2lxcmSdjhrX0hpb7btywJxu)

## Installation

### Install it from source
You can install neodm by compiling from source, here's the list of dependencies required:

#### hostmake dependencies:
 - `pkg-config`
 - `cmake`
 - `gcc` or `clang`

#### buildtime dependencies:
 - `ncurses-devel`
 - `sqlite-devel`
 - `cdk`
 - `libaria2-devel`

#### Building
```
git clone https://github.com/LinArcX/neodm
mkdir build; mkdir release; cd build
cmake -DCMAKE_BUILD_TYPE=RELEASE ../;
make
```
And finally, run it:

`./release/neodm`

## Who's Neo?
<h1 align="center">
    <img src="assets/images/neo.gif" align="center" width="400"/>
</h1>

## Donate
- Bitcoin: `13T28Yd37qPtuxwTFPXeG9dWPahwDzWHjy`
<img src="assets/donate/Bitcoin.png" width="200" align="center" />

- Monero: `48VdRG9BNePEcrUr6Vx6sJeVz6EefGq5e2F5S9YV2eJtd5uAwjJ7Afn6YeVTWsw6XGS6mXueLywEea3fBPztUbre2Lhia7e`
<img src="assets/donate/Monero.png" align="center" />

## License
![License](https://img.shields.io/github/license/LinArcX/neodm.svg)
