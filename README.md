# Linear Algebra Engine
A C++ engine for linear algebra calculations, built from scratch.

## Motivation
I'm building an AI engine in C++ and this is its linear algebra foundation.
I'm also using it to practice implementing what I learn from Strang's
Linear Algebra — coding the math as I go.

## Features

### Vec
- Initializer list syntax: `Vec v = {1.0f, 2.0f, 3.0f}`
- Dot product: `v * w`
- Scalar multiplication: `v * s`
- Vector addition and subtraction (supports mixed dimensions)
- Unary minus: `-v`
- L2 norm: `v.length()`
- Cosine of the angle between two vectors: `v.cos(w)`

### Matrix
- Column-major storage (matrix = array of column vectors)
- Initializer list syntax: `Matrix m = {v1, v2, v3}`
- Construct from a Vec array (validates matching dimensions)
- Forced construction: pads shorter vectors with zeros to match the largest dimension
- Matrix-vector multiplication: `m * v`
- Matrix addition: `A + B`
- Scalar multiplication: `m * s`

## Usage
Run `/Release/LinearAlgebra.exe`

To calculate with different vectors or matrices, modify the values in `main()`.
Input features are not yet implemented.
