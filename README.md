# Linear Algebra Engine

A C++ engine for linear algebra calculations, built from scratch.

## Motivation
I'm building an AI engine in C++ and this is its linear algebra foundation.
I'm also using it to practice implementing what I learn from Strang's 
Linear Algebra — coding the math as I go.

## Features
Currently supports:
- Dot product between two vectors
- Vector addition (supports different dimensions)
- Vector length (L2 norm)
- Cosine of the angle between two vectors

More features will be added continuously.

## Usage
Run `/Release/LinearAlgebra.exe`

To calculate with different vectors, modify the values in `main()`.
Input features are not yet implemented.

## Roadmap
- Initializer list support: `Vec x = {1, 2, 3};`
- `std::cout << v` output overloading
- `arccos()` for finding the angle in degrees
- Matrix struct
- More vector operations
