# Vendored Wayland — provenance and the Cact additions

This repository is the CactOS port of the Wayland libraries: the upstream tree is
carried in place, the build system is CactOS's own (upstream's is removed), and
only the four libraries are built — `libwayland-client`, `-server`, `-egl`,
`-cursor` — into the loader's fixed bases `0x24000000`, `0x25000000`,
`0x26000000`, `0x27000000`.

| | |
|---|---|
| Upstream | <https://gitlab.freedesktop.org/wayland/wayland> |
| Ref | tag `1.26.0` — commit `87cc8a8` |
| Branch | `cact` (upstream tag + the Cact commits) |
| Sync tool | `./sync.sh` (`.cactsync` rebase mode, see `sync.conf`) |

**Removed from upstream:** `meson.build` and `meson_options.txt`.  Upstream's
build probes the host through the cross compiler and hard-codes a versioned
soname, so this port owns its build, as the other ports do.  A sync onto a newer
tag conflicts on those two paths — keep CactOS's files.

## Files CactOS adds

| Path | Role |
|---|---|
| `meson.build` | Builds the four libraries; generates the protocol + version headers |
| `meson_options.txt` | `cactlib`, `ffiinc`, `lr_lib`, `cpkg_py` paths |
| `config.h` | The probe answers, pinned instead of probed |
| `wayland.ld.in` | Fixed-base linker script template (meson substitutes `@base@`) |
| `cross/x86_64-cact-clang.ini` | The workspace's standard CactOS cross file |
| `pkg/wayland/manifest` | `.cpkg` directory-package manifest |
| `sync.conf`, `sync.sh` | cactsync manifest and entry point |
| `PORTING-x86_64.md` | What "building for CactOS" means here |

## Deliberate deviations from upstream

1. **The protocol code comes from the host's `wayland-scanner`.**  Upstream
   builds a scanner and (when cross-compiling) requires a native one; the host
   here carries exactly the pinned version, 1.26.0, so the port runs it directly.
   Five files come out of `protocol/wayland.xml`: the installed
   `wayland-client-protocol.h` and `wayland-server-protocol.h`, the core-only
   `-core.h` variants the libraries themselves include, and the shared
   `wayland-protocol.c` (`public-code`).
2. **`-nostdlibinc`** (the host libc's headers are not searched; clang's builtin
   `stdarg.h` and friends still are), so a header CactLibc lacks is a hard error
   rather than a silent grab of the host's glibc copy.
3. **`HAVE_MEMFD_CREATE` is off** (see `config.h`): CactOS has the memfd *device*
   but CactLibc has no `memfd_create()` wrapper and the kernel has no sealing
   (`MFD_ALLOW_SEALING` / `F_ADD_SEALS` / `F_SEAL_*`), which is what Wayland's shm
   pools want it for.  With it off Wayland takes its anonymous-file fallback over
   `/dev/shm`.  Adding memfd + seals to CactOS is the better fix, and this define
   should then be turned back on.
4. **Sonames are unversioned** (`libwayland-client.so`, not `.so.0`): ld.so and
   `dlopen()` resolve a `DT_NEEDED` name as a file under `/usr/lib`, and the
   cctkfs image carries no symlinks.
5. **`-fvisibility=hidden`** plus the headers' `WL_EXPORT`, as upstream does — it
   is also what keeps the duplicated `wayland-util`/`connection` code (compiled
   into both the client and the server library, as upstream links them) out of
   the exported symbol table.
6. **`-w`**: Wayland is vendored third-party code.

No upstream source file is edited.
