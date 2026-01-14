{
  description = "Duplo - duplicate code block finder";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            # Build tools
            cmake
            gnumake
            gcc

            # Testing
            bats

            # Optional: file watcher for watch.sh
            fswatch

            # Optional: for sanitizers and profiling
            gperftools
          ];

          shellHook = ''
            echo "Duplo development environment"
            echo "Build: mkdir -p build && cd build && cmake .. && make"
            echo "Test:  bats --recursive tests"
          '';
        };
      });
}
