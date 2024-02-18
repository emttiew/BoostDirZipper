# Overview 

**BoostDirZipper** is a lightweight console tool designed for efficient file compression and decompression. It utilizes the `boost iostreams` library and modern C++ techniques to manage file archives effectively, *including the option to skip adding files with identical binary content.

Currently, BoostDirZipper only works with the gzip file format.

*WIP

## Usage

```bash
❯ ./BoostDirZipper c|compress|d|decompress <source_path> <sink_path>
```
## Build

```bash
❯ mkdir build && cd build
❯ cmake ..
❯ make install
```