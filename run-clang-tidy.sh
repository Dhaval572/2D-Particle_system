#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR="build"
SRC_DIR="src"
FIX_FLAG=""

# Parse --fix flag
for arg in "$@"; do
  if [ "$arg" = "--fix" ]; then
    FIX_FLAG="--fix"
  fi
done

# First positional arg selects the category
CATEGORY="${1:-all}"

case "$CATEGORY" in
  modernize)    CHECKS="modernize-*" ;;
  performance)  CHECKS="performance-*" ;;
  bugprone)     CHECKS="bugprone-*" ;;
  all)          CHECKS="" ;;
  --fix)        CHECKS="" ;;  # bare --fix means all checks with fix
  *)
    echo "Usage: $0 [modernize|performance|bugprone|all] [--fix]"
    exit 1
    ;;
esac

if [ -z "$CHECKS" ]; then
  CHECKS_FLAG=""
else
  CHECKS_FLAG="--checks=$CHECKS"
fi

echo "Running clang-tidy with checks: ${CHECKS:-all from .clang-tidy}"
[ -n "$FIX_FLAG" ] && echo "Auto-fix is ENABLED"
echo ""

for f in "$SRC_DIR"/*.cpp; do
  echo "=== Analyzing $f ==="
  clang-tidy -p "$BUILD_DIR" $CHECKS_FLAG $FIX_FLAG "$f"
done
