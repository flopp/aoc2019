import sys

lines = list(filter(lambda line: len(line) > 0, sys.stdin.read().split('\n')))
height = len(lines)
width = len(lines[0])

best = 0
best_x = 0
best_y = 0

asteroids = []
for y in range(height):
    for x in range(width):
        if lines[y][x] != '#':
            continue
        asteroids.append((x, y))

for (x, y) in asteroids:
    blocked = [False] * (width * height)

    for (x1, y1) in asteroids:
        if x == x1 and y == y1:
            continue
        dx = x1 - x
        dy = y1 - y

        for (x2, y2) in asteroids:
            if x == x2 and y == y2:
                continue
            if x1 == x2 and y1 == y2:
                continue
            ok_x = (x == x2) if (dx == 0) else (((x2 - x) % dx) == 0)
            ok_y = (y == y2) if (dy == 0) else (((y2 - y) % dy) == 0)
            if ok_x and ok_y:
                if dx != 0 and dy != 0:
                    if ((x2 - x) / dx) != ((y2 - y) / dy):
                        continue
                print(f'dx={dx} dy={dy}')
                print(f'{x2},{y2} blocked by {x1},{y1} from {x},{y}')
                blocked[x2 + width * y2] = True
    
    visible = 0
    for (x1, y1) in asteroids:
        if not blocked[x1 + width * y1]:
            visible += 1
    visible -= 1
    if visible > best:
        best = visible
        best_x = x
        best_y = y

print(f'{x},{y} {best}')

