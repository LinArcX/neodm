<h4 align="center">
  <img src="https://img.shields.io/github/languages/top/LinArcX/neodm.svg"/>  <img src="https://img.shields.io/github/repo-size/LinArcX/neodm.svg"/>  <img src="https://img.shields.io/github/tag/LinArcX/neodm.svg?colorB=green"/>
</h4>

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

### Distributions
#### Void
`sudo xbps-install -S neodm`

#### Arch
https://aur.archlinux.org/packages/neodm-git

## Who's Neo?
<h1 align="center">
    <img src="assets/images/neo.gif" align="center" width="400"/>
</h1>

## License
![License](https://img.shields.io/github/license/LinArcX/neodm.svg)
