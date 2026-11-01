#!/usr/bin/env python3
"""Render text as big stroke-based ASCII art.

Usage:
    python bigtext.py hello            # 6 rows tall (default)
    python bigtext.py -r 10 "hi 42"    # 10 rows tall, width derived
    python bigtext.py -r 8 -c 6 abc    # fixed width of 6 cols per glyph
"""
import argparse
import math

# Glyphs: lists of polylines in unit coords. x: 0..1 left->right, y: 0..1 top->bottom.
O = [(0.25, 0), (0.75, 0), (1, 0.2), (1, 0.8), (0.75, 1), (0.25, 1), (0, 0.8), (0, 0.2), (0.25, 0)]
P = [(0, 1), (0, 0), (0.75, 0), (1, 0.2), (1, 0.35), (0.75, 0.55), (0, 0.55)]
S = [(1, 0.15), (0.75, 0), (0.25, 0), (0, 0.15), (0, 0.35), (0.25, 0.5), (0.75, 0.5),
     (1, 0.65), (1, 0.85), (0.75, 1), (0.25, 1), (0, 0.85)]

GLYPHS = {
    "A": [[(0, 1), (0.49, 0)], [(0.51, 0), (1, 1)], [(0.22, 0.6), (0.78, 0.6)]],
    "B": [[(0, 0), (0, 1)],
          [(0, 0), (0.7, 0), (1, 0.15), (1, 0.35), (0.7, 0.5), (0, 0.5)],
          [(0.7, 0.5), (1, 0.65), (1, 0.85), (0.7, 1), (0, 1)]],
    "C": [[(1, 0.15), (0.75, 0), (0.25, 0), (0, 0.2), (0, 0.8), (0.25, 1), (0.75, 1), (1, 0.85)]],
    "D": [[(0, 0), (0, 1)], [(0, 0), (0.6, 0), (1, 0.3), (1, 0.7), (0.6, 1), (0, 1)]],
    "E": [[(1, 0), (0, 0), (0, 1), (1, 1)], [(0, 0.5), (0.7, 0.5)]],
    "F": [[(1, 0), (0, 0), (0, 1)], [(0, 0.5), (0.7, 0.5)]],
    "G": [[(1, 0.15), (0.75, 0), (0.25, 0), (0, 0.2), (0, 0.8), (0.25, 1), (0.75, 1),
           (1, 0.8), (1, 0.55), (0.55, 0.55)]],
    "H": [[(0, 0), (0, 1)], [(1, 0), (1, 1)], [(0, 0.5), (1, 0.5)]],
    "I": [[(0.5, 0), (0.5, 1)], [(0.1, 0), (0.9, 0)], [(0.1, 1), (0.9, 1)]],
    "J": [[(1, 0), (1, 0.8), (0.7, 1), (0.3, 1), (0, 0.8)]],
    "K": [[(0, 0), (0, 1)], [(1, 0), (0, 0.55)], [(0.25, 0.4), (1, 1)]],
    "L": [[(0, 0), (0, 1), (1, 1)]],
    "M": [[(0, 1), (0, 0), (0.5, 0.6), (1, 0), (1, 1)]],
    "N": [[(0, 1), (0, 0), (1, 1), (1, 0)]],
    "O": [O],
    "P": [P],
    "Q": [O, [(0.6, 0.7), (1, 1)]],
    "R": [P, [(0.5, 0.55), (1, 1)]],
    "S": [S],
    "T": [[(0, 0), (1, 0)], [(0.5, 0), (0.5, 1)]],
    "U": [[(0, 0), (0, 0.8), (0.25, 1), (0.75, 1), (1, 0.8), (1, 0)]],
    "V": [[(0, 0), (0.49, 1)], [(0.51, 1), (1, 0)]],
    "W": [[(0, 0), (0.25, 1), (0.5, 0.4), (0.75, 1), (1, 0)]],
    "X": [[(0, 0), (1, 1)], [(1, 0), (0, 1)]],
    "Y": [[(0, 0), (0.5, 0.5)], [(1, 0), (0.5, 0.5)], [(0.5, 0.5), (0.5, 1)]],
    "Z": [[(0, 0), (1, 0), (0, 1), (1, 1)]],
    "0": [O, [(0.8, 0.2), (0.2, 0.8)]],
    "1": [[(0.2, 0.2), (0.6, 0), (0.6, 1)], [(0.2, 1), (1, 1)]],
    "2": [[(0, 0.2), (0.25, 0), (0.75, 0), (1, 0.2), (1, 0.4), (0, 1), (1, 1)]],
    "3": [[(0, 0.1), (0.25, 0), (0.75, 0), (1, 0.15), (1, 0.35), (0.75, 0.5), (0.3, 0.5)],
          [(0.75, 0.5), (1, 0.65), (1, 0.85), (0.75, 1), (0.25, 1), (0, 0.9)]],
    "4": [[(0.75, 1), (0.75, 0), (0, 0.7), (1, 0.7)]],
    "5": [[(1, 0), (0, 0), (0, 0.45), (0.75, 0.4), (1, 0.6), (1, 0.85), (0.75, 1), (0.25, 1), (0, 0.85)]],
    "6": [[(0.9, 0.1), (0.6, 0), (0.25, 0), (0, 0.25), (0, 0.8), (0.25, 1), (0.75, 1),
           (1, 0.8), (1, 0.6), (0.75, 0.45), (0.25, 0.45), (0, 0.6)]],
    "7": [[(0, 0), (1, 0), (0.35, 1)]],
    "8": [[(0.25, 0), (0.75, 0), (1, 0.12), (1, 0.38), (0.75, 0.5), (0.25, 0.5), (0, 0.38), (0, 0.12), (0.25, 0)],
          [(0.25, 0.5), (0, 0.62), (0, 0.88), (0.25, 1), (0.75, 1), (1, 0.88), (1, 0.62), (0.75, 0.5)]],
    "9": [[(1, 0.4), (0.75, 0.55), (0.25, 0.55), (0, 0.4), (0, 0.2), (0.25, 0), (0.75, 0),
           (1, 0.2), (1, 0.75), (0.75, 1), (0.4, 1), (0.1, 0.9)]],
    "-": [[(0.1, 0.5), (0.9, 0.5)]],
    "_": [[(0, 1), (1, 1)]],
    ".": [[(0.5, 1)]],
    "!": [[(0.5, 0), (0.5, 0.7)], [(0.5, 1)]],
    ":": [[(0.5, 0.25)], [(0.5, 0.75)]],
    "?": [[(0, 0.2), (0.25, 0), (0.75, 0), (1, 0.2), (1, 0.35), (0.5, 0.6), (0.5, 0.72)], [(0.5, 1)]],
    "/": [[(1, 0), (0, 1)]],
    "\\": [[(0, 0), (1, 1)]],
}

# Width relative to the default (0.55 of 2*rows). Anything not listed uses the default.
WIDTH = {"I": 0.35, "M": 0.8, "W": 0.8, "-": 0.4, ".": 0.2, "!": 0.2, ":": 0.2, "/": 0.4, "\\": 0.4, "1": 0.45}
DEFAULT_W = 0.55


def classify(dx, dy):
    """Pick a stroke char from the segment's visual slope (cells are ~2x taller than wide)."""
    if dx == 0 and dy == 0:
        return "."
    ang = math.degrees(math.atan2(abs(dy) * 2, abs(dx)))
    if ang > 80:
        return "|"
    if ang < 20:
        return "-"
    return "\\" if dx * dy > 0 else "/"


def bresenham(x0, y0, x1, y1):
    dx, dy = abs(x1 - x0), -abs(y1 - y0)
    sx = 1 if x0 < x1 else -1
    sy = 1 if y0 < y1 else -1
    err = dx + dy
    while True:
        yield x0, y0
        if x0 == x1 and y0 == y1:
            break
        e2 = 2 * err
        if e2 >= dy:
            err += dy
            x0 += sx
        if e2 <= dx:
            err += dx
            y0 += sy


def stroke_glyph(ch, rows, cols):
    grid = [[" "] * cols for _ in range(rows)]
    to_cell = lambda p: (round(p[0] * (cols - 1)), round(p[1] * (rows - 1)))
    for stroke in GLYPHS[ch]:
        if len(stroke) == 1:
            x, y = to_cell(stroke[0])
            grid[y][x] = "."
            continue
        for a, b in zip(stroke, stroke[1:]):
            (x0, y0), (x1, y1) = to_cell(a), to_cell(b)
            c = classify(x1 - x0, y1 - y0)
            for x, y in bresenham(x0, y0, x1, y1):
                grid[y][x] = c
    return grid


def pillow_glyph(ch, rows, cols):
    """Fallback for chars without stroke data: rasterize a real font and threshold it."""
    try:
        from PIL import Image, ImageDraw, ImageFont
    except ImportError:
        return None
    size = max(rows * 4, 16)
    try:
        font = ImageFont.truetype("DejaVuSansMono.ttf", size)
    except OSError:
        font = ImageFont.load_default(size)
    img = Image.new("L", (size * 2, size * 2), 0)
    ImageDraw.Draw(img).text((size // 2, size // 2), ch, font=font, fill=255)
    box = img.getbbox()
    if not box:
        return None
    px = img.crop(box).resize((cols, rows), Image.BOX).load()
    return [["#" if px[x, y] > 80 else " " for x in range(cols)] for y in range(rows)]


def big_text(text, rows, cols=None, gap=1):
    glyphs = []
    for ch in text:
        key = ch.upper()
        c = cols or max(1, round(rows * 2 * WIDTH.get(key, DEFAULT_W)))
        if key in "AV" and not cols and c % 2:
            c += 1  # even width lets the apex be two adjacent cells: /\\
        if ch == " ":
            g = [[" "] * (cols or max(2, round(rows * 0.8))) for _ in range(rows)]
        elif key in GLYPHS:
            g = stroke_glyph(key, rows, c)
        else:
            g = pillow_glyph(ch, rows, c) or [[" "] * c for _ in range(rows)]
        glyphs.append(g)
    sep = " " * gap
    lines = [sep.join("".join(g[r]) for g in glyphs).rstrip() for r in range(rows)]
    return "\n".join(lines)


def main():
    ap = argparse.ArgumentParser(description="Render text as big terminal characters.")
    ap.add_argument("text", nargs="+", help="text to render")
    ap.add_argument("-r", "--rows", type=int, default=6, help="height in rows (default 6)")
    ap.add_argument("-c", "--cols", type=int, default=None,
                    help="fixed width per glyph (default: derived from rows)")
    ap.add_argument("-g", "--gap", type=int, default=1, help="columns between glyphs (default 1)")
    args = ap.parse_args()
    print(big_text(" ".join(args.text), max(1, args.rows), args.cols, args.gap))


if __name__ == "__main__":
    main()
