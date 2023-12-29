import time

t1 = time.time()

def isprime(n):
    nn = n - 1
    for i in range(2,nn):
        if n % i == 0:
            return False
    return True


def primes(n):
    count = 0

    for i in range(2,n):
        if isprime(i):
            count = count + 1
    return count


N = 10 * 10000

print(primes(N),time.time() - t1)