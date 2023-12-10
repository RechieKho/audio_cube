# `Cube` project template

`Cube` is a C project with a project structure (`Cube` project structure) aim to be modular,
in which `Cube` project could easily embedded into other `Cube` projects with 0 configuration.

What it is:

- Building desktop application and static library on a unix-like environment.
- Include other `Cube` projects as thirdparties with 0 configuration.
- Focus on modularity.

## Prerequisite

Basic Unix-like environment with basic C tools installed.
These are the application required:

- `ar`
- `clang`
- `mkdir`
- `cp`
- `echo`
- `make`
- `cat`
- `rm`
- `git`

`Cube` projects must be a `git` repository.

## `Cube` project structure

| Directory       | Description                                                                                                                                                                                                           |
| --------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `build`         | Store distributable files of the project and its thirdparty `Cube` projects.                                                                                                                                          |
| `build/bin`     | Stores built binary of the project and its thirdparty `Cube` projects. The binary has version as suffix (`<BIN_NAME>.<VERSION>`).                                                                                     |
| `build/include` | Stores header files of the project and its thirdparty `Cube` projects. The header files is stored in their own directory with its project's name and version as the name (`build/include/<PROJECT_NAME>/<VERSION>/`). |
| `build/lib`     | Stores built binary with the file with the naming format as `lib<PROJECT_NAME>.<VERSION>.a`.                                                                                                                          |
| `cube`          | Stores all the thirdparty `Cube` projects. It is handled by the `Cube` makefile.                                                                                                                                      |
| `gen`           | Stores the generated object files of the project.                                                                                                                                                                     |
| `include`       | Stores the header files to be distributed, it will be copied to the `build/include` directory.                                                                                                                        |
| `source`        | Stores all the `.c` source files.                                                                                                                                                                                     |
| `source/bin`    | Stores source files that will be build into executables. Each source file compiles to an executable with the same name and output to `build/bin`. The executable is linked to library in `build/lib`.                 |
| `source/lib`    | Stores source files that will be build into a static library, output to `build/lib`.                                                                                                                                  |

> [!NOTE]
>
> Project name (`<PROJECT_NAME>`) is the name of the directory your project resided.
> Version (`<VERSION>`) is the `git` hash or tag (if available).

## `Cube` makefile

The `Cube` makefile resides at the root of the project. It should automatically handle compiling and linking of the project and its thirdparty `Cube` projects.
It should:

- Consider `release` and `debug` build.
- Output the distributed files to the root `build` directory (more on [`Cube` project structure](#cube-project-structure)).
- Clean the build.
- Tell the thirdparty `Cube` projects to do the same.

### Considering build

There will be two build, `release` build and `debug` build.
The `Cube` makefile should have the `release` phony target and the `debug` phony target for the build.
A macro `DEBUG` is defined when compiling in `debug` build, while a macro `RELEASE` is defined when compiling in `release` build.

### Outputing distributed files

These are the important makefile variables:

- `ROOT_BUILD_BIN_DIR` - Path of the root `build/bin` directory.
- `ROOT_BUILD_INCLUDE_DIR` - Path of the root `build/include` directory.
- `ROOT_BUILD_LIB_DIR` - Path of the root `build/lib` directory.
- `ROOT_DEPENDENCIES_FILE` - Path of a dependency file recording dependencies and its sequence.

The parent `Cube` projects should defined these variables.
For the thirdparty `Cube` projects, the root `build` directory is passed to the makefile from the parent `Cube` projects with `export`.
The `Cube` makefile should record the library to `ROOT_DEPENDENCIES_FILE` (once it is compiled, _not recompiled_ as we don't want duplicates) in order to record the sequence of the dependencies.

The `ROOT_DEPENDENCIES_FILE` must named `<PROJECT_NAME>.<VERSION>.DEPENDENCIES` and reside in `ROOT_BUILD_LIB_DIR`.
The libraries depends on the libraries before itself in the `ROOT_DEPENDENCIES_FILE`.

### Clean the build

The `Cube` makefile should have a `clean` phony target that delete the file generated _by itself only_ and _not the files generated by the thirdparty `Cube` projects or the parent `Cube` projects_.
The root `Cube` project should be the one to clean up the root `build` directory.

### Compile thirdparty `Cube` projects

The `Cube` makefile should call `make` for building or cleaning on the thirdparty `Cube` projects living in the `cube` directory before building or cleaning itself.
Thus, the `Cube` makefile should have the `build-cube-release` phony target and the `build-cube-debug` phony target for the build, and it should be called before compiling self.

## `Cube` porting makefile

`Cube` porting makefile is a makefile that is purposed to make non-`Cube` project to be able to act as a thirdparty `Cube` project. It is nearly the same as the `Cube` makefile which should do these:

- Consider `release` and `debug` build.
- Output the distributed files to the root `build` directory (more on [`Cube` project structure](#cube-project-structure)).
- Clean the build.

Since itself is not really a `Cube` project so handling thirdparty `Cube` projects is unnecessary.

## Versioning

The version of the library is the `git` hash or tag (if available) of the current commit.
It is incorporated into the distributed files' name (as stated in the [`Cube` project structure](#cube-project-structure)).
A macro `VERSION` is also defined when compiling.
Unfortunately, the symbols of the library do not automatically incorporate the version.
Given this dependency tree:

```
parent
| - child_a
| | - child_b (version: v1_0_1)
| - child_b (version: v1_0_9)
```

Both `child_b v1_0_9` and `child_b v1_0_1` will output their own static library to the root `build` directory (the `parent`'s `build` directory).
Assuming both have the function `foo(int, int)`, it would have a linker duplicate symbol error when linking both library together.

To fix this issue, the programmer should consider the version when naming the function.

```c
#define APPEND_VERSION(identifier) identifier##VERSION

int APPEND_VERSION(foo)(int a, int b);
#define foo(a, b) (APPEND_VERSION(identifier)(a, b))

```

## Cross-compiling

Since it is all makefiles, it captures from the environment variables. Cross-compiling isn't in the job scope of the `Cube` makefile, the programmer should install the appropriate tools and setup the environment variables for cross-compiling.
