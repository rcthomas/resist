# resist

## Developer Guide

### Workflow

* The `resist` project follows a pull-only model.
* @rcthomas is the only person with write access to the repository.
* @rcthomas shall wait for comment from another person before merging his own pull requests.
* Every commit should compile and run correctly.
* "Work-in-progress" commits do not belong anywhere in this repository.
* Compilation with -Wall -Werror (or equivalent for non-GCC compilers) should succeed on every commit.
* Code style is enforced before commit and compliance is required before merge.

### Coding Style

* To enforce style we rely on [`uncrustify`](https://github.com/uncrustify/uncrustify).
* Right now developers need to have `uncrustify` installed.
* The `uncrustify.cfg` file encodes our standard, and is adapted from the `kr-indent.cfg` example.
* Hence the coding style for this project is based on K&R with some minor modifications.
* Before making a commit, execute `make format` --- if that passes go ahead and commit.
* If it doesn't pass, you can use `uncrustify -c uncrustify.cfg --replace --no-backup file...` as a short cut.
* If a PR is made that fails to pass the formatting check, it is summarily rejected.

### Versioning and Release Schedule

* We observe [semantic versioning](http://semver.org/).
* Minor releases will happen in February, May, August, and November with patches as needed in between.
* First release is thus scheduled for February 2017.
