#!/usr/bin/env bash

set -euo pipefail

git remote add upstream https://github.com/sunqm/libcint || true
git fetch upstream
git merge upstream/master --strategy ours
