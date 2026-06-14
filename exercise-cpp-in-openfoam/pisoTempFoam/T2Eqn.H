// Solve passive temperature transport
fvScalarMatrix T2Eqn
(
    fvm::ddt(T2)
  + fvm::div(phi, T2)
  - fvm::laplacian(DT2, T2)
);
T2Eqn.solve();
