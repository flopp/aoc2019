import math
import sys

lines = list(filter(lambda line: len(line) > 0, sys.stdin.read().split('\n')))
height = len(lines)
width = len(lines[0])

asteroids = []
for y in range(height):
    for x in range(width):
        if lines[y][x] != '#':
            assert lines[y][x] == '.'
            continue
        asteroids.append((x, y))

best = 0
best_x = 0
best_y = 0
best_slopes = None

for (x, y) in asteroids:
    slopes = dict()
    for (x1, y1) in asteroids:
        dx, dy = x1 - x, y1 - y
        if dx == 0:
            if dy == 0:
                continue
            dy /= abs(dy)
        elif dy == 0:
            dx /= abs(dx)
        else:
            gcd = math.gcd(abs(dx), abs(dy))
            dx /= gcd
            dy /= gcd
        key = f'{dx},{dy}'
        if key in slopes:
            slopes[key].append((x1, y1))
        else:
            slopes[key] = [(x1, y1)]
    visible = len(slopes)
    if visible > best:
        best = visible
        best_slopes = slopes
        best_x = x
        best_y = y

print(f'part1: {best_x},{best_y} {best}')

angle_asteroids = []
for asteroids_list in best_slopes.values():
    (x, y) = asteroids_list[0]
    dx, dy = x - best_x, y - best_y
    angle = 90 + (math.atan2(dy, dx) * 180.0 / math.pi)
    while angle < 0:
        angle += 360
    while angle >= 360:
        angle -= 360
    angle_asteroids.append((angle, sorted(asteroids_list, key=lambda xy: abs(xy[0] - best_x) + abs(xy[1] - best_y))))

angle_asteroids.sort(key=lambda x: x[0])

index = 0
while len(angle_asteroids) > 0:
    new_list = []
    for (angle, asteroids_list) in angle_asteroids:
        index += 1
        (x, y) = asteroids_list[0]
        if index == 200:
            print(f'part2: {x},{y} {100 * x + y}')
        new_asteroids_list = asteroids_list[1:]
        if len(new_asteroids_list) > 0:
            new_list.append((angle, new_asteroids_list))
    angle_asteroids = new_list
