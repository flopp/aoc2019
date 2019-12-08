import sys

width = int(sys.argv[1])
height = int(sys.argv[2])
layers = list(zip(*(iter(sys.stdin.read()),) * (width * height)))

min_zeros = None
product12 = 0
for layer in layers:
    zeros = layer.count('0')
    if min_zeros is None or zeros < min_zeros:
        min_zeros = zeros
        product12 = layer.count('1') * layer.count('2')
print(f'checksum: {product12}')

for y in range(height):
    for x in range(width):
        for layer in layers:
            if layer[x + y * width] != '2':
                print(layer[x + y * width], end='')
                break
        else:
            print('?', end='')
    print('')
