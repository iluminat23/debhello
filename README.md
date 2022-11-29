# debhello <!-- omit in toc -->

- [debhello](#debhello)
- [branches](#branches)
  - [master](#master)
  - [deb](#deb)
- [debianization](#debianization)
- [step-by-step](#step-by-step)
  - [1. debmake](#1-debmake)
  - [2. debian/rules](#2-debianrules)
  - [3. debian/control](#3-debiancontrol)
  - [4. debian/changelog](#4-debianchangelog)
  - [5. debian/copyright](#5-debiancopyright)
  - [6. 3.0 source format](#6-30-source-format)
  - [7. debian/watch](#7-debianwatch)

## debhello

The *Hello World* Debian package.

This repository contains the following branches:

- master: <https://github.com/iluminat23/debhello/tree/master>
- deb: <https://github.com/iluminat23/debhello/tree/deb>

## branches

### master

The *master* branch contains a simple program written in C. It can be build with CMake.

### deb

The *deb* branch contains an example debianization. If you get stuck or need an example you can
checkout the *deb* branch.

## debianization

To create a Debian package is often called debianizing. And the part which implements the packaging
is therefore called *debianization*.

A detailed guide can be found here: [Guide for Debian Maintainers](https://www.debian.org/doc/manuals/debmake-doc/index.en.html)

## step-by-step

This is a step-by-step list of **one** way to create a debianization.

### 1. debmake

[What is debmake?](https://www.debian.org/doc/manuals/debmake-doc/ch04.en.html#what-debmake)

The *debmake* command is the helper script for the Debian packaging.

- It always sets most of the obvious option states and values to reasonable defaults.
- It generates the upstream tarball and its required symlink if they are missing.
- It doesn’t overwrite the existing configuration files in the `debian/` directory.
- It supports the multiarch package.
- It creates good template files such as the `debian/copyright` file compliant with *DEP-5*.

A simple `debmake` call will fail:

```text
$ debmake
I: set parameters
I: sanity check of parameters
E: invalid parent directory for setting package/version: debhello
E: rename parent directory to "package-version".
```

This happens as the (upstream)-*version* is unknown to `debmake`. We need to provide the *version*
to `debmake`. Either in the name of the directory name. In our case this would be something like
`debhello-1.0.0`. Or we can specify the version with the `--upstreamversion` (`-u`) option.

```text
$ debmake -u 1.0.0
I: set parameters
I: sanity check of parameters
W: -u "1.0.0" != auto set value ""
I: pkg="debhello", ver="1.0.0", rev="1"
I: *** start packaging in "debhello". ***
W: parent directory should be "debhello-1.0.0".  (If you use pbuilder, this may be OK.)
I: provide debhello_1.0.0.orig.tar.gz for non-native Debian package
I: pwd = "/tmp"
I: $ ln -sf debhello-1.0.0.tar.gz debhello_1.0.0.orig.tar.gz
I: pwd = "/tmp/debhello"
I: parse binary package settings:
I: binary package=debhello Type=bin / Arch=any M-A=foreign
I: analyze the source tree
I: build_type = Cmake
I: scan source for copyright+license text and file extensions
I:  40 %, ext = c
I:  20 %, ext = text
I:  20 %, ext = md
I:  20 %, ext = json
I: check_all_licenses
I: .....
I: check_all_licenses completed for 5 files.
I: bunch_all_licenses
I: format_all_licenses
I: make debian/* template files
I: single binary package
I: debmake -x "1" ...
I: creating => debian/control
I: creating => debian/copyright
I: substituting => /usr/share/debmake/extra0/changelog
I: creating => debian/changelog
I: substituting => /usr/share/debmake/extra0/rules
I: creating => debian/rules
I: substituting => /usr/share/debmake/extra1/README.Debian
I: creating => debian/README.Debian
I: substituting => /usr/share/debmake/extra1/watch
I: creating => debian/watch
I: substituting => /usr/share/debmake/extra1source/format
I: creating => debian/source/format
I: substituting => /usr/share/debmake/extra1source/local-options
I: creating => debian/source/local-options
I: substituting => /usr/share/debmake/extra1patches/series
I: creating => debian/patches/series
I: run "debmake -x2" to get more template files
I: $ wrap-and-sort
```

Now we have a template debianization to start with. The debianization is located in the `debian/`
directory.

```text
debian/
├── changelog
├── control
├── copyright
├── patches
│   └── series
├── README.Debian
├── rules
├── source
│   ├── format
│   └── local-options
└── watch
```

### 2. debian/rules

The [debian/rules](https://www.debian.org/doc/manuals/debmake-doc/ch05.en.html#rules) script is the executable script to build the Debian package. The content of the
template `debian/rules` file should look something:

```make
#!/usr/bin/make -f
# You must remove unused comment lines for the released package.
#export DH_VERBOSE = 1
#export DEB_BUILD_MAINT_OPTIONS = hardening=+all
#export DEB_CFLAGS_MAINT_APPEND  = -Wall -pedantic
#export DEB_LDFLAGS_MAINT_APPEND = -Wl,--as-needed

%:
	dh $@  

#override_dh_auto_configure:
#	dh_auto_configure -- \
#	      -DCMAKE_LIBRARY_ARCHITECTURE="$(DEB_TARGET_MULTIARCH)"
```

For our simple debhello program we only a few lines of the template. All unused lines need to be
removed. Our `debian/rules` file might look something like this:

```make
#!/usr/bin/make -f
export DEB_BUILD_MAINT_OPTIONS = hardening=+all
export DEB_CFLAGS_MAINT_APPEND  = -Wall -pedantic
export DEB_LDFLAGS_MAINT_APPEND = -Wl,--as-needed

%:
	dh $@  
```

With build tools like CMake or autotools the rules file is most likely pretty simple as most of the
software specific parts are already done there. But it is possible to model even rather complex build scenarios within the `debian/rules` file.

### 3. debian/control

The [debian/control](https://www.debian.org/doc/manuals/debmake-doc/ch05.en.html#control) file consists of blocks of meta data separated by a blank line. Each block of
meta data defines the following in this order:

- meta data for the Debian source package
- meta data for the Debian binary packages

See [Chapter 5 - Control files and their fields](https://www.debian.org/doc/debian-policy/ch-controlfields.html) of the “Debian Policy Manual” for the definition
of each meta data. The template `debian/control` file should look something like this:

```text
Source: debhello
Section: unknown
Priority: optional
Maintainer: Philipp Rosenberger <p.rosenberger@kunbus.com>
Build-Depends: cmake, debhelper-compat (= 12)
Standards-Version: 4.5.0
Homepage: <insert the upstream URL, if relevant>

Package: debhello
Architecture: any
Multi-Arch: foreign
Depends: ${misc:Depends}, ${shlibs:Depends}
Description: auto-generated package by debmake
 This Debian binary package was auto-generated by the
 debmake(1) command provided by the debmake package.
```

Many of the entries in the template `debian/control` file are ok as they are. Some need to be
changed. You might also need to add some extra properties for your package. For our *debhello*
package the following `debian/control` file might be enough:

```text
Source: debhello
Section: utils
Priority: optional
Maintainer: Philipp Rosenberger <p.rosenberger@kunbus.com>
Build-Depends: cmake, debhelper-compat (= 12)
Standards-Version: 4.5.0
Homepage: https://github.com/iluminat23/debhello

Package: debhello
Architecture: any
Multi-Arch: foreign
Depends: ${misc:Depends}, ${shlibs:Depends}
Description: example debian package
 This Debian package is used as example to show how the packaging process
 of Debian works
```

### 4. debian/changelog

The [debian/changelog](https://www.debian.org/doc/manuals/debmake-doc/ch05.en.html#changelog) file records the Debian package history and defines the upstream package version and the Debian revision in its
first line. The changes need to be documented in the specific, formal, and concise style. The
*debmake* command creates the initial template file with the upstream package version and the Debian
revision. The distribution is set to UNRELEASED to prevent accidental upload to the Debian archive.

```text
debhello (1.0.0-1) UNRELEASED; urgency=low

  * Initial release. Closes: #nnnn
    <nnnn is the bug number of your ITP>

 -- Philipp Rosenberger <p.rosenberger@kunbus.com>  Tue, 29 Nov 2022 11:15:51 +0100
```

The version is already correct. als already described, we need to replace the `UNRELEASED` with
something correct. As packages get into the Debian distribution through *unstable*. This is used in
most of the Debian packages. But it depends on the target of the package what should be put in here.

```text
debhello (1.0.0-1) unstable; urgency=low

  * Initial release.

 -- Philipp Rosenberger <p.rosenberger@kunbus.com>  Mon, 11 Jul 2022 14:06:22 +0200
```

### 5. debian/copyright

Debian takes the copyright and license matters very seriously. The “Debian Policy Manual” enforces
having a summary of them in the [debian/copyright](https://www.debian.org/doc/manuals/debmake-doc/ch05.en.html#copyright) file in the package.

You should format it as a [machine-readable debian/copyright file](https://www.debian.org/doc/packaging-manuals/copyright-format/1.0/) (DEP-5).

The debmake command creates the initial DEP-5 compatible template file by scanning the entire source tree. It uses an internal license checker to classify each license text.

```text
Format: https://www.debian.org/doc/packaging-manuals/copyright-format/1.0/
Upstream-Name: debhello
Upstream-Contact: <preferred name and address to reach the upstream project>
Source: <url://example.com>
#
# Please double check copyright with the licensecheck(1) command.

Files:     CMakeLists.txt
           README.md
           debhello_config.h.in
           main.c
Copyright: __NO_COPYRIGHT_NOR_LICENSE__
License:   __NO_COPYRIGHT_NOR_LICENSE__

#----------------------------------------------------------------------------
# Files marked as NO_LICENSE_TEXT_FOUND may be covered by the following
# license/copyright files.
```

You need to provide detailed copyright information on **all** files. In this case it is rather easy.
All files are licensed under the same license. You could also use the `*` to match all files. But
keeping at least the upstream source files and the debianization files separate is a good idea.

```text
Format: https://www.debian.org/doc/packaging-manuals/copyright-format/1.0/
Upstream-Name: debhello
Upstream-Contact: KUNBUS GmbH <support@kunbus.com>
Source: https://github.com/iluminat23/debhello

Files:     CMakeLists.txt
           README.md
           debhello_config.h.in
           main.c
Copyright: KUNBUS GmbH 2022
License:   GPL-2+

Files:     debian/*
Copyright: KUNBUS GmbH 2022
License:   GPL-2+

License: GPL-2+
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; version 2 dated June, 1991, or (at
 your option) any later version.
 .
 On Debian systems, the complete text of version 2 of the GNU General
 Public License can be found in '/usr/share/common-licenses/GPL-2'.
```

### 6. 3.0 source format

There are different source formats. Inmost cases the `3.0 (quilt)` format is the correct/best
choice. The `3.0 (native)` format is only intended for packages which are directly related to
Debian and the distribution. And even then it is recommended to use `3.0 (quilt)`.

The source format is defined in the `debian/source/format` file.

### 7. debian/watch

The file named `watch` in the `debian` directory is used to check for newer versions of upstream software is available and to download it if necessary.
<https://wiki.debian.org/debian/watch>

| :warning: As of 2022-10-02, the watch file here is not working for Releases page due to ?GitHub changing the representation of the page. The links are dynamically loaded in JavaScript instead of static text. See thread for the discussions: <https://lists.debian.org/debian-devel/2022/09/msg00224.html> |
|--------------------------------------------------------------------------------------------------|

```text
version=4
opts=filenamemangle=s/.+\/v?(\d\S+)\.tar\.gz/<project>-$1\.tar\.gz/ \
  https://github.com/iluminat23/debhello/tags .*/v?(\d\S+)\.tar\.gz
```
