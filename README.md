# stackuse-vp

Fork of [riscv-vp][riscv-vp github] for the [FDL21][fdl conf] paper: *In-Vivo Stack Overflow Detection and Stack Size Estimation for Low-End Multithreaded Operating Systems using Virtual Prototypes*.

## Installation

For instructions on building/installing this software. Refer to the
original [riscv-vp installation instructions][riscv-vp build].

## Additional Resources

* The *In-Vivo Stack Overflow Detection and Stack Size Estimation for
  Low-End Multithreaded Operating Systems using Virtual Prototypes*
  paper about to be published as part of FDL 2021.
* The [stack-usage-db tool][stack-usage-db github] which can be used to
  generate stack usage databases, passed to the VP, using the
  `--stack-usage` command-line flag.
* The [artifacts][zenodo artifacts] used for evaluating this
  implementation in the aforementioned paper using the [RIOT][riot web]
  operating system.

## Acknowledgements

This work was supported in part by the German Federal Ministry of
Education and Research (BMBF) within the project Scale4Edge under
contract no. 16ME0127 and within the project VerSys under contract
no. 01IW19001.

[riscv-vp github]: https://github.com/agra-uni-bremen/riscv-vp
[fdl conf]: http://www.fdl-conference.org/
[riscv-vp build]: https://github.com/agra-uni-bremen/riscv-vp/blob/3ed392b508dc962a717170f45e1ecf8e2434e750/README.md#1-build-the-risc-v-gnu-toolchain
[stack-usage-db github]: https://github.com/agra-uni-bremen/stack-usage-db
[zenodo artifacts]: https://doi.org/10.5281/zenodo.5091709
[riot web]: https://riot-os.org
