<img alt="Example myfile" src="out.gif" />


# myfile

It is:

* One file.
* Executable on Linux and MacOS.
* It stores content.
* When the file is executed $EDITOR edits that content.

## Usage

1. [Download the latest version][latest].
2. Run it with your favorite editor.

```
wget https://github.com/pelletier/myfile/releases/latest/download/myfile
chmod +x myfile
EDITOR=nano ./myfile
```


Note: this project uses [Actually Portable Executables (APE)][ape]. If you
encounter any issue running `myfile` make sure to check [their platform
notes][platform].

## Build

Create `myfile`:

```
make
```

### Requirements

* MacOS/Linux should work fine.
* `make`
* `curl`
* `zip`


## What?

This program serves no real purpose. I like the idea of a useful self-contained
executable that you can just download and run to do something useful. I also
enjoy programs that are at least reasonably portable. I had heard of the
[Cosmopolitan][cosmo] project and decided to check it out, play with it, and
learn a little bit more about it.

At the core of [Cosmopolitan][cosmo] are [Actually Portable Executables
(APE)][ape]. I knew coming in that APE files are a kind of zip file. I was
curious to see if they could be modified. It turns out, `zip` can add or update
such files correctly! So, I decided to write this little bit of code to use that
feature to write a program that carries mutable data. A simple text file with an
editor interaction would do.

[`myfile.c`][c] is definitely not the best C you'll ever read. Many shortcuts
were taken because I only had a couple of hours to dedicate to this project. My
first pass executed whatever `zip` and `unzip` programs were available on the host.
It worked but wasn't very satisfying. It didn't accomplish the goal of being
self-contained and required dependencies to be installed system-wide.

What if I could apply the same portability treatment to `zip` and `unzip` and
store them inside my own executable!?

The [Cosmopolitan][cosmo] project provides a set of [pre-built APEs][cosmos] of
well-known programs, which includes those programs! It seems like building their
APEs from source wouldn't be too difficult anyway, but that was a real time
saver. Quickly, I had a working `Makefile` that would grab the executables,
compile myfile, and inject them into it.

Now there is a bootstrapping issue. In order to execute those APEs, they need to
be extracted from the zip first. Browsing the cosmo libc source, I realized many
of their I/O functions have a special mode when the provided path starts with
`/zip`. In this case, "extracting a file from the program's zip structure"
boiled down to simply `open("/zip/zip")` and read it out to a new file on disk!
It can then be executed like any other program.

Unfortunately, `write()` treats `/zip/` as read-only. So this trick allows
extracting from the binary, but not writing back to it. It's fine because we now
have the APE version of `zip` inside `myfile`, which is now extracted and is
used to write back to the binary.

It also seems like `execve` has some support for `/zip`. Maybe it is possible to
execute the bundled `zip` without extracting it manually, but I could not get it
working during my limited hacking session.

## More

- Cosmopolian distributes actually portable versions of `nano`, `vim`, and
  `emacs`. Would be cool to bundle at least one them in myfile so that even
  editing does not require a separate program to be installed.
- Windows support should be doable given APE are supposed to work there. However
  I've heard self-updating programs are a pain on that platform.
- Wouldn't it be great if the content of the files could be edited without
  needing to create a tempory file on disk?


[ape]: https://justine.lol/ape.html
[c]: ./myfile.c
[cosmo]: https://github.com/jart/cosmopolitan
[cosmos]: https://cosmo.zip/pub/cosmos/
[latest]: https://github.com/pelletier/myfile/releases/latest/download/myfile
[platform]: https://github.com/jart/cosmopolitan?tab=readme-ov-file#platform-notes
