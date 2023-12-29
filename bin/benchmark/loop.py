import time

t1 = time.time()

def loop(n):
    count = 0

    for i in range(n):
        count += 1;
    return count


N = 10 * 10000000

print(loop(N),time.time() - t1)