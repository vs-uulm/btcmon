# btcmon

Sourcecode of the unobtrusive bitcoin monitoring software.

## Functionality:

The software consumes transaction info and produces a fit for a lognormal distribution modelling the dissemination latency of transactions in the network. See also https://github.com/vs-uulm/CoinView for modelling and evaluation.

Consumes input of the form:

<timestamp in ms>,<originator id as sha-256 hash in hexadecimal string representation>, <hash of BTC Transaction in hexadecimal string representation>

e.g.:

1547779473468,6cf1100aaccec75da23995512fc7c7a5b6e25224f5903af011e78691c03d0455,a73578820a41aa6180621bcd90af1997c88794b33d8db2f004ee37c3e09b10ec

Data used to validate this: https://zenodo.org/record/2547396

## Installation

Checkout the repository, with all dependencies installed, call:

```
conan install . --build missing
cmake .
make
```

Alternatively use docker to build, and execute the previous commands after starting the container:

```
# linux bash
docker run -it -v $(PWD):/home/conan /bin/bash

# windows powershell
docker run -it -v ${PWD}:/home/conan /bin/bash
```

(Only tested to build for linux)

## Options

BTCMon takes various options:

  --help                produce help message
  --m arg (=4.5)        initial µ of lognormal distribution
  --s arg (=1)          initial sigma of lognormal distribution
  --measures arg (=8)   amount of required measurements

## Dependencies:

 * Conan (conan.io) for library management
   * Used to install boost1.69:program option
 * CMake
 
