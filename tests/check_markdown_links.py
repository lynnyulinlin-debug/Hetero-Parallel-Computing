#!/usr/bin/env python3
"""Check repository-local Markdown file and heading links without third-party tools."""

from __future__ import annotations

import re
import sys
import unicodedata
from pathlib import Path
from urllib.parse import unquote


ROOT = Path(__file__).resolve().parents[1]
MARKDOWN_ROOTS = (ROOT / "README.md", ROOT / "docs", ROOT / "examples")
SKIPPED_FILES = {ROOT / "docs" / "HANDOVER.md"}
LINK_PATTERN = re.compile(r"(?<!\!)\[[^\]]*\]\(([^)\s]+)(?:\s+['\"][^)]*['\"])?\)")
HEADING_PATTERN = re.compile(r"^#{1,6}\s+(.+?)\s*#*\s*$")


def markdown_files() -> list[Path]:
    files: list[Path] = []
    for root in MARKDOWN_ROOTS:
        if root.is_file():
            files.append(root)
        elif root.is_dir():
            files.extend(root.rglob("*.md"))
    return sorted(path for path in files if path not in SKIPPED_FILES)


def github_slug(heading: str) -> str:
    """Produce the subset of GitHub heading slugs used by this repository."""
    normalized = unicodedata.normalize("NFKD", heading).strip().lower()
    kept: list[str] = []
    for char in normalized:
        category = unicodedata.category(char)
        if char == "-" or not (category.startswith("P") or category.startswith("S")):
            kept.append(char)
    return re.sub(r"\s+", "-", "".join(kept))


def headings(path: Path) -> set[str]:
    slugs: set[str] = set()
    duplicates: dict[str, int] = {}
    for line in path.read_text(encoding="utf-8").splitlines():
        match = HEADING_PATTERN.match(line)
        if not match:
            continue
        base = github_slug(match.group(1))
        count = duplicates.get(base, 0)
        slug = base if count == 0 else f"{base}-{count}"
        duplicates[base] = count + 1
        slugs.add(slug)
    return slugs


def is_external(target: str) -> bool:
    return target.startswith(("http://", "https://", "mailto:", "tel:"))


def check_link(source: Path, raw_target: str, line_number: int, cache: dict[Path, set[str]]) -> str | None:
    target = unquote(raw_target)
    if is_external(target):
        return None

    path_text, separator, anchor = target.partition("#")
    if not path_text:
        destination = source
    elif path_text.startswith("/"):
        # Absolute filesystem paths are intentionally not portable documentation links.
        return f"{source.relative_to(ROOT)}:{line_number}: absolute local link is not allowed: {raw_target}"
    else:
        destination = (source.parent / path_text).resolve()

    if not destination.exists():
        return f"{source.relative_to(ROOT)}:{line_number}: missing target: {raw_target}"
    if separator and anchor:
        if destination.is_dir():
            destination = destination / "README.md"
        if destination.suffix.lower() != ".md":
            return f"{source.relative_to(ROOT)}:{line_number}: anchor target is not Markdown: {raw_target}"
        anchors = cache.setdefault(destination, headings(destination))
        if anchor not in anchors:
            return f"{source.relative_to(ROOT)}:{line_number}: missing heading #{anchor}: {raw_target}"
    return None


def main() -> int:
    errors: list[str] = []
    cache: dict[Path, set[str]] = {}
    for source in markdown_files():
        for line_number, line in enumerate(source.read_text(encoding="utf-8").splitlines(), start=1):
            for match in LINK_PATTERN.finditer(line):
                error = check_link(source, match.group(1), line_number, cache)
                if error:
                    errors.append(error)

    if errors:
        print("Markdown link check failed:", file=sys.stderr)
        print("\n".join(errors), file=sys.stderr)
        return 1
    print(f"Markdown link check passed for {len(markdown_files())} files.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
