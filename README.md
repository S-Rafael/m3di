# About `m3di` v1.0

`m3di` is a simple command line program for numerically evaluating 
a mathematical quantity known as the *meromorphic 3D-index*, a topological 
invariant of 3-dimensional manifolds with torus boundary. The mathematical 
definition of the meromorphic 3D-index as a "state integral" associated to
3-dimensional triangulations with angles appears in
[this paper](https://link.springer.com/article/10.1007/s40687-018-0166-9) 
by Garoufalidis-Kashaev. 
Subsequently, `m3di` was used to produce the numerical results of
[this paper](http://arxiv.org/).

## Software and hardware requirements

Generally `m3di` is meant to run on compute servers running Linux or similar
UNIX-based systems. Smaller computations can be performed on ordinary PCs.

In order to compile and install `m3di` you will need:

* The GNU toolchain, including GNU make and the GNU C++ compiler (`g++`) 
  version 6.1 or later;
* The [jsoncpp](https://github.com/open-source-parsers/jsoncpp) library
  with header files visible to the compiler;
* If the system you intend to run `m3di` on doesn't have `jsoncpp`, you may have
  to link it statically.

Currently, the project relies on a simple Makefile which may require certain 
adjustments in case it doesn't work on your system. 

## Building and installing `m3di`

Building `m3di` should be straightforward on most Linux distributions:

1. Navigate to the source directory `src`:

   `$ cd /path/to/the/directory/src`
2. Build the project:

   `$ make`
3. If `make` finished without errors, install `m3di` as root:

   `# make install`

   On systems with `sudo` available, you may instead use
   
   `$ sudo make install`
4. If you don't have superuser rights and wish to perform a local 
   install, you can replace step 3 with simply copying the binary
   `m3di` to any local directory in your `$PATH`.

## Using `m3di`

To compute a state integral with `m3di`, you need two pieces of data:

* A [JSON data](https://www.json.org/) file describing the triangulation. Example files are shipped
  in the `census` subdirectory.

* Command line parameters controlling the parameter `"hbar"` and the number of
  sample points in each coordinate direction of the integration domain.

Given a valid JSON data file `'example.json'`, a command of the form

`m3di integrate example.json <Re_hbar> <Im_hbar> <samples>`

will perform numerical integration and print its result as JSON data to stdout.
Note that the real part `Re_hbar` of the complex parameter "hbar" must be negative.
For example, for hbar = -0.1+0i and 10000 samples per dimension, the command will be

`m3di integrate example.json -0.1 0 10000`

Use the stream redirection operator (`>`) if you wish to save the output to a JSON file.
If a single dash (`-`) is used instead of the input file name, then `m3di` reads 
JSON data from the standard input instead.

### Write mode

`m3di` has an alternate mode called _write mode_. In this mode, the program
does not compute the state integral, but rather prints the values of the
integrand at the sample points. This is useful if you want to plot the integrand.
For example,

`m3di write example.json -0.1 0 10000 > data.json`

will store the data needed to plot the integrand as `data.json`.

## Format of the JSON data files

This section describes the content of the input and output [JSON data](https://www.json.org/) files.
You may also look at example files shipped with the program (in the `'census'` subdirectory).

### Input files

The input files describe the combinatorics of an ideal triangulation
of a 3-manifold with torus boundary.

The required data fields at top level are:

| Key | Value type | Meaning |
| --- | ---------- | ------- |
| `"N"` | Number | The number of tetrahedra in the triangulation; must be ≥2. |
| `"a"` | Array  | the "initial" strict angle structure. Must be an array of length 3N whose entries are of type `Number` and represent angles in units of π on the normal quadrilaterals of the triangulation. Hence, for example, an angle of π/3 could be encoded as '0.333333333'. The program does not verify whether the given vector satisfies the angle structure equations. |
| `"L"` | Array | The matrix of leading-trailing deformations corresponding to the edges and a pair of peripheral curves on the triangulation. |

The matrix `"L"` is encoded as an `Array` of matrix rows. 
Each matrix row is itself an `Array` with entries of type `Number`; these entries should be integers.
The entries in each row correspond to normal quadrilaterals which should 
be ordered in the same way as in the array `"a"`.

The initial N rows of `"L"` contain coefficients of leading-trailing 
deformations along the edges of the triangulation.
Then there are two additional rows corresponding to a pair of 
peripheripheral curves spanning the first homology
group of the boundary torus.
In other words, the length of the array `"L"` should be N+2, and each of
its elements should be an `Array` of length 3N and elements of type `Number`.

### Output format in _integrate mode_

When run in the _integrate mode_, `m3di` prints to standard output
a valid JSON string containing three top-level objects: `input`, `output` and `statistics`.

#### The `input` object

The top-level object `input` essentially mirrors the user-provided input
for bookkeeping purposes. It contains the following data:

| Key | Value type | Meaning |
| --- | ---------- | ------- |
| `"hbar"` | String | Textual representation of the complex number that `m3di` understood to be the value of the parameter `'hbar'` passed to it on the command line. |
| `"hbar_real"` | Number | The real part of `'hbar'` as parsed by `m3di`. |
| `"hbar_imag"` | Number | The imaginary part of `'hbar'` as parsed by `m3di`. |
| `"samples"`        | Number | The number of samples per dimension as set on the command line. |
| `"triangulation JSON"` | String | The path to the JSON input file as passed on the command line. |

#### The `output` object

The object `output` contains the result of the integration, i.e., the state integral:

| Key | Value type | Meaning |
| --- | ---------- | ------- |
| `"real"`  | Number | The real part of the state integral. |
| `"imag"`  | Number | The imaginary part of the state integral. |

Additionally, the `statistics` object contains some information about the computation process.
It is provided AS-IS and no guarantees about its contents are made in this version of `m3di`.

### Output format in _write mode_

In _write mode_, the output JSON still contains the `input` object
whose values have the same type and meaning as in _integrate mode_.
Unlike in _integrate mode_, the top-level object named `"output"`
contains only a single key `"points"` of type `Array` with length equal to
the value of `input.samples` raised to the power N-1, where N comes from the triangulation JSON file.
Each entry of the array `"points"` is an `Object` containing the following data:

| Key | Value type | Meaning |
| --- | ---------- | ------- |
| `"t"` | Array    | An array with N-1 entries of type `Number`. These entries are coordinates of a point in the circle-valued angle space with respect to the basepoint given by `"a"` and the leading-trailing coordinates derived from the first N-1 rows of the matrix `"L"`. Each coordinate varies between 0 and 2π. |
| `"real"` | Number | The approximate real part of the value of the integrand at the sample point specified by `"t"`. |
| `"imag"` | Number | The approximate imaginary part of the value of the integrand at the sample point specified by `"t"`. |

## Authorship and license information

The program `m3di` was developed by [Rafael M. Siejakowski](https://rs-math.net) 
and then released under the GNU GPL v2 license.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License 
version 2 as published by the Free Software Foundation; 
later versions of the GNU General Public Licence do NOT apply.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 
02110-1301, USA.


