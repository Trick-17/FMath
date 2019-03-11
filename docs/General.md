General Concepts
=======================================================

The library revolves around the `Field` template class, which is a convenience
wrapper around `std::vector` with added operators and functions. The Eigen library
is used for vector operations.

Expression templates ensure that mathematical operations are used efficiently,
avoiding temporaries.

The library provides convenience `typedef`s:

- `FMath::scalar`      = default is `double`, can be defined differently by `FMATH_SCALAR_TYPE`
- `FMath::ScalarField` = `FMath::Field<FMath::scalar>`
- `FMath::Vector3`     = `Eigen::Vector3<FMath::scalar>`
- `FMath::VectorX`     = `Eigen::VectorX<FMath::scalar>`
- `FMath::VectorField` = `FMath::Field<FMath::Vector3>`