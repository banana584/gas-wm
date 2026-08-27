{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

  outputs = { nixpkgs, ... }: {
    devShells.x86_64-linux.default =
      nixpkgs.legacyPackages.x86_64-linux.mkShell {
        packages = [
          nixpkgs.legacyPackages.x86_64-linux.gcc
          nixpkgs.legacyPackages.x86_64-linux.clang
          nixpkgs.legacyPackages.x86_64-linux.cmake
          nixpkgs.legacyPackages.x86_64-linux.pkg-config
          nixpkgs.legacyPackages.x86_64-linux.lua5_4
        ];
      };
  };
}
